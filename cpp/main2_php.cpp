#include <cstdlib>
#include <chrono>
#include <cstring>
#include <iostream>

class GdlNode;
void printGdlNode(GdlNode& node, int level);

#include "PhpParser.h"

class PhpParser2: public PhpParser
{
    StringDescriptor tag;

public:

    PhpParser2(Stream* stream): PhpParser(stream), tag("")
    {
    }

    virtual void setStringStart(size_t& parsedSize)
    {
        if (parsedSize == 0) return;

        StringDescriptor tag(this->stream->getCurrentDataPtr() - parsedSize, parsedSize);
        this->tag = tag;
    }

    virtual void checkStringEnd(size_t& parsedSize)
    {
        if (parsedSize == 0) return;

        StringDescriptor tag(this->stream->getCurrentDataPtr() - parsedSize, parsedSize);
        if (! (tag == this->tag)) {
            parsedSize = 0;
        }
    }
};

void printGdlNode(GdlNode* rootNode, int level = 1)
{
    std::string indent = "";
    for (int i = 1; i < level; i++) {
        indent += ".   ";
    }

    std::cout << indent << "GdlNode: {"<< std::endl;
    std::cout << indent << ".   " << "name: " << PhpParser::ruleNames[rootNode->getRuleName()] << std::endl;
    auto valueType = rootNode->getValueType();
    if (valueType == GdlNode::VALUE_STRING) {
        std::cout << indent << ".   " << "stringValue: '" << rootNode->getStringValue().getStdString() << "'" << std::endl;
    }
    else {
        std::cout << indent << ".   " << "offset: " << std::to_string(rootNode->getListValue().offset) << std::endl;
        std::cout << indent << ".   " << "totalCount: " << std::to_string(rootNode->getListValue().totalCount) << std::endl;
        std::cout << indent << ".   " << "listValue: " << std::to_string(rootNode->getListValue().count) << ": [" << std::endl;

        GdlNode* element = rootNode->getFirst();

        for (size_t i = 0; i < rootNode->getListValue().count; i++) {
            printGdlNode(element, level + 2);

            auto diff = (element->getValueType() == GdlNode::VALUE_LIST ? element->getListValue().totalCount : 1);
            element += diff;
        }

        std::cout << indent << ".   " << "]" << std::endl;
    }
    std::cout << indent << "}" << std::endl;
}

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

int main(int argc, char** argv)
{
    StringDescriptor content = getFileContent("../GdlParser.php");
    Stream stream(content);
    PhpParser2 parser(&stream);

    auto t1 = std::chrono::high_resolution_clock::now();
    bool isParsed = parser.parseProgram();
    auto t2 = std::chrono::high_resolution_clock::now();

    if (isParsed == false) {
        std::cout << "Parse error" << std::endl;
        return 0;
    }

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << "Duration: " << ((double)duration) / 1000000 << std::endl;

    std::cout << stream.getPos() << std::endl;

    if (argc > 1 && std::string(argv[1]) == "show") {
        printGdlNode(parser.getRootNode());
    }
}
