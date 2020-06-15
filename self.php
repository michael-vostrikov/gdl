<?php

$selfGrammar = ['Grammar', [
    ['Rule', [
        ['RuleName', 'Grammar'],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'delimiter'],
                    ]],
                    ['Quantifier', [['', '*']]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'Rule'],
                    ]],
                    ['Quantifier', [['', '*']]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'delimiter'],
        ['Flags', [
            ['Skip', '-'],
            ['Lexeme', 'L'],
        ]],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'space'],
                    ]],
                ]],
            ]],
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'comment'],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'space'],
        ['Flags', [
            ['Skip', '-'],
            ['Lexeme', 'L'],
        ]],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RegexpLiteral', [
                            ['SymbolRange', [
                                ['Symbol', " "],
                            ]],
                            ['SymbolRange', [
                                ['Symbol', "\n"],
                            ]],
                            ['SymbolRange', [
                                ['Symbol', "\t"],
                            ]],
                            ['SymbolRange', [
                                ['Symbol', "\r"],
                            ]],
                        ]],
                    ]],
                    ['Quantifier', [['', '+']]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'comment'],
        ['Flags', [
            ['Skip', '-'],
            ['Lexeme', 'L'],
        ]],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '/'],
                            ['Symbol', '*'],
                        ]],
                    ]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'AnySymbol'],
                    ]],
                    ['Quantifier', [['', '*']]],
                    ['LookAhead', [['', '>>']]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '*'],
                            ['Symbol', '/'],
                        ]],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'Rule'],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'RuleName'],
                    ]],
                    ['Cut', '!'],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'Flags'],
                    ]],
                    ['Quantifier', [['', '?']]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'delimiter'],
                    ]],
                    ['Quantifier', [['', '*']]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', ':'],
                        ]],
                    ]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'delimiter'],
                    ]],
                    ['Quantifier', [['', '*']]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'RuleBody'],
                    ]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', ';'],
                        ]],
                    ]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'delimiter'],
                    ]],
                    ['Quantifier', [['', '*']]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'RuleName'],
        ['Flags', [
            ['Lexeme', 'L'],
        ]],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'ID'],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'ID'],
        ['Flags', [
            ['Lexeme', 'L'],
        ]],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RegexpLiteral', [
                            ['SymbolRange', [
                                ['Symbol', 'a'],
                                ['Symbol', 'z'],
                            ]],
                            ['SymbolRange', [
                                ['Symbol', 'A'],
                                ['Symbol', 'Z'],
                            ]],
                            ['SymbolRange', [
                                ['Symbol', '_'],
                            ]],
                        ]],
                    ]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RegexpLiteral', [
                            ['SymbolRange', [
                                ['Symbol', 'a'],
                                ['Symbol', 'z'],
                            ]],
                            ['SymbolRange', [
                                ['Symbol', 'A'],
                                ['Symbol', 'Z'],
                            ]],
                            ['SymbolRange', [
                                ['Symbol', '_'],
                            ]],
                            ['SymbolRange', [
                                ['Symbol', '0'],
                                ['Symbol', '9'],
                            ]],
                        ]],
                    ]],
                    ['Quantifier', [['', '*']]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'Flags'],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '('],
                        ]],
                    ]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'Skip'],
                    ]],
                    ['Quantifier', [['', '?']]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['InlineRule', [
                            ['RuleBody', [
                                ['Statement', [
                                    ['Expression', [
                                        ['Element', [
                                            ['RuleName', 'Lexeme'],
                                        ]],
                                    ]],
                                ]],
                                ['Statement', [
                                    ['Expression', [
                                        ['Element', [
                                            ['RuleName', 'KeepSpaces'],
                                        ]],
                                    ]],
                                ]],
                            ]],
                        ]],
                    ]],
                    ['Quantifier', [['', '?']]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', ')'],
                        ]],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'Lexeme'],
        ['Flags', [
            ['Lexeme', 'L'],
        ]],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', 'L'],
                        ]],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'KeepSpaces'],
        ['Flags', [
            ['Lexeme', 'L'],
        ]],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', 'S'],
                        ]],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'Skip'],
        ['Flags', [
            ['Lexeme', 'L'],
        ]],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '-'],
                        ]],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'RuleBody'],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '|'],
                        ]],
                    ]],
                    ['Quantifier', [['', '?']]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'delimiter'],
                    ]],
                    ['Quantifier', [['', '*']]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'Statement'],
                    ]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['InlineRule', [
                            ['RuleBody', [
                                ['Statement', [
                                    ['Expression', [
                                        ['Element', [
                                            ['StringLiteral', [
                                                ['Symbol', '|'],
                                            ]],
                                        ]],
                                    ]],
                                    ['Expression', [
                                        ['Element', [
                                            ['RuleName', 'delimiter'],
                                        ]],
                                        ['Quantifier', [['', '*']]],
                                    ]],
                                    ['Expression', [
                                        ['Element', [
                                            ['RuleName', 'Statement'],
                                        ]],
                                    ]],
                                ]],
                            ]],
                        ]],
                    ]],
                    ['Quantifier', [['', '*']]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'Statement'],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'Expression'],
                    ]],
                    ['Quantifier', [['', '+']]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'Expression'],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'Element'],
                    ]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'FunctionCall'],
                    ]],
                    ['Quantifier', [['', '?']]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'Quantifier'],
                    ]],
                    ['Quantifier', [['', '?']]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'LookAhead'],
                    ]],
                    ['Quantifier', [['', '?']]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'Cut'],
                    ]],
                    ['Quantifier', [['', '?']]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'delimiter'],
                    ]],
                    ['Quantifier', [['', '*']]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'Element'],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'RuleName'],
                    ]],
                ]],
            ]],
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'StringLiteral'],
                    ]],
                ]],
            ]],
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'RegexpLiteral'],
                    ]],
                ]],
            ]],
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'InlineRule'],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'Quantifier'],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '*'],
                        ]],
                    ]],
                ]],
            ]],
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '?'],
                        ]],
                    ]],
                ]],
            ]],
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '+'],
                        ]],
                    ]],
                ]],
            ]],
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '{'],
                        ]],
                    ]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'Count'],
                    ]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '}'],
                        ]],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'Count'],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'IntValue'],
                    ]],
                ]],
            ]],
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'FunctionCall'],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'IntValue'],
        ['Flags', [
            ['Lexeme', 'L'],
        ]],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RegexpLiteral', [
                            ['SymbolRange', [
                                ['Symbol', '0'],
                                ['Symbol', '9'],
                            ]],
                        ]],
                    ]],
                    ['Quantifier', [['', '+']]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'FunctionCall'],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '='],
                            ['Symbol', '{'],
                        ]],
                    ]],
                    ['Cut', '!'],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'FunctionName'],
                    ]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['InlineRule', [
                            ['RuleBody', [
                                ['Statement', [
                                    ['Expression', [
                                        ['Element', [
                                            ['StringLiteral', [
                                                ['Symbol', ','],
                                            ]],
                                        ]],
                                    ]],
                                    ['Expression', [
                                        ['Element', [
                                            ['RuleName', 'FunctionName'],
                                        ]],
                                    ]],
                                ]],
                            ]],
                        ]],
                    ]],
                    ['Quantifier', [['', '*']]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '}'],
                        ]],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'FunctionName'],
        ['Flags', [
            ['Lexeme', 'L'],
        ]],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'ID'],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'LookAhead'],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '>'],
                            ['Symbol', '>'],
                        ]],
                    ]],
                ]],
            ]],
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '>'],
                        ]],
                    ]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'Element'],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'Cut'],
        ['Flags', [
            ['Lexeme', 'L'],
        ]],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '!'],
                        ]],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'InlineRule'],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '('],
                        ]],
                    ]],
                    ['Cut', '!'],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'RuleBody'],
                    ]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', ')'],
                        ]],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'StringLiteral'],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '\''],
                        ]],
                    ]],
                    ['Cut', '!'],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'Symbol'],
                    ]],
                    ['FunctionCall', [['FunctionName', 'symbolStr']]],
                    ['Quantifier', [['', '+']]],
                    ['LookAhead', [['', '>>']]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '\''],
                        ]],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'RegexpLiteral'],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '['],
                        ]],
                    ]],
                    ['Cut', '!'],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'SymbolRange'],
                    ]],
                    ['Quantifier', [['', '+']]],
                    ['LookAhead', [['', '>>']]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', ']'],
                        ]],
                    ]],
                ]],
            ]],
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'AnySymbolLiteral'],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'SymbolRange'],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'Symbol'],
                    ]],
                    ['FunctionCall', [['FunctionName', 'symbolStr']]],
                    ['LookAhead', [
                        ['', '>'],
                        ['Element', [
                            ['StringLiteral', [
                                ['Symbol', ']'],
                            ]],
                        ]],
                    ]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['InlineRule', [
                            ['RuleBody', [
                                ['Statement', [
                                    ['Expression', [
                                        ['Element', [
                                            ['StringLiteral', [
                                                ['Symbol', '-'],
                                            ]],
                                        ]],
                                        ['Cut', '!'],
                                    ]],
                                    ['Expression', [
                                        ['Element', [
                                            ['RuleName', 'Symbol'],
                                        ]],
                                        ['FunctionCall', [['FunctionName', 'symbolStr']]],
                                        ['LookAhead', [
                                            ['', '>'],
                                            ['Element', [
                                                ['StringLiteral', [
                                                    ['Symbol', ']'],
                                                ]],
                                            ]],
                                        ]],
                                    ]],
                                ]],
                            ]],
                        ]],
                    ]],
                    ['Quantifier', [['', '?']]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'Symbol'],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'HexCodeSymbol'],
                    ]],
                ]],
            ]],
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'EscapedSymbol'],
                    ]],
                ]],
            ]],
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'AnySymbol'],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'HexCodeSymbol'],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '\\'],
                            ['Symbol', 'x'],
                        ]],
                    ]],
                    ['Cut', '!'],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'HexDigit'],
                    ]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'HexDigit'],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'HexDigit'],
        ['Flags', [
            ['Lexeme', 'L'],
        ]],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RegexpLiteral', [
                            ['SymbolRange', [
                                ['Symbol', '0'],
                                ['Symbol', '9'],
                            ]],
                            ['SymbolRange', [
                                ['Symbol', 'A'],
                                ['Symbol', 'F'],
                            ]],
                        ]],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'EscapedSymbol'],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '\\'],
                        ]],
                    ]],
                    ['Cut', '!'],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'AnySymbol'],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'AnySymbol'],
        ['Flags', [
            ['Lexeme', 'L'],
        ]],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RegexpLiteral', [
                            ['AnySymbolLiteral', '.'],
                        ]],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'AnySymbolLiteral'],
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', '.'],
                        ]],
                    ]],
                ]],
            ]],
        ]],
    ]],
]];

return $selfGrammar;
