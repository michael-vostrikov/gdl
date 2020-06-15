#include "StringDescriptor.h"
#include "GdlNode.h"
#include "Stream.h"

class JsonParser
{
protected:

    Stream* stream;

public:
    
    enum RuleName {
        Empty, Json,Obj,Pair,Arr,Value,String,Esc,Unicode,Hex,SafeCodePoint,Number,Int,Exp
    };

    static const char* ruleNames[];
    

    JsonParser(Stream* stream): stream(stream)
    {
    }

    size_t systemParseString(const char* string, size_t size)
    {
        size_t initialPos = this->stream->getPos();
        size_t parsedLength = 0;

        for (size_t i = 0; i < size; i++) {
            if (this->stream->eof()) {
                parsedLength = 0;
                break;
            }

            char symbol = this->stream->readSymbol();
            char contentSymbol = string[i];
            if (contentSymbol != symbol) {
                this->stream->setPos(initialPos);
                parsedLength = 0;
                break;
            }

            parsedLength++;
        }

        return parsedLength;
    }

    size_t systemParseRegexp(const char* str, int size)
    {
        if (this->stream->eof()) {
            return 0;
        }

        size_t parsedLength = 0;
        uint8_t symbol = this->stream->getCurrentSymbol();

        const char* data = str;
        for (size_t i = 0; i < size; i += 2) {
            uint8_t from = data[i];
            uint8_t to = data[i + 1];
            if (symbol >= from && symbol <= to) {
                parsedLength = 1;
                break;
            }
        }

        if (parsedLength == 0) {
            return 0;
        }

        this->stream->readSymbol();

        return parsedLength;
    }

    void skipSpaces()
    {
        while (!this->stream->eof()) {
            char symbol = this->stream->readSymbol();

            if (! (symbol == ' ' || symbol == '\n' || symbol == '\t' || symbol == '\r')) {
                this->stream->setPos(this->stream->getPos() - 1);
                break;
            }
        }
    }
    
    

    GdlNode* parseJson()
    {
        GdlNode* res = NULL;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '{':
            case '"':
            case '-':
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
            case 't':
            case 'f':
            case 'n':
                res = this->statement1Json();
            break;
        };

