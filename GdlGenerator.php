<?php

namespace Gdl;

/**
 * Parser generator for specific language
 */
class GdlGenerator
{
    /** @var GdlNode[] */
    protected $ruleMap;

    /** @var string */
    protected $grammarName;

    public function __construct(GdlNode $grammar, string $grammarName)
    {
        $this->grammarName = $grammarName;

        $this->ruleMap = [];
        foreach ($grammar->getArray('Rule') as $rule) {
            $this->ruleMap[$rule->get('RuleName')->getValue()] = $rule;
        }
    }

    public function generateFile(string $filename)
    {
        $content = $this->generateClass();
        file_put_contents($filename, $content);
    }

    public $elementFunctions = [];
    public $countFunctions = [];

    public function generateClass()
    {
        $this->elementFunctions = [];
        $this->countFunctions = [];

        $classBody = $this->generateClassBody();
        $content = $this->generateClassWrapper($classBody);

        return $content;
    }

    public function generateClassWrapper(string $classBody)
    {

        $ruleNameList = [];
        foreach ($this->ruleMap as $rule) {
            $ruleName = $this->getRuleName($rule);
            $ruleNameList[] = $ruleName;
        }

        $enum = "
    enum RuleName {
        Empty, " . implode(', ', $ruleNameList) . "
    };
";

        $ruleNames = "
const char* {$this->grammarName}Parser::ruleNames[] = {
    \"\", \"" . implode('", "', $ruleNameList) . "\"
};
";

        $functionDeclarationsCode = '';
        foreach($this->elementFunctions as $functionName => $argumentType) {
            $functionDeclarationsCode .= "    virtual void {$functionName}({$argumentType}&) = 0;" . "\n";
        }
        foreach($this->countFunctions as $functionName) {
            $functionDeclarationsCode .= "    virtual int {$functionName}() = 0;" . "\n";
        }
        $functionDeclarationsCode = trim($functionDeclarationsCode);

        return <<<SRC
#include "StringDescriptor.h"
#include "GdlNode.h"
#include "Stream.h"

class {$this->grammarName}Parser
{
protected:

    Stream* stream;

    std::vector<GdlNode> pool;

public:
    {$enum}
    static const char* ruleNames[];


    {$this->grammarName}Parser(Stream* stream): stream(stream)
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

            if (! (symbol == ' ' || symbol == '\\n' || symbol == '\\t' || symbol == '\\r')) {
                break;
            }

            this->stream->incPos();
        }
    }

    {$classBody}

    {$functionDeclarationsCode}
};

{$ruleNames}

SRC;
    }

    protected $currentRuleName;

    public function generateClassBody()
    {
        $content = '';

        foreach ($this->ruleMap as $rule) {
            $ruleName = $this->getRuleName($rule);
            $this->currentRuleName = $ruleName;

            $content .= $this->generateRuleMethod($rule, $ruleName, false);
        }

        return $content;
    }

    public function getRuleName(GdlNode $rule)
    {
        return $rule->get('RuleName')->getValue();
    }

    public function generateRuleMethodName(string $ruleName)
    {
        return 'parse' . ucfirst($ruleName);
    }

    public function generateStatementMethodName(string $ruleName, int $statementIndex)
    {
        return "statement" . ($statementIndex + 1) . ucfirst($ruleName);
    }

    public function generateInlineRuleName($suffix, $inlineRuleIndex)
    {
        return "inline" . ($inlineRuleIndex + 1) . '_' . ucfirst($suffix);
    }

    public function getIsLexeme($ruleName)
    {
        $ruleFlags = $this->ruleMap[$ruleName]->get('Flags');
        $isLexeme = ($ruleFlags !== null && $ruleFlags->get('Lexeme') !== null);

        return $isLexeme;
    }

    public function generateRuleMethod(GdlNode $rule, $ruleName, bool $isInline)
    {
        $ruleMethodName = $this->generateRuleMethodName($ruleName);
        $statementSwitch = trim($this->generateStatementSwitch($rule, $ruleName));

        $isLexeme = $this->getIsLexeme($this->currentRuleName);
        if (!$isLexeme) {

            $createResultNode = trim('
        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);
            ');

            $updateResultNode = trim('
            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);
            ');

            if ($isInline) {
                $createResultNode = '';
                $updateResultNode = '';
            }

            $content = <<<SRC


    virtual ssize_t {$ruleMethodName}()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            {$statementSwitch}
        };

        return parsedCount;
    }
