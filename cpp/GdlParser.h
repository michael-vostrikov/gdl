#include <vector>
#include <unordered_map>
#include <iostream>

struct RuleCallInfo
{
    StringDescriptor ruleName;
    size_t pos;
};

class GdlParser
{
protected:

    std::unordered_map<StringDescriptor, GdlNode*> ruleMap;
    Stream* stream;
    std::vector<std::string> errors;
    std::vector<RuleCallInfo> ruleCallStack;

public:

    GdlParser(GdlNode* grammar): ruleMap({}), errors({}), ruleCallStack({}), stream(NULL)
    {
        this->initRules(grammar);
    }

    virtual void initRules(GdlNode* grammar)
    {
        auto ruleList = grammar->getArray("Rule");
        for (auto it = ruleList.begin(); it != ruleList.end(); ++it) {
            GdlNode* rule = *it;

            StringDescriptor ruleName = rule->get("RuleName")->getStringValue();
            this->ruleMap[ruleName] = rule;
        }
    }

    virtual GdlNode* parse(StringDescriptor mainRuleName, Stream* stream, bool checkEof = true)
    {
        this->stream = stream;
        this->errors.clear();
        this->ruleCallStack.clear();

        GdlNode* rule = this->getRule(mainRuleName);
        GdlNode* result = this->parseRule(rule);

        if (checkEof && !this->stream->eof()) {
            this->errors.push_back(std::string("Unexpected data at ") + this->stream->getLineAndColumn() + " (" + std::to_string(this->stream->getPos()) + ")");
        }

        if (result == NULL) {
            this->errors.push_back(std::string("Parsing failed"));
        }

        return result;
    }

    virtual std::vector<std::string> getErrors()
    {
        return this->errors;
    }

    virtual GdlNode* getRule(StringDescriptor ruleName)
    {
        auto it = this->ruleMap.find(ruleName);
        if (it == this->ruleMap.end()) {
            throw std::runtime_error(std::string("Unknown rule: ") + ruleName.getStdString());
        }

        GdlNode* rule = it->second;
        return rule;
    }

protected:

    virtual GdlNode* parseRule(GdlNode* rule)
    {
        GdlNode* ruleNameObj = rule->get("RuleName");
        StringDescriptor ruleName = (ruleNameObj != NULL ? ruleNameObj->getStringValue() : "()");
        this->ruleCallStack.push_back(RuleCallInfo{ruleName, this->stream->getPos()});

        std::pair<bool, std::vector<GdlNode*>> parsedStatementResult(false, {});

        auto statementList = rule->get("RuleBody")->getArray("Statement");
        auto initialPos = this->stream->getPos();
        for (auto it = statementList.begin(); it != statementList.end(); ++it) {
            GdlNode* statement = *it;

            parsedStatementResult = this->parseStatement(statement);
            if (parsedStatementResult.first) {
                break;
            }

            // try parse next variant from same position
            this->stream->setPos(initialPos);
        }

        this->ruleCallStack.pop_back();

        return (parsedStatementResult.first ? new GdlNode(ruleName, parsedStatementResult.second) : NULL);
    }

    virtual std::pair<bool, std::vector<GdlNode*>> parseStatement(GdlNode* statement)
    {
        std::vector<GdlNode*> parsedStatement;

        auto expressionList = statement->getArray("Expression");
        bool cut = false;

        bool isParsed = true;
        std::pair<bool, std::vector<GdlNode*>> parsedExpressionResult;
        for (auto it = expressionList.begin(); it != expressionList.end(); ++it) {
            GdlNode* expression = *it;

            GdlNode* lookAheadElement = NULL;
            auto lookAhead = expression->get("LookAhead");
            if (lookAhead != NULL) {
                if (lookAhead->get("Element") != NULL) {
                    lookAheadElement = lookAhead->get("Element");
                }
                else {
                    auto next = std::next(it);
                    if (next != expressionList.end()) {
                        lookAheadElement = (*next)->get("Element");
                    }
                }
            }

            parsedExpressionResult = this->parseExpression(expression, lookAheadElement);
            if (!parsedExpressionResult.first) {
                if (cut) {
                    this->handleError(expression);
                    continue;
                }
                else {
                    isParsed = false;
                    parsedStatement.clear();
                    break;
                }
            }

            if (expression->get("Cut") != NULL) {
                cut = true;
            }

            auto parsedExpressionElementList = parsedExpressionResult.second;
            if (!parsedExpressionElementList.empty()) {
                // skip elements with name started with small letter
                StringDescriptor name = parsedExpressionElementList[0]->getName();  // all parsed elements in expression have same name
                bool isSmallLetter = (name.getSize() != 0 && (name.getDataPtr()[0] >= 'a' && name.getDataPtr()[0] <= 'z'));

                if (!isSmallLetter) {
                    if (name == "()") {
                        for (auto it = parsedExpressionElementList.begin(); it != parsedExpressionElementList.end(); ++it) {
                            GdlNode* inlineRule = *it;
                            this->addToParsedStatement(parsedStatement, inlineRule->getListValue());
                        }
                    }
                    else {
                        this->addToParsedStatement(parsedStatement, parsedExpressionElementList);
                    }
                }
            }
        }

        return std::make_pair(isParsed, parsedStatement);
    }

