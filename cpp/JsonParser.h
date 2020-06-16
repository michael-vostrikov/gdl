#include "StringDescriptor.h"
#include "GdlNode.h"
#include "Stream.h"

class JsonParser
{
protected:

    Stream* stream;

    std::vector<GdlNode> pool;

public:
    
    enum RuleName {
        Empty, Json, Obj, Pair, Arr, Value, String, Esc, Unicode, Hex, SafeCodePoint, Number, Int, Exp
    };

    static const char* ruleNames[];


    JsonParser(Stream* stream): stream(stream)
    {
        this->pool.reserve(stream->getContent().getSize() / 2);
    }

    size_t createGdlNode(uint32_t ruleName, ListDescriptor value)
    {
        size_t newIndex = this->pool.size();
        GdlNode node(ruleName, value);
        this->pool.push_back(node);

        return newIndex;
    }

    size_t createGdlNode(uint32_t ruleName, StringDescriptor value)
    {
        size_t newIndex = this->pool.size();
        GdlNode node(ruleName, value);
        this->pool.push_back(node);

        return newIndex;
    }

    GdlNode* getRootNode()
    {
        return &this->pool.front();
    }

    size_t systemParseString(const char* string, size_t size)
    {
        size_t initialPos = this->stream->getPos();
        size_t parsedSize = 0;

        for (size_t i = 0; i < size; i++) {
            if (this->stream->eof()) {
                this->stream->setPos(initialPos);
                parsedSize = 0;
                break;
            }

            char symbol = this->stream->readSymbol();
            char contentSymbol = string[i];
            if (contentSymbol != symbol) {
                this->stream->setPos(initialPos);
                parsedSize = 0;
                break;
            }

            parsedSize++;
        }

        return parsedSize;
    }

    size_t systemParseRegexp(const char* str, int size)
    {
        if (this->stream->eof()) {
            return 0;
        }

        size_t parsedSize = 0;
        uint8_t symbol = this->stream->getCurrentSymbol();

        const char* data = str;
        for (size_t i = 0; i < size; i += 2) {
            uint8_t from = data[i];
            uint8_t to = data[i + 1];
            if (symbol >= from && symbol <= to) {
                this->stream->incPos();
                parsedSize = 1;
                break;
            }
        }

        return parsedSize;
    }

    virtual void skipSpaces()
    {
        while (!this->stream->eof()) {
            char symbol = this->stream->getCurrentSymbol();

            if (! (symbol == ' ' || symbol == '\n' || symbol == '\t' || symbol == '\r')) {
                break;
            }

            this->stream->incPos();
        }
    }

    

