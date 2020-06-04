<?php

namespace Gdl;

class Stream
{
    protected $content;
    protected $pos;

    public function __construct(string $content)
    {
        $this->content = $content;
        $this->reset();
    }

    public function reset()
    {
        $this->pos = 0;
    }

    public function getContent()
    {
        return $this->content;
    }

    public function getPos(): int
    {
        return $this->pos;
    }

    public function setPos(int $pos)
    {
        $pos = max(0, min($pos, strlen($this->content)));
        $this->pos = $pos;
    }

    public function readSymbol(): ?string
    {
        if ($this->pos >= strlen($this->content)) {
            return null;
        }

        $symbol = $this->content[$this->pos];
        $this->pos++;

        return $symbol;
    }

    /**
     * Note that you should check on eof before read
     * @return bool
     */
    public function eof(): bool
    {
        return ($this->pos === strlen($this->content));
    }

    public function getLineAndColumn($pos = null): array
    {
        $line = 1;
        $column = 1;

        if ($pos === null) {
            $pos = $this->pos;
        }

        for ($i = 0; $i < $pos; $i++) {
            if ($this->content[$i] === "\n") {
                $line++;
                $column = 1;
            }
            elseif ($this->content[$i] === "\t") {
                $column += 4;
            }
            else {
                $column++;
            }
        }

        return [$line, $column];
    }
}
