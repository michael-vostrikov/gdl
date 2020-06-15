<?php

namespace Gdl;

use Exception;

/**
 * Grammar Definition Language parser
 */
class GdlParser
{
    /** @var GdlNode[]  Keys are rule names */
    protected $ruleMap;

    /** @var Stream */
    protected $stream;

    protected $errors = [];
    protected $ruleCallStack = [];

    protected $lexemeLevel = 0;

    public function __construct(GdlNode $grammar)
    {
        $this->initRules($grammar);
    }

    public function initRules(GdlNode $grammar)
    {
        $this->ruleMap = [];
        foreach ($grammar->getArray('Rule') as $rule) {
            $this->ruleMap[$rule->get('RuleName')->getValue()] = $rule;
        }
    }

    public function parse(string $mainRuleName, Stream $stream, $checkEof = true)
    {
        $this->stream = $stream;
        $this->errors = [];
        $this->ruleCallStack = [];

        $rule = $this->getRule($mainRuleName);
        $result = $this->parseRule($rule);

        if ($checkEof && !$this->stream->eof()) {
            $this->errors[] = 'Unexpected data at ' . implode(':', $this->stream->getLineAndColumn()) . ' (' . $this->stream->getPos() . ')';
        }

        if ($result === null) {
            $this->errors[] = 'Parsing failed';
        }

        return $result;
    }

    public function getErrors()
    {
        return $this->errors;
    }

    public function getRule(string $ruleName)
    {
        if (!isset($this->ruleMap[$ruleName])) {
            throw new Exception('Unknown rule: ' . $ruleName);
        }

        return $this->ruleMap[$ruleName];
    }

    protected function parseRule(GdlNode $rule)
    {
        $ruleName = $rule->get('RuleName');
        $ruleNameStr = ($ruleName !== null ? $ruleName->getValue() : '()');
        $this->ruleCallStack[] = [$ruleNameStr, $this->stream->getPos()];

        $ruleFlags = $rule->get('Flags');
        $isLexeme = ($ruleFlags !== null && $ruleFlags->get('Lexeme') !== null);
        if ($isLexeme) $this->lexemeLevel++;

        $parsedRule = null;

        $statementList = $rule->get('RuleBody')->getArray('Statement');
        $initialPos = $this->stream->getPos();
        foreach ($statementList as $statement) {
            $parsedRule = $this->parseStatement($statement);
            if ($parsedRule !== null) {
                break;
            }

            // try parse next variant from same position
            $this->stream->setPos($initialPos);
        }

        array_pop($this->ruleCallStack);

        if ($isLexeme) $this->lexemeLevel--;

        return ($parsedRule === null ? null : ($this->lexemeLevel > 0 ? $parsedRule : new GdlNode($ruleNameStr, $parsedRule)));
    }

    protected function parseStatement(GdlNode $statement)
    {
        $parsedStatement = [];

        $expressionList = $statement->getArray('Expression');
        $cut = false;
        foreach ($expressionList as $i => $expression) {
            $lookAheadElement = null;
            $lookAhead = $expression->get('LookAhead');
            if ($lookAhead !== null) {
                if ($lookAhead->get('Element') !== null) {
                    $lookAheadElement = $lookAhead->get('Element');
                }
                elseif (isset($expressionList[$i + 1])) {
                    $lookAheadElement = $expressionList[$i + 1]->get('Element');
                }
            }

            $parsedExpression = $this->parseExpression($expression, $lookAheadElement);
            if ($parsedExpression === null) {
                if ($cut) {
                    $this->handleError($expression);
                    continue;
                }
                else {
                    $parsedStatement = null;
                    break;
                }
            }

            if ($expression->get('Cut') !== null) {
                $cut = true;
            }

            if (!empty($parsedExpression)) {
                $specificElement = $expression->get('Element')->getFirst();
                $elementType = $specificElement->getName();

                if ($elementType === "InlineRule") {
                    foreach ($parsedExpression as $inlineRule) {
                        $this->addToParsedStatement($parsedStatement, ($this->lexemeLevel > 0 ? [$inlineRule] : $inlineRule->getValue()));
                    }
                }
                else {
                    $needSkip = false;
                    if ($elementType === "RuleName") {
                        $name = $specificElement->getValue();
                        $ruleFlags = $this->ruleMap[$name]->get('Flags');
                        $needSkip = ($ruleFlags !== null && $ruleFlags->get('Skip') !== null);
                    }

                    if (!$needSkip) {
                        $this->addToParsedStatement($parsedStatement, $parsedExpression);
                    }
                }
            }
        }

        return ($this->lexemeLevel > 0 && $parsedStatement !== null ? implode('', $parsedStatement) : $parsedStatement);
    }

