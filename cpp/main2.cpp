#include <cstdlib>
#include <chrono>
#include <cstring>
#include <iostream>

#include "JsonParser.h"

void printGdlNode(GdlNode* rule, int level = 1)
{
    std::string indent = "";
    for (int i = 1; i < level; i++) {
        indent += ".   ";
    }

    std::cout << indent << "GdlNode: {"<< std::endl;
    std::cout << indent << ".   " << "name: " << JsonParser::ruleNames[rule->getName()] << std::endl;
    int valueType = (int)rule->getValueType();
    if (valueType == 1) {
        std::cout << indent << ".   " << "stringValue: '" << rule->getStringValue().getStdString() << "'" << std::endl;
    }
    else {
        std::cout << indent << ".   " << "listValue: " << std::to_string(rule->getListValue().size()) << ": [" << std::endl;
        for (auto it = rule->getListValue().begin(); it != rule->getListValue().end(); ++it) {
            printGdlNode(*it, level + 2);
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
    StringDescriptor content = getFileContent("../big.json");
    Stream stream(content);
    JsonParser parser(&stream);

    auto t1 = std::chrono::high_resolution_clock::now();
    GdlNode* res = parser.parseJson();
    auto t2 = std::chrono::high_resolution_clock::now();

    if (res == NULL) {
        std::cout << "NULL";
        return 0;
    }

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << "Duration: " << ((double)duration) / 1000000 << std::endl;

    std::cout << stream.getPos() << std::endl;

    if (argc > 1 && std::string(argv[1]) == "show") {
        printGdlNode(res);
    }
}
