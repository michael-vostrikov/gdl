#include <cstring>
#include <string>

class StringDescriptor
{
    char* data;
    size_t size;

public:

    StringDescriptor(char* data, size_t size): data(data), size(size)
    {
    }

    StringDescriptor(const char* data): data((char*)data), size(strlen(data))
    {
    }

    char* getDataPtr() const
    {
        return this->data;
    }

    size_t getSize() const
    {
        return this->size;
    }

    void expand(size_t diff)
    {
        this->size += diff;
    }

    std::string getStdString() const
    {
        return std::string(this->data, this->size);
    }

    bool operator == (const StringDescriptor& s) const
    {
        if (this->size != s.size) {
            return false;
        }

        if (memcmp(this->data, s.data, this->size) != 0) {
            return false;
        }

        return true;
    }
};

namespace std {
    template <>
    struct hash<StringDescriptor>
    {
        std::size_t operator()(const StringDescriptor& k) const
        {
            size_t size = k.getSize();
            char* data = k.getDataPtr();

            size_t pos = 0;
            unsigned long hash = 5381;
            while (pos < size) {
                hash = ((hash << 5) + hash) + (unsigned long)data[pos];
                pos++;
            }

            return hash;
        }
    };
}