    virtual void addToParsedStatement(std::vector<GdlNode*>& parsedStatement, std::vector<GdlNode*>& parsedExpressionElementList)
    {
        for (auto it = parsedExpressionElementList.begin(); it != parsedExpressionElementList.end(); ++it) {
            GdlNode* node = *it;
            parsedStatement.push_back(node);
        }
    }

    virtual void handleError(GdlNode* expression)
    {
        auto elementType = expression->get("Element")->getFirst()->getName();
        std::string name;
        if (elementType == "StringLiteral" || elementType == "RegexpLiteral") {
            name = expression->toString().getStdString();
        }
        else {
            name = expression->get("Element")->getFirst()->getStringValue().getStdString();
        }

        int i = this->ruleCallStack.size() - 1;
        while (this->ruleCallStack[i].ruleName == "()") { i--; }
        auto ruleInfo = this->ruleCallStack[i];

        this->errors.push_back(std::string("Expected ") + name + " at " + this->stream->getLineAndColumn() + " (" + std::to_string(this->stream->getPos()) + ")"
            + " in " + ruleInfo.ruleName.getStdString() + " at " + this->stream->getLineAndColumn(ruleInfo.pos) + " (" + std::to_string(ruleInfo.pos) + ")");
    }

    virtual std::pair<bool, std::vector<GdlNode*>> parseExpression(GdlNode* expression, GdlNode* lookAheadElement)
    {
        auto element = expression->get("Element");

        std::vector<StringDescriptor> elementFunctionNameList;
        if (expression->get("FunctionCall") != NULL) {
            auto functionNameList = expression->get("FunctionCall")->getArray("FunctionName");
            for (auto it = functionNameList.begin(); it != functionNameList.end(); ++it) {
                GdlNode* functionName = *it;
                elementFunctionNameList.push_back(functionName->getStringValue());
            }
        }

        auto quantifier = expression->get("Quantifier");
        StringDescriptor quantifierType = "";
        uint32_t countVal = 0;
        if (quantifier != NULL) {
            GdlNode* count = quantifier->get("Count");
            if (count == NULL) {
                quantifierType = quantifier->get("")->getStringValue();
            }
            else {
                quantifierType = "{}";
                if (count->get("IntValue") != NULL) {
                    countVal = atoi(count->get("IntValue")->getStringValue().getStdString().c_str());
                }
                else if (count->get("FunctionCall") != NULL) {
                    StringDescriptor countFunctionName = count->get("FunctionCall")->get("FunctionName")->getStringValue();
                    countVal = this->callCountFunction(countFunctionName);
                }
            }
        }

        std::vector<GdlNode*> parsedElementList;
        size_t initialElementPos;
        while (true) {
            initialElementPos = this->stream->getPos();

            GdlNode* parsedLookAheadElement = NULL;
            if (lookAheadElement != NULL) {
                parsedLookAheadElement = this->parseElement(lookAheadElement);
                this->stream->setPos(initialElementPos);

                if (parsedLookAheadElement != NULL) {
                    break;
                }
            }

            GdlNode* parsedElement = this->parseElement(element);
            if (!elementFunctionNameList.empty() && parsedElement != NULL) {
                for (auto it = elementFunctionNameList.begin(); it != elementFunctionNameList.end(); ++it) {
                    StringDescriptor elementFunctionName = *it;

                    this->callElementFunction(elementFunctionName, parsedElement);
                    if (parsedElement == NULL) {
                        break;
                    }
                }
            }

            if (parsedElement != NULL) {
                parsedElementList.push_back(parsedElement);
            }
            else {
                this->stream->setPos(initialElementPos);
                break;
            }

            if (quantifierType.getSize() == 0 || quantifierType == "?" || quantifierType == "{}" && parsedElementList.size() == countVal) {
                break;
            }
        }

        bool countDoesNotMatch = ((quantifierType.getSize() == 0 || quantifierType == "+") ? parsedElementList.empty()
            : ((quantifierType == "{}") ? parsedElementList.size() != countVal : false)
        );
        if (countDoesNotMatch) {
            parsedElementList.clear();
            return std::make_pair(false, parsedElementList);
        }

        return std::make_pair(true, parsedElementList);
    }

    virtual GdlNode* parseElement(GdlNode* element)
    {
        auto specificElement = element->getFirst();
        StringDescriptor elementType = specificElement->getName();

        GdlNode* parsedElement = NULL;
        if (elementType == "RuleName") {
            StringDescriptor ruleName = specificElement->getStringValue();
            auto rule = this->getRule(ruleName);
            parsedElement = this->parseRule(rule);
        }
        else if (elementType == "StringLiteral") {
            parsedElement = this->parseStringLiteral(specificElement);
        }
        else if (elementType == "RegexpLiteral") {
            parsedElement = this->parseRegexpLiteral(specificElement);
        }
        else if (elementType == "InlineRule") {
            parsedElement = this->parseRule(specificElement);
        }
        else {
            throw std::runtime_error(std::string("Unknown element type: ") + elementType.getStdString());
        }

        return parsedElement;
    }

