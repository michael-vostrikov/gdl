<?php

namespace Gdl;

class GdlTextParser extends GdlParser
{
    protected $keepSpaces = false;
    protected $keepSpacesLevel = 0;

    protected function setKeepSpaces(GdlNode &$element)
    {
        $this->keepSpaces = true;
        $this->keepSpacesLevel = count($this->ruleCallStack);
    }

    protected function clearKeepSpaces(GdlNode &$element)
    {
        $this->keepSpaces = false;
        $this->keepSpacesLevel = 0;
    }

    public function parse(string $mainRuleName, Stream $stream, $checkEof = true)
    {
        $this->stream = $stream;
        $this->skipSpaces();

        return parent::parse($mainRuleName, $stream, $checkEof);
    }

    protected function parseStatement(GdlNode $statement)
    {
        $parsedStatement = parent::parseStatement($statement);

        // auto-clear flag on statement fail
        if ($this->keepSpaces && $parsedStatement === null && count($this->ruleCallStack) <= $this->keepSpacesLevel) {
            $this->keepSpaces = false;
            $this->keepSpacesLevel = 0;
        }

        return $parsedStatement;
    }

    protected function parseExpression(GdlNode $element, ?GdlNode $lookAheadElement = null)
    {
        $res = parent::parseExpression($element, $lookAheadElement);

        if ($res !== null && !$this->keepSpaces) {
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
