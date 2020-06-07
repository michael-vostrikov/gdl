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
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RegexpLiteral', [
                            ['SymbolRange', [
                                ['Symbol', [['AnySymbol', [['', ' ']]]]],
                            ]],
                            ['SymbolRange', [
                                ['Symbol', [['EscapedSymbol', [['AnySymbol', [['', 'n']]]]]]],
                            ]],
                            ['SymbolRange', [
                                ['Symbol', [['EscapedSymbol', [['AnySymbol', [['', 't']]]]]]],
                            ]],
                            ['SymbolRange', [
                                ['Symbol', [['EscapedSymbol', [['AnySymbol', [['', 'r']]]]]]],
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
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', [['AnySymbol', [['', '/']]]]],
                            ['Symbol', [['AnySymbol', [['', '*']]]]],
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
                            ['Symbol', [['AnySymbol', [['', '*']]]]],
                            ['Symbol', [['AnySymbol', [['', '/']]]]],
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
                        ['RuleName', 'delimiter'],
                    ]],
                    ['Quantifier', [['', '*']]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', [['AnySymbol', [['', ':']]]]],
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
                            ['Symbol', [['AnySymbol', [['', ';']]]]],
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
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RegexpLiteral', [
                            ['SymbolRange', [
                                ['Symbol', [['AnySymbol', [['', 'a']]]]],
                                ['Symbol', [['AnySymbol', [['', 'z']]]]],
                            ]],
                            ['SymbolRange', [
                                ['Symbol', [['AnySymbol', [['', 'A']]]]],
                                ['Symbol', [['AnySymbol', [['', 'Z']]]]],
                            ]],
                            ['SymbolRange', [
                                ['Symbol', [['AnySymbol', [['', '_']]]]],
                            ]],
                        ]],
                    ]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RegexpLiteral', [
                            ['SymbolRange', [
                                ['Symbol', [['AnySymbol', [['', 'a']]]]],
                                ['Symbol', [['AnySymbol', [['', 'z']]]]],
                            ]],
                            ['SymbolRange', [
                                ['Symbol', [['AnySymbol', [['', 'A']]]]],
                                ['Symbol', [['AnySymbol', [['', 'Z']]]]],
                            ]],
                            ['SymbolRange', [
                                ['Symbol', [['AnySymbol', [['', '_']]]]],
                            ]],
                            ['SymbolRange', [
                                ['Symbol', [['AnySymbol', [['', '0']]]]],
                                ['Symbol', [['AnySymbol', [['', '9']]]]],
                            ]],
                        ]],
                    ]],
                    ['Quantifier', [['', '*']]],
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
                            ['Symbol', [['AnySymbol', [['', '|']]]]],
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
                                                ['Symbol', [['AnySymbol', [['', '|']]]]],
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
                            ['Symbol', [['AnySymbol', [['', '*']]]]],
                        ]],
                    ]],
                ]],
            ]],
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', [['AnySymbol', [['', '?']]]]],
                        ]],
                    ]],
                ]],
            ]],
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', [['AnySymbol', [['', '+']]]]],
                        ]],
                    ]],
                ]],
            ]],
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', [['AnySymbol', [['', '{']]]]],
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
                            ['Symbol', [['AnySymbol', [['', '}']]]]],
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
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RegexpLiteral', [
                            ['SymbolRange', [
                                ['Symbol', [['AnySymbol', [['', '0']]]]],
                                ['Symbol', [['AnySymbol', [['', '9']]]]],
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
                            ['Symbol', [['AnySymbol', [['', '=']]]]],
                            ['Symbol', [['AnySymbol', [['', '{']]]]],
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
                                                ['Symbol', [['AnySymbol', [['', ',']]]]],
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
                            ['Symbol', [['AnySymbol', [['', '}']]]]],
                        ]],
                    ]],
                ]],
            ]],
        ]],
    ]],
    ['Rule', [
        ['RuleName', 'FunctionName'],
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
                            ['Symbol', [['AnySymbol', [['', '>']]]]],
                            ['Symbol', [['AnySymbol', [['', '>']]]]],
                        ]],
                    ]],
                ]],
            ]],
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', [['AnySymbol', [['', '>']]]]],
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
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', [['AnySymbol', [['', '!']]]]],
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
                            ['Symbol', [['AnySymbol', [['', '(']]]]],
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
                            ['Symbol', [['AnySymbol', [['', ')']]]]],
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
                            ['Symbol', [['AnySymbol', [['', '\'']]]]],
                        ]],
                    ]],
                    ['Cut', '!'],
                ]],
                ['Expression', [
                    ['Element', [
                        ['RuleName', 'Symbol'],
                    ]],
                    ['Quantifier', [['', '+']]],
                    ['LookAhead', [['', '>>']]],
                ]],
                ['Expression', [
                    ['Element', [
                        ['StringLiteral', [
                            ['Symbol', [['AnySymbol', [['', '\'']]]]],
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
                            ['Symbol', [['AnySymbol', [['', '[']]]]],
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
                            ['Symbol', [['AnySymbol', [['', ']']]]]],
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
                    ['LookAhead', [
                        ['', '>'],
                        ['Element', [
                            ['StringLiteral', [
                                ['Symbol', [['AnySymbol', [['', ']']]]]],
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
                                                ['Symbol', [['AnySymbol', [['', '-']]]]],
                                            ]],
                                        ]],
                                        ['Cut', '!'],
                                    ]],
                                    ['Expression', [
                                        ['Element', [
                                            ['RuleName', 'Symbol'],
                                        ]],
                                        ['LookAhead', [
                                            ['', '>'],
                                            ['Element', [
                                                ['StringLiteral', [
                                                    ['Symbol', [['AnySymbol', [['', ']']]]]],
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
                            ['Symbol', [['EscapedSymbol', [['AnySymbol', [['', '\\']]]]]]],
                            ['Symbol', [['AnySymbol', [['', 'x']]]]],
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
        ['RuleBody', [
            ['Statement', [
                ['Expression', [
                    ['Element', [
                        ['RegexpLiteral', [
                            ['SymbolRange', [
                                ['Symbol', [['AnySymbol', [['', '0']]]]],
                                ['Symbol', [['AnySymbol', [['', '9']]]]],
                            ]],
                            ['SymbolRange', [
                                ['Symbol', [['AnySymbol', [['', 'A']]]]],
                                ['Symbol', [['AnySymbol', [['', 'F']]]]],
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
                            ['Symbol', [['EscapedSymbol', [['AnySymbol', [['', '\\']]]]]]],
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
                            ['Symbol', [['AnySymbol', [['', '.']]]]],
                        ]],
                    ]],
                ]],
            ]],
        ]],
    ]],
]];

return $selfGrammar;
