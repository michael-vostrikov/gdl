<?php

namespace Gdl;

use Exception;

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

    public function generateClass()
    {
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
        Empty, " . implode(',', $ruleNameList) . "
    };
";

        $ruleNames = "
const char* {$this->grammarName}Parser::ruleNames[] = {
    \"\", \"" . implode('","', $ruleNameList) . "\"
};
";

        return <<<SRC
#include "StringDescriptor.h"
#include "GdlNode.h"
#include "Stream.h"

class {$this->grammarName}Parser
{
protected:

    Stream* stream;

public:
    {$enum}
    static const char* ruleNames[];
    

    {$this->grammarName}Parser(Stream* stream): stream(stream)
    {
    }

    size_t systemParseString(const char* string, size_t size)
    {
        size_t initialPos = this->stream->getPos();
        size_t parsedLength = 0;

        for (size_t i = 0; i < size; i++) {
            if (this->stream->eof()) {
                parsedLength = 0;
                break;
            }

            char symbol = this->stream->readSymbol();
            char contentSymbol = string[i];
            if (contentSymbol != symbol) {
                this->stream->setPos(initialPos);
                parsedLength = 0;
                break;
            }

            parsedLength++;
        }

        return parsedLength;
    }

    size_t systemParseRegexp(const char* str, int size)
    {
        if (this->stream->eof()) {
            return 0;
        }

        size_t parsedLength = 0;
        uint8_t symbol = this->stream->getCurrentSymbol();

        const char* data = str;
        for (size_t i = 0; i < size; i += 2) {
            uint8_t from = data[i];
            uint8_t to = data[i + 1];
            if (symbol >= from && symbol <= to) {
                parsedLength = 1;
                break;
            }
        }

        if (parsedLength == 0) {
            return 0;
        }

        this->stream->readSymbol();

        return parsedLength;
    }

    void skipSpaces()
    {
        while (!this->stream->eof()) {
            char symbol = this->stream->readSymbol();

            if (! (symbol == ' ' || symbol == '\\n' || symbol == '\\t' || symbol == '\\r')) {
                this->stream->setPos(this->stream->getPos() - 1);
                break;
            }
        }
    }
    
    {$classBody}
};

{$ruleNames}

SRC;
    }

    public function generateClassBody()
    {
        $content = '';

        foreach ($this->ruleMap as $rule) {
            $content .= $this->generateRuleMethod($rule);
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

    protected $currentRuleName;

    public function generarateRuleParams($ruleName)
    {
        $ruleFlags = $this->ruleMap[$ruleName]->get('Flags');
        $isLexeme = ($ruleFlags !== null && $ruleFlags->get('Lexeme') !== null);

        $resultType = 'GdlNode*';
        $emptyValue = 'NULL';
        $initCode = 'if (res == NULL) res = new GdlNode(ruleName);';
        if ($isLexeme) {
            $resultType = 'size_t';
            $emptyValue = '0';
            $initCode = '';
        }

        return [$resultType, $emptyValue, $initCode, $isLexeme];
    }

    public function generateRuleMethod(GdlNode $rule)
    {
        $ruleName = $this->getRuleName($rule);
        $this->currentRuleName = $ruleName;

        $ruleMethodName = $this->generateRuleMethodName($ruleName);
        $statementSwitch = trim($this->generateStatementSwitch($rule, $ruleName, false));

        list($resultType, $emptyValue, $initCode) = $this->generarateRuleParams($this->currentRuleName);

        $content = <<<SRC


    {$resultType} {$ruleMethodName}()
    {
        {$resultType} res = {$emptyValue};

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            {$statementSwitch}
        };

        return res;
    }
SRC;

        foreach ($rule->get('RuleBody')->getArray('Statement') as $statementIndex => $statement) {
            $statementMethodName = $this->generateStatementMethodName($ruleName, $statementIndex);
            $statementMethodBody = trim($this->generateStatementMethodBody($ruleName, $statementIndex, $statement));

            $content .= <<<SRC


    {$resultType} {$statementMethodName}()
    {
        {$resultType} res = {$emptyValue};
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::{$this->currentRuleName};
        
        {$resultType} parsedElement = {$emptyValue};
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator last;
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            {$statementMethodBody}
            
            isParsed = true;
            {$initCode}
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return {$emptyValue};
        }

        return res;
    }
SRC;

            $expressionList = $statement->getArray('Expression');
            $inlineRuleIndex = 0;
            foreach ($expressionList as $expressionIndex => $expression) {
                $element = $expression->get('Element');

                $specificElement = $element->getFirst();
                $elementType = $specificElement->getName();
                $isInlineRule = ($elementType === "InlineRule");

                // TODO: check inline look-ahead
                if ($isInlineRule) {
                    $innerRuleName = $this->generateInlineRuleName($ruleName . '_' . ($statementIndex + 1), $inlineRuleIndex);
                    $content .= $this->generateInlineRuleMethod($specificElement, $innerRuleName);
                    $inlineRuleIndex++;
                }
            }
        }

        return $content;
    }

    public function generateInlineRuleMethod(GdlNode $rule, string $ruleName)
    {
        $ruleMethodName = $this->generateRuleMethodName($ruleName);
        $statementSwitch = trim($this->generateStatementSwitch($rule, $ruleName, true));

        list($resultType, $emptyValue, $initCode) = $this->generarateRuleParams($this->currentRuleName);

        $content = <<<SRC


    bool {$ruleMethodName}({$resultType}& res)
    {
        bool isParsed = false;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            {$statementSwitch}
        };

        return isParsed;
    }
