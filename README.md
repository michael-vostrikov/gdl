# Grammar Definition Language

Parser for data that matches any given grammar.  
Grammars are written in a special text language.

```
NowdocString:
    '<<<\'' StringTag={setStringStart} '\'' '\n' Content={str} '\n' StringTag={checkStringEnd}
;

Content:
    .*>('\n' StringTag={checkStringEnd})
;

StringTag:
    [a-zA-Z]+
;
```

Engine in PHP version takes 384 lines of code.

Description:  
https://habr.com/ru/post/506188/

There is a parser generator for C++ code in `GdlGenerator.php`. Generated parser works very fast.  
Usage example is in `parser_test.php:testGenerator()`.
