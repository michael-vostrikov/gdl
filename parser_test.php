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

        $stream->reset();
        $parser2 = new GdlParser($grammar1);
        $t1 = microtime(1);
        $grammar2 = $parser2->parse($mainRuleName, $stream);
        $t2 = microtime(1);
        echo ($t2 - $t1) . "\n";

        $this->assertTrue(!empty($grammar2->getArray('Rule')));

        $this->assertTrue(($grammar1->toArray() === $grammar2->toArray()));
    }
}