    virtual ssize_t parseJson()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '{':
            case '"':
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case 't':
            case 'f':
            case 'n':
                parsedCount = this->statement1Json();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Json()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Json;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseValue();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseObj()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '{':
                parsedCount = this->statement1Obj();
                if (parsedCount == -1) parsedCount = this->statement2Obj();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Obj()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Obj;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("{", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parsePair();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            while (true) {
                elementParsedCount = this->parseInline1_Obj_1();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("}", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2Obj()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Obj;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("{", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("}", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_Obj_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case ',':
                parsedCount = this->statement1Inline1_Obj_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_Obj_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Obj;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(",", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parsePair();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parsePair()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '"':
                parsedCount = this->statement1Pair();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Pair()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Pair;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseString();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::String, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(":", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseValue();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseArr()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '[':
                parsedCount = this->statement1Arr();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Arr()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Arr;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("[", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseInline1_Arr_1();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("]", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_Arr_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '{':
            case '"':
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case 't':
            case 'f':
            case 'n':
                parsedCount = this->statement1Inline1_Arr_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_Arr_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Arr;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseValue();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            while (true) {
                elementParsedCount = this->parseInline1_Inline1_Arr_1_1();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_Inline1_Arr_1_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case ',':
                parsedCount = this->statement1Inline1_Inline1_Arr_1_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_Inline1_Arr_1_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Arr;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(",", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseValue();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseValue()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '{':
                parsedCount = this->statement1Value();
            break;

            case '"':
                parsedCount = this->statement2Value();
            break;

            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                parsedCount = this->statement3Value();
            break;

            case '[':
                parsedCount = this->statement4Value();
            break;

            case 't':
                parsedCount = this->statement5Value();
            break;

            case 'f':
                parsedCount = this->statement6Value();
            break;

            case 'n':
                parsedCount = this->statement7Value();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Value()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Value;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseObj();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2Value()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Value;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseString();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::String, lexemeValue);
            totalParsedElementCount++;

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement3Value()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Value;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseNumber();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::Number, lexemeValue);
            totalParsedElementCount++;

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement4Value()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Value;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseArr();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement5Value()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Value;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("true", 4);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement6Value()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Value;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("false", 5);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement7Value()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Value;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("null", 4);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    size_t parseString()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '"':
                parsedSize = this->statement1String();
            break;
        };

        return parsedSize;
    }

    size_t statement1String()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("\"", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            while (true) {
                elementInitialStreamPos = this->stream->getPos();
                
                elementInitialStreamPos = this->stream->getPos();
                elementParsedSize = this->systemParseString("\"", 1);
    
                isElementParsed = (elementParsedSize != 0);
                if (isElementParsed) {
                    this->stream->setPos(elementInitialStreamPos);
                    isElementParsed = false;
                }
                else {
                    elementInitialStreamPos = this->stream->getPos();
                    elementParsedSize = this->parseInline1_String_1();
        
                    isElementParsed = (elementParsedSize != 0);
                }
                if (!isElementParsed) break;
                parsedSize += elementParsedSize;
            }

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("\"", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    size_t parseInline1_String_1()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\\':
                parsedSize = this->statement1Inline1_String_1();
                if (parsedSize == 0) parsedSize = this->statement2Inline1_String_1();
            break;

            case '\x20':
            case '!':
            case '"':
            case '#':
            case '$':
            case '%':
            case '&':
            case '\'':
            case '(':
            case ')':
            case '*':
            case '+':
            case ',':
            case '-':
            case '.':
            case '/':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case ':':
            case ';':
            case '<':
            case '=':
            case '>':
            case '?':
            case '@':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '[':
            case ']':
            case '^':
            case '_':
            case '`':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case '{':
            case '|':
            case '}':
            case '~':
            case '':
            case '\x80':
            case '\x81':
            case '\x82':
            case '\x83':
            case '\x84':
            case '\x85':
            case '\x86':
            case '\x87':
            case '\x88':
            case '\x89':
            case '\x8A':
            case '\x8B':
            case '\x8C':
            case '\x8D':
            case '\x8E':
            case '\x8F':
            case '\x90':
            case '\x91':
            case '\x92':
            case '\x93':
            case '\x94':
            case '\x95':
            case '\x96':
            case '\x97':
            case '\x98':
            case '\x99':
            case '\x9A':
            case '\x9B':
            case '\x9C':
            case '\x9D':
            case '\x9E':
            case '\x9F':
            case '\xA0':
            case '\xA1':
            case '\xA2':
            case '\xA3':
            case '\xA4':
            case '\xA5':
            case '\xA6':
            case '\xA7':
            case '\xA8':
            case '\xA9':
            case '\xAA':
            case '\xAB':
            case '\xAC':
            case '\xAD':
            case '\xAE':
            case '\xAF':
            case '\xB0':
            case '\xB1':
            case '\xB2':
            case '\xB3':
            case '\xB4':
            case '\xB5':
            case '\xB6':
            case '\xB7':
            case '\xB8':
            case '\xB9':
            case '\xBA':
            case '\xBB':
            case '\xBC':
            case '\xBD':
            case '\xBE':
            case '\xBF':
            case '\xC0':
            case '\xC1':
            case '\xC2':
            case '\xC3':
            case '\xC4':
            case '\xC5':
            case '\xC6':
            case '\xC7':
            case '\xC8':
            case '\xC9':
            case '\xCA':
            case '\xCB':
            case '\xCC':
            case '\xCD':
            case '\xCE':
            case '\xCF':
            case '\xD0':
            case '\xD1':
            case '\xD2':
            case '\xD3':
            case '\xD4':
            case '\xD5':
            case '\xD6':
            case '\xD7':
            case '\xD8':
            case '\xD9':
            case '\xDA':
            case '\xDB':
            case '\xDC':
            case '\xDD':
            case '\xDE':
            case '\xDF':
            case '\xE0':
            case '\xE1':
            case '\xE2':
            case '\xE3':
            case '\xE4':
            case '\xE5':
            case '\xE6':
            case '\xE7':
            case '\xE8':
            case '\xE9':
            case '\xEA':
            case '\xEB':
            case '\xEC':
            case '\xED':
            case '\xEE':
            case '\xEF':
            case '\xF0':
            case '\xF1':
            case '\xF2':
            case '\xF3':
            case '\xF4':
            case '\xF5':
            case '\xF6':
            case '\xF7':
            case '\xF8':
            case '\xF9':
            case '\xFA':
            case '\xFB':
            case '\xFC':
            case '\xFD':
            case '\xFE':
            case '\xFF':
                parsedSize = this->statement2Inline1_String_1();
            break;
        };

        return parsedSize;
    }

    size_t statement1Inline1_String_1()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseEsc();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    size_t statement2Inline1_String_1()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseSafeCodePoint();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    size_t parseEsc()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\\':
                parsedSize = this->statement1Esc();
            break;
        };

        return parsedSize;
    }

    size_t statement1Esc()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("\\", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseInline1_Esc_1();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    size_t parseInline1_Esc_1()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '"':
            case '\\':
            case '/':
            case 'b':
            case 'f':
            case 'n':
            case 'r':
            case 't':
                parsedSize = this->statement1Inline1_Esc_1();
            break;

            case 'u':
                parsedSize = this->statement2Inline1_Esc_1();
            break;
        };

        return parsedSize;
    }

    size_t statement1Inline1_Esc_1()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseRegexp("\"\"\\\\//bbffnnrrtt", 16);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    size_t statement2Inline1_Esc_1()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseUnicode();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    size_t parseUnicode()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'u':
                parsedSize = this->statement1Unicode();
            break;
        };

        return parsedSize;
    }

    size_t statement1Unicode()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("u", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseHex();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseHex();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseHex();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseHex();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    size_t parseHex()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
                parsedSize = this->statement1Hex();
            break;
        };

        return parsedSize;
    }

    size_t statement1Hex()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseRegexp("09afAF", 6);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    size_t parseSafeCodePoint()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\x20':
            case '!':
            case '"':
            case '#':
            case '$':
            case '%':
            case '&':
            case '\'':
            case '(':
            case ')':
            case '*':
            case '+':
            case ',':
            case '-':
            case '.':
            case '/':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case ':':
            case ';':
            case '<':
            case '=':
            case '>':
            case '?':
            case '@':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '[':
            case '\\':
            case ']':
            case '^':
            case '_':
            case '`':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case '{':
            case '|':
            case '}':
            case '~':
            case '':
            case '\x80':
            case '\x81':
            case '\x82':
            case '\x83':
            case '\x84':
            case '\x85':
            case '\x86':
            case '\x87':
            case '\x88':
            case '\x89':
            case '\x8A':
            case '\x8B':
            case '\x8C':
            case '\x8D':
            case '\x8E':
            case '\x8F':
            case '\x90':
            case '\x91':
            case '\x92':
            case '\x93':
            case '\x94':
            case '\x95':
            case '\x96':
            case '\x97':
            case '\x98':
            case '\x99':
            case '\x9A':
            case '\x9B':
            case '\x9C':
            case '\x9D':
            case '\x9E':
            case '\x9F':
            case '\xA0':
            case '\xA1':
            case '\xA2':
            case '\xA3':
            case '\xA4':
            case '\xA5':
            case '\xA6':
            case '\xA7':
            case '\xA8':
            case '\xA9':
            case '\xAA':
            case '\xAB':
            case '\xAC':
            case '\xAD':
            case '\xAE':
            case '\xAF':
            case '\xB0':
            case '\xB1':
            case '\xB2':
            case '\xB3':
            case '\xB4':
            case '\xB5':
            case '\xB6':
            case '\xB7':
            case '\xB8':
            case '\xB9':
            case '\xBA':
            case '\xBB':
            case '\xBC':
            case '\xBD':
            case '\xBE':
            case '\xBF':
            case '\xC0':
            case '\xC1':
            case '\xC2':
            case '\xC3':
            case '\xC4':
            case '\xC5':
            case '\xC6':
            case '\xC7':
            case '\xC8':
            case '\xC9':
            case '\xCA':
            case '\xCB':
            case '\xCC':
            case '\xCD':
            case '\xCE':
            case '\xCF':
            case '\xD0':
            case '\xD1':
            case '\xD2':
            case '\xD3':
            case '\xD4':
            case '\xD5':
            case '\xD6':
            case '\xD7':
            case '\xD8':
            case '\xD9':
            case '\xDA':
            case '\xDB':
            case '\xDC':
            case '\xDD':
            case '\xDE':
            case '\xDF':
            case '\xE0':
            case '\xE1':
            case '\xE2':
            case '\xE3':
            case '\xE4':
            case '\xE5':
            case '\xE6':
            case '\xE7':
            case '\xE8':
            case '\xE9':
            case '\xEA':
            case '\xEB':
            case '\xEC':
            case '\xED':
            case '\xEE':
            case '\xEF':
            case '\xF0':
            case '\xF1':
            case '\xF2':
            case '\xF3':
            case '\xF4':
            case '\xF5':
            case '\xF6':
            case '\xF7':
            case '\xF8':
            case '\xF9':
            case '\xFA':
            case '\xFB':
            case '\xFC':
            case '\xFD':
            case '\xFE':
            case '\xFF':
                parsedSize = this->statement1SafeCodePoint();
            break;
        };

