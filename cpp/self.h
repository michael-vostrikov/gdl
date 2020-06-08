
GdlNode* selfGrammar = new GdlNode("Grammar", {
    new GdlNode("Rule", {
        new GdlNode("RuleName", "Grammar"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "delimiter"),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "*")}),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "Rule"),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "*")}),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "delimiter"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "space"),
                    }),
                }),
            }),
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "comment"),
                    }),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "space"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RegexpLiteral", {
                            new GdlNode("SymbolRange", {
                                new GdlNode("Symbol", " "),
                            }),
                            new GdlNode("SymbolRange", {
                                new GdlNode("Symbol", "\n"),
                            }),
                            new GdlNode("SymbolRange", {
                                new GdlNode("Symbol", "\t"),
                            }),
                            new GdlNode("SymbolRange", {
                                new GdlNode("Symbol", "\r"),
                            }),
                        }),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "+")}),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "comment"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", "/"),
                            new GdlNode("Symbol", "*"),
                        }),
                    }),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "AnySymbol"),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "*")}),
                    new GdlNode("LookAhead", {new GdlNode("", ">>")}),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", "*"),
                            new GdlNode("Symbol", "/"),
                        }),
                    }),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "Rule"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "RuleName"),
                    }),
                    new GdlNode("FunctionCall", {new GdlNode("FunctionName", "str")}),
                    new GdlNode("Cut", "!"),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "delimiter"),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "*")}),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", ":"),
                        }),
                    }),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "delimiter"),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "*")}),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "RuleBody"),
                    }),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", ";"),
                        }),
                    }),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "delimiter"),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "*")}),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "RuleName"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "ID"),
                    }),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "ID"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RegexpLiteral", {
                            new GdlNode("SymbolRange", {
                                new GdlNode("Symbol", "a"),
                                new GdlNode("Symbol", "z"),
                            }),
                            new GdlNode("SymbolRange", {
                                new GdlNode("Symbol", "A"),
                                new GdlNode("Symbol", "Z"),
                            }),
                            new GdlNode("SymbolRange", {
                                new GdlNode("Symbol", "_"),
                            }),
                        }),
                    }),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RegexpLiteral", {
                            new GdlNode("SymbolRange", {
                                new GdlNode("Symbol", "a"),
                                new GdlNode("Symbol", "z"),
                            }),
                            new GdlNode("SymbolRange", {
                                new GdlNode("Symbol", "A"),
                                new GdlNode("Symbol", "Z"),
                            }),
                            new GdlNode("SymbolRange", {
                                new GdlNode("Symbol", "_"),
                            }),
                            new GdlNode("SymbolRange", {
                                new GdlNode("Symbol", "0"),
                                new GdlNode("Symbol", "9"),
                            }),
                        }),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "*")}),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "RuleBody"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", "|"),
                        }),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "?")}),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "delimiter"),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "*")}),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "Statement"),
                    }),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("InlineRule", {
                            new GdlNode("RuleBody", {
                                new GdlNode("Statement", {
                                    new GdlNode("Expression", {
                                        new GdlNode("Element", {
                                            new GdlNode("StringLiteral", {
                                                new GdlNode("Symbol", "|"),
                                            }),
                                        }),
                                    }),
                                    new GdlNode("Expression", {
                                        new GdlNode("Element", {
                                            new GdlNode("RuleName", "delimiter"),
                                        }),
                                        new GdlNode("Quantifier", {new GdlNode("", "*")}),
                                    }),
                                    new GdlNode("Expression", {
                                        new GdlNode("Element", {
                                            new GdlNode("RuleName", "Statement"),
                                        }),
                                    }),
                                }),
                            }),
                        }),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "*")}),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "Statement"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "Expression"),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "+")}),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "Expression"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "Element"),
                    }),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "FunctionCall"),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "?")}),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "Quantifier"),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "?")}),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "Cut"),
                    }),
                    new GdlNode("FunctionCall", {new GdlNode("FunctionName", "str")}),
                    new GdlNode("Quantifier", {new GdlNode("", "?")}),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "LookAhead"),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "?")}),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "delimiter"),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "*")}),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "Element"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "RuleName"),
                    }),
                    new GdlNode("FunctionCall", {new GdlNode("FunctionName", "str")}),
                }),
            }),
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "StringLiteral"),
                    }),
                }),
            }),
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "RegexpLiteral"),
                    }),
                }),
            }),
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "InlineRule"),
                    }),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "Quantifier"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", "*"),
                        }),
                    }),
                }),
            }),
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", "?"),
                        }),
                    }),
                }),
            }),
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", "+"),
                        }),
                    }),
                }),
            }),
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", "{"),
                        }),
                    }),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "Count"),
                    }),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", "}"),
                        }),
                    }),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "Count"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "IntValue"),
                    }),
                    new GdlNode("FunctionCall", {new GdlNode("FunctionName", "str")}),
                }),
            }),
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "FunctionCall"),
                    }),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "IntValue"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RegexpLiteral", {
                            new GdlNode("SymbolRange", {
                                new GdlNode("Symbol", "0"),
                                new GdlNode("Symbol", "9"),
                            }),
                        }),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "+")}),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "FunctionCall"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", "="),
                            new GdlNode("Symbol", "{"),
                        }),
                    }),
                    new GdlNode("Cut", "!"),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "FunctionName"),
                    }),
                    new GdlNode("FunctionCall", {new GdlNode("FunctionName", "str")}),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("InlineRule", {
                            new GdlNode("RuleBody", {
                                new GdlNode("Statement", {
                                    new GdlNode("Expression", {
                                        new GdlNode("Element", {
                                            new GdlNode("StringLiteral", {
                                                new GdlNode("Symbol", ","),
                                            }),
                                        }),
                                    }),
                                    new GdlNode("Expression", {
                                        new GdlNode("Element", {
                                            new GdlNode("RuleName", "FunctionName"),
                                        }),
                                        new GdlNode("FunctionCall", {new GdlNode("FunctionName", "str")}),
                                    }),
                                }),
                            }),
                        }),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "*")}),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", "}"),
                        }),
                    }),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "FunctionName"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "ID"),
                    }),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "LookAhead"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", ">"),
                            new GdlNode("Symbol", ">"),
                        }),
                    }),
                }),
            }),
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", ">"),
                        }),
                    }),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "Element"),
                    }),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "Cut"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", "!"),
                        }),
                    }),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "InlineRule"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", "("),
                        }),
                    }),
                    new GdlNode("Cut", "!"),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "RuleBody"),
                    }),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", ")"),
                        }),
                    }),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "StringLiteral"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", "'"),
                        }),
                    }),
                    new GdlNode("Cut", "!"),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "Symbol"),
                    }),
                    new GdlNode("FunctionCall", {new GdlNode("FunctionName", "symbolStr")}),
                    new GdlNode("Quantifier", {new GdlNode("", "+")}),
                    new GdlNode("LookAhead", {new GdlNode("", ">>")}),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", "'"),
                        }),
                    }),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "RegexpLiteral"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", "["),
                        }),
                    }),
                    new GdlNode("Cut", "!"),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "SymbolRange"),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "+")}),
                    new GdlNode("LookAhead", {new GdlNode("", ">>")}),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", "]"),
                        }),
                    }),
                }),
            }),
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "AnySymbolLiteral"),
                    }),
                    new GdlNode("FunctionCall", {new GdlNode("FunctionName", "str")}),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "SymbolRange"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "Symbol"),
                    }),
                    new GdlNode("FunctionCall", {new GdlNode("FunctionName", "symbolStr")}),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("InlineRule", {
                            new GdlNode("RuleBody", {
                                new GdlNode("Statement", {
                                    new GdlNode("Expression", {
                                        new GdlNode("Element", {
                                            new GdlNode("StringLiteral", {
                                                new GdlNode("Symbol", "-"),
                                            }),
                                        }),
                                        new GdlNode("Cut", "!"),
                                    }),
                                    new GdlNode("Expression", {
                                        new GdlNode("Element", {
                                            new GdlNode("RuleName", "Symbol"),
                                        }),
                                        new GdlNode("FunctionCall", {new GdlNode("FunctionName", "symbolStr")}),
                                    }),
                                }),
                            }),
                        }),
                    }),
                    new GdlNode("Quantifier", {new GdlNode("", "?")}),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "Symbol"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "HexCodeSymbol"),
                    }),
                }),
            }),
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "EscapedSymbol"),
                    }),
                }),
            }),
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "AnySymbol"),
                    }),
                    new GdlNode("FunctionCall", {new GdlNode("FunctionName", "str")}),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "HexCodeSymbol"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", "\\"),
                            new GdlNode("Symbol", "x"),
                        }),
                    }),
                    new GdlNode("Cut", "!"),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "HexDigit"),
                    }),
                    new GdlNode("FunctionCall", {new GdlNode("FunctionName", "str")}),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "HexDigit"),
                    }),
                    new GdlNode("FunctionCall", {new GdlNode("FunctionName", "str")}),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "HexDigit"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RegexpLiteral", {
                            new GdlNode("SymbolRange", {
                                new GdlNode("Symbol", "0"),
                                new GdlNode("Symbol", "9"),
                            }),
                            new GdlNode("SymbolRange", {
                                new GdlNode("Symbol", "A"),
                                new GdlNode("Symbol", "F"),
                            }),
                        }),
                    }),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "EscapedSymbol"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", "\\"),
                        }),
                    }),
                    new GdlNode("Cut", "!"),
                }),
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RuleName", "AnySymbol"),
                    }),
                    new GdlNode("FunctionCall", {new GdlNode("FunctionName", "str")}),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "AnySymbol"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("RegexpLiteral", {
                            new GdlNode("AnySymbolLiteral", "."),
                        }),
                    }),
                }),
            }),
        }),
    }),
    new GdlNode("Rule", {
        new GdlNode("RuleName", "AnySymbolLiteral"),
        new GdlNode("RuleBody", {
            new GdlNode("Statement", {
                new GdlNode("Expression", {
                    new GdlNode("Element", {
                        new GdlNode("StringLiteral", {
                            new GdlNode("Symbol", "."),
                        }),
                    }),
                }),
            }),
        }),
    }),
});
