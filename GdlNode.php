<?php

namespace Gdl;

class GdlNode
{
    /** @var string */
    protected $name;

    /** @var static[]|string */
    protected $value;

    protected $hashValue = [];

    public function __construct($name, $value)
    {
        $this->name = $name;
        $this->value = $value;

        $this->init();
    }

    public function init()
    {
        if (is_array($this->value)) {
            foreach ($this->value as &$element) {
                $this->hashValue[$element->getName()][] = $element;
            }
            unset($element);
        }
    }

    public function getName()
    {
        return $this->name;
    }

    public function getValue()
    {
        return $this->value;
    }

    public function setValue($value)
    {
        $this->value = $value;
        $this->init();
    }

    public function get($name): ?GdlNode
    {
        return $this->hashValue[$name][0] ?? null;
    }

    public function getFirst()
    {
        return $this->value[0];
    }

    /**
     * @param string $name
     * @return GdlNode[]
     */
    public function getArray($name): array
    {
        return $this->hashValue[$name] ?? [];
    }

    public function toString()
    {
        if (is_null($this->value)) {
            return null;
        }

        if (is_string($this->value)) {
            return $this->value;
        }

        $str = [];
        foreach ($this->value as $n => $item) {
            $str[] = $item->toString();
        }

        return implode('', $str);
    }

    public function toArray()
    {
        $name = $this->getName();
        $value = [];

        if (is_array($this->value)) {
            foreach ($this->value as $element) {
                $elementRes = $element->toArray();
                $value[] = $elementRes;
            }
        }
        else {
            $value = $this->value;
        }

        return [$name, $value];
    }
}
