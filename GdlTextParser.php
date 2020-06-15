<?php

namespace Gdl;

class GdlTextParser extends GdlParser
{
    protected $needKeepSpaces = false;

    public function parse(string $mainRuleName, Stream $stream, $checkEof = true)
    {
        $this->stream = $stream;

        $rule = $this->ruleMap[$mainRuleName];
        $ruleFlags = $rule->get('Flags');
        $needKeepSpaces = ($ruleFlags !== null && ($ruleFlags->get('KeepSpaces') !== null || $ruleFlags->get('Lexeme') !== null));

        if (!$needKeepSpaces) {
            $this->skipSpaces();
        }

        return parent::parse($mainRuleName, $stream, $checkEof);
    }

    protected function parseRule(GdlNode $rule)
    {
        $isInline = ($rule->get('RuleName') === null);
        if ($isInline) {
            // keep existing value for needKeepSpaces
            return parent::parseRule($rule);
        }

        $ruleFlags = $rule->get('Flags');
        $prevNeedKeepSpaces = $this->needKeepSpaces;
        $this->needKeepSpaces = ($ruleFlags !== null && ($ruleFlags->get('KeepSpaces') !== null || $ruleFlags->get('Lexeme') !== null));

        $res = parent::parseRule($rule);

        $this->needKeepSpaces = $prevNeedKeepSpaces;

        return $res;
    }

    protected function parseExpression(GdlNode $expression, ?GdlNode $lookAheadElement = null)
    {
        $res = parent::parseExpression($expression, $lookAheadElement);

        if ($res !== null && !$this->needKeepSpaces) {
            $this->skipSpaces();
        }

        return $res;
    }

    public function skipSpaces()
    {
        while (!$this->stream->eof()) {
            $symbol = $this->stream->readSymbol();

            if (! ($symbol === " " || $symbol === "\n" || $symbol === "\t"|| $symbol === "\r")) {
                $this->stream->setPos($this->stream->getPos() - 1);
                break;
            }
        }
    }
}
