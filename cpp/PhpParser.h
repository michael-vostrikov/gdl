#include "StringDescriptor.h"
#include "GdlNode.h"
#include "Stream.h"

class PhpParser
{
protected:

    Stream* stream;

    std::vector<GdlNode> pool;

public:
    
    enum RuleName {
        Empty, Program, PhpProgram, comment, TopStatement, NamespaceStatement, NamespaceName, NamespacePart, ID, UseStatement, ClassDeclaration, ClassName, ClassReference, InterfaceReference, ClassBody, ClassBodyStatement, PropertyDeclaration, PropertyName, VariableName, AccessModifier, MethodDeclaration, FunctionDeclaration, FunctionName, ArgumentList, Argument, TypeReference, ReferenceModifier, FunctionBody, StatementList, Statement, AssignStatement, List, AssignExpression, ExpressionStatement, VariableReference, ConstantName, AccessList, AccessElement, PropertyAccess, MethodCall, ExpressionList, ArrayElementAccess, IfStatement, StatementBlock, ForStatement, ForeachStatement, ForeachKey, ReturnStatement, BreakStatement, ContinueStatement, ThrowStatement, TryStatement, CatchStatement, WhileStatement, DoWhileStatement, SwitchStatement, CaseList, Case, Expression, TernaryExpression, LogicalOrExpression, LogicalAndExpression, CompareExpression, NullCoalesceExpression, AddExpression, MulExpression, UnaryExpression, PrimaryExpression, NewExpression, FunctionCall, Literal, NullLiteral, LogicalLiteral, NumberLiteral, DecimalNumberLiteral, HexNumberLiteral, StringLiteral, SingleQuoteString, DoubleQuoteString, NowdocString, HeredocString, StringTag, Symbol, HexCodeSymbol, HexDigit, EscapedSymbol, AnySymbol, ArrayLiteral, ArrayElementList, ArrayElement
    };

    static const char* ruleNames[];


    PhpParser(Stream* stream): stream(stream)
    {
        this->pool.reserve(stream->getContent().getSize() / 2);
    }

    size_t createGdlNode(uint32_t ruleName, ListDescriptor value)
    {
        size_t newIndex = this->pool.size();
        GdlNode node(ruleName, value);
        this->pool.push_back(node);

        return newIndex;
    }

    size_t createGdlNode(uint32_t ruleName, StringDescriptor value)
    {
        size_t newIndex = this->pool.size();
        GdlNode node(ruleName, value);
        this->pool.push_back(node);

        return newIndex;
    }

    GdlNode* getRootNode()
    {
        return &this->pool.front();
    }

    size_t systemParseString(const char* string, size_t size)
    {
        size_t initialPos = this->stream->getPos();
        size_t parsedSize = 0;

        for (size_t i = 0; i < size; i++) {
            if (this->stream->eof()) {
                this->stream->setPos(initialPos);
                parsedSize = 0;
                break;
            }

            char symbol = this->stream->readSymbol();
            char contentSymbol = string[i];
            if (contentSymbol != symbol) {
                this->stream->setPos(initialPos);
                parsedSize = 0;
                break;
            }

            parsedSize++;
        }

        return parsedSize;
    }

    size_t systemParseRegexp(const char* str, int size)
    {
        if (this->stream->eof()) {
            return 0;
        }

        size_t parsedSize = 0;
        uint8_t symbol = this->stream->getCurrentSymbol();

        const char* data = str;
        for (size_t i = 0; i < size; i += 2) {
            uint8_t from = data[i];
            uint8_t to = data[i + 1];
            if (symbol >= from && symbol <= to) {
                this->stream->incPos();
                parsedSize = 1;
                break;
            }
        }

        return parsedSize;
    }

    virtual void skipSpaces()
    {
        while (!this->stream->eof()) {
            char symbol = this->stream->getCurrentSymbol();

            if (! (symbol == ' ' || symbol == '\n' || symbol == '\t' || symbol == '\r')) {
                break;
            }

            this->stream->incPos();
        }
    }

    

    virtual ssize_t parseProgram()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\x00':
            case '\x01':
            case '\x02':
            case '\x03':
            case '\x04':
            case '\x05':
            case '\x06':
            case '\x07':
            case '\x08':
            case '\x09':
            case '\x0A':
            case '\x0B':
            case '\x0C':
            case '\x0D':
            case '\x0E':
            case '\x0F':
            case '\x10':
            case '\x11':
            case '\x12':
            case '\x13':
            case '\x14':
            case '\x15':
            case '\x16':
            case '\x17':
            case '\x18':
            case '\x19':
            case '\x1A':
            case '\x1B':
            case '\x1C':
            case '\x1D':
            case '\x1E':
            case '\x1F':
            case '\x20':
            case '!':
            case '"':
            case '#':
            case '$':
            case '%':
            case '&':
            case '\'':
            case '(':
            case ')':
            case '*':
            case '+':
            case ',':
            case '-':
            case '.':
            case '/':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case ':':
            case ';':
            case '=':
            case '>':
            case '?':
            case '@':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '[':
            case '\\':
            case ']':
            case '^':
            case '_':
            case '`':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case '{':
            case '|':
            case '}':
            case '~':
            case '':
            case '\x80':
            case '\x81':
            case '\x82':
            case '\x83':
            case '\x84':
            case '\x85':
            case '\x86':
            case '\x87':
            case '\x88':
            case '\x89':
            case '\x8A':
            case '\x8B':
            case '\x8C':
            case '\x8D':
            case '\x8E':
            case '\x8F':
            case '\x90':
            case '\x91':
            case '\x92':
            case '\x93':
            case '\x94':
            case '\x95':
            case '\x96':
            case '\x97':
            case '\x98':
            case '\x99':
            case '\x9A':
            case '\x9B':
            case '\x9C':
            case '\x9D':
            case '\x9E':
            case '\x9F':
            case '\xA0':
            case '\xA1':
            case '\xA2':
            case '\xA3':
            case '\xA4':
            case '\xA5':
            case '\xA6':
            case '\xA7':
            case '\xA8':
            case '\xA9':
            case '\xAA':
            case '\xAB':
            case '\xAC':
            case '\xAD':
            case '\xAE':
            case '\xAF':
            case '\xB0':
            case '\xB1':
            case '\xB2':
            case '\xB3':
            case '\xB4':
            case '\xB5':
            case '\xB6':
            case '\xB7':
            case '\xB8':
            case '\xB9':
            case '\xBA':
            case '\xBB':
            case '\xBC':
            case '\xBD':
            case '\xBE':
            case '\xBF':
            case '\xC0':
            case '\xC1':
            case '\xC2':
            case '\xC3':
            case '\xC4':
            case '\xC5':
            case '\xC6':
            case '\xC7':
            case '\xC8':
            case '\xC9':
            case '\xCA':
            case '\xCB':
            case '\xCC':
            case '\xCD':
            case '\xCE':
            case '\xCF':
            case '\xD0':
            case '\xD1':
            case '\xD2':
            case '\xD3':
            case '\xD4':
            case '\xD5':
            case '\xD6':
            case '\xD7':
            case '\xD8':
            case '\xD9':
            case '\xDA':
            case '\xDB':
            case '\xDC':
            case '\xDD':
            case '\xDE':
            case '\xDF':
            case '\xE0':
            case '\xE1':
            case '\xE2':
            case '\xE3':
            case '\xE4':
            case '\xE5':
            case '\xE6':
            case '\xE7':
            case '\xE8':
            case '\xE9':
            case '\xEA':
            case '\xEB':
            case '\xEC':
            case '\xED':
            case '\xEE':
            case '\xEF':
            case '\xF0':
            case '\xF1':
            case '\xF2':
            case '\xF3':
            case '\xF4':
            case '\xF5':
            case '\xF6':
            case '\xF7':
            case '\xF8':
            case '\xF9':
            case '\xFA':
            case '\xFB':
            case '\xFC':
            case '\xFD':
            case '\xFE':
            case '\xFF':
                parsedCount = this->statement1Program();
            break;

            case '<':
                parsedCount = this->statement1Program();
                if (parsedCount == -1) parsedCount = this->statement1Program();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Program()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Program;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            this->skipSpaces();

