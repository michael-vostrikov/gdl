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
        foreach ($languageGrammar->getArray('Rule') as $rule) {
            $rule->get('RuleName')->setValue($rule->get('RuleName')->toString());
        }

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
    (Word '\n')*
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


        $mainRuleName = $languageGrammar->get('Rule')->get('RuleName')->toString();
        $languageParser = new GdlParser($languageGrammar);

        $source = <<<'SRC'
тест
тест
абв

SRC;
        $tree = $languageParser->parse($mainRuleName, new Stream($source));
        foreach ($tree->getArray('Word') as $word) {
            $word->setValue($word->toString());
        }

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
        foreach ($tree->getArray('Word') as $word) {
            $word->setValue($word->toString());
        }

        $this->assertTrue($languageParser->getErrors() === [
            'Unexpected data at 2:1 (9)',
        ]);
        $this->assertTrue($tree->toArray() === ['Data', [
            ['Word', 'тест'],
            ['', "\n"],
        ]]);
    }
}