    protected function addToParsedStatement(array &$parsedStatement, $parsedExpression)
    {
        foreach ($parsedExpression as $parsedElement) {
            $parsedStatement[] = $parsedElement;
        }
    }

    protected function handleError(GdlNode $expression)
    {
        $elementType = $expression->get('Element')->getFirst()->getName();
        if ($elementType === 'StringLiteral' || $elementType === 'RegexpLiteral') {
            $name = $expression->toString();
        }
        else {
            $name = $expression->get('Element')->getFirst()->toString();
        }

        $i = count($this->ruleCallStack) - 1;
        while ($this->ruleCallStack[$i][0] == '()') { $i--; }
        $ruleInfo = $this->ruleCallStack[$i];

        $this->errors[] = 'Expected ' . $name . ' at ' . implode(':', $this->stream->getLineAndColumn()) . ' (' . $this->stream->getPos() . ')'
            . ' in ' . $ruleInfo[0] . ' at ' . implode(':', $this->stream->getLineAndColumn($ruleInfo[1])) . ' (' . $ruleInfo[1] . ')';
    }

    protected function parseExpression(GdlNode $expression, ?GdlNode $lookAheadElement = null)
    {
        $element = $expression->get('Element');

        $elementFunctionNameList = null;
        if ($expression->get('FunctionCall') !== null) {
            foreach ($expression->get('FunctionCall')->getArray('FunctionName') as $functionName) {
                $elementFunctionNameList[] = $functionName->getValue();
            }
        }

        $quantifier = $expression->get('Quantifier');
        $quantifierType = null;
        $countVal = 0;
        if ($quantifier !== null) {
            $count = $quantifier->get('Count');
            if ($count === null) {
                $quantifierType = $quantifier->getFirst()->getValue();
            }
            else {
                $quantifierType = '{}';
                if ($count->get('IntValue') !== null) {
                    $countVal = intval($count->get('IntValue')->getValue());
                }
                elseif ($count->get('FunctionCall') !== null) {
                    $countFunctionName = $count->get('FunctionCall')->get('FunctionName')->getValue();
                    $countVal = $this->$countFunctionName();
                }
            }
        }

        $parsedElementList = [];
        while (true) {
            $initialElementPos = $this->stream->getPos();

            $parsedLookAheadElement = null;
            if ($lookAheadElement !== null) {
                $parsedLookAheadElement = $this->parseElement($lookAheadElement);
                $this->stream->setPos($initialElementPos);

                if ($parsedLookAheadElement !== null) {
                    break;
                }
            }

            $parsedElement = $this->parseElement($element);
            if ($elementFunctionNameList !== null && $parsedElement !== null) {
                foreach ($elementFunctionNameList as $elementFunctionName) {
                    $this->$elementFunctionName($parsedElement);
                    if ($parsedElement === null) {
                        break;
                    }
                }
            }

            if ($parsedElement !== null) {
                $parsedElementList[] = $parsedElement;
            }
            else {
                $this->stream->setPos($initialElementPos);
                break;
            }

            if ($quantifierType === null || $quantifierType === '?' || $quantifierType === '{}' && count($parsedElementList) === $countVal) {
                break;
            }
        }

        $countDoesNotMatch = (($quantifierType === null || $quantifierType === '+') ? count($parsedElementList) === 0
            : (($quantifierType === '{}') ? count($parsedElementList) !== $countVal : false)
        );
        if ($countDoesNotMatch) {
            $parsedElementList = null;
        }

        return $parsedElementList;
    }

