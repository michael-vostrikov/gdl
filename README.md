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

There are PHP and C++ versions.  
Engine in PHP version takes 367 lines of code. Engine in C++ version takes a little more and has acceptable performance (though it is slower than usual well-known parsers).

Description:  
https://habr.com/ru/post/506188/

Also there is a generator for C++ code in a branch [generator](../../tree/generator)