        return res;
    }

    GdlNode* statement1Json()
    {
        GdlNode* res = NULL;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::Json;
        
        GdlNode* parsedElement = NULL;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            parsedElement = this->parseValue();
            if (parsedElement == NULL) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);
            res->addToList(parsedElement);

            // ------------------------------------------------
            
            isParsed = true;
            if (res == NULL) res = new GdlNode(ruleName);
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return NULL;
        }

        return res;
    }

    GdlNode* parseObj()
    {
        GdlNode* res = NULL;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '{':
                res = this->statement1Obj();
                if (res == NULL) res = this->statement2Obj();
            break;
        };

        return res;
    }

    GdlNode* statement1Obj()
    {
        GdlNode* res = NULL;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::Obj;
        
        GdlNode* parsedElement = NULL;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString("{", 1);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);

            this->skipSpaces();

            // ------------------------------------------------

            parsedElement = this->parsePair();
            if (parsedElement == NULL) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);
            res->addToList(parsedElement);

            this->skipSpaces();

            // ------------------------------------------------

            while (true) {
                isInlineParsed = this->parseInline1_Obj_1(res);
                if (isInlineParsed == false) {
                    break;
                }

                
                
            }

            this->skipSpaces();

            // ------------------------------------------------

            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString("}", 1);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);

            this->skipSpaces();

            // ------------------------------------------------
            
            isParsed = true;
            if (res == NULL) res = new GdlNode(ruleName);
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return NULL;
        }

        return res;
    }

    bool parseInline1_Obj_1(GdlNode*& res)
    {
        bool isParsed = false;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case ',':
                isParsed = this->statement1Inline1_Obj_1(res);
            break;
        };

        return isParsed;
    }

    bool statement1Inline1_Obj_1(GdlNode*& res)
    {
        
        GdlNode* initialRes = res;
        std::vector<GdlNode*>::iterator last;
        if (res != NULL) last = res->getListValue().end() - 1;
            

        size_t initialPos = this->stream->getPos();
    
        auto ruleName = RuleName::Obj;
        
        GdlNode* parsedElement = NULL;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString(",", 1);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);

            this->skipSpaces();

            // ------------------------------------------------

            parsedElement = this->parsePair();
            if (parsedElement == NULL) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);
            res->addToList(parsedElement);

            this->skipSpaces();

            // ------------------------------------------------

            isParsed = true;
            
        GdlNode* initialRes = res;
        std::vector<GdlNode*>::iterator last;
        if (res != NULL) last = res->getListValue().end() - 1;
            
        } while (false);
        
        if (!isParsed) {
            this->stream->setPos(initialPos);
            
        if (res != NULL && initialRes != NULL) res->getListValue().erase(last + 1, res->getListValue().end());
        res = initialRes;
            
        }

        return isParsed;
    }

    GdlNode* statement2Obj()
    {
        GdlNode* res = NULL;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::Obj;
        
        GdlNode* parsedElement = NULL;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString("{", 1);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);

            this->skipSpaces();

            // ------------------------------------------------

            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString("}", 1);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);

            this->skipSpaces();

            // ------------------------------------------------
            
            isParsed = true;
            if (res == NULL) res = new GdlNode(ruleName);
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return NULL;
        }

        return res;
    }

    GdlNode* parsePair()
    {
        GdlNode* res = NULL;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '"':
                res = this->statement1Pair();
            break;
        };

        return res;
    }

    GdlNode* statement1Pair()
    {
        GdlNode* res = NULL;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::Pair;
        
        GdlNode* parsedElement = NULL;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->parseString();
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);
            parsedElement = new GdlNode(String, StringDescriptor(streamData, parsedSize));
            res->addToList(parsedElement);

            this->skipSpaces();

            // ------------------------------------------------

            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString(":", 1);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);

            this->skipSpaces();

            // ------------------------------------------------

            parsedElement = this->parseValue();
            if (parsedElement == NULL) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);
            res->addToList(parsedElement);

            this->skipSpaces();

            // ------------------------------------------------
            
            isParsed = true;
            if (res == NULL) res = new GdlNode(ruleName);
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return NULL;
        }

        return res;
    }

    GdlNode* parseArr()
    {
        GdlNode* res = NULL;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '[':
                res = this->statement1Arr();
            break;
        };

        return res;
    }

    GdlNode* statement1Arr()
    {
        GdlNode* res = NULL;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::Arr;
        
        GdlNode* parsedElement = NULL;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString("[", 1);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);

            this->skipSpaces();

            // ------------------------------------------------

            isInlineParsed = this->parseInline1_Arr_1(res);
            if (! (isInlineParsed == false)) {
                
                
            }

            this->skipSpaces();

            // ------------------------------------------------

            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString("]", 1);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);

            this->skipSpaces();

            // ------------------------------------------------
            
            isParsed = true;
            if (res == NULL) res = new GdlNode(ruleName);
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return NULL;
        }

        return res;
    }

    bool parseInline1_Arr_1(GdlNode*& res)
    {
        bool isParsed = false;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '{':
            case '"':
            case '-':
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
            case 't':
            case 'f':
            case 'n':
                isParsed = this->statement1Inline1_Arr_1(res);
            break;
        };

        return isParsed;
    }

    bool statement1Inline1_Arr_1(GdlNode*& res)
    {
        
        GdlNode* initialRes = res;
        std::vector<GdlNode*>::iterator last;
        if (res != NULL) last = res->getListValue().end() - 1;
            

        size_t initialPos = this->stream->getPos();
    
        auto ruleName = RuleName::Arr;
        
        GdlNode* parsedElement = NULL;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            parsedElement = this->parseValue();
            if (parsedElement == NULL) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);
            res->addToList(parsedElement);

            this->skipSpaces();

            // ------------------------------------------------

            while (true) {
                isInlineParsed = this->parseInline1_Inline1_Arr_1_1(res);
                if (isInlineParsed == false) {
                    break;
                }

                
                
            }

            this->skipSpaces();

            // ------------------------------------------------

            isParsed = true;
            
        GdlNode* initialRes = res;
        std::vector<GdlNode*>::iterator last;
        if (res != NULL) last = res->getListValue().end() - 1;
            
        } while (false);
        
        if (!isParsed) {
            this->stream->setPos(initialPos);
            
        if (res != NULL && initialRes != NULL) res->getListValue().erase(last + 1, res->getListValue().end());
        res = initialRes;
            
        }

        return isParsed;
    }

    bool parseInline1_Inline1_Arr_1_1(GdlNode*& res)
    {
        bool isParsed = false;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case ',':
                isParsed = this->statement1Inline1_Inline1_Arr_1_1(res);
            break;
        };

        return isParsed;
    }

    bool statement1Inline1_Inline1_Arr_1_1(GdlNode*& res)
    {
        
        GdlNode* initialRes = res;
        std::vector<GdlNode*>::iterator last;
        if (res != NULL) last = res->getListValue().end() - 1;
            

        size_t initialPos = this->stream->getPos();
    
        auto ruleName = RuleName::Arr;
        
        GdlNode* parsedElement = NULL;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString(",", 1);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);

            this->skipSpaces();

            // ------------------------------------------------

            parsedElement = this->parseValue();
            if (parsedElement == NULL) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);
            res->addToList(parsedElement);

            this->skipSpaces();

            // ------------------------------------------------

            isParsed = true;
            
        GdlNode* initialRes = res;
        std::vector<GdlNode*>::iterator last;
        if (res != NULL) last = res->getListValue().end() - 1;
            
        } while (false);
        
        if (!isParsed) {
            this->stream->setPos(initialPos);
            
        if (res != NULL && initialRes != NULL) res->getListValue().erase(last + 1, res->getListValue().end());
        res = initialRes;
            
        }

        return isParsed;
    }

    GdlNode* parseValue()
    {
        GdlNode* res = NULL;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '{':
                res = this->statement1Value();
            break;

            case '"':
                res = this->statement2Value();
            break;

            case '-':
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
                res = this->statement3Value();
            break;

            case '[':
                res = this->statement4Value();
            break;

            case 't':
                res = this->statement5Value();
            break;

            case 'f':
                res = this->statement6Value();
            break;

            case 'n':
                res = this->statement7Value();
            break;
        };

        return res;
    }

    GdlNode* statement1Value()
    {
        GdlNode* res = NULL;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::Value;
        
        GdlNode* parsedElement = NULL;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            parsedElement = this->parseObj();
            if (parsedElement == NULL) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);
            res->addToList(parsedElement);

            // ------------------------------------------------
            
            isParsed = true;
            if (res == NULL) res = new GdlNode(ruleName);
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return NULL;
        }

        return res;
    }

    GdlNode* statement2Value()
    {
        GdlNode* res = NULL;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::Value;
        
        GdlNode* parsedElement = NULL;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->parseString();
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);
            parsedElement = new GdlNode(String, StringDescriptor(streamData, parsedSize));
            res->addToList(parsedElement);

            // ------------------------------------------------
            
            isParsed = true;
            if (res == NULL) res = new GdlNode(ruleName);
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return NULL;
        }

        return res;
    }

    GdlNode* statement3Value()
    {
        GdlNode* res = NULL;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::Value;
        
        GdlNode* parsedElement = NULL;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->parseNumber();
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);
            parsedElement = new GdlNode(Number, StringDescriptor(streamData, parsedSize));
            res->addToList(parsedElement);

            // ------------------------------------------------
            
            isParsed = true;
            if (res == NULL) res = new GdlNode(ruleName);
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return NULL;
        }

        return res;
    }

    GdlNode* statement4Value()
    {
        GdlNode* res = NULL;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::Value;
        
        GdlNode* parsedElement = NULL;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            parsedElement = this->parseArr();
            if (parsedElement == NULL) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);
            res->addToList(parsedElement);

            // ------------------------------------------------
            
            isParsed = true;
            if (res == NULL) res = new GdlNode(ruleName);
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return NULL;
        }

        return res;
    }

    GdlNode* statement5Value()
    {
        GdlNode* res = NULL;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::Value;
        
        GdlNode* parsedElement = NULL;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString("true", 4);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);

            // ------------------------------------------------
            
            isParsed = true;
            if (res == NULL) res = new GdlNode(ruleName);
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return NULL;
        }

        return res;
    }

    GdlNode* statement6Value()
    {
        GdlNode* res = NULL;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::Value;
        
        GdlNode* parsedElement = NULL;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString("false", 5);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);

            // ------------------------------------------------
            
            isParsed = true;
            if (res == NULL) res = new GdlNode(ruleName);
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return NULL;
        }

        return res;
    }

    GdlNode* statement7Value()
    {
        GdlNode* res = NULL;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::Value;
        
        GdlNode* parsedElement = NULL;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString("null", 4);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            if (res == NULL) res = new GdlNode(ruleName);

            // ------------------------------------------------
            
            isParsed = true;
            if (res == NULL) res = new GdlNode(ruleName);
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return NULL;
        }

        return res;
    }

    size_t parseString()
    {
        size_t res = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '"':
                res = this->statement1String();
            break;
        };

        return res;
    }

    size_t statement1String()
    {
        size_t res = 0;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::String;
        
        size_t parsedElement = 0;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString("\"", 1);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------

            while (true) {
                initialElementPos = this->stream->getPos();
    
                streamData = this->stream->getCurrentDataPtr(); 
                parsedSize = this->systemParseString("\"", 1);
                if (! (parsedSize == 0)) {
                    this->stream->setPos(initialElementPos);
                    break;
                }
    
                isInlineParsed = this->parseInline1_String_1(res);
                if (isInlineParsed == false) {
                    break;
                }

                
                
            }

            // ------------------------------------------------

            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString("\"", 1);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------
            
            isParsed = true;
            
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return 0;
        }

        return res;
    }

    bool parseInline1_String_1(size_t& res)
    {
        bool isParsed = false;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\\':
                isParsed = this->statement1Inline1_String_1(res);
                if (isParsed == false) isParsed = this->statement2Inline1_String_1(res);
            break;

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
                isParsed = this->statement2Inline1_String_1(res);
            break;
        };

        return isParsed;
    }

    bool statement1Inline1_String_1(size_t& res)
    {
        
        size_t initialRes = res;
            

        size_t initialPos = this->stream->getPos();
    
        auto ruleName = RuleName::String;
        
        size_t parsedElement = 0;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->parseEsc();
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------

            isParsed = true;
            
        size_t initialRes = res;
            
        } while (false);
        
        if (!isParsed) {
            this->stream->setPos(initialPos);
            
        res = initialRes;
            
        }

        return isParsed;
    }

    bool statement2Inline1_String_1(size_t& res)
    {
        
        size_t initialRes = res;
            

        size_t initialPos = this->stream->getPos();
    
        auto ruleName = RuleName::String;
        
        size_t parsedElement = 0;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->parseSafeCodePoint();
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------

            isParsed = true;
            
        size_t initialRes = res;
            
        } while (false);
        
        if (!isParsed) {
            this->stream->setPos(initialPos);
            
        res = initialRes;
            
        }

        return isParsed;
    }

    size_t parseEsc()
    {
        size_t res = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '\\':
                res = this->statement1Esc();
            break;
        };

        return res;
    }

    size_t statement1Esc()
    {
        size_t res = 0;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::Esc;
        
        size_t parsedElement = 0;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString("\\", 1);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------

            isInlineParsed = this->parseInline1_Esc_1(res);
            if (isInlineParsed == false) {
                // TODO: cut
                break;
            }

            // ------------------------------------------------
            
            isParsed = true;
            
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return 0;
        }

        return res;
    }

    bool parseInline1_Esc_1(size_t& res)
    {
        bool isParsed = false;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '"':
            case '\\':
            case '/':
            case 'b':
            case 'f':
            case 'n':
            case 'r':
            case 't':
                isParsed = this->statement1Inline1_Esc_1(res);
            break;

            case 'u':
                isParsed = this->statement2Inline1_Esc_1(res);
            break;
        };

        return isParsed;
    }

    bool statement1Inline1_Esc_1(size_t& res)
    {
        
        size_t initialRes = res;
            

        size_t initialPos = this->stream->getPos();
    
        auto ruleName = RuleName::Esc;
        
        size_t parsedElement = 0;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseRegexp("\"\"\\\\//bbffnnrrtt", 16);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------

            isParsed = true;
            
        size_t initialRes = res;
            
        } while (false);
        
        if (!isParsed) {
            this->stream->setPos(initialPos);
            
        res = initialRes;
            
        }

        return isParsed;
    }

    bool statement2Inline1_Esc_1(size_t& res)
    {
        
        size_t initialRes = res;
            

        size_t initialPos = this->stream->getPos();
    
        auto ruleName = RuleName::Esc;
        
        size_t parsedElement = 0;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->parseUnicode();
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------

            isParsed = true;
            
        size_t initialRes = res;
            
        } while (false);
        
        if (!isParsed) {
            this->stream->setPos(initialPos);
            
        res = initialRes;
            
        }

        return isParsed;
    }

    size_t parseUnicode()
    {
        size_t res = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'u':
                res = this->statement1Unicode();
            break;
        };

        return res;
    }

    size_t statement1Unicode()
    {
        size_t res = 0;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::Unicode;
        
        size_t parsedElement = 0;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString("u", 1);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------

            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->parseHex();
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------

            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->parseHex();
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------

            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->parseHex();
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------

            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->parseHex();
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------
            
            isParsed = true;
            
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return 0;
        }

        return res;
    }

    size_t parseHex()
    {
        size_t res = 0;

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
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
                res = this->statement1Hex();
            break;
        };

        return res;
    }

    size_t statement1Hex()
    {
        size_t res = 0;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::Hex;
        
        size_t parsedElement = 0;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseRegexp("09afAF", 6);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------
            
            isParsed = true;
            
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return 0;
        }

        return res;
    }

    size_t parseSafeCodePoint()
    {
        size_t res = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
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
                res = this->statement1SafeCodePoint();
            break;
        };

        return res;
    }

    size_t statement1SafeCodePoint()
    {
        size_t res = 0;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::SafeCodePoint;
        
        size_t parsedElement = 0;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseRegexp("\x20\xFF", 2);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------
            
            isParsed = true;
            
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return 0;
        }

        return res;
    }

    size_t parseNumber()
    {
        size_t res = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '-':
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
                res = this->statement1Number();
            break;
        };

        return res;
    }

    size_t statement1Number()
    {
        size_t res = 0;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::Number;
        
        size_t parsedElement = 0;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString("-", 1);
            if (! (parsedSize == 0)) {
                
                res += parsedSize;
            }

            // ------------------------------------------------

            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->parseInt();
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------

            isInlineParsed = this->parseInline1_Number_1(res);
            if (! (isInlineParsed == false)) {
                
                
            }

            // ------------------------------------------------

            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->parseExp();
            if (! (parsedSize == 0)) {
                
                res += parsedSize;
            }

            // ------------------------------------------------
            
            isParsed = true;
            
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return 0;
        }

        return res;
    }

    bool parseInline1_Number_1(size_t& res)
    {
        bool isParsed = false;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '.':
                isParsed = this->statement1Inline1_Number_1(res);
            break;
        };

        return isParsed;
    }

    bool statement1Inline1_Number_1(size_t& res)
    {
        
        size_t initialRes = res;
            

        size_t initialPos = this->stream->getPos();
    
        auto ruleName = RuleName::Number;
        
        size_t parsedElement = 0;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString(".", 1);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------

            parsedCount = 0;
            outerParsedSize = res;
            
            while (true) {
                streamData = this->stream->getCurrentDataPtr(); 
                parsedSize = this->systemParseRegexp("09", 2);
                if (parsedSize == 0) {
                    break;
                }

                
                res += parsedSize;
                
                parsedCount++;
            }
            if (parsedCount < 1) {
                res = outerParsedSize;
                // TODO: cut
                break;
            }

            // ------------------------------------------------

            isParsed = true;
            
        size_t initialRes = res;
            
        } while (false);
        
        if (!isParsed) {
            this->stream->setPos(initialPos);
            
        res = initialRes;
            
        }

        return isParsed;
    }

    size_t parseInt()
    {
        size_t res = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case '0':
                res = this->statement1Int();
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
                res = this->statement2Int();
            break;
        };

        return res;
    }

    size_t statement1Int()
    {
        size_t res = 0;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::Int;
        
        size_t parsedElement = 0;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseString("0", 1);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------
            
            isParsed = true;
            
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return 0;
        }

        return res;
    }

    size_t statement2Int()
    {
        size_t res = 0;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::Int;
        
        size_t parsedElement = 0;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseRegexp("19", 2);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------

            while (true) {
                streamData = this->stream->getCurrentDataPtr(); 
                parsedSize = this->systemParseRegexp("09", 2);
                if (parsedSize == 0) {
                    break;
                }

                
                res += parsedSize;
            }

            // ------------------------------------------------
            
            isParsed = true;
            
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return 0;
        }

        return res;
    }

    size_t parseExp()
    {
        size_t res = 0;

        char chr = this->stream->getCurrentSymbol();
        switch (chr) {
            case 'E':
            case 'e':
                res = this->statement1Exp();
            break;
        };

        return res;
    }

    size_t statement1Exp()
    {
        size_t res = 0;
        size_t initialPos = this->stream->getPos();

        auto ruleName = RuleName::Exp;
        
        size_t parsedElement = 0;
        char* streamData = NULL;
        size_t parsedSize = 0;
        bool cut = false;  // TODO

        size_t initialElementPos = 0;
        size_t parsedCount = 0;
        
        std::vector<GdlNode*>::iterator outerLast;
        size_t outerParsedSize = 0;
        size_t countVal = 0;
        
        bool isInlineParsed = false;

        bool isParsed = false;
        do {
            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseRegexp("EEee", 4);
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------

            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->systemParseRegexp("++--", 4);
            if (! (parsedSize == 0)) {
                
                res += parsedSize;
            }

            // ------------------------------------------------

            streamData = this->stream->getCurrentDataPtr(); 
            parsedSize = this->parseInt();
            if (parsedSize == 0) {
                // TODO: cut
                break;
            }
            
            res += parsedSize;

            // ------------------------------------------------
            
            isParsed = true;
            
        } while (false);

        if (!isParsed) {
            this->stream->setPos(initialPos);
            return 0;
        }

        return res;
    }
    
    
};


const char* JsonParser::ruleNames[] = {
    "", "Json","Obj","Pair","Arr","Value","String","Esc","Unicode","Hex","SafeCodePoint","Number","Int","Exp"
};

