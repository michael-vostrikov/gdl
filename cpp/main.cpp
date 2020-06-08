#include <cstdlib>
#include <chrono>
#include <cstring>
#include <iostream>

#include "StringDescriptor.h"
#include "GdlNode.h"
#include "Stream.h"
#include "GdlParser.h"
#include "self.h"

StringDescriptor getFileContent(const char* filename)
{
    FILE* f = fopen(filename, "rb");

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* data = new char[size];
    size_t res = fread(data, 1, size, f);

    StringDescriptor content(data, size);

    return content;
}

GdlNode* f1()
{
    StringDescriptor content2 = getFileContent("../php.gdl");

    GdlParser gdlParser2(selfGrammar);

    Stream stream2(content2);
    GdlNode* phpGrammar = gdlParser2.parse("Grammar", &stream2);

    auto errors2 = gdlParser2.getErrors();
    if (!errors2.empty()) {
        std::cout << "Errors:" << std::endl;
        for (auto it = errors2.begin(); it != errors2.end(); ++it) {
            std::cout << *it << std::endl;
        }
        std::cout << std::endl;
    }
    else {
        auto ruleList = phpGrammar->getArray("Rule");
        if (ruleList.size() == 0) {
            std::cout << "No rules parsed" << std::endl;
        }

        std::cout << "PHP grammar parsing OK" << std::endl;
    }

    return phpGrammar;
}

GdlNode* f2(GdlNode* phpGrammar)
{
    StringDescriptor phpSrc = getFileContent("../GdlParser.php");

    class GdlTextParser: public GdlParser
    {
    public:
        GdlTextParser(GdlNode* grammar): GdlParser(grammar)
        {
        }

        GdlNode* parse(StringDescriptor mainRuleName, Stream* stream, bool checkEof = true)
        {
            this->stream = stream;
            this->skipSpaces();

            return GdlParser::parse(mainRuleName, stream, checkEof);
        }

    protected:

        bool keepSpaces = false;
        size_t keepSpacesLevel = 0;

        void callElementFunction(StringDescriptor functionName, GdlNode*& element)
        {
            if (functionName == "setKeepSpaces") {
                this->setKeepSpaces(element);
            }
            else if (functionName == "clearKeepSpaces") {
                this->clearKeepSpaces(element);
            }
            else {
                GdlParser::callElementFunction(functionName, element);
            }
        }

        void setKeepSpaces(GdlNode*& element)
        {
            this->keepSpaces = true;
            this->keepSpacesLevel = this->ruleCallStack.size();
        }

        void clearKeepSpaces(GdlNode*& element)
        {
            this->keepSpaces = false;
            this->keepSpacesLevel = 0;
        }

        std::pair<bool, std::vector<GdlNode*>> parseStatement(GdlNode* statement)
        {
            auto res = GdlParser::parseStatement(statement);

            // auto-clear flag on statement fail
            if (this->keepSpaces && !res.first && this->ruleCallStack.size() <= this->keepSpacesLevel) {
                this->keepSpaces = false;
                this->keepSpacesLevel = 0;
            }

            return res;
        }

        std::pair<bool, std::vector<GdlNode*>> parseExpression(GdlNode* expression, GdlNode* lookAheadElement)
        {
            auto res = GdlParser::parseExpression(expression, lookAheadElement);

            if (res.first && !this->keepSpaces) {
                this->skipSpaces();
            }

            return res;
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
    };

    GdlTextParser phpParser(phpGrammar);

    Stream stream3(phpSrc);

    auto t1 = std::chrono::high_resolution_clock::now();
    StringDescriptor mainRuleName = phpGrammar->get("Rule")->get("RuleName")->getStringValue();
    GdlNode* phpTree = phpParser.parse(mainRuleName, &stream3);
    auto t2 = std::chrono::high_resolution_clock::now();

    auto errors3 = phpParser.getErrors();
    if (!errors3.empty()) {
        std::cout << "Errors:" << std::endl;
        for (auto it = errors3.begin(); it != errors3.end(); ++it) {
            std::cout << *it << std::endl;
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "PHP source parsing OK" << std::endl;
    }

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << "Duration: " << ((double)duration) / 1000000 << std::endl;

    return phpTree;
}

int main(void)
{
    GdlNode* phpGrammar = f1();
    f2(phpGrammar);
}
