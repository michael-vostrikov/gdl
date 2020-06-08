grammar Php;

WS:
    [ \t\r\n\u000C]+ -> skip
;

COMMENT:
   '/*' .*? '*/' -> channel(HIDDEN)
;

LINE_COMMENT:
   '//' ~[\r\n]* -> channel(HIDDEN)
;

program:
    '<?php' aPhpProgram
;

aPhpProgram:
    aTopStatement*
;

aTopStatement:
      aNamespaceStatement
    | aUseStatement
    | aClassDeclaration
    | aFunctionDeclaration
    | aStatementList
;

aNamespaceStatement:
    'namespace' aNamespaceName ';'
;

aNamespaceName:
    aNamespacePart ('\\' aNamespacePart)*
;

aNamespacePart:
    ID
;

ID:
    [a-zA-Z_] [a-zA-Z_0-9]*
;

aUseStatement:
    'use' aNamespaceName (',' aNamespaceName)* ';'
;

aClassDeclaration:
    'class' aClassName
        ('extends' aClassReference)?
        ('implements' aInterfaceReference)?
        '{' aClassBody? '}'
;

aClassName:
    ID
;

aClassReference:
    '\\'? aClassName ('\\' aClassName)*
;

aInterfaceReference:
    '\\'? aClassName ('\\' aClassName)*
;

aClassBody:
    aClassBodyStatement+
;

aClassBodyStatement:
      aPropertyDeclaration
    | aMethodDeclaration
;

aPropertyDeclaration:
    aAccessModifier aPropertyName aAssignExpression? ';'
;

aPropertyName:
    aVariableName
;

aVariableName:
    '$' ID
;

aAccessModifier:
      'private'
    | 'protected'
    | 'public'
;

aMethodDeclaration:
    aAccessModifier? aFunctionDeclaration
;

aFunctionDeclaration:
    'function' aFunctionName '(' aArgumentList? ')'
        '{' aFunctionBody? '}'
;

aFunctionName:
    ID
;

aArgumentList:
    aArgument (',' aArgument)*
;

aArgument:
    aTypeReference? aReferenceModifier? aVariableName aAssignExpression?
;

aTypeReference:
    '?'? aClassReference
;

aReferenceModifier:
    '&'
;

aFunctionBody:
    aStatementList
;

aStatementList:
    aStatement+
;

aStatement:
      aIfStatement
    | aForeachStatement
    | aForStatement
    | aReturnStatement
    | aBreakStatement
    | aContinueStatement
    | aWhileStatement
    | aDoWhileStatement
    | aSwitchStatement
    | aThrowStatement
    | aTryStatement
    | aAssignStatement
    | aExpressionStatement
;

aAssignStatement:
    (aList | aUnaryExpression) aAssignExpression ';'
;

aList:
    'list' '(' aExpressionList ')'
;

aAssignExpression:
    ('=' | '.=' | '+=' | '-=' | '*=' | '/=' | '%=') aExpression
;

aExpressionStatement:
    aExpression ';'
;

aVariableReference:
    (aClassReference '::')? (aVariableName | aConstantName)
;

aConstantName:
    ID
;

aAccessList:
    aAccessElement+
;

aAccessElement:
      aMethodCall
    | aPropertyAccess
    | aArrayElementAccess
;

aPropertyAccess:
    '->' ID
;

aMethodCall:
      '->' aFunctionCall
    |  ('++' | '--')
;

aExpressionList:
    aExpression (',' aExpression)*
;

aArrayElementAccess:
    '[' aExpression? ']'
;

aIfStatement:
    'if' '(' aExpression ')' aStatementBlock
        ('elseif' '(' aExpression ')' aStatementBlock)*
        ('else' aStatementBlock)?
;

aStatementBlock:
      aStatement
    | '{' aStatementList? '}'
;

aForStatement:
    'for' '(' aExpression? ';' aExpression? ';' aExpression? ')' aStatementBlock
;

aForeachStatement:
    'foreach' '(' aExpression 'as' aForeachKey? aReferenceModifier? aVariableName ')' aStatementBlock
;

aForeachKey:
    aVariableName '=>'
;

aReturnStatement:
    'return' aExpression ';'
;

aBreakStatement:
    'break' ';'
;

aContinueStatement:
    'continue' ';'
;

aThrowStatement:
    'throw' aExpression ';'
;

aTryStatement:
    'try' '{' aStatementList '}' aCatchStatement?
;

aCatchStatement:
    'catch' '(' aArgument ')' '{' aStatementList '}'
;

aWhileStatement:
    'while' '(' aExpression ')' aStatementBlock
;

aDoWhileStatement:
    'do' aStatementBlock 'while' '(' aExpression ')' ';'
;

aSwitchStatement:
    'switch' '{' aCaseList '}'
;

aCaseList:
    aCase+
;

aCase:
    'case' aExpression ':' aStatementList
;

aExpression:
    aTernaryExpression aAssignExpression?
;

aTernaryExpression:
    aLogicalOrExpression ('?' aExpression ':' aExpression)*
;

aLogicalOrExpression:
    aLogicalAndExpression ('||' aLogicalAndExpression)*
;

aLogicalAndExpression:
    aCompareExpression ('&&' aCompareExpression)*
;

aCompareExpression:
    aNullCoalesceExpression (('===' | '!==' | '==' | '!=' | '>=' | '<=' | '>' | '<') aNullCoalesceExpression)*
;

aNullCoalesceExpression:
    aAddExpression ('??' aAddExpression)*
;

aAddExpression:
    aMulExpression (('+' | '-' | '.' | '&' | '|' | '^') aMulExpression)*
;

aMulExpression:
    aUnaryExpression (('*' | '/' | '%') aUnaryExpression)*
;

aUnaryExpression:
    ('+' | '-' | '!' | '~')? aPrimaryExpression aAccessList?
;

aPrimaryExpression:
      aNewExpression
    | aFunctionCall
    | aVariableReference
    | aLiteral
    | '(' aExpression ')'
;

aNewExpression:
    'new' aClassReference ('(' aExpressionList? ')')?
;

aFunctionCall:
    (aFunctionName | aVariableReference) '(' aExpressionList? ')'
;

aLiteral:
      StringLiteral
    | NumberLiteral
    | aArrayLiteral
    | NullLiteral
    | LogicalLiteral
;

NullLiteral:
    'null'
;

LogicalLiteral:
      'false'
    | 'true'
;

NumberLiteral:
      HexNumberLiteral
    | DecimalNumberLiteral
;

DecimalNumberLiteral:
    [0-9]+ ('.' [0-9]+)?
;

HexNumberLiteral:
    '0x' [0-9A-F]+
;

StringLiteral:
      SingleQuoteString
    | DoubleQuoteString
;

SingleQuoteString:
    '\'' Symbol*? '\''
;

DoubleQuoteString:
    '"' Symbol*? '"'
;

Symbol:
      HexCodeSymbol
    | EscapedSymbol
    | AnySymbol
;

HexCodeSymbol:
    '\\x' HexDigit HexDigit
;

HexDigit:
    [0-9A-F]
;

EscapedSymbol:
    '\\' AnySymbol
;

AnySymbol:
    .
;

aArrayLiteral:
    '[' aArrayElementList? ']'
;

aArrayElementList:
    aArrayElement (',' aArrayElement)* ','?
;

aArrayElement:
    aExpression ('=>' aExpression)?
;
