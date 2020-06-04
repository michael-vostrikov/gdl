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

    public function __construct(GdlNode $grammar)
    {
        $this->initRules($grammar);
    }

    public function initRules(GdlNode $grammar)
    {
        $this->ruleMap = [];
        foreach ($grammar->getArray('Rule') as $rule) {
            $this->ruleMap[$rule->get('RuleName')->toString()] = $rule;
        }
    }

    public function parse(string $mainRuleName, Stream $stream)
    {
        $this->stream = $stream;

        $rule = $this->getRule($mainRuleName);
        $result = $this->parseRule($rule);

        return $result;
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
        $ruleNameStr = ($ruleName !== null ? $ruleName->toString() : '()');

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

        return ($parsedRule === null ? null : new GdlNode($ruleNameStr, $parsedRule));
    }

    protected function parseStatement(GdlNode $statement)
    {
        $parsedStatement = [];

        $expressionList = $statement->getArray('Expression');
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
                $parsedStatement = null;
                break;
            }

            if (!empty($parsedExpression)) {
                // skip elements with name started with small letter
                $name = $parsedExpression[0]->getName();  // all parsed elements in expression have same name
                $isSmallLetter = (!empty($name) && ($name[0] >= 'a' && $name[0] <= 'z'));

                if (!$isSmallLetter) {
                    if ($name === '()') {
                        foreach ($parsedExpression as $inlineRule) {
                            $this->addToParsedStatement($parsedStatement, $inlineRule->getValue());
                        }
                    }
                    else {
                        $this->addToParsedStatement($parsedStatement, $parsedExpression);
                    }
                }
            }
        }

        return $parsedStatement;
    }

    protected function addToParsedStatement(array &$parsedStatement, array $parsedExpression)
    {
        foreach ($parsedExpression as $parsedElement) {
            $parsedStatement[] = $parsedElement;
        }
    }

    protected function parseExpression(GdlNode $expression, ?GdlNode $lookAheadElement = null)
    {
        $element = $expression->get('Element');

        $quantifier = $expression->get('Quantifier');
        $quantifierType = null;
        if ($quantifier !== null) {
            $quantifierType = $quantifier->get('')->getValue();
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

            if ($parsedElement !== null) {
                $parsedElementList[] = $parsedElement;
            }
            else {
                $this->stream->setPos($initialElementPos);
                break;
            }

            if ($quantifierType === null || $quantifierType === '?') {
                break;
            }
        }

        $countDoesNotMatch = (($quantifierType === null || $quantifierType === '+') ? empty($parsedElementList) : false);
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
            $rule = $this->getRule($specificElement->toString());
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

        return $parsedElement;
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
            $str = $this->getSymbolStr($symbol);
            if ($contentSymbol !== $str) {
                $parsedValue = null;
                break;
            }

            $parsedValue .= $contentSymbol;
        }

        return ($parsedValue === null ? null : new GdlNode('', $parsedValue));
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
                        $strFrom = $this->getSymbolStr($symbolList[0]);
                        $strTo = $this->getSymbolStr($symbolList[1]);

                        // use build-in PHP string comparison
                        if ($contentSymbol >= $strFrom && $contentSymbol <= $strTo) {
                            $parsedValue = $contentSymbol;
                            break;
                        }
                    }
                    elseif (count($symbolList) === 1) {
                        $str = $this->getSymbolStr($symbolList[0]);

                        if ($contentSymbol === $str) {
                            $parsedValue = $contentSymbol;
                            break;
                        }
                    }
                }
            }
        }

        return ($parsedValue === null ? null : new GdlNode('', $parsedValue));
    }

    protected $escapedSymbols = ['s' => ' ', 't' => "\t", 'r' => "\r", 'n' => "\n"];

    protected function getSymbolStr(GdlNode $symbol)
    {
        $str = null;
        $specificElement = $symbol->getFirst();
        $elementType = $specificElement->getName();

        if ($elementType === 'AnySymbol') {
            $str = $specificElement->get('')->getValue();
        }
        elseif ($elementType === 'EscapedSymbol') {
            $str = $specificElement->get('AnySymbol')->get('')->getValue();
            $str = (isset($this->escapedSymbols[$str]) ? $this->escapedSymbols[$str] : $str);
        }
        elseif ($elementType === 'HexCodeSymbol') {
            list($hexDigit1, $hexDigit2) = $specificElement->getArray('HexDigit');
            $intValue1 = ord($hexDigit1->get('')->getValue()) - 0x30;
            $intValue2 = ord($hexDigit2->get('')->getValue()) - 0x30;
            $intValue1 -= ($intValue1 >= 0x0A ? 0x07 : 0);
            $intValue2 -= ($intValue2 >= 0x0A ? 0x07 : 0);
            $code = $intValue1 * 0x10 + $intValue2;
            $str = chr($code);
        }

        return $str;
    }
}