SRC;

            foreach ($rule->get('RuleBody')->getArray('Statement') as $statementIndex => $statement) {
                $statementMethodName = $this->generateStatementMethodName($ruleName, $statementIndex);
                $statementMethodBody = trim($this->generateStatementMethodBody($ruleName, $statementIndex, $statement));

                // we need to create GdlNode after other elements because it allows to avoid special handling for inline rules
                // otherwise we would need to store index for current rule and pass it to functions for inline rules
                // tree has right-to-left, root node is last element in this->pool

                $content .= <<<SRC


    virtual ssize_t {$statementMethodName}()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::{$this->currentRuleName};
        ssize_t totalParsedElementCount = 0;

        {$createResultNode}

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
            {$statementMethodBody}

            {$updateResultNode}

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }
SRC;
            }
        }
        else {

            $content = <<<SRC


    size_t {$ruleMethodName}()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            {$statementSwitch}
        };

        return parsedSize;
    }
SRC;

            foreach ($rule->get('RuleBody')->getArray('Statement') as $statementIndex => $statement) {
                $statementMethodName = $this->generateStatementMethodName($ruleName, $statementIndex);
                $statementMethodBody = trim($this->generateStatementMethodBody($ruleName, $statementIndex, $statement));

                $content .= <<<SRC


    size_t {$statementMethodName}()
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
            {$statementMethodBody}

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }
SRC;
            }
        }

        foreach ($rule->get('RuleBody')->getArray('Statement') as $statementIndex => $statement) {
            $expressionList = $statement->getArray('Expression');
            $inlineRuleIndex = 0;
            foreach ($expressionList as $expressionIndex => $expression) {
                $element = $expression->get('Element');

                $specificElement = $element->getFirst();
                $elementType = $specificElement->getName();
                $isInlineRule = ($elementType === "InlineRule");

                if ($isInlineRule) {
                    $innerRuleName = $this->generateInlineRuleName($ruleName . '_' . ($statementIndex + 1), $inlineRuleIndex);
                    $content .= $this->generateRuleMethod($specificElement, $innerRuleName, true);
                    $inlineRuleIndex++;
                }

                $lookAheadElement = null;
                $lookAhead = $expression->get('LookAhead');
                if ($lookAhead !== null) {
                    if ($lookAhead->get('Element') !== null) {
                        $lookAheadElement = $lookAhead->get('Element');
                    } elseif (isset($expressionList[$expressionIndex + 1])) {
                        $lookAheadElement = $expressionList[$expressionIndex + 1]->get('Element');
                    }
                }

                if ($lookAheadElement !== null) {
                    $lookAheadSpecificElement = $lookAheadElement->getFirst();
                    $lookAheadElementType = $lookAheadSpecificElement->getName();
                    $isInlineRule = ($lookAheadElementType === "InlineRule");

                    if ($isInlineRule) {
                        $innerRuleName = $this->generateInlineRuleName($ruleName . '_' . ($statementIndex + 1), $inlineRuleIndex);
                        $content .= $this->generateRuleMethod($lookAheadSpecificElement, $innerRuleName, true);
                        $inlineRuleIndex++;
                    }
                }
            }
        }

        return $content;
    }

    public function generateStatementSwitch(GdlNode $rule, string $ruleName)
    {
        $content = '';

        $ruleChars = $this->getRuleChars($rule);

        // 'A' => 0, 1
        // 'B' => 0, 2
        // 'C' => 0, 1

        $handledChars = [];
        foreach ($ruleChars as $char => $statementIndexList) {
            if (isset($handledChars[$char])) continue;

            $sameIndexesChars = [];
            foreach ($ruleChars as $char2 => $statementIndexList2) {
                if ($statementIndexList2 === $statementIndexList) {
                    $sameIndexesChars[] = $char2;
                    $handledChars[$char2] = 1;
                }
            }

            $content .= $this->generateStatementSwitchBranch($sameIndexesChars, $statementIndexList, $ruleName);
        }

        return $content;
    }

    public function getRuleChars(GdlNode $rule)
    {
        $ruleChars = [];

        foreach ($rule->get('RuleBody')->getArray('Statement') as $statementIndex => $statement) {
            foreach ($statement->getArray('Expression') as $expression) {
                $quantifier = $expression->get('Quantifier');
                $quantifierType = '';
                if ($quantifier !== null) {
                    $count = $quantifier->get('Count');
                    if ($count === null) {
                        $quantifierType = $quantifier->get('')->getValue();
                    }
                    else {
                        // count with function can be 0
                        $quantifierType = '*';
                    }
                }

                $element = $expression->get('Element');
                $specificElement = $element->getFirst();
                $elementType = $specificElement->getName();

                if ($elementType === 'RuleName') {
                    $innerRuleName = $specificElement->getValue();
                    $innerRule = $this->ruleMap[$innerRuleName];

                    $innerChars = $this->getRuleChars($innerRule);
                    foreach ($innerChars as $innerChar => $innerStatementIndexList) {
                        $ruleChars[$innerChar][] = $statementIndex;
                    }
                }
                elseif ($elementType === 'InlineRule') {
                    $innerRule = $specificElement;
                    $innerChars = $this->getRuleChars($innerRule);

                    foreach ($innerChars as $innerChar => $innerStatementIndexList) {
                        $ruleChars[$innerChar][] = $statementIndex;
                    }
                }
                elseif ($elementType === 'StringLiteral') {
                    $symbol = $specificElement->getArray('Symbol')[0];

                    $chr = $symbol->getValue();
                    $ruleChars[$chr][] = $statementIndex;
                }
                elseif ($elementType === 'RegexpLiteral') {
                    if ($specificElement->get('AnySymbolLiteral')) {
                        for ($i = 0x00; $i <= 0xFF; $i++) {
                            $chr = chr($i);
                            $ruleChars[$chr][] = $statementIndex;
                        }
                    } else {
                        $symbolRangeList = $specificElement->getArray('SymbolRange');

                        foreach ($symbolRangeList as $symbolRange) {
                            $symbolList = $symbolRange->getArray('Symbol');
                            if (count($symbolList) == 2) {
                                list($from, $to) = $symbolList;
                                for ($i = ord($from->getValue()); $i <= ord($to->getValue()); $i++) {
                                    $chr = chr($i);
                                    $ruleChars[$chr][] = $statementIndex;
                                }
                            } elseif (count($symbolList) == 1) {
                                $chr = $symbolList[0]->getValue();
                                $ruleChars[$chr][] = $statementIndex;
                            }
                        }
                    }
                }

                if ($quantifierType == '' || $quantifierType == '+') {
                    // till first required element
                    break;
                }
            }
        }

        return $ruleChars;
    }

    public function generateStatementSwitchBranch(array $sameIndexesChars, array $statementIndexList, string $ruleName)
    {
        $content = '';
        $isLexeme = $this->getIsLexeme($this->currentRuleName);

        foreach ($sameIndexesChars as $char) {
            $char = $this->generateSymbolStr($char, 1);
            $content .= "
            case '{$char}':";
        }

        foreach ($statementIndexList as $i => $statementIndex) {
            $statementMethodName = $this->generateStatementMethodName($ruleName, $statementIndex);

            if (!$isLexeme) {
                if ($i === 0) {
                    $content .= "
                parsedCount = this->{$statementMethodName}();";
                } else {
                    $content .= "
                if (parsedCount == -1) parsedCount = this->{$statementMethodName}();";
                }
            }
            else {
                if ($i === 0) {
                    $content .= "
                parsedSize = this->{$statementMethodName}();";
                } else {
                    $content .= "
                if (parsedSize == 0) parsedSize = this->{$statementMethodName}();";
                }
            }
        }

        $content .= "
            break;"
        . "\n";

        return $content;
    }

    public function generateStatementMethodBody(string $ruleName, int $statementIndex, GdlNode $statement)
    {
        $content = '';

        $ruleFlags = $this->ruleMap[$this->currentRuleName]->get('Flags');
        $keepSpaces = ($ruleFlags !== null && $ruleFlags->get('KeepSpaces') !== null);
        $isLexeme = ($ruleFlags !== null && $ruleFlags->get('Lexeme') !== null);
        $needKeepSpaces = ($keepSpaces || $isLexeme);

        if (!$needKeepSpaces && $this->currentRuleName === array_key_first($this->ruleMap)) {
            // skip initial spaces only for main rule
            $content .= 'this->skipSpaces();' . "\n";
        }

        $expressionList = $statement->getArray('Expression');
        $inlineRuleIndex = 0;
        foreach ($expressionList as $expressionIndex => $expression) {

            $lookAheadElement = null;
            $lookAhead = $expression->get('LookAhead');
            if ($lookAhead !== null) {
                if ($lookAhead->get('Element') !== null) {
                    $lookAheadElement = $lookAhead->get('Element');
                }
                elseif (isset($expressionList[$expressionIndex + 1])) {
                    $lookAheadElement = $expressionList[$expressionIndex + 1]->get('Element');
                }
            }

            $expressionContent = $this->generateExpressionCode($ruleName, $expression, $lookAheadElement, $inlineRuleIndex, $statementIndex);

            $content .= "\n" . '            ' . trim($expressionContent) . "\n";
            if (!$needKeepSpaces) {
                $content .= "\n" . '            this->skipSpaces();' . "\n";
            }
            $content .= "\n" . '            // ------------------------------------------------' . "\n";
        }

        return $content;
    }

    public function generateExpressionCode(string $ruleName, GdlNode $expression, ?GdlNode $lookAheadElement, int &$inlineRuleIndex, int $statementIndex)
    {
        $content = '';

        $quantifier = $expression->get('Quantifier');
        $quantifierType = '';
        $countVal = '';
        if ($quantifier !== null) {
            $count = $quantifier->get('Count');
            if ($count === null) {
                $quantifierType = $quantifier->get('')->getValue();
            }
            else {
                $quantifierType = '{}';
                if ($count->get('FunctionCall') === null) {
                    $countVal = (int)$count->get('IntValue')->getValue();
                }
                else {
                    $countFunctionName = $count->get('FunctionCall')->get('FunctionName')->getValue();
                    $this->countFunctions[] = $countFunctionName;

                    $countVal = "this->{$countFunctionName}()";
                }
            }
        }

        $element = $expression->get('Element');

        $functionNameList = [];
        if ($expression->get('FunctionCall') !== null) {
            foreach ($expression->get('FunctionCall')->getArray('FunctionName') as $functionName) {
                $functionNameList[] = $functionName->getValue();
            }
        }

        $currentIsLexeme = $this->getIsLexeme($this->currentRuleName);

        list($elementCode, $addElementCode) = $this->generateElementCode($ruleName, $element, $inlineRuleIndex, $statementIndex, $functionNameList);


        if ($lookAheadElement !== null) {
            list($lookAheadElementCode) = $this->generateElementCode($ruleName, $lookAheadElement, $inlineRuleIndex, $statementIndex);
            $lookAheadElementCode = trim($lookAheadElementCode);

            $elementCode = str_replace("\n", "\n    ", $elementCode);

            $elementCode = "
            elementInitialStreamPos = this->stream->getPos();
            "
            . (!$currentIsLexeme ? "elementInitialPoolSize = this->pool.size();" : "")
            . "
            {$lookAheadElementCode}
            if (isElementParsed) {
                this->stream->setPos(elementInitialStreamPos);
                "
                . (!$currentIsLexeme ? "this->pool.resize(elementInitialPoolSize);" : "")
                . "isElementParsed = false;
            }
            else {
                {$elementCode}
            }
            ";

            $elementCode = trim($elementCode);
        }


        // TODO: cut

        if ($quantifierType === '') {
            $content = "
            {$elementCode}
            if (!isElementParsed) break;
            {$addElementCode}
            ";
        }
        else if ($quantifierType === '?') {
            $content = "
            {$elementCode}
            if (isElementParsed) {
                {$addElementCode}
            }
            ";
        }
        else if ($quantifierType === '*') {
            $elementCode = str_replace("\n", "\n    ", $elementCode);

            $content = "
            while (true) {
                {$elementCode}
                if (!isElementParsed) break;
                {$addElementCode}
            }
            ";
        }
        else if ($quantifierType === '+') {
            $elementCode = str_replace("\n", "\n    ", $elementCode);

            $content = "
            expressionInitialStreamPos = this->stream->getPos();
            "
            . (!$currentIsLexeme ? "expressionInitialPoolSize = this->pool.size();" : "")
            . "quantifierParsedElementCount = 0;
            while (true) {
                {$elementCode}
                if (!isElementParsed) break;
                {$addElementCode}
                
                quantifierParsedElementCount++;
            }
            if (quantifierParsedElementCount < 1) {
                this->stream->setPos(expressionInitialStreamPos);
                " . (!$currentIsLexeme ? "this->pool.resize(expressionInitialPoolSize);" : "") . "
                break;
            }
            ";
        }
        else if ($quantifierType === '{}') {
            $elementCode = str_replace("\n", "\n    ", $elementCode);
            $currentIsLexeme = $this->getIsLexeme($this->currentRuleName);

            $content = "
            expressionInitialStreamPos = this->stream->getPos();
            "
            . (!$currentIsLexeme ? "expressionInitialPoolSize = this->pool.size();" : "")
            . "quantifierParsedElementCount = 0;
            quantifierRequiredElementCount = {$countVal};
            while (quantifierParsedElementCount < quantifierRequiredElementCount) {
                {$elementCode}
                if (!isElementParsed) {
                    break;
                }

                {$addElementCode}

                quantifierParsedElementCount++;
            }
            if (quantifierParsedElementCount != quantifierRequiredElementCount) {
                this->stream->setPos(expressionInitialStreamPos);
                " . (!$currentIsLexeme ? "this->pool.resize(expressionInitialPoolSize);" : "") . "
                break;
            }
            ";
        }

        return $content;
    }

    public function generateElementCode(string $ruleName, GdlNode $element, int &$inlineRuleIndex, int $statementIndex, $functionNameList = [])
    {
        $specificElement = $element->getFirst();
        $elementType = $specificElement->getName();
        $isInlineRule = ($elementType === "InlineRule");

        $elementCode = '';
        $addElementCode = '';

        $currentIsLexeme = $this->getIsLexeme($this->currentRuleName);
        $innerIsLexeme = false;

        if ($elementType === 'RuleName') {
            $innerRuleName = $specificElement->getValue();
            $ruleMethodName = $this->generateRuleMethodName($innerRuleName);

            $innerIsLexeme = $this->getIsLexeme($innerRuleName);

            $ruleFlags = $this->ruleMap[$innerRuleName]->get('Flags');
            $needSkip = ($ruleFlags !== null && $ruleFlags->get('Skip') !== null);
            if (!$innerIsLexeme) {
                // rules should not be in lexemes so it's ok not to check this combination here

                $elementCode = "elementParsedCount = this->{$ruleMethodName}();";
                $addElementCode = "totalParsedElementCount++;";

                if ($needSkip) {
                    $addElementCode = "";
                }
            }
            else {
                $elementCode = "
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->{$ruleMethodName}();
            ";

                if (!$currentIsLexeme) {
                    // nodes for lexemes are created only in rules for performance
                    $addElementCode = "
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::{$innerRuleName}, lexemeValue);
            totalParsedElementCount++;
            ";
                    if ($needSkip) {
                        $addElementCode = "";
                    }
                }
                else {
                    $addElementCode = 'parsedSize += elementParsedSize;';
                }
            }
        }
        elseif ($elementType === 'InlineRule') {
            $innerRuleName = $this->generateInlineRuleName($ruleName . '_' . ($statementIndex + 1), $inlineRuleIndex);
            $ruleMethodName = $this->generateRuleMethodName($innerRuleName);

            $innerIsLexeme = $currentIsLexeme;

            if (!$currentIsLexeme) {
                $elementCode = "elementParsedCount = this->{$ruleMethodName}();";
                $addElementCode = "totalParsedElementCount += elementParsedCount;";
            }
            else {
                $elementCode = "
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->{$ruleMethodName}();
            ";
                $addElementCode = "parsedSize += elementParsedSize;";
            }
        }
        elseif ($elementType === 'StringLiteral') {
            $innerIsLexeme = true;

            list($str, $len) = $this->generateStringValue($specificElement);

            $elementCode = "
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(\"{$str}\", $len);
            ";

            if (!$currentIsLexeme) {
                // skip unnamed literals in rules, they are just for markup
                $addElementCode = "";

                /*
                $addElementCode = "
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::Empty, lexemeValue);
            totalParsedElementCount++;
            ";
                */
            }
            else {
                $addElementCode = "parsedSize += elementParsedSize;";
            }
        }
        elseif ($elementType === 'RegexpLiteral') {
            $innerIsLexeme = true;

            list($str, $len) = $this->generateRegexpValue($specificElement);

            $elementCode = "
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseRegexp(\"{$str}\", $len);
            ";

            if (!$currentIsLexeme) {
                // skip unnamed literals in rules, they are just for markup
                $addElementCode = "";

                /*
                $addElementCode = "
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::Empty, lexemeValue);
            totalParsedElementCount++;
            ";
                */
            }
            else {
                $addElementCode = 'parsedSize += elementParsedSize;';
            }
        }

        if (!$innerIsLexeme) {
            $elementVariableName = 'elementParsedCount';
            $functionArgumentType = 'ssize_t';
        }
        else {
            $elementVariableName = 'elementParsedSize';
            $functionArgumentType = 'size_t';
        }

        foreach ($functionNameList as $functionName) {
            $elementCode .= "\n" . "            this->{$functionName}({$elementVariableName});";
            $this->elementFunctions[$functionName] = $functionArgumentType;
        }

        $elementCode = trim($elementCode);
        $addElementCode = trim($addElementCode);

        if ($currentIsLexeme || $innerIsLexeme) {
            $elementCode .= "\n\n            isElementParsed = (elementParsedSize != 0);";
        }
        else {
            $elementCode .= "\n\n            isElementParsed = (elementParsedCount != -1);";
        }

        if ($isInlineRule) $inlineRuleIndex++;

        return [$elementCode, $addElementCode];
    }

    public function generateSymbolStr(string $char, $quotesMode)
    {
        $code = ord($char);
        if ($code <= 0x20 || $code >= 0x80) {
            $char = sprintf('\\x%02X', $code);
        }
        else if ($quotesMode === 0 && $code === ord('"')) {
            $char = '\\"';
        }
        else if ($quotesMode === 1 && $code === ord('\'')) {
            $char = '\\\'';
        }
        else if ($code === ord('\\')) {
            $char = '\\\\';
        }

        return $char;
    }

    public function generateStringValue(GdlNode $element)
    {
        $str = '';
        $len = 0;
        foreach ($element->getArray('Symbol') as $symbol) {
            $chr = $symbol->getValue();
            $chr = $this->generateSymbolStr($chr, 0);

            $str .= $chr;
            $len++;
        }

        return [$str, $len];
    }

    public function generateRegexpValue(GdlNode $element)
    {
        $str = '';
        $len = 0;
        if ($element->getArray('AnySymbolLiteral')) {
            $str = '\x00\xFF';
            return [$str, 2];
        }

        foreach ($element->getArray('SymbolRange') as $symbolRange) {
            $symbolList = $symbolRange->getArray('Symbol');
            if (count($symbolList) == 2) {
                $str .= $this->generateSymbolStr($symbolList[0]->getValue(), 0);
                $str .= $this->generateSymbolStr($symbolList[1]->getValue(), 0);
            }
            else if (count($symbolList) == 1) {
                // make double symbol for unified handling
                $str .= $this->generateSymbolStr($symbolList[0]->getValue(), 0);
                $str .= $this->generateSymbolStr($symbolList[0]->getValue(), 0);
            }
            $len += 2;
        }

        return [$str, $len];
    }
}
