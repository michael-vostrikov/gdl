<?php

namespace Gdl;

use Exception;

/**
 * Parser generator for specific language
 */
class GdlGenerator
{
    /** @var GdlNode */
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

    // TODO: generate rule tokens

    public function generateClassWrapper(string $classBody)
    {
        return <<<SRC
#include "StringDescriptor.h"
#include "GdlNode.h"
#include "Stream.h"

class {$this->grammarName}Parser
{
protected:

    Stream* stream;

public:

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

SRC;
    }

    // TODO: Flags: KeepSpaces, Lexeme, KeepTerminals

    public function generateClassBody()
    {
        $content = '';

        $ruleNameList = [];
        foreach ($this->ruleMap as $rule) {
            $ruleName = $this->getRuleName($rule);
            $ruleNameList[] = $ruleName;
        }

        $content .= "
            enum TokenName {
                Empty, " . implode(',', $ruleNameList) . "
            };
        ";

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

    public function generateRuleMethod(GdlNode $rule)
    {
        $ruleName = $this->getRuleName($rule);
        $this->currentRuleName = $ruleName;

        $ruleMethodName = $this->generateRuleMethodName($ruleName);
        $statementSwitch = trim($this->generateStatementSwitch($rule, $ruleName, false));

        $content = <<<SRC


    GdlNode* {$ruleMethodName}()
    {
        GdlNode* res = NULL;
        size_t initialPos = this->stream->getPos();

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            {$statementSwitch}
        };

        if (res == NULL) {   
            this->stream->setPos(initialPos);
        }

        return res;
    }
SRC;

        foreach ($rule->get('RuleBody')->getArray('Statement') as $statementIndex => $statement) {
            $statementMethodName = $this->generateStatementMethodName($ruleName, $statementIndex);
            $statementMethodBody = trim($this->generateStatementMethodBody($ruleName, $statementIndex, $statement));

            $content .= <<<SRC


    GdlNode* {$statementMethodName}()
    {
        GdlNode* res = NULL;

        auto ruleName = TokenName::{$this->currentRuleName};
        
        GdlNode* parsedElement = NULL;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator last;
        std::vector<GdlNode*>::iterator outerLast;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            {$statementMethodBody}
            
            isParsed = true;
            if (res == NULL) {
                res = new GdlNode(ruleName);
            }
        } while (false);

        if (!isParsed) {
            return NULL;
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

        $content = <<<SRC


    bool {$ruleMethodName}(GdlNode*& res)
    {
        bool isParsed = false;
        size_t initialPos = this->stream->getPos();

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            {$statementSwitch}
        };

        if (isParsed == false) {   
            this->stream->setPos(initialPos);
        }

        return isParsed;
    }
SRC;

        foreach ($rule->get('RuleBody')->getArray('Statement') as $statementIndex => $statement) {
            $statementMethodName = $this->generateStatementMethodName($ruleName, $statementIndex);
            $statementMethodBody = trim($this->generateStatementMethodBody($ruleName, $statementIndex, $statement));

            $content .= <<<SRC


    bool {$statementMethodName}(GdlNode*& res)
    {
        auto ruleName = TokenName::{$this->currentRuleName};
        
        GdlNode* parsedElement = NULL;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator last;
        std::vector<GdlNode*>::iterator outerLast;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            {$statementMethodBody}

            isParsed = true;
            if (res == NULL) {
                res = new GdlNode(ruleName);
            }
        } while (false);
        
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

        foreach ($sameIndexesChars as $char) {
            $char = $this->generateSymbolStr($char, 1);
            $content .= <<<SRC
            case '{$char}':

SRC;
        }

        foreach ($statementIndexList as $i => $statementIndex) {
            $statementMethodName = $this->generateStatementMethodName($ruleName, $statementIndex);

            if (!$isInline) {
                if ($i === 0) {
                    $content .= <<<SRC
                res = this->{$statementMethodName}();

SRC;
                } else {
                    $content .= <<<SRC
                if (res == NULL) res = this->{$statementMethodName}();

SRC;
                }
            }
            else {
                if ($i === 0) {
                    $content .= <<<SRC
                isParsed = this->{$statementMethodName}(res);

SRC;
                } else {
                    $content .= <<<SRC
                if (isParsed == false) isParsed = this->{$statementMethodName}(res);

SRC;
                }
            }
        }

        $content .= <<<SRC
            break;


SRC;

        return $content;
    }

    public function generateStatementMethodBody(string $ruleName, int $statementIndex, GdlNode $statement)
    {
        $content = '';

        // TODO: check keep spaces flag
        $content .= 'this->skipSpaces();' . "\n";

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
            $content .= "\n" . '            this->skipSpaces();' . "\n";
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
        if ($quantifier !== null) {
            $count = $quantifier->get('Count');
            if ($count === null) {
                $quantifierType = $quantifier->get('')->getValue();
            }
            else {
                $quantifierType = '{}';
                // TODO: countVal
                throw new Exception('Not implemented');
            }
        }

        $element = $expression->get('Element');

        list($elementCode, $breakCondition, $addElementStr) = $this->generateElementCode($ruleName, $element, $inlineRuleIndex, $statementIndex);

        $createResultStr = 'if (res == NULL) res = new GdlNode(ruleName);';


        if ($lookAheadElement !== null) {
            list($lookAheadElementCode, $lookAheadBreakCondition) = $this->generateElementCode($ruleName, $lookAheadElement, $inlineRuleIndex, $statementIndex);

            $lookAheadElementCode = trim($lookAheadElementCode);
            $elementCode = "
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

            // TODO: remove setPos()
            $content = "
            while (true) {
                initialElementPos = this->stream->getPos();

                {$elementCode}
                if ({$breakCondition}) {
                    this->stream->setPos(initialElementPos);
                    break;
                }

                {$createResultStr}
                {$addElementStr}
            }
            ";
        }
        else if ($quantifierType === '+') {
            $elementCode = str_replace("\n", "\n    ", $elementCode);

            // TODO: check erase
            $content = "
            parsedCount = 0;
            while (true) {
                initialElementPos = this->stream->getPos();
                
                {$elementCode}
                if ({$breakCondition}) {
                    this->stream->setPos(initialElementPos);
                    break;
                }

                {$createResultStr}
                {$addElementStr}
            }
            if (parsedCount == 0) {
                // TODO: cut
                break;
            }
            ";
        }
        else if ($quantifierType === '{}') {
            // outerLast
            throw new Exception('Not implemented');
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

        if ($elementType === 'RuleName') {
            $innerRuleName = $specificElement->getValue();
            $ruleMethodName = $this->generateRuleMethodName($innerRuleName);

            $elementCode = "parsedElement = this->{$ruleMethodName}();";

            $breakCondition = "parsedElement == NULL";

            $addElementStr = "res->addToList(parsedElement);";
        }
        elseif ($elementType === 'InlineRule') {
            $innerRuleName = $this->generateInlineRuleName($ruleName . '_' . ($statementIndex + 1), $inlineRuleIndex);
            $ruleMethodName = $this->generateRuleMethodName($innerRuleName);

            $elementCode = "
            last = res->getListValue().end();
            isInlineParsed = this->{$ruleMethodName}(res);
            if (isInlineParsed == false) {
                res->getListValue().erase(last, res->getListValue().end());
            }
            ";

            $breakCondition = "isInlineParsed == false";

            $addElementStr = "";
        }
        elseif ($elementType === 'StringLiteral') {
            // TODO: keep terminals flag

            list($str, $len) = $this->generateStringValue($specificElement);

            $elementCode = "
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString(\"{$str}\", $len);
            ";

            $breakCondition = "parsedSize == 0";

            $addElementStr = '
            parsedElement = new GdlNode(0, StringDescriptor(streamData, parsedSize));
            res->addToList(parsedElement);
            ';
        }
        elseif ($elementType === 'RegexpLiteral') {
            // TODO: keep terminals flag

            list($str, $len) = $this->generateRegexpValue($specificElement);

            $elementCode = "
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseRegexp(\"{$str}\", $len);
            ";

            $breakCondition = "parsedSize == 0";

            $addElementStr = '
            parsedElement = new GdlNode(0, StringDescriptor(streamData, parsedSize));
            res->addToList(parsedElement);
            ';
        }

        $elementCode = trim($elementCode);
        $addElementStr = trim($addElementStr);


        if ($isInlineRule) $inlineRuleIndex++;

        return [$elementCode, $breakCondition, $addElementStr];
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