            while (true) {
                elementInitialStreamPos = this->stream->getPos();
                elementInitialPoolSize = this->pool.size();
                elementInitialStreamPos = this->stream->getPos();
                elementParsedSize = this->systemParseString("<?php", 5);
    
                isElementParsed = (elementParsedSize != 0);
                if (isElementParsed) {
                    this->stream->setPos(elementInitialStreamPos);
                    this->pool.resize(elementInitialPoolSize);isElementParsed = false;
                }
                else {
                    elementInitialStreamPos = this->stream->getPos();
                    elementParsedSize = this->systemParseRegexp("\x00\xFF", 2);
        
                    isElementParsed = (elementParsedSize != 0);
                }
                if (!isElementParsed) break;
                
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("<?php", 5);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parsePhpProgram();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parsePhpProgram()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'n':
            case 'u':
            case 'c':
            case 'f':
            case 'i':
            case 'r':
            case 'b':
            case 'w':
            case 'd':
            case 's':
            case 't':
            case '/':
            case 'l':
            case '+':
            case '-':
            case '!':
            case '~':
            case 'a':
            case 'e':
            case 'g':
            case 'h':
            case 'j':
            case 'k':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'v':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement1PhpProgram();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1PhpProgram()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::PhpProgram;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            while (true) {
                elementParsedCount = this->parseTopStatement();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseComment()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '/':
                parsedCount = this->statement1Comment();
                if (parsedCount == -1) parsedCount = this->statement2Comment();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Comment()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::comment;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("/*", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            while (true) {
                elementInitialStreamPos = this->stream->getPos();
                elementInitialPoolSize = this->pool.size();
                elementInitialStreamPos = this->stream->getPos();
                elementParsedSize = this->systemParseString("*/", 2);
    
                isElementParsed = (elementParsedSize != 0);
                if (isElementParsed) {
                    this->stream->setPos(elementInitialStreamPos);
                    this->pool.resize(elementInitialPoolSize);isElementParsed = false;
                }
                else {
                    elementInitialStreamPos = this->stream->getPos();
                    elementParsedSize = this->parseAnySymbol();
        
                    isElementParsed = (elementParsedSize != 0);
                }
                if (!isElementParsed) break;
                lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::AnySymbol, lexemeValue);
            totalParsedElementCount++;
            }

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("*/", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2Comment()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::comment;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("//", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            while (true) {
                elementInitialStreamPos = this->stream->getPos();
                elementInitialPoolSize = this->pool.size();
                elementInitialStreamPos = this->stream->getPos();
                elementParsedSize = this->systemParseString("\x0A", 1);
    
                isElementParsed = (elementParsedSize != 0);
                if (isElementParsed) {
                    this->stream->setPos(elementInitialStreamPos);
                    this->pool.resize(elementInitialPoolSize);isElementParsed = false;
                }
                else {
                    elementInitialStreamPos = this->stream->getPos();
                    elementParsedSize = this->parseAnySymbol();
        
                    isElementParsed = (elementParsedSize != 0);
                }
                if (!isElementParsed) break;
                lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::AnySymbol, lexemeValue);
            totalParsedElementCount++;
            }

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("\x0A", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseTopStatement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'n':
                parsedCount = this->statement1TopStatement();
                if (parsedCount == -1) parsedCount = this->statement5TopStatement();
            break;

            case 'u':
                parsedCount = this->statement2TopStatement();
                if (parsedCount == -1) parsedCount = this->statement5TopStatement();
            break;

            case 'c':
                parsedCount = this->statement3TopStatement();
                if (parsedCount == -1) parsedCount = this->statement5TopStatement();
            break;

            case 'f':
                parsedCount = this->statement4TopStatement();
                if (parsedCount == -1) parsedCount = this->statement5TopStatement();
            break;

            case 'i':
            case 'r':
            case 'b':
            case 'w':
            case 'd':
            case 's':
            case 't':
            case '/':
            case 'l':
            case '+':
            case '-':
            case '!':
            case '~':
            case 'a':
            case 'e':
            case 'g':
            case 'h':
            case 'j':
            case 'k':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'v':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement5TopStatement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1TopStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::TopStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseNamespaceStatement();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2TopStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::TopStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseUseStatement();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement3TopStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::TopStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseClassDeclaration();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement4TopStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::TopStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseFunctionDeclaration();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement5TopStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::TopStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseStatementList();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseNamespaceStatement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'n':
                parsedCount = this->statement1NamespaceStatement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1NamespaceStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::NamespaceStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("namespace", 9);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseNamespaceName();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(";", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseNamespaceName()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
                parsedCount = this->statement1NamespaceName();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1NamespaceName()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::NamespaceName;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseNamespacePart();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::NamespacePart, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            while (true) {
                elementParsedCount = this->parseInline1_NamespaceName_1();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_NamespaceName_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\\':
                parsedCount = this->statement1Inline1_NamespaceName_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_NamespaceName_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::NamespaceName;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("\\", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseNamespacePart();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::NamespacePart, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    size_t parseNamespacePart()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
                parsedSize = this->statement1NamespacePart();
            break;
        };

        return parsedSize;
    }

    size_t statement1NamespacePart()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseID();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    size_t parseID()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
                parsedSize = this->statement1ID();
            break;
        };

        return parsedSize;
    }

    size_t statement1ID()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseRegexp("azAZ__", 6);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            while (true) {
                elementInitialStreamPos = this->stream->getPos();
                elementParsedSize = this->systemParseRegexp("azAZ__09", 8);
    
                isElementParsed = (elementParsedSize != 0);
                if (!isElementParsed) break;
                parsedSize += elementParsedSize;
            }

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    virtual ssize_t parseUseStatement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'u':
                parsedCount = this->statement1UseStatement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1UseStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::UseStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("use", 3);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseNamespaceName();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            while (true) {
                elementParsedCount = this->parseInline1_UseStatement_1();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(";", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_UseStatement_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case ',':
                parsedCount = this->statement1Inline1_UseStatement_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_UseStatement_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::UseStatement;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(",", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseNamespaceName();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseClassDeclaration()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'c':
                parsedCount = this->statement1ClassDeclaration();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1ClassDeclaration()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ClassDeclaration;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("class", 5);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseClassName();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::ClassName, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseInline1_ClassDeclaration_1();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseInline2_ClassDeclaration_1();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("{", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseClassBody();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("}", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_ClassDeclaration_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'e':
                parsedCount = this->statement1Inline1_ClassDeclaration_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_ClassDeclaration_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ClassDeclaration;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("extends", 7);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseClassReference();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline2_ClassDeclaration_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'i':
                parsedCount = this->statement1Inline2_ClassDeclaration_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline2_ClassDeclaration_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ClassDeclaration;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("implements", 10);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseInterfaceReference();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    size_t parseClassName()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
                parsedSize = this->statement1ClassName();
            break;
        };

        return parsedSize;
    }

    size_t statement1ClassName()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseID();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    virtual ssize_t parseClassReference()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\\':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
                parsedCount = this->statement1ClassReference();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1ClassReference()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ClassReference;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("\\", 1);

            isElementParsed = (elementParsedSize != 0);
            if (isElementParsed) {
                
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseClassName();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::ClassName, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            while (true) {
                elementParsedCount = this->parseInline1_ClassReference_1();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_ClassReference_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\\':
                parsedCount = this->statement1Inline1_ClassReference_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_ClassReference_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ClassReference;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("\\", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseClassName();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::ClassName, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInterfaceReference()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\\':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
                parsedCount = this->statement1InterfaceReference();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1InterfaceReference()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::InterfaceReference;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("\\", 1);

            isElementParsed = (elementParsedSize != 0);
            if (isElementParsed) {
                
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseClassName();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::ClassName, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            while (true) {
                elementParsedCount = this->parseInline1_InterfaceReference_1();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_InterfaceReference_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\\':
                parsedCount = this->statement1Inline1_InterfaceReference_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_InterfaceReference_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::InterfaceReference;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("\\", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseClassName();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::ClassName, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseClassBody()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'p':
            case 'f':
            case '/':
                parsedCount = this->statement1ClassBody();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1ClassBody()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ClassBody;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            expressionInitialStreamPos = this->stream->getPos();
            expressionInitialPoolSize = this->pool.size();quantifierParsedElementCount = 0;
            while (true) {
                elementParsedCount = this->parseClassBodyStatement();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount++;
                
                quantifierParsedElementCount++;
            }
            if (quantifierParsedElementCount < 1) {
                this->stream->setPos(expressionInitialStreamPos);
                this->pool.resize(expressionInitialPoolSize);
                break;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseClassBodyStatement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'p':
                parsedCount = this->statement1ClassBodyStatement();
                if (parsedCount == -1) parsedCount = this->statement2ClassBodyStatement();
            break;

            case 'f':
                parsedCount = this->statement2ClassBodyStatement();
            break;

            case '/':
                parsedCount = this->statement3ClassBodyStatement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1ClassBodyStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ClassBodyStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parsePropertyDeclaration();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2ClassBodyStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ClassBodyStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseMethodDeclaration();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement3ClassBodyStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ClassBodyStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseComment();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parsePropertyDeclaration()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'p':
                parsedCount = this->statement1PropertyDeclaration();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1PropertyDeclaration()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::PropertyDeclaration;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseAccessModifier();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parsePropertyName();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseAssignExpression();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(";", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parsePropertyName()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '$':
                parsedCount = this->statement1PropertyName();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1PropertyName()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::PropertyName;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseVariableName();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::VariableName, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    size_t parseVariableName()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '$':
                parsedSize = this->statement1VariableName();
            break;
        };

        return parsedSize;
    }

    size_t statement1VariableName()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("$", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseID();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    virtual ssize_t parseAccessModifier()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'p':
                parsedCount = this->statement1AccessModifier();
                if (parsedCount == -1) parsedCount = this->statement2AccessModifier();
                if (parsedCount == -1) parsedCount = this->statement3AccessModifier();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1AccessModifier()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AccessModifier;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("private", 7);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2AccessModifier()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AccessModifier;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("protected", 9);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement3AccessModifier()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AccessModifier;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("public", 6);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseMethodDeclaration()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'p':
            case 'f':
                parsedCount = this->statement1MethodDeclaration();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1MethodDeclaration()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::MethodDeclaration;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseAccessModifier();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseFunctionDeclaration();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseFunctionDeclaration()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'f':
                parsedCount = this->statement1FunctionDeclaration();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1FunctionDeclaration()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::FunctionDeclaration;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("function", 8);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseFunctionName();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::FunctionName, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("(", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseArgumentList();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(")", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("{", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseFunctionBody();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("}", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    size_t parseFunctionName()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
                parsedSize = this->statement1FunctionName();
            break;
        };

        return parsedSize;
    }

    size_t statement1FunctionName()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseID();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    virtual ssize_t parseArgumentList()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '?':
            case '\\':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '&':
            case '$':
                parsedCount = this->statement1ArgumentList();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1ArgumentList()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ArgumentList;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseArgument();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            while (true) {
                elementParsedCount = this->parseInline1_ArgumentList_1();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_ArgumentList_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case ',':
                parsedCount = this->statement1Inline1_ArgumentList_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_ArgumentList_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ArgumentList;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(",", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseArgument();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseArgument()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '?':
            case '\\':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '&':
            case '$':
                parsedCount = this->statement1Argument();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Argument()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Argument;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseTypeReference();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseReferenceModifier();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseVariableName();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::VariableName, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseAssignExpression();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseTypeReference()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '?':
            case '\\':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
                parsedCount = this->statement1TypeReference();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1TypeReference()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::TypeReference;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("?", 1);

            isElementParsed = (elementParsedSize != 0);
            if (isElementParsed) {
                
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseClassReference();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseReferenceModifier()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '&':
                parsedCount = this->statement1ReferenceModifier();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1ReferenceModifier()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ReferenceModifier;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("&", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseFunctionBody()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'i':
            case 'f':
            case 'r':
            case 'b':
            case 'c':
            case 'w':
            case 'd':
            case 's':
            case 't':
            case '/':
            case 'l':
            case '+':
            case '-':
            case '!':
            case '~':
            case 'n':
            case 'a':
            case 'e':
            case 'g':
            case 'h':
            case 'j':
            case 'k':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'u':
            case 'v':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement1FunctionBody();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1FunctionBody()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::FunctionBody;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseStatementList();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseStatementList()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'i':
            case 'f':
            case 'r':
            case 'b':
            case 'c':
            case 'w':
            case 'd':
            case 's':
            case 't':
            case '/':
            case 'l':
            case '+':
            case '-':
            case '!':
            case '~':
            case 'n':
            case 'a':
            case 'e':
            case 'g':
            case 'h':
            case 'j':
            case 'k':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'u':
            case 'v':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement1StatementList();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1StatementList()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::StatementList;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            expressionInitialStreamPos = this->stream->getPos();
            expressionInitialPoolSize = this->pool.size();quantifierParsedElementCount = 0;
            while (true) {
                elementInitialStreamPos = this->stream->getPos();
                elementInitialPoolSize = this->pool.size();
                elementInitialStreamPos = this->stream->getPos();
                elementParsedSize = this->systemParseString("}", 1);
    
                isElementParsed = (elementParsedSize != 0);
                if (isElementParsed) {
                    this->stream->setPos(elementInitialStreamPos);
                    this->pool.resize(elementInitialPoolSize);isElementParsed = false;
                }
                else {
                    elementParsedCount = this->parseStatement();
        
                    isElementParsed = (elementParsedCount != -1);
                }
                if (!isElementParsed) break;
                totalParsedElementCount++;
                
                quantifierParsedElementCount++;
            }
            if (quantifierParsedElementCount < 1) {
                this->stream->setPos(expressionInitialStreamPos);
                this->pool.resize(expressionInitialPoolSize);
                break;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseStatement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'i':
                parsedCount = this->statement1Statement();
                if (parsedCount == -1) parsedCount = this->statement13Statement();
                if (parsedCount == -1) parsedCount = this->statement14Statement();
            break;

            case 'f':
                parsedCount = this->statement2Statement();
                if (parsedCount == -1) parsedCount = this->statement3Statement();
                if (parsedCount == -1) parsedCount = this->statement13Statement();
                if (parsedCount == -1) parsedCount = this->statement14Statement();
            break;

            case 'r':
                parsedCount = this->statement4Statement();
                if (parsedCount == -1) parsedCount = this->statement13Statement();
                if (parsedCount == -1) parsedCount = this->statement14Statement();
            break;

            case 'b':
                parsedCount = this->statement5Statement();
                if (parsedCount == -1) parsedCount = this->statement13Statement();
                if (parsedCount == -1) parsedCount = this->statement14Statement();
            break;

            case 'c':
                parsedCount = this->statement6Statement();
                if (parsedCount == -1) parsedCount = this->statement13Statement();
                if (parsedCount == -1) parsedCount = this->statement14Statement();
            break;

            case 'w':
                parsedCount = this->statement7Statement();
                if (parsedCount == -1) parsedCount = this->statement13Statement();
                if (parsedCount == -1) parsedCount = this->statement14Statement();
            break;

            case 'd':
                parsedCount = this->statement8Statement();
                if (parsedCount == -1) parsedCount = this->statement13Statement();
                if (parsedCount == -1) parsedCount = this->statement14Statement();
            break;

            case 's':
                parsedCount = this->statement9Statement();
                if (parsedCount == -1) parsedCount = this->statement13Statement();
                if (parsedCount == -1) parsedCount = this->statement14Statement();
            break;

            case 't':
                parsedCount = this->statement10Statement();
                if (parsedCount == -1) parsedCount = this->statement11Statement();
                if (parsedCount == -1) parsedCount = this->statement13Statement();
                if (parsedCount == -1) parsedCount = this->statement14Statement();
            break;

            case '/':
                parsedCount = this->statement12Statement();
            break;

            case 'l':
            case '+':
            case '-':
            case '!':
            case '~':
            case 'n':
            case 'a':
            case 'e':
            case 'g':
            case 'h':
            case 'j':
            case 'k':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'u':
            case 'v':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement13Statement();
                if (parsedCount == -1) parsedCount = this->statement14Statement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Statement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Statement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseIfStatement();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2Statement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Statement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseForeachStatement();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement3Statement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Statement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseForStatement();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement4Statement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Statement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseReturnStatement();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement5Statement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Statement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseBreakStatement();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement6Statement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Statement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseContinueStatement();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement7Statement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Statement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseWhileStatement();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement8Statement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Statement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseDoWhileStatement();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement9Statement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Statement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseSwitchStatement();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement10Statement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Statement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseThrowStatement();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement11Statement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Statement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseTryStatement();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement12Statement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Statement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseComment();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement13Statement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Statement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseAssignStatement();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement14Statement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Statement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseExpressionStatement();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseAssignStatement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'l':
            case '+':
            case '-':
            case '!':
            case '~':
            case 'n':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement1AssignStatement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1AssignStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AssignStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseInline1_AssignStatement_1();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount += elementParsedCount;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseAssignExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(";", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_AssignStatement_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'l':
                parsedCount = this->statement1Inline1_AssignStatement_1();
                if (parsedCount == -1) parsedCount = this->statement2Inline1_AssignStatement_1();
            break;

            case '+':
            case '-':
            case '!':
            case '~':
            case 'n':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement2Inline1_AssignStatement_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_AssignStatement_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AssignStatement;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseList();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2Inline1_AssignStatement_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AssignStatement;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseUnaryExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseList()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'l':
                parsedCount = this->statement1List();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1List()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::List;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("list", 4);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("(", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpressionList();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(")", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseAssignExpression()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '=':
            case '.':
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
                parsedCount = this->statement1AssignExpression();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1AssignExpression()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AssignExpression;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseInline1_AssignExpression_1();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount += elementParsedCount;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_AssignExpression_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '=':
                parsedCount = this->statement1Inline1_AssignExpression_1();
            break;

            case '.':
                parsedCount = this->statement2Inline1_AssignExpression_1();
            break;

            case '+':
                parsedCount = this->statement3Inline1_AssignExpression_1();
            break;

            case '-':
                parsedCount = this->statement4Inline1_AssignExpression_1();
            break;

            case '*':
                parsedCount = this->statement5Inline1_AssignExpression_1();
            break;

            case '/':
                parsedCount = this->statement6Inline1_AssignExpression_1();
            break;

            case '%':
                parsedCount = this->statement7Inline1_AssignExpression_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_AssignExpression_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AssignExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("=", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2Inline1_AssignExpression_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AssignExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(".=", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement3Inline1_AssignExpression_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AssignExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("+=", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement4Inline1_AssignExpression_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AssignExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("-=", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement5Inline1_AssignExpression_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AssignExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("*=", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement6Inline1_AssignExpression_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AssignExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("/=", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement7Inline1_AssignExpression_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AssignExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("%=", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseExpressionStatement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '+':
            case '-':
            case '!':
            case '~':
            case 'n':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement1ExpressionStatement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1ExpressionStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ExpressionStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(";", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseVariableReference()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\\':
            case '$':
                parsedCount = this->statement1VariableReference();
            break;

            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
                parsedCount = this->statement1VariableReference();
                if (parsedCount == -1) parsedCount = this->statement1VariableReference();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1VariableReference()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::VariableReference;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseInline1_VariableReference_1();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseInline2_VariableReference_1();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount += elementParsedCount;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_VariableReference_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\\':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
                parsedCount = this->statement1Inline1_VariableReference_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_VariableReference_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::VariableReference;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseClassReference();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("::", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline2_VariableReference_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '$':
                parsedCount = this->statement1Inline2_VariableReference_1();
            break;

            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
                parsedCount = this->statement2Inline2_VariableReference_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline2_VariableReference_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::VariableReference;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseVariableName();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::VariableName, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2Inline2_VariableReference_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::VariableReference;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseConstantName();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::ConstantName, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    size_t parseConstantName()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
                parsedSize = this->statement1ConstantName();
            break;
        };

        return parsedSize;
    }

    size_t statement1ConstantName()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseID();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    virtual ssize_t parseAccessList()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '-':
            case '+':
            case '[':
                parsedCount = this->statement1AccessList();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1AccessList()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AccessList;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            expressionInitialStreamPos = this->stream->getPos();
            expressionInitialPoolSize = this->pool.size();quantifierParsedElementCount = 0;
            while (true) {
                elementParsedCount = this->parseAccessElement();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount++;
                
                quantifierParsedElementCount++;
            }
            if (quantifierParsedElementCount < 1) {
                this->stream->setPos(expressionInitialStreamPos);
                this->pool.resize(expressionInitialPoolSize);
                break;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseAccessElement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '-':
                parsedCount = this->statement1AccessElement();
                if (parsedCount == -1) parsedCount = this->statement2AccessElement();
            break;

            case '+':
                parsedCount = this->statement1AccessElement();
            break;

            case '[':
                parsedCount = this->statement3AccessElement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1AccessElement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AccessElement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseMethodCall();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2AccessElement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AccessElement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parsePropertyAccess();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement3AccessElement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AccessElement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseArrayElementAccess();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parsePropertyAccess()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '-':
                parsedCount = this->statement1PropertyAccess();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1PropertyAccess()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::PropertyAccess;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("->", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseID();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::ID, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseMethodCall()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '-':
                parsedCount = this->statement1MethodCall();
                if (parsedCount == -1) parsedCount = this->statement2MethodCall();
            break;

            case '+':
                parsedCount = this->statement2MethodCall();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1MethodCall()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::MethodCall;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("->", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseFunctionCall();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2MethodCall()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::MethodCall;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseInline1_MethodCall_2();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount += elementParsedCount;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_MethodCall_2()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '+':
                parsedCount = this->statement1Inline1_MethodCall_2();
            break;

            case '-':
                parsedCount = this->statement2Inline1_MethodCall_2();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_MethodCall_2()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::MethodCall;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("++", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2Inline1_MethodCall_2()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::MethodCall;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("--", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseExpressionList()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '+':
            case '-':
            case '!':
            case '~':
            case 'n':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement1ExpressionList();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1ExpressionList()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ExpressionList;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            while (true) {
                elementParsedCount = this->parseInline1_ExpressionList_1();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_ExpressionList_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case ',':
                parsedCount = this->statement1Inline1_ExpressionList_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_ExpressionList_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ExpressionList;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(",", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseArrayElementAccess()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '[':
                parsedCount = this->statement1ArrayElementAccess();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1ArrayElementAccess()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ArrayElementAccess;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("[", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("]", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseIfStatement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'i':
                parsedCount = this->statement1IfStatement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1IfStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::IfStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("if", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("(", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(")", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseStatementBlock();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            while (true) {
                elementParsedCount = this->parseInline1_IfStatement_1();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseInline2_IfStatement_1();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_IfStatement_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'e':
                parsedCount = this->statement1Inline1_IfStatement_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_IfStatement_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::IfStatement;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("elseif", 6);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("(", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(")", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseStatementBlock();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline2_IfStatement_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'e':
                parsedCount = this->statement1Inline2_IfStatement_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline2_IfStatement_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::IfStatement;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("else", 4);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseStatementBlock();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseStatementBlock()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'i':
            case 'f':
            case 'r':
            case 'b':
            case 'c':
            case 'w':
            case 'd':
            case 's':
            case 't':
            case '/':
            case 'l':
            case '+':
            case '-':
            case '!':
            case '~':
            case 'n':
            case 'a':
            case 'e':
            case 'g':
            case 'h':
            case 'j':
            case 'k':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'u':
            case 'v':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement1StatementBlock();
            break;

            case '{':
                parsedCount = this->statement2StatementBlock();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1StatementBlock()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::StatementBlock;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseStatement();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2StatementBlock()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::StatementBlock;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("{", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseStatementList();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("}", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseForStatement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'f':
                parsedCount = this->statement1ForStatement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1ForStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ForStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("for", 3);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("(", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(";", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(";", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(")", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseStatementBlock();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseForeachStatement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'f':
                parsedCount = this->statement1ForeachStatement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1ForeachStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ForeachStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("foreach", 7);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("(", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("as", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseForeachKey();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseReferenceModifier();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseVariableName();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::VariableName, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(")", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseStatementBlock();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseForeachKey()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '$':
                parsedCount = this->statement1ForeachKey();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1ForeachKey()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ForeachKey;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseVariableName();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::VariableName, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("=>", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseReturnStatement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'r':
                parsedCount = this->statement1ReturnStatement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1ReturnStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ReturnStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("return", 6);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(";", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseBreakStatement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'b':
                parsedCount = this->statement1BreakStatement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1BreakStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::BreakStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("break", 5);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(";", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseContinueStatement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'c':
                parsedCount = this->statement1ContinueStatement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1ContinueStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ContinueStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("continue", 8);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(";", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseThrowStatement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 't':
                parsedCount = this->statement1ThrowStatement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1ThrowStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ThrowStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("throw", 5);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(";", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseTryStatement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 't':
                parsedCount = this->statement1TryStatement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1TryStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::TryStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("try", 3);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("{", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseStatementList();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("}", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseCatchStatement();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseCatchStatement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'c':
                parsedCount = this->statement1CatchStatement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1CatchStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::CatchStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("catch", 5);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("(", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseArgument();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(")", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("{", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseStatementList();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("}", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseWhileStatement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'w':
                parsedCount = this->statement1WhileStatement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1WhileStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::WhileStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("while", 5);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("(", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(")", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseStatementBlock();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseDoWhileStatement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'd':
                parsedCount = this->statement1DoWhileStatement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1DoWhileStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::DoWhileStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("do", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseStatementBlock();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("while", 5);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("(", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(")", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(";", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseSwitchStatement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 's':
                parsedCount = this->statement1SwitchStatement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1SwitchStatement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::SwitchStatement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("switch", 6);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("{", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseCaseList();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("}", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseCaseList()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'c':
                parsedCount = this->statement1CaseList();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1CaseList()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::CaseList;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            expressionInitialStreamPos = this->stream->getPos();
            expressionInitialPoolSize = this->pool.size();quantifierParsedElementCount = 0;
            while (true) {
                elementParsedCount = this->parseCase();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount++;
                
                quantifierParsedElementCount++;
            }
            if (quantifierParsedElementCount < 1) {
                this->stream->setPos(expressionInitialStreamPos);
                this->pool.resize(expressionInitialPoolSize);
                break;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseCase()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'c':
                parsedCount = this->statement1Case();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Case()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Case;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("case", 4);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(":", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseStatementList();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseExpression()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '+':
            case '-':
            case '!':
            case '~':
            case 'n':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement1Expression();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Expression()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Expression;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseTernaryExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseAssignExpression();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseTernaryExpression()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '+':
            case '-':
            case '!':
            case '~':
            case 'n':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement1TernaryExpression();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1TernaryExpression()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::TernaryExpression;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseLogicalOrExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            while (true) {
                elementParsedCount = this->parseInline1_TernaryExpression_1();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_TernaryExpression_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '?':
                parsedCount = this->statement1Inline1_TernaryExpression_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_TernaryExpression_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::TernaryExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("?", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(":", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseLogicalOrExpression()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '+':
            case '-':
            case '!':
            case '~':
            case 'n':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement1LogicalOrExpression();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1LogicalOrExpression()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::LogicalOrExpression;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseLogicalAndExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            while (true) {
                elementParsedCount = this->parseInline1_LogicalOrExpression_1();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_LogicalOrExpression_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '|':
                parsedCount = this->statement1Inline1_LogicalOrExpression_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_LogicalOrExpression_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::LogicalOrExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("||", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseLogicalAndExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseLogicalAndExpression()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '+':
            case '-':
            case '!':
            case '~':
            case 'n':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement1LogicalAndExpression();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1LogicalAndExpression()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::LogicalAndExpression;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseCompareExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            while (true) {
                elementParsedCount = this->parseInline1_LogicalAndExpression_1();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_LogicalAndExpression_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '&':
                parsedCount = this->statement1Inline1_LogicalAndExpression_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_LogicalAndExpression_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::LogicalAndExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("&&", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseCompareExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseCompareExpression()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '+':
            case '-':
            case '!':
            case '~':
            case 'n':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement1CompareExpression();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1CompareExpression()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::CompareExpression;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseNullCoalesceExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            while (true) {
                elementParsedCount = this->parseInline1_CompareExpression_1();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_CompareExpression_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '=':
            case '!':
            case '>':
            case '<':
                parsedCount = this->statement1Inline1_CompareExpression_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_CompareExpression_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::CompareExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseInline1_Inline1_CompareExpression_1_1();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount += elementParsedCount;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseNullCoalesceExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_Inline1_CompareExpression_1_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '=':
                parsedCount = this->statement1Inline1_Inline1_CompareExpression_1_1();
                if (parsedCount == -1) parsedCount = this->statement3Inline1_Inline1_CompareExpression_1_1();
            break;

            case '!':
                parsedCount = this->statement2Inline1_Inline1_CompareExpression_1_1();
                if (parsedCount == -1) parsedCount = this->statement4Inline1_Inline1_CompareExpression_1_1();
            break;

            case '>':
                parsedCount = this->statement5Inline1_Inline1_CompareExpression_1_1();
                if (parsedCount == -1) parsedCount = this->statement7Inline1_Inline1_CompareExpression_1_1();
            break;

            case '<':
                parsedCount = this->statement6Inline1_Inline1_CompareExpression_1_1();
                if (parsedCount == -1) parsedCount = this->statement8Inline1_Inline1_CompareExpression_1_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_Inline1_CompareExpression_1_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::CompareExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("===", 3);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2Inline1_Inline1_CompareExpression_1_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::CompareExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("!==", 3);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement3Inline1_Inline1_CompareExpression_1_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::CompareExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("==", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement4Inline1_Inline1_CompareExpression_1_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::CompareExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("!=", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement5Inline1_Inline1_CompareExpression_1_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::CompareExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(">=", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement6Inline1_Inline1_CompareExpression_1_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::CompareExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("<=", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement7Inline1_Inline1_CompareExpression_1_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::CompareExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(">", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement8Inline1_Inline1_CompareExpression_1_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::CompareExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("<", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseNullCoalesceExpression()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '+':
            case '-':
            case '!':
            case '~':
            case 'n':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement1NullCoalesceExpression();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1NullCoalesceExpression()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::NullCoalesceExpression;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseAddExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            while (true) {
                elementParsedCount = this->parseInline1_NullCoalesceExpression_1();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_NullCoalesceExpression_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '?':
                parsedCount = this->statement1Inline1_NullCoalesceExpression_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_NullCoalesceExpression_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::NullCoalesceExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("??", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseAddExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseAddExpression()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '+':
            case '-':
            case '!':
            case '~':
            case 'n':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement1AddExpression();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1AddExpression()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AddExpression;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseMulExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            while (true) {
                elementParsedCount = this->parseInline1_AddExpression_1();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_AddExpression_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '+':
            case '-':
            case '.':
            case '&':
            case '|':
            case '^':
                parsedCount = this->statement1Inline1_AddExpression_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_AddExpression_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AddExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseInline1_Inline1_AddExpression_1_1();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount += elementParsedCount;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseMulExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_Inline1_AddExpression_1_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '+':
                parsedCount = this->statement1Inline1_Inline1_AddExpression_1_1();
            break;

            case '-':
                parsedCount = this->statement2Inline1_Inline1_AddExpression_1_1();
            break;

            case '.':
                parsedCount = this->statement3Inline1_Inline1_AddExpression_1_1();
            break;

            case '&':
                parsedCount = this->statement4Inline1_Inline1_AddExpression_1_1();
            break;

            case '|':
                parsedCount = this->statement5Inline1_Inline1_AddExpression_1_1();
            break;

            case '^':
                parsedCount = this->statement6Inline1_Inline1_AddExpression_1_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_Inline1_AddExpression_1_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AddExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("+", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2Inline1_Inline1_AddExpression_1_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AddExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("-", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement3Inline1_Inline1_AddExpression_1_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AddExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(".", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement4Inline1_Inline1_AddExpression_1_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AddExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("&", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement5Inline1_Inline1_AddExpression_1_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AddExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("|", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement6Inline1_Inline1_AddExpression_1_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::AddExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("^", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseMulExpression()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '+':
            case '-':
            case '!':
            case '~':
            case 'n':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement1MulExpression();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1MulExpression()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::MulExpression;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseUnaryExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            while (true) {
                elementParsedCount = this->parseInline1_MulExpression_1();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_MulExpression_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '*':
            case '/':
            case '%':
                parsedCount = this->statement1Inline1_MulExpression_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_MulExpression_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::MulExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseInline1_Inline1_MulExpression_1_1();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount += elementParsedCount;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseUnaryExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_Inline1_MulExpression_1_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '*':
                parsedCount = this->statement1Inline1_Inline1_MulExpression_1_1();
            break;

            case '/':
                parsedCount = this->statement2Inline1_Inline1_MulExpression_1_1();
            break;

            case '%':
                parsedCount = this->statement3Inline1_Inline1_MulExpression_1_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_Inline1_MulExpression_1_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::MulExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("*", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2Inline1_Inline1_MulExpression_1_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::MulExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("/", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement3Inline1_Inline1_MulExpression_1_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::MulExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("%", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseUnaryExpression()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '+':
            case '-':
            case '!':
            case '~':
            case 'n':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement1UnaryExpression();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1UnaryExpression()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::UnaryExpression;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseInline1_UnaryExpression_1();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parsePrimaryExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseAccessList();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_UnaryExpression_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '+':
                parsedCount = this->statement1Inline1_UnaryExpression_1();
            break;

            case '-':
                parsedCount = this->statement2Inline1_UnaryExpression_1();
            break;

            case '!':
                parsedCount = this->statement3Inline1_UnaryExpression_1();
            break;

            case '~':
                parsedCount = this->statement4Inline1_UnaryExpression_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_UnaryExpression_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::UnaryExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("+", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2Inline1_UnaryExpression_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::UnaryExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("-", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement3Inline1_UnaryExpression_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::UnaryExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("!", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement4Inline1_UnaryExpression_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::UnaryExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("~", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parsePrimaryExpression()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'n':
                parsedCount = this->statement1PrimaryExpression();
                if (parsedCount == -1) parsedCount = this->statement2PrimaryExpression();
                if (parsedCount == -1) parsedCount = this->statement3PrimaryExpression();
                if (parsedCount == -1) parsedCount = this->statement4PrimaryExpression();
            break;

            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
                parsedCount = this->statement2PrimaryExpression();
                if (parsedCount == -1) parsedCount = this->statement3PrimaryExpression();
            break;

            case 'f':
            case 't':
                parsedCount = this->statement2PrimaryExpression();
                if (parsedCount == -1) parsedCount = this->statement3PrimaryExpression();
                if (parsedCount == -1) parsedCount = this->statement4PrimaryExpression();
            break;

            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
                parsedCount = this->statement4PrimaryExpression();
            break;

            case '(':
                parsedCount = this->statement5PrimaryExpression();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1PrimaryExpression()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::PrimaryExpression;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseNewExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2PrimaryExpression()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::PrimaryExpression;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseFunctionCall();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement3PrimaryExpression()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::PrimaryExpression;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseVariableReference();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement4PrimaryExpression()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::PrimaryExpression;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseLiteral();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement5PrimaryExpression()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::PrimaryExpression;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("(", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(")", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseNewExpression()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'n':
                parsedCount = this->statement1NewExpression();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1NewExpression()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::NewExpression;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("new", 3);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseClassReference();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseInline1_NewExpression_1();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_NewExpression_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '(':
                parsedCount = this->statement1Inline1_NewExpression_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_NewExpression_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::NewExpression;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("(", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpressionList();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(")", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseFunctionCall()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
                parsedCount = this->statement1FunctionCall();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1FunctionCall()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::FunctionCall;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseInline1_FunctionCall_1();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount += elementParsedCount;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("(", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpressionList();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(")", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_FunctionCall_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
                parsedCount = this->statement1Inline1_FunctionCall_1();
                if (parsedCount == -1) parsedCount = this->statement2Inline1_FunctionCall_1();
            break;

            case '\\':
            case '$':
                parsedCount = this->statement2Inline1_FunctionCall_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_FunctionCall_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::FunctionCall;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseFunctionName();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::FunctionName, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2Inline1_FunctionCall_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::FunctionCall;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseVariableReference();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseLiteral()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\'':
            case '"':
            case '<':
                parsedCount = this->statement1Literal();
            break;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                parsedCount = this->statement2Literal();
            break;

            case '[':
                parsedCount = this->statement3Literal();
            break;

            case 'n':
                parsedCount = this->statement4Literal();
            break;

            case 'f':
            case 't':
                parsedCount = this->statement5Literal();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Literal()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Literal;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseStringLiteral();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2Literal()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Literal;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseNumberLiteral();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement3Literal()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Literal;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseArrayLiteral();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement4Literal()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Literal;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseNullLiteral();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement5Literal()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Literal;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseLogicalLiteral();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseNullLiteral()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'n':
                parsedCount = this->statement1NullLiteral();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1NullLiteral()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::NullLiteral;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("null", 4);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseLogicalLiteral()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'f':
                parsedCount = this->statement1LogicalLiteral();
            break;

            case 't':
                parsedCount = this->statement2LogicalLiteral();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1LogicalLiteral()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::LogicalLiteral;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("false", 5);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2LogicalLiteral()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::LogicalLiteral;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("true", 4);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseNumberLiteral()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '0':
                parsedCount = this->statement1NumberLiteral();
                if (parsedCount == -1) parsedCount = this->statement2NumberLiteral();
            break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                parsedCount = this->statement2NumberLiteral();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1NumberLiteral()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::NumberLiteral;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseHexNumberLiteral();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2NumberLiteral()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::NumberLiteral;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseDecimalNumberLiteral();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseDecimalNumberLiteral()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                parsedCount = this->statement1DecimalNumberLiteral();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1DecimalNumberLiteral()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::DecimalNumberLiteral;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            expressionInitialStreamPos = this->stream->getPos();
            expressionInitialPoolSize = this->pool.size();quantifierParsedElementCount = 0;
            while (true) {
                elementInitialStreamPos = this->stream->getPos();
                elementParsedSize = this->systemParseRegexp("09", 2);
    
                isElementParsed = (elementParsedSize != 0);
                if (!isElementParsed) break;
                
                
                quantifierParsedElementCount++;
            }
            if (quantifierParsedElementCount < 1) {
                this->stream->setPos(expressionInitialStreamPos);
                this->pool.resize(expressionInitialPoolSize);
                break;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseInline1_DecimalNumberLiteral_1();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_DecimalNumberLiteral_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '.':
                parsedCount = this->statement1Inline1_DecimalNumberLiteral_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_DecimalNumberLiteral_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::DecimalNumberLiteral;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(".", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            expressionInitialStreamPos = this->stream->getPos();
            expressionInitialPoolSize = this->pool.size();quantifierParsedElementCount = 0;
            while (true) {
                elementInitialStreamPos = this->stream->getPos();
                elementParsedSize = this->systemParseRegexp("09", 2);
    
                isElementParsed = (elementParsedSize != 0);
                if (!isElementParsed) break;
                
                
                quantifierParsedElementCount++;
            }
            if (quantifierParsedElementCount < 1) {
                this->stream->setPos(expressionInitialStreamPos);
                this->pool.resize(expressionInitialPoolSize);
                break;
            }

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseHexNumberLiteral()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '0':
                parsedCount = this->statement1HexNumberLiteral();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1HexNumberLiteral()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::HexNumberLiteral;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("0x", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            expressionInitialStreamPos = this->stream->getPos();
            expressionInitialPoolSize = this->pool.size();quantifierParsedElementCount = 0;
            while (true) {
                elementInitialStreamPos = this->stream->getPos();
                elementParsedSize = this->systemParseRegexp("09AF", 4);
    
                isElementParsed = (elementParsedSize != 0);
                if (!isElementParsed) break;
                
                
                quantifierParsedElementCount++;
            }
            if (quantifierParsedElementCount < 1) {
                this->stream->setPos(expressionInitialStreamPos);
                this->pool.resize(expressionInitialPoolSize);
                break;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseStringLiteral()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\'':
                parsedCount = this->statement1StringLiteral();
            break;

            case '"':
                parsedCount = this->statement2StringLiteral();
            break;

            case '<':
                parsedCount = this->statement3StringLiteral();
                if (parsedCount == -1) parsedCount = this->statement4StringLiteral();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1StringLiteral()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::StringLiteral;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseSingleQuoteString();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2StringLiteral()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::StringLiteral;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseDoubleQuoteString();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement3StringLiteral()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::StringLiteral;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseNowdocString();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement4StringLiteral()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::StringLiteral;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseHeredocString();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseSingleQuoteString()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\'':
                parsedCount = this->statement1SingleQuoteString();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1SingleQuoteString()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::SingleQuoteString;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("'", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            while (true) {
                elementInitialStreamPos = this->stream->getPos();
                elementInitialPoolSize = this->pool.size();
                elementInitialStreamPos = this->stream->getPos();
                elementParsedSize = this->systemParseString("'", 1);
    
                isElementParsed = (elementParsedSize != 0);
                if (isElementParsed) {
                    this->stream->setPos(elementInitialStreamPos);
                    this->pool.resize(elementInitialPoolSize);isElementParsed = false;
                }
                else {
                    elementParsedCount = this->parseSymbol();
        
                    isElementParsed = (elementParsedCount != -1);
                }
                if (!isElementParsed) break;
                totalParsedElementCount++;
            }

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("'", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseDoubleQuoteString()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '"':
                parsedCount = this->statement1DoubleQuoteString();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1DoubleQuoteString()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::DoubleQuoteString;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("\"", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            while (true) {
                elementInitialStreamPos = this->stream->getPos();
                elementInitialPoolSize = this->pool.size();
                elementInitialStreamPos = this->stream->getPos();
                elementParsedSize = this->systemParseString("\"", 1);
    
                isElementParsed = (elementParsedSize != 0);
                if (isElementParsed) {
                    this->stream->setPos(elementInitialStreamPos);
                    this->pool.resize(elementInitialPoolSize);isElementParsed = false;
                }
                else {
                    elementParsedCount = this->parseSymbol();
        
                    isElementParsed = (elementParsedCount != -1);
                }
                if (!isElementParsed) break;
                totalParsedElementCount++;
            }

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("\"", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseNowdocString()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '<':
                parsedCount = this->statement1NowdocString();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1NowdocString()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::NowdocString;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("<<<'", 4);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseStringTag();
            
            this->setStringStart(elementParsedSize);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::StringTag, lexemeValue);
            totalParsedElementCount++;

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("'", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("\x0A", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            while (true) {
                elementInitialStreamPos = this->stream->getPos();
                elementInitialPoolSize = this->pool.size();
                elementParsedCount = this->parseInline1_NowdocString_1();
    
                isElementParsed = (elementParsedCount != -1);
                if (isElementParsed) {
                    this->stream->setPos(elementInitialStreamPos);
                    this->pool.resize(elementInitialPoolSize);isElementParsed = false;
                }
                else {
                    elementParsedCount = this->parseSymbol();
        
                    isElementParsed = (elementParsedCount != -1);
                }
                if (!isElementParsed) break;
                totalParsedElementCount++;
            }

            // ------------------------------------------------

            elementParsedCount = this->parseInline2_NowdocString_1();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount += elementParsedCount;

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_NowdocString_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\x0A':
                parsedCount = this->statement1Inline1_NowdocString_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_NowdocString_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::NowdocString;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("\x0A", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseStringTag();
            
            this->checkStringEnd(elementParsedSize);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::StringTag, lexemeValue);
            totalParsedElementCount++;

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline2_NowdocString_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\x0A':
                parsedCount = this->statement1Inline2_NowdocString_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline2_NowdocString_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::NowdocString;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("\x0A", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseStringTag();
            
            this->checkStringEnd(elementParsedSize);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::StringTag, lexemeValue);
            totalParsedElementCount++;

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseHeredocString()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '<':
                parsedCount = this->statement1HeredocString();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1HeredocString()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::HeredocString;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("<<<", 3);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseStringTag();
            
            this->setStringStart(elementParsedSize);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::StringTag, lexemeValue);
            totalParsedElementCount++;

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("\x0A", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            while (true) {
                elementInitialStreamPos = this->stream->getPos();
                elementInitialPoolSize = this->pool.size();
                elementParsedCount = this->parseInline1_HeredocString_1();
    
                isElementParsed = (elementParsedCount != -1);
                if (isElementParsed) {
                    this->stream->setPos(elementInitialStreamPos);
                    this->pool.resize(elementInitialPoolSize);isElementParsed = false;
                }
                else {
                    elementParsedCount = this->parseSymbol();
        
                    isElementParsed = (elementParsedCount != -1);
                }
                if (!isElementParsed) break;
                totalParsedElementCount++;
            }

            // ------------------------------------------------

            elementParsedCount = this->parseInline2_HeredocString_1();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount += elementParsedCount;

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_HeredocString_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\x0A':
                parsedCount = this->statement1Inline1_HeredocString_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_HeredocString_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::HeredocString;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("\x0A", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseStringTag();
            
            this->checkStringEnd(elementParsedSize);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::StringTag, lexemeValue);
            totalParsedElementCount++;

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline2_HeredocString_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\x0A':
                parsedCount = this->statement1Inline2_HeredocString_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline2_HeredocString_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::HeredocString;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("\x0A", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseStringTag();
            
            this->checkStringEnd(elementParsedSize);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::StringTag, lexemeValue);
            totalParsedElementCount++;

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    size_t parseStringTag()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
                parsedSize = this->statement1StringTag();
            break;
        };

        return parsedSize;
    }

    size_t statement1StringTag()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            expressionInitialStreamPos = this->stream->getPos();
            quantifierParsedElementCount = 0;
            while (true) {
                elementInitialStreamPos = this->stream->getPos();
                elementParsedSize = this->systemParseRegexp("azAZ", 4);
    
                isElementParsed = (elementParsedSize != 0);
                if (!isElementParsed) break;
                parsedSize += elementParsedSize;
                
                quantifierParsedElementCount++;
            }
            if (quantifierParsedElementCount < 1) {
                this->stream->setPos(expressionInitialStreamPos);
                
                break;
            }

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    virtual ssize_t parseSymbol()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\\':
                parsedCount = this->statement1Symbol();
                if (parsedCount == -1) parsedCount = this->statement2Symbol();
                if (parsedCount == -1) parsedCount = this->statement3Symbol();
            break;

            case '\x00':
            case '\x01':
            case '\x02':
            case '\x03':
            case '\x04':
            case '\x05':
            case '\x06':
            case '\x07':
            case '\x08':
            case '\x09':
            case '\x0A':
            case '\x0B':
            case '\x0C':
            case '\x0D':
            case '\x0E':
            case '\x0F':
            case '\x10':
            case '\x11':
            case '\x12':
            case '\x13':
            case '\x14':
            case '\x15':
            case '\x16':
            case '\x17':
            case '\x18':
            case '\x19':
            case '\x1A':
            case '\x1B':
            case '\x1C':
            case '\x1D':
            case '\x1E':
            case '\x1F':
            case '\x20':
            case '!':
            case '"':
            case '#':
            case '$':
            case '%':
            case '&':
            case '\'':
            case '(':
            case ')':
            case '*':
            case '+':
            case ',':
            case '-':
            case '.':
            case '/':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case ':':
            case ';':
            case '<':
            case '=':
            case '>':
            case '?':
            case '@':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '[':
            case ']':
            case '^':
            case '_':
            case '`':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case '{':
            case '|':
            case '}':
            case '~':
            case '':
            case '\x80':
            case '\x81':
            case '\x82':
            case '\x83':
            case '\x84':
            case '\x85':
            case '\x86':
            case '\x87':
            case '\x88':
            case '\x89':
            case '\x8A':
            case '\x8B':
            case '\x8C':
            case '\x8D':
            case '\x8E':
            case '\x8F':
            case '\x90':
            case '\x91':
            case '\x92':
            case '\x93':
            case '\x94':
            case '\x95':
            case '\x96':
            case '\x97':
            case '\x98':
            case '\x99':
            case '\x9A':
            case '\x9B':
            case '\x9C':
            case '\x9D':
            case '\x9E':
            case '\x9F':
            case '\xA0':
            case '\xA1':
            case '\xA2':
            case '\xA3':
            case '\xA4':
            case '\xA5':
            case '\xA6':
            case '\xA7':
            case '\xA8':
            case '\xA9':
            case '\xAA':
            case '\xAB':
            case '\xAC':
            case '\xAD':
            case '\xAE':
            case '\xAF':
            case '\xB0':
            case '\xB1':
            case '\xB2':
            case '\xB3':
            case '\xB4':
            case '\xB5':
            case '\xB6':
            case '\xB7':
            case '\xB8':
            case '\xB9':
            case '\xBA':
            case '\xBB':
            case '\xBC':
            case '\xBD':
            case '\xBE':
            case '\xBF':
            case '\xC0':
            case '\xC1':
            case '\xC2':
            case '\xC3':
            case '\xC4':
            case '\xC5':
            case '\xC6':
            case '\xC7':
            case '\xC8':
            case '\xC9':
            case '\xCA':
            case '\xCB':
            case '\xCC':
            case '\xCD':
            case '\xCE':
            case '\xCF':
            case '\xD0':
            case '\xD1':
            case '\xD2':
            case '\xD3':
            case '\xD4':
            case '\xD5':
            case '\xD6':
            case '\xD7':
            case '\xD8':
            case '\xD9':
            case '\xDA':
            case '\xDB':
            case '\xDC':
            case '\xDD':
            case '\xDE':
            case '\xDF':
            case '\xE0':
            case '\xE1':
            case '\xE2':
            case '\xE3':
            case '\xE4':
            case '\xE5':
            case '\xE6':
            case '\xE7':
            case '\xE8':
            case '\xE9':
            case '\xEA':
            case '\xEB':
            case '\xEC':
            case '\xED':
            case '\xEE':
            case '\xEF':
            case '\xF0':
            case '\xF1':
            case '\xF2':
            case '\xF3':
            case '\xF4':
            case '\xF5':
            case '\xF6':
            case '\xF7':
            case '\xF8':
            case '\xF9':
            case '\xFA':
            case '\xFB':
            case '\xFC':
            case '\xFD':
            case '\xFE':
            case '\xFF':
                parsedCount = this->statement3Symbol();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Symbol()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Symbol;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseHexCodeSymbol();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement2Symbol()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Symbol;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseEscapedSymbol();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t statement3Symbol()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::Symbol;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseAnySymbol();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::AnySymbol, lexemeValue);
            totalParsedElementCount++;

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseHexCodeSymbol()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\\':
                parsedCount = this->statement1HexCodeSymbol();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1HexCodeSymbol()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::HexCodeSymbol;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("\\x", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseHexDigit();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::HexDigit, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseHexDigit();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::HexDigit, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    size_t parseHexDigit()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
                parsedSize = this->statement1HexDigit();
            break;
        };

        return parsedSize;
    }

    size_t statement1HexDigit()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseRegexp("09AF", 4);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    virtual ssize_t parseEscapedSymbol()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\\':
                parsedCount = this->statement1EscapedSymbol();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1EscapedSymbol()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::EscapedSymbol;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("\\", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->parseAnySymbol();

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            lexemeValue = StringDescriptor(this->stream->getContent().getDataPtr() + elementInitialStreamPos, elementParsedSize);
            this->createGdlNode(RuleName::AnySymbol, lexemeValue);
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    size_t parseAnySymbol()
    {
        size_t parsedSize = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\x00':
            case '\x01':
            case '\x02':
            case '\x03':
            case '\x04':
            case '\x05':
            case '\x06':
            case '\x07':
            case '\x08':
            case '\x09':
            case '\x0A':
            case '\x0B':
            case '\x0C':
            case '\x0D':
            case '\x0E':
            case '\x0F':
            case '\x10':
            case '\x11':
            case '\x12':
            case '\x13':
            case '\x14':
            case '\x15':
            case '\x16':
            case '\x17':
            case '\x18':
            case '\x19':
            case '\x1A':
            case '\x1B':
            case '\x1C':
            case '\x1D':
            case '\x1E':
            case '\x1F':
            case '\x20':
            case '!':
            case '"':
            case '#':
            case '$':
            case '%':
            case '&':
            case '\'':
            case '(':
            case ')':
            case '*':
            case '+':
            case ',':
            case '-':
            case '.':
            case '/':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case ':':
            case ';':
            case '<':
            case '=':
            case '>':
            case '?':
            case '@':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '[':
            case '\\':
            case ']':
            case '^':
            case '_':
            case '`':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case '{':
            case '|':
            case '}':
            case '~':
            case '':
            case '\x80':
            case '\x81':
            case '\x82':
            case '\x83':
            case '\x84':
            case '\x85':
            case '\x86':
            case '\x87':
            case '\x88':
            case '\x89':
            case '\x8A':
            case '\x8B':
            case '\x8C':
            case '\x8D':
            case '\x8E':
            case '\x8F':
            case '\x90':
            case '\x91':
            case '\x92':
            case '\x93':
            case '\x94':
            case '\x95':
            case '\x96':
            case '\x97':
            case '\x98':
            case '\x99':
            case '\x9A':
            case '\x9B':
            case '\x9C':
            case '\x9D':
            case '\x9E':
            case '\x9F':
            case '\xA0':
            case '\xA1':
            case '\xA2':
            case '\xA3':
            case '\xA4':
            case '\xA5':
            case '\xA6':
            case '\xA7':
            case '\xA8':
            case '\xA9':
            case '\xAA':
            case '\xAB':
            case '\xAC':
            case '\xAD':
            case '\xAE':
            case '\xAF':
            case '\xB0':
            case '\xB1':
            case '\xB2':
            case '\xB3':
            case '\xB4':
            case '\xB5':
            case '\xB6':
            case '\xB7':
            case '\xB8':
            case '\xB9':
            case '\xBA':
            case '\xBB':
            case '\xBC':
            case '\xBD':
            case '\xBE':
            case '\xBF':
            case '\xC0':
            case '\xC1':
            case '\xC2':
            case '\xC3':
            case '\xC4':
            case '\xC5':
            case '\xC6':
            case '\xC7':
            case '\xC8':
            case '\xC9':
            case '\xCA':
            case '\xCB':
            case '\xCC':
            case '\xCD':
            case '\xCE':
            case '\xCF':
            case '\xD0':
            case '\xD1':
            case '\xD2':
            case '\xD3':
            case '\xD4':
            case '\xD5':
            case '\xD6':
            case '\xD7':
            case '\xD8':
            case '\xD9':
            case '\xDA':
            case '\xDB':
            case '\xDC':
            case '\xDD':
            case '\xDE':
            case '\xDF':
            case '\xE0':
            case '\xE1':
            case '\xE2':
            case '\xE3':
            case '\xE4':
            case '\xE5':
            case '\xE6':
            case '\xE7':
            case '\xE8':
            case '\xE9':
            case '\xEA':
            case '\xEB':
            case '\xEC':
            case '\xED':
            case '\xEE':
            case '\xEF':
            case '\xF0':
            case '\xF1':
            case '\xF2':
            case '\xF3':
            case '\xF4':
            case '\xF5':
            case '\xF6':
            case '\xF7':
            case '\xF8':
            case '\xF9':
            case '\xFA':
            case '\xFB':
            case '\xFC':
            case '\xFD':
            case '\xFE':
            case '\xFF':
                parsedSize = this->statement1AnySymbol();
            break;
        };

        return parsedSize;
    }

    size_t statement1AnySymbol()
    {
        size_t parsedSize = 0;
        
        size_t initialStreamPos = this->stream->getPos();

        size_t elementInitialStreamPos = 0;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;
        
        size_t expressionInitialStreamPos = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseRegexp("\x00\xFF", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;
            parsedSize += elementParsedSize;

            // ------------------------------------------------

            return parsedSize;

        } while (false);
        
        this->stream->setPos(initialStreamPos);
        
        return false;
    }

    virtual ssize_t parseArrayLiteral()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '[':
                parsedCount = this->statement1ArrayLiteral();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1ArrayLiteral()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ArrayLiteral;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("[", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseArrayElementList();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount++;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("]", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseArrayElementList()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '+':
            case '-':
            case '!':
            case '~':
            case 'n':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement1ArrayElementList();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1ArrayElementList()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ArrayElementList;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseArrayElement();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            while (true) {
                elementParsedCount = this->parseInline1_ArrayElementList_1();
    
                isElementParsed = (elementParsedCount != -1);
                if (!isElementParsed) break;
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(",", 1);

            isElementParsed = (elementParsedSize != 0);
            if (isElementParsed) {
                
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_ArrayElementList_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case ',':
                parsedCount = this->statement1Inline1_ArrayElementList_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_ArrayElementList_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ArrayElementList;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString(",", 1);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseArrayElement();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseArrayElement()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '+':
            case '-':
            case '!':
            case '~':
            case 'n':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '\\':
            case '$':
            case '\'':
            case '"':
            case '<':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '[':
            case '(':
                parsedCount = this->statement1ArrayElement();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1ArrayElement()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ArrayElement;
        ssize_t totalParsedElementCount = 0;

        ListDescriptor value(this->pool.size(), 0, 0);
        size_t resultNodeIndex = this->createGdlNode(ruleName, value);

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseInline1_ArrayElement_1();

            isElementParsed = (elementParsedCount != -1);
            if (isElementParsed) {
                totalParsedElementCount += elementParsedCount;
            }

            this->skipSpaces();

            // ------------------------------------------------

            this->pool[resultNodeIndex].getListValue().count = totalParsedElementCount;
            this->pool[resultNodeIndex].getListValue().totalCount = (size_t)(this->pool.size() - initialPoolSize);

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual ssize_t parseInline1_ArrayElement_1()
    {
        ssize_t parsedCount = -1;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '=':
                parsedCount = this->statement1Inline1_ArrayElement_1();
            break;
        };

        return parsedCount;
    }

    virtual ssize_t statement1Inline1_ArrayElement_1()
    {    
        size_t initialStreamPos = this->stream->getPos();
        size_t initialPoolSize = this->pool.size();

        auto ruleName = RuleName::ArrayElement;
        ssize_t totalParsedElementCount = 0;

        

        StringDescriptor lexemeValue("");

        size_t elementInitialStreamPos = 0;
        size_t elementInitialPoolSize = 0;

        ssize_t elementParsedCount = false;
        size_t elementParsedSize = 0;
        bool isElementParsed = false;

        size_t expressionInitialStreamPos = 0;
        size_t expressionInitialPoolSize = 0;

        size_t quantifierParsedElementCount = 0;
        size_t quantifierRequiredElementCount = 0;
 
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        do {
            elementInitialStreamPos = this->stream->getPos();
            elementParsedSize = this->systemParseString("=>", 2);

            isElementParsed = (elementParsedSize != 0);
            if (!isElementParsed) break;

            this->skipSpaces();

            // ------------------------------------------------

            elementParsedCount = this->parseExpression();

            isElementParsed = (elementParsedCount != -1);
            if (!isElementParsed) break;
            totalParsedElementCount++;

            this->skipSpaces();

            // ------------------------------------------------

            

            return totalParsedElementCount;

        } while (false);

        this->stream->setPos(initialStreamPos);
        this->pool.resize(initialPoolSize);

        return -1;
    }

    virtual void setStringStart(size_t&) = 0;
    virtual void checkStringEnd(size_t&) = 0;
};


const char* PhpParser::ruleNames[] = {
    "", "Program", "PhpProgram", "comment", "TopStatement", "NamespaceStatement", "NamespaceName", "NamespacePart", "ID", "UseStatement", "ClassDeclaration", "ClassName", "ClassReference", "InterfaceReference", "ClassBody", "ClassBodyStatement", "PropertyDeclaration", "PropertyName", "VariableName", "AccessModifier", "MethodDeclaration", "FunctionDeclaration", "FunctionName", "ArgumentList", "Argument", "TypeReference", "ReferenceModifier", "FunctionBody", "StatementList", "Statement", "AssignStatement", "List", "AssignExpression", "ExpressionStatement", "VariableReference", "ConstantName", "AccessList", "AccessElement", "PropertyAccess", "MethodCall", "ExpressionList", "ArrayElementAccess", "IfStatement", "StatementBlock", "ForStatement", "ForeachStatement", "ForeachKey", "ReturnStatement", "BreakStatement", "ContinueStatement", "ThrowStatement", "TryStatement", "CatchStatement", "WhileStatement", "DoWhileStatement", "SwitchStatement", "CaseList", "Case", "Expression", "TernaryExpression", "LogicalOrExpression", "LogicalAndExpression", "CompareExpression", "NullCoalesceExpression", "AddExpression", "MulExpression", "UnaryExpression", "PrimaryExpression", "NewExpression", "FunctionCall", "Literal", "NullLiteral", "LogicalLiteral", "NumberLiteral", "DecimalNumberLiteral", "HexNumberLiteral", "StringLiteral", "SingleQuoteString", "DoubleQuoteString", "NowdocString", "HeredocString", "StringTag", "Symbol", "HexCodeSymbol", "HexDigit", "EscapedSymbol", "AnySymbol", "ArrayLiteral", "ArrayElementList", "ArrayElement"
};