SRC;

        foreach ($rule->get('RuleBody')->getArray('Statement') as $statementIndex => $statement) {
            $statementMethodName = $this->generateStatementMethodName($ruleName, $statementIndex);
            $statementMethodBody = trim($this->generateStatementMethodBody($ruleName, $statementIndex, $statement));

            $content .= <<<SRC


    bool {$statementMethodName}({$resultType}& res)
    {
        size_t initialPos = this->stream->getPos();
    
        auto ruleName = RuleName::{$this->currentRuleName};
        
        {$resultType} parsedElement = {$emptyValue};
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator last;
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            {$statementMethodBody}

            isParsed = true;
            {$initCode}
        } while (false);
        
        if (!isParsed) {
            this->stream->setPos(initialPos);
        }

        return isParsed;
    }
SRC;

            $expressionList = $statement->getArray('Expression');
            $inlineRuleIndex = 0;
            foreach ($expressionList as $expressionIndex => $expression) {
                $element = $expression->get('Element');

                $specificElement = $element->getFirst();
                $elementType = $specificElement->getName();
                $isInlineRule = ($elementType === "InlineRule");

                // TODO: check inline look-ahead
                if ($isInlineRule) {
                    $innerRuleName = $this->generateInlineRuleName($ruleName . '_' . ($statementIndex + 1), $inlineRuleIndex);
                    $content .= $this->generateInlineRuleMethod($specificElement, $innerRuleName);
                    $inlineRuleIndex++;
                }
            }
        }

        return $content;
    }


    public function generateStatementSwitch(GdlNode $rule, string $ruleName, bool $isInline)
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

            $content .= $this->generateStatementSwitchBranch($sameIndexesChars, $statementIndexList, $ruleName, $isInline);
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

    public function generateStatementSwitchBranch(array $sameIndexesChars, array $statementIndexList, string $ruleName, $isInline)
    {
        $content = '';
        list($resultType, $emptyValue, $initCode) = $this->generarateRuleParams($this->currentRuleName);

        foreach ($sameIndexesChars as $char) {
            $char = $this->generateSymbolStr($char, 1);
            $content .= "
            case '{$char}':";
        }

        foreach ($statementIndexList as $i => $statementIndex) {
            $statementMethodName = $this->generateStatementMethodName($ruleName, $statementIndex);

            if (!$isInline) {
                if ($i === 0) {
                    $content .= "
                res = this->{$statementMethodName}();";
                } else {
                    $content .= "
                if (res == {$emptyValue}) res = this->{$statementMethodName}();";
                }
            }
            else {
                if ($i === 0) {
                    $content .= "
                isParsed = this->{$statementMethodName}(res);";
                } else {
                    $content .= "
                if (isParsed == false) isParsed = this->{$statementMethodName}(res);";
                }
            }
        }

        $content .= "
            break;" . "\n";

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

        // TODO: functions

        $quantifier = $expression->get('Quantifier');
        $quantifierType = '';
        $countVal = 0;
        if ($quantifier !== null) {
            $count = $quantifier->get('Count');
            if ($count === null) {
                $quantifierType = $quantifier->get('')->getValue();
            }
            else {
                $quantifierType = '{}';
                if ($count->get('FunctionName') === null) {
                    $countVal = (int)$count->get('IntValue')->getValue();
                }
                else {
                    throw new Exception('Not implemented');
                }
            }
        }

        $element = $expression->get('Element');

        list($elementCode, $breakCondition, $addElementStr, $createResultStr) = $this->generateElementCode($ruleName, $element, $inlineRuleIndex, $statementIndex);

        if ($lookAheadElement !== null) {
            list($lookAheadElementCode, $lookAheadBreakCondition) = $this->generateElementCode($ruleName, $lookAheadElement, $inlineRuleIndex, $statementIndex);

            $initialPosCode = 'initialElementPos = this->stream->getPos();';

            $lookAheadElementCode = trim($lookAheadElementCode);
            $elementCode = "
            {$initialPosCode}

            {$lookAheadElementCode}
            if (! ({$lookAheadBreakCondition})) {
                this->stream->setPos(initialElementPos);
                break;
            }

            {$elementCode}
            ";

            $elementCode = trim($elementCode);
        }

        if ($quantifierType === '') {
            $content = "
            {$elementCode}
            if ({$breakCondition}) {
                // TODO: cut
                break;
            }
            {$createResultStr}
            {$addElementStr}
            ";
        }
        else if ($quantifierType === '?') {
            $content = "
            {$elementCode}
            if (! ({$breakCondition})) {
                {$createResultStr}
                {$addElementStr}
            }
            ";
        }
        else if ($quantifierType === '*') {
            $elementCode = str_replace("\n", "\n    ", $elementCode);

            $content = "
            while (true) {
                {$elementCode}
                if ({$breakCondition}) {
                    break;
                }

                {$createResultStr}
                {$addElementStr}
            }
            ";
        }
        else if ($quantifierType === '+' || $quantifierType === '{}') {
            $elementCode = str_replace("\n", "\n    ", $elementCode);
            list(,,, $currentIsLexeme) = $this->generarateRuleParams($this->currentRuleName);

            $outerBreakCondition = ($quantifierType === '+' ? 'parsedCount < 1' : 'parsedCount != ' . $countVal);

            $content = "
            parsedCount = 0;
            " . (!$currentIsLexeme ? "outerLast = res->getListValue().end();" : "outerParsedSize = res;") . "
            while (true) {
                {$elementCode}
                if ({$breakCondition}) {
                    break;
                }

                {$createResultStr}
                {$addElementStr}
                
                parsedCount++;
            }
            if ({$outerBreakCondition}) {
                " . (!$currentIsLexeme ? "res->getListValue().erase(outerLast, res->getListValue().end());" : "res = outerParsedSize;") . "
                // TODO: cut
                break;
            }
            ";
        }

        return $content;
    }

    public function generateElementCode(string $ruleName, GdlNode $element, int &$inlineRuleIndex, int $statementIndex)
    {
        $specificElement = $element->getFirst();
        $elementType = $specificElement->getName();
        $isInlineRule = ($elementType === "InlineRule");

        $elementCode = '';
        $breakCondition = '';
        $addElementStr = '';
        $createResultStr = '';

        list(,,, $currentIsLexeme) = $this->generarateRuleParams($this->currentRuleName);

        if (!$currentIsLexeme && $elementType !== 'InlineRule') {
            $createResultStr = 'if (res == NULL) res = new GdlNode(ruleName);';
        }
        else {
            $createResultStr = '';
        }

        if ($elementType === 'RuleName') {
            $innerRuleName = $specificElement->getValue();
            $ruleMethodName = $this->generateRuleMethodName($innerRuleName);

            list($resultType, $emptyValue, $initCode, $isLexeme) = $this->generarateRuleParams($innerRuleName);

            if (!$isLexeme) {
                $elementCode = "parsedElement = this->{$ruleMethodName}();";
                $breakCondition = "parsedElement == {$emptyValue}";
                $addElementStr = "res->addToList(parsedElement);";
            }
            else {
                $elementCode = "
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->{$ruleMethodName}();
            ";

                $breakCondition = "parsedSize == {$emptyValue}";

                if (!$currentIsLexeme) {
                    $addElementStr = "
            parsedElement = new GdlNode({$innerRuleName}, StringDescriptor(streamData, parsedSize));
            res->addToList(parsedElement);
            ";
                }
                else {
                    $addElementStr = '
            res += parsedSize;
            ';
                }
            }
        }
        elseif ($elementType === 'InlineRule') {
            $innerRuleName = $this->generateInlineRuleName($ruleName . '_' . ($statementIndex + 1), $inlineRuleIndex);
            $ruleMethodName = $this->generateRuleMethodName($innerRuleName);

            if (!$currentIsLexeme) {
                $elementCode = "
            last = res->getListValue().end();
            isInlineParsed = this->{$ruleMethodName}(res);
            if (isInlineParsed == false) {
                res->getListValue().erase(last, res->getListValue().end());
            }
            ";
            }
            else {
                $elementCode = "
            parsedSize = res;
            isInlineParsed = this->{$ruleMethodName}(res);
            if (isInlineParsed == false) {
                res = parsedSize;
            }
            ";
            }

            $breakCondition = "isInlineParsed == false";
            $addElementStr = "";
        }
        elseif ($elementType === 'StringLiteral') {
            list($str, $len) = $this->generateStringValue($specificElement);

            $elementCode = "
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString(\"{$str}\", $len);
            ";

            $breakCondition = "parsedSize == 0";

            if (!$currentIsLexeme) {
                // skip unnamed literals in rules, they are just for markup

                /*
                $addElementStr = '
            parsedElement = new GdlNode(0, StringDescriptor(streamData, parsedSize));
            res->addToList(parsedElement);
            ';
                */
            }
            else {
                $addElementStr = '
            res += parsedSize;
            ';
            }
        }
        elseif ($elementType === 'RegexpLiteral') {
            list($str, $len) = $this->generateRegexpValue($specificElement);

            $elementCode = "
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseRegexp(\"{$str}\", $len);
            ";

            $breakCondition = "parsedSize == 0";

            if (!$currentIsLexeme) {
                // skip unnamed literals in rules, they are just for markup

                /*
                $addElementStr = '
            parsedElement = new GdlNode(0, StringDescriptor(streamData, parsedSize));
            res->addToList(parsedElement);
            ';
                */
            }
            else {
                $addElementStr = '
            res += parsedSize;
            ';
            }
        }

        $elementCode = trim($elementCode);
        $addElementStr = trim($addElementStr);


        if ($isInlineRule) $inlineRuleIndex++;

        return [$elementCode, $breakCondition, $addElementStr, $createResultStr ];
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