    // override this function to use specific count function
    virtual uint32_t callCountFunction(StringDescriptor functionName)
    {
        throw std::runtime_error(std::string("Not implemented: ") + functionName.getStdString());
        return 0;
    }

    // override this function to use specific element function
    virtual void callElementFunction(StringDescriptor functionName, GdlNode*& element)
    {
        if (functionName == "str") {
            this->str(element);
        }
        else if (functionName == "symbolStr") {
            this->symbolStr(element);
        }
        else {
            throw std::runtime_error(std::string("Not implemented: ") + functionName.getStdString());
        }
    }

    virtual void str(GdlNode*& parsedElement)
    {
        parsedElement->setStringValue(parsedElement->toString());
    }

    virtual void symbolStr(GdlNode*& parsedElement)
    {
        parsedElement->setStringValue(this->getSymbolStr(parsedElement));
    }

    virtual GdlNode* parseStringLiteral(GdlNode* element)
    {
        size_t initialPos = this->stream->getPos();
        size_t parsedValueSize = 0;

        auto symbolList = element->getArray("Symbol");
        for (auto it = symbolList.begin(); it != symbolList.end(); ++it) {
            GdlNode* symbol = *it;

            if (this->stream->eof()) {
                parsedValueSize = 0;
                break;
            }

            uint8_t contentSymbol = this->stream->readSymbol();
            uint8_t strSymbol = symbol->getStringValue().getDataPtr()[0];
            if (contentSymbol != strSymbol) {
                parsedValueSize = 0;
                break;
            }

            parsedValueSize += 1;
        }

        if (parsedValueSize == 0) {
            return NULL;
        }

        StringDescriptor parsedValue(this->stream->getContent().getDataPtr() + initialPos, parsedValueSize);

        return new GdlNode("", parsedValue);
    }

    virtual GdlNode* parseRegexpLiteral(GdlNode* element)
    {
        size_t initialPos = this->stream->getPos();
        size_t parsedValueSize = 0;

        if (!this->stream->eof()) {
            uint8_t contentSymbol = this->stream->readSymbol();

            if (element->get("AnySymbolLiteral") != NULL) {
                parsedValueSize = 1;
            }
            else {
                auto symbolRangeList = element->getArray("SymbolRange");
                for (auto it = symbolRangeList.begin(); it != symbolRangeList.end(); ++it) {
                    GdlNode* symbolRange = *it;
                    auto symbolList = symbolRange->getArray("Symbol");

                    if (symbolList.size() == 2) {
                        uint8_t strFrom = symbolList[0]->getStringValue().getDataPtr()[0];
                        uint8_t strTo = symbolList[1]->getStringValue().getDataPtr()[0];

                        if (contentSymbol >= strFrom && contentSymbol <= strTo) {
                            parsedValueSize = 1;
                            break;
                        }
                    }
                    else if (symbolList.size() == 1) {
                        uint8_t str = symbolList[0]->getStringValue().getDataPtr()[0];

                        if (contentSymbol == str) {
                            parsedValueSize = 1;
                            break;
                        }
                    }
                }
            }
        }

        if (parsedValueSize == 0) {
            return NULL;
        }

        StringDescriptor parsedValue(this->stream->getContent().getDataPtr() + initialPos, parsedValueSize);

        return new GdlNode("", parsedValue);
    }

    virtual StringDescriptor getSymbolStr(GdlNode* symbolNode)
    {
        char chr = '\0';
        char* data = new char[1];

        auto subNode = symbolNode->getFirst();
        StringDescriptor subElementType = subNode->getName();

        if (subElementType == "AnySymbol") {
            chr = subNode->getStringValue().getDataPtr()[0];
        }
        else if (subElementType == "EscapedSymbol") {
            chr = subNode->get("AnySymbol")->getStringValue().getDataPtr()[0];
            if (chr == 's') chr = ' ';
            else if (chr == 'n') chr = '\n';
            else if (chr == 't') chr = '\t';
            else if (chr == 'r') chr = '\r';
        }
        else if (subElementType == "HexCodeSymbol") {
            auto hexDigitList = subNode->getArray("HexDigit");
            uint8_t intValue1 = ((uint8_t)(hexDigitList[0]->getStringValue().getDataPtr()[0])) - 0x30;
            uint8_t intValue2 = ((uint8_t)(hexDigitList[1]->getStringValue().getDataPtr()[0])) - 0x30;
            intValue1 -= (intValue1 >= 0x0A ? 0x07 : 0);
            intValue2 -= (intValue2 >= 0x0A ? 0x07 : 0);
            uint8_t code = intValue1 * 0x10 + intValue2;
            chr = (char)code;
        }

        data[0] = chr;
        StringDescriptor str(data, 1);

        return str;
    }
};
