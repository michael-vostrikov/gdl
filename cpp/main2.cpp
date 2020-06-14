#include <cstdlib>
#include <chrono>
#include <cstring>
#include <iostream>

#include "JsonParser.h"

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

int main(void)
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
}
