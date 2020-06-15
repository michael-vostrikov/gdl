<?php

namespace Gdl;

use Exception;
use ReflectionClass;
use ReflectionMethod;

require_once('autoload.php');

$app = new GdlParserTestApp();
$app->run($argv);
return;

class GdlParserTestApp
{
    public function run($argv)
    {
        $filterMethod = $argv[1] ?? null;

        $class = new ReflectionClass($this);
        $methods = $class->getMethods();
        $methods = array_filter($methods, function (ReflectionMethod $method) use ($filterMethod) {
            $methodName = $method->getName();
            if (substr($methodName, 0, 4) === 'test') {
                if ($filterMethod === null || $filterMethod === $methodName) {
                    return true;
                }
            }
            return false;
        });

        if (empty($methods)) {
            echo 'No tests found' . "\n";
            return;
        }

        $isAllSuccess = true;
        foreach ($methods as $method) {
            $method = $method->getName();
            $res = true;
            try {
                $this->$method();
            }
            catch (Exception $e) {
                echo $e;
                $res = false;
            }

            echo ($res === false ? 'x' : 'v') . '   ' . $method . "\n";
            echo '------------------------------------------------------------------------' . "\n";

            if ($res === false) {
                $isAllSuccess = false;
            }
        }
        echo ($isAllSuccess === true ? 'OK' : 'Some tests failed') . "\n";
    }

    public function getSelfGrammar()
    {
        $grammarData = include(__DIR__ . '/self.php');
        return $this->createNode($grammarData[0], $grammarData[1]);
    }

    public function createNode($name, $value)
    {
        if (is_array($value)) {
            foreach ($value as &$element) {
                if (! $element instanceof GdlNode) {
                    $element = $this->createNode($element[0], $element[1]);
                }
            }
            unset($element);
        }

        $node = new GdlNode($name, $value);

        return $node;
    }

    public function assertTrue($res, $message = 'Assert false is true')
    {
        if (!$res) {
            throw new Exception($message);
        }
    }

    public function assertEmpty($value, $message = 'Assert not empty is empty')
    {
        if (!empty($value)) {
            dump($value);
            throw new Exception($message);
        }
    }

    /**
     * Export parser result tree as string with PHP array literal definition, needed to prepare data for tests
     * @param array $data
     * @param int $level
     * @return string
     */
    public function exportTree($data, $level = 1)
    {
        $str = [];
        $indent = str_repeat('    ', $level - 1);
        if (is_array($data[1])) {
            $str[] = $indent . '[' . $this->exportStr($data[0]) . ', [';
            foreach ($data[1] as $value) {
                $str[] = $this->exportTree($value, $level + 1);
            }

            $str[] = $indent . ']],';
        }
        elseif (is_string($data[1])) {
            $str[] = $indent . '[' . $this->exportStr($data[0]) . ', '
                . $this->exportStr($data[1]) . '],';
        }

        return implode("\n", $str) . ($level === 1 ? "\n" : '');
    }

    public function exportStr($str)
    {
        $str = str_replace("'", "\\'", $str);
        $str2 = str_replace("\n", "\\n", $str);
        if ($str2 !== $str) {
            $str = '"' . $str2 . '"';
        }
        else {
            $str = "'" . $str2 . "'";
        }
        return $str;
    }

    public function testSelfParsing()
    {
        $selfGdlFilename = __DIR__ . '/self.gdl';
        $content = file_get_contents($selfGdlFilename);
        $stream = new Stream($content);
        $mainRuleName = 'Grammar';

        $stream->reset();
        $parser = new GdlParser($this->getSelfGrammar());
        $t1 = microtime(1);
        $grammar1 = $parser->parse($mainRuleName, $stream);
        $t2 = microtime(1);
        echo ($t2 - $t1) . "\n";

        $this->assertTrue(!empty($grammar1->getArray('Rule')));
        $this->assertEmpty($parser->getErrors());

        $stream->reset();
        $parser2 = new GdlParser($grammar1);
        $t1 = microtime(1);
        $GLOBALS['test'] = 1;
        $grammar2 = $parser2->parse($mainRuleName, $stream);
        $t2 = microtime(1);
        echo ($t2 - $t1) . "\n";

        $this->assertTrue(!empty($grammar2->getArray('Rule')));
        $this->assertEmpty($parser2->getErrors());

        $this->assertTrue(($grammar1->toArray() === $grammar2->toArray()));
    }