        return parsedSize;
    }

    size_t statement1SafeCodePoint()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseRegexp("\x20\xFF", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    size_t parseNumber()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                parsedSize = this->statement1Number();
            break;
        };

        return parsedSize;
    }

    size_t statement1Number()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("-", 1);

            isElementParsed = (elementParsedSize != 0);
            if (isElementParsed) {
                parsedSize += elementParsedSize;
            }

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseInt();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseInline1_Number_1();

            isElementParsed = (elementParsedSize != 0);
            if (isElementParsed) {
                parsedSize += elementParsedSize;
            }

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseExp();

            isElementParsed = (elementParsedSize != 0);
            if (isElementParsed) {
                parsedSize += elementParsedSize;
            }

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    size_t parseInline1_Number_1()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '.':
                parsedSize = this->statement1Inline1_Number_1();
            break;
        };

        return parsedSize;
    }

    size_t statement1Inline1_Number_1()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(".", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            expressionInitialStreamPos = this->stream->getPos();
            quantifierParsedElementCount = 0;
            while (true) {
                elementInitialStreamPos = this->stream->getPos();
                elementParsedSize = this->systemParseRegexp("09", 2);
    
                isElementParsed = (elementParsedSize != 0);
                if (!isElementParsed) break;
                parsedSize += elementParsedSize;
                
                quantifierParsedElementCount++;
            }
            if (quantifierParsedElementCount < 1) {
                this->stream->setPos(expressionInitialStreamPos);
                
                break;
            }

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    size_t parseInt()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '0':
                parsedSize = this->statement1Int();
            break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                parsedSize = this->statement2Int();
            break;
        };

        return parsedSize;
    }

    size_t statement1Int()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("0", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    size_t statement2Int()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseRegexp("19", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            while (true) {
                elementInitialStreamPos = this->stream->getPos();
                elementParsedSize = this->systemParseRegexp("09", 2);
    
                isElementParsed = (elementParsedSize != 0);
                if (!isElementParsed) break;
                parsedSize += elementParsedSize;
            }

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    size_t parseExp()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'E':
            case 'e':
                parsedSize = this->statement1Exp();
            break;
        };

        return parsedSize;
    }

    size_t statement1Exp()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseRegexp("EEee", 4);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseRegexp("++--", 4);

            isElementParsed = (elementParsedSize != 0);
            if (isElementParsed) {
                parsedSize += elementParsedSize;
            }

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseInt();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    
};


const char* JsonParser::ruleNames[] = {
    "", "Json", "Obj", "Pair", "Arr", "Value", "String", "Esc", "Unicode", "Hex", "SafeCodePoint", "Number", "Int", "Exp"
};