    protected function parseElement(GdlNode $element)
    {
        $specificElement = $element->getFirst();
        $elementType = $specificElement->getName();

        if ($elementType === 'RuleName') {
            $rule = $this->getRule($specificElement->getValue());
            $parsedElement = $this->parseRule($rule);
        }
        elseif ($elementType === 'StringLiteral') {
            $parsedElement = $this->parseStringLiteral($specificElement);
        }
        elseif ($elementType === 'RegexpLiteral') {
            $parsedElement = $this->parseRegexpLiteral($specificElement);
        }
        elseif ($elementType === 'InlineRule') {
            $parsedElement = $this->parseRule($specificElement);
        }
        else {
            throw new Exception('Unknown element type: ' . $elementType);
        }

        if (is_string($parsedElement) && $this->lexemeLevel === 0) {
            return new GdlNode('', $parsedElement);
        }

        return $parsedElement;
    }

    protected function str(GdlNode &$parsedElement)
    {
        $parsedElement->setValue($parsedElement->toString());
    }

    protected function symbolStr(GdlNode &$parsedElement)
    {
        $parsedElement->setValue($this->getSymbolStr($parsedElement));
    }

    protected function parseStringLiteral(GdlNode $element)
    {
        $parsedValue = null;

        $symbolList = $element->getArray('Symbol');
        foreach ($symbolList as $symbol) {
            if ($this->stream->eof()) {
                $parsedValue = null;
                break;
            }

            $contentSymbol = $this->stream->readSymbol();
            $str = $symbol->getValue();
            if ($contentSymbol !== $str) {
                $parsedValue = null;
                break;
            }

            $parsedValue .= $contentSymbol;
        }

        return ($parsedValue === null ? null : $parsedValue);
    }

    protected function parseRegexpLiteral(GdlNode $element)
    {
        $parsedValue = null;

        if (!$this->stream->eof()) {
            $contentSymbol = $this->stream->readSymbol();
            if ($element->get('AnySymbolLiteral') !== null) {
                $parsedValue = $contentSymbol;
            }
            else {
                $symbolRangeList = $element->getArray('SymbolRange');
                foreach ($symbolRangeList as $symbolRange) {
                    $symbolList = $symbolRange->getArray('Symbol');

                    if (count($symbolList) === 2) {
                        $strFrom = $symbolList[0]->getValue();
                        $strTo = $symbolList[1]->getValue();

                        // use build-in PHP string comparison
                        if ($contentSymbol >= $strFrom && $contentSymbol <= $strTo) {
                            $parsedValue = $contentSymbol;
                            break;
                        }
                    }
                    elseif (count($symbolList) === 1) {
                        $str = $symbolList[0]->getValue();

                        if ($contentSymbol === $str) {
                            $parsedValue = $contentSymbol;
                            break;
                        }
                    }
                }
            }
        }

        return ($parsedValue === null ? null : $parsedValue);
    }

    protected $escapedSymbols = ['s' => ' ', 't' => "\t", 'r' => "\r", 'n' => "\n"];

    protected function getSymbolStr(GdlNode $symbol)
    {
        $str = null;
        $specificElement = $symbol->getFirst();
        $elementType = $specificElement->getName();

        if ($elementType === 'AnySymbol') {
            $str = $specificElement->getValue();
        }
        elseif ($elementType === 'EscapedSymbol') {
            $str = $specificElement->get('AnySymbol')->getValue();
            $str = (isset($this->escapedSymbols[$str]) ? $this->escapedSymbols[$str] : $str);
        }
        elseif ($elementType === 'HexCodeSymbol') {
            list($hexDigit1, $hexDigit2) = $specificElement->getArray('HexDigit');
            $intValue1 = ord($hexDigit1->getValue()) - 0x30;
            $intValue2 = ord($hexDigit2->getValue()) - 0x30;
            $intValue1 -= ($intValue1 >= 0x0A ? 0x07 : 0);
            $intValue2 -= ($intValue2 >= 0x0A ? 0x07 : 0);
            $code = $intValue1 * 0x10 + $intValue2;
            $str = chr($code);
        }

        return $str;
    }
}