    public function testParsingErrors()
    {
        $mainRuleName = 'Grammar';
        $gdlParser = new GdlParser($this->getSelfGrammar());

        $grammarSource = <<<'SRC'
Program:
;

Test:;

SRC;
        $languageGrammar = $gdlParser->parse($mainRuleName, new Stream($grammarSource));

        $this->assertTrue($languageGrammar->toArray() === ['Grammar', [
            ['Rule', [
                ['RuleName', 'Program'],
                ['', ':'],
                ['', ';'],
            ]],
            ['Rule', [
                ['RuleName', 'Test'],
                ['', ':'],
                ['', ';'],
            ]],
        ]]);
        $this->assertTrue($gdlParser->getErrors() === [
            'Expected RuleBody at 2:1 (9) in Rule at 1:1 (0)',
            'Expected RuleBody at 4:6 (17) in Rule at 4:1 (12)',
        ]);


        $grammarSource = <<<'SRC'
SRC;
        $languageGrammar = $gdlParser->parse($mainRuleName, new Stream($grammarSource));

        $this->assertTrue($languageGrammar->toArray() === ['Grammar', []]);
        $this->assertEmpty($gdlParser->getErrors());
    }

    public function testUtf8()
    {
        $mainRuleName = 'Grammar';
        $gdlParser = new GdlParser($this->getSelfGrammar());

        $grammarSource = <<<'SRC'
Data:
    (Word={str} '\n')*
;

Word:
    Utf8RussianLetter+
;

Utf8RussianLetter:
    | [\xD0][\x90-\xBF]  /* А-Яа-п */
    | [\xD1][\x80-\x8F]  /* р-я */
    | [\xD0][\x01]       /* Ё */
    | [\xD1][\x91]       /* ё */
;
SRC;
        $languageGrammar = $gdlParser->parse($mainRuleName, new Stream($grammarSource));

        $this->assertEmpty($gdlParser->getErrors());


        $mainRuleName = $languageGrammar->get('Rule')->get('RuleName')->getValue();
        $languageParser = new GdlParser($languageGrammar);

        $source = <<<'SRC'
тест
тест
абв

SRC;
        $tree = $languageParser->parse($mainRuleName, new Stream($source));

        $this->assertEmpty($languageParser->getErrors());
        $this->assertTrue($tree->toArray() === ['Data', [
            ['Word', 'тест'],
            ['', "\n"],
            ['Word', 'тест'],
            ['', "\n"],
            ['Word', 'абв'],
            ['', "\n"],
        ]]);


        $source = <<<'SRC'
тест
test
123

SRC;
        $tree = $languageParser->parse($mainRuleName, new Stream($source));

        $this->assertTrue($languageParser->getErrors() === [
            'Unexpected data at 2:1 (9)',
        ]);
        $this->assertTrue($tree->toArray() === ['Data', [
            ['Word', 'тест'],
            ['', "\n"],
        ]]);
    }

    public function testCount()
    {
        $mainRuleName = 'Grammar';
        $gdlParser = new GdlParser($this->getSelfGrammar());

        $grammarSource = <<<'SRC'
Data:
    Count={str,setCount} '\n' Element={str}{={getCount}}
;

Count:
    [0-9]{4}
;

Element:
    [a-zA-Z0-9]+ '\n'
;
SRC;
        $languageGrammar = $gdlParser->parse($mainRuleName, new Stream($grammarSource));

        $this->assertEmpty($gdlParser->getErrors());


        $mainRuleName = $languageGrammar->get('Rule')->get('RuleName')->getValue();

        $languageParser = new class($languageGrammar) extends GdlParser {
            protected $cnt;

            public function setCount(GdlNode &$parsedElement)
            {
                $this->cnt = intval($parsedElement->getValue());
            }

            public function getCount(): int
            {
                return $this->cnt;
            }
        };


        $source = <<<'SRC'
0006
Element1
Element2
Element3
Element4
Element5
Element6

SRC;
        $tree = $languageParser->parse($mainRuleName, new Stream($source));

        $this->assertEmpty($languageParser->getErrors());
        $this->assertTrue($tree->toArray() === ['Data', [
            ['Count', '0006'],
            ['', "\n"],
            ['Element', "Element1\n"],
            ['Element', "Element2\n"],
            ['Element', "Element3\n"],
            ['Element', "Element4\n"],
            ['Element', "Element5\n"],
            ['Element', "Element6\n"],
        ]]);


        $source = <<<'SRC'
0006
Element1
Element2
Element3

SRC;
        $tree = $languageParser->parse($mainRuleName, new Stream($source));

        $this->assertTrue($tree === null);
        $this->assertTrue($languageParser->getErrors() === [
            'Unexpected data at 1:1 (0)',
            'Parsing failed',
        ]);


        $source = <<<'SRC'
0006
Element1
Element2
Element3
Element4
Element5
Element6
Element7

SRC;
        $tree = $languageParser->parse($mainRuleName, new Stream($source));

        $this->assertTrue($tree->toArray() === ['Data', [
            ['Count', '0006'],
            ['', "\n"],
            ['Element', "Element1\n"],
            ['Element', "Element2\n"],
            ['Element', "Element3\n"],
            ['Element', "Element4\n"],
            ['Element', "Element5\n"],
            ['Element', "Element6\n"],
        ]]);
        $this->assertTrue($languageParser->getErrors() === [
            'Unexpected data at 8:1 (59)',
        ]);
    }

