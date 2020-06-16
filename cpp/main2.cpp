#include <cstdlib>
#include <chrono>
#include <cstring>
#include <iostream>

class GdlNode;
void printGdlNode(GdlNode& node, int level);

#include "JsonParser.h"

void printGdlNode(GdlNode* rootNode, int level = 1)
{
    std::string indent = "";
    for (int i = 1; i < level; i++) {
        indent += ".   ";
    }

    std::cout << indent << "GdlNode: {"<< std::endl;
    std::cout << indent << ".   " << "name: " << JsonParser::ruleNames[rootNode->getRuleName()] << std::endl;
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
    StringDescriptor content = getFileContent("../big.json");
    Stream stream(content);
    JsonParser parser(&stream);

    auto t1 = std::chrono::high_resolution_clock::now();
    ssize_t parsedCount = parser.parseJson();
    auto t2 = std::chrono::high_resolution_clock::now();

    if (parsedCount == -1) {
        std::cout << "Parse error";
        return 0;
    }

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << "Duration: " << ((double)duration) / 1000000 << std::endl;

    std::cout << stream.getPos() << std::endl;

    if (argc > 1 && std::string(argv[1]) == "show") {
        printGdlNode(parser.getRootNode());
    }

    if (argc > 1 && std::string(argv[1]) == "arr") {
        auto root = parser.getRootNode();
        auto valueList = root->get(JsonParser::RuleName::Value)->get(JsonParser::RuleName::Arr)->getArray(JsonParser::RuleName::Value);
        std::cout << "array size: " << valueList.size() << std::endl;

        int i = 0;
        for (auto it = valueList.begin(); it != valueList.end(); ++it) {
            GdlNode* node = *it;
            auto idValue = node->get(JsonParser::RuleName::Obj)->getArray(JsonParser::RuleName::Pair)[0]->get(JsonParser::RuleName::Value)->get(JsonParser::RuleName::Number)->getStringValue().getStdString();
            std::cout << "id: " << idValue << std::endl;

            i++;
            if (i >= 10) break;
        }
    }
}