    public function testComplexString()
    {
        $mainRuleName = 'Grammar';
        $gdlParser = new GdlParser($this->getSelfGrammar());

        $grammarSource = <<<'SRC'
Data:
    (ComplexString '\n')*
;

ComplexString:
    '<<<\'' StringTag={str,setStringStart} '\'' '\n' Content={str} '\n' StringTag={str,checkStringEnd}
;

Content:
    .*>('\n' StringTag={str,checkStringEnd})
;

StringTag:
    [a-zA-Z]+
;
SRC;
        $languageGrammar = $gdlParser->parse($mainRuleName, new Stream($grammarSource));

        $this->assertEmpty($gdlParser->getErrors());


        $mainRuleName = $languageGrammar->get('Rule')->get('RuleName')->getValue();

        $languageParser = new class($languageGrammar) extends GdlParser {
            protected $complexStringTag;

            public function setStringStart(GdlNode &$parsedElement)
            {
                $this->complexStringTag = $parsedElement->getValue();
            }

            public function checkStringEnd(GdlNode &$parsedElement)
            {
                if ($parsedElement->getValue() !== $this->complexStringTag) {
                    $parsedElement = null;
                }
            }
        };


        $source = <<<'SRC'
<<<'test'
<<<'abc'
a b c
abc
test
<<<'tag'
1 2 3 4
tag

SRC;
        $tree = $languageParser->parse($mainRuleName, new Stream($source));

        $this->assertEmpty($languageParser->getErrors());
        $this->assertTrue($tree->toArray() === ['Data', [
            ['ComplexString', [
                ['', '<<<\''],
                ['StringTag', 'test'],
                ['', '\''],
                ['', "\n"],
                ['Content', "<<<'abc'\na b c\nabc"],
                ['', "\n"],
                ['StringTag', 'test'],
            ]],
            ['', "\n"],
            ['ComplexString', [
                ['', '<<<\''],
                ['StringTag', 'tag'],
                ['', '\''],
                ['', "\n"],
                ['Content', "1 2 3 4"],
                ['', "\n"],
                ['StringTag', 'tag'],
            ]],
            ['', "\n"],
        ]]);
    }

    public function testPhp()
    {
        $mainRuleName = 'Grammar';
        $gdlParser = new GdlParser($this->getSelfGrammar());

        $grammarSource = file_get_contents(__DIR__ . '/php.gdl');
        $stream = new Stream($grammarSource);

        $t1 = microtime(1);
        $languageGrammar = $gdlParser->parse($mainRuleName, $stream);
        $t2 = microtime(1);
        echo ($t2 - $t1) . "\n";

        $this->assertEmpty($gdlParser->getErrors());


        $mainRuleName = $languageGrammar->get('Rule')->get('RuleName')->getValue();

        $languageParser = new class($languageGrammar) extends GdlTextParser {
            protected $complexStringTag;

            public function setStringStart(GdlNode &$parsedElement)
            {
                $this->complexStringTag = $parsedElement->getValue();
            }

            public function checkStringEnd(GdlNode &$parsedElement)
            {
                if ($parsedElement->getValue() !== $this->complexStringTag) {
                    $parsedElement = null;
                }
            }

            protected function handleError(GdlNode $expression)
            {
                parent::handleError($expression);

                while (!$this->stream->eof()) {
                    $symbol = $this->stream->readSymbol();
                    if ($symbol === "\n") {
                        break;
                    }
                }

                $i = count($this->ruleCallStack) - 1;
                while ($this->ruleCallStack[$i][0] == '()') { $i--; }
                $ruleName = $this->ruleCallStack[$i][0];

                if ($ruleName === 'FunctionDeclaration' || $ruleName === 'StatementBlock') {
                    $level = 1;
                    while (!$this->stream->eof()) {
                        $symbol = $this->stream->readSymbol();
                        if ($symbol === '{') {
                            $level++;
                        }
                        else if ($symbol === '}') {
                            $level--;
                            if ($level == 0) {
                                break;
                            }
                        }
                    }
                }
            }
        };

        $source = file_get_contents(__DIR__ . '/GdlParser.php');

        $t1 = microtime(1);
        $tree = $languageParser->parse($mainRuleName, new Stream($source));
        $t2 = microtime(1);
        echo ($t2 - $t1) . "\n";

        $this->assertEmpty($languageParser->getErrors());
        $this->assertTrue(!empty($tree));
    }

    public function testGenerator()
    {
        $mainRuleName = 'Grammar';
        $gdlParser = new GdlParser($this->getSelfGrammar());

        $grammarSource = file_get_contents(__DIR__ . '/json.gdl');
        $stream = new Stream($grammarSource);

        $languageGrammar = $gdlParser->parse($mainRuleName, $stream);
        $this->assertEmpty($gdlParser->getErrors());
        $this->assertTrue(!empty($languageGrammar));

        $generator = new GdlGenerator($languageGrammar, "Json");
        $generator->generateFile(__DIR__ . '/cpp/JsonParser.h');
    }
}
