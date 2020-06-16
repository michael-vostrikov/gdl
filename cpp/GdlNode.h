#include <vector>
#include <algorithm>

class GdlNode;

class ListDescriptor
{
public:
    size_t offset;
    size_t count;
    size_t totalCount;  // with children elements

    ListDescriptor()
        : offset(0), count(0), totalCount(0)
    {
    }

    ListDescriptor(size_t offset, size_t count, size_t totalCount)
        : offset(offset), count(count), totalCount(totalCount)
    {
    }
};

class GdlNode
{
public:

    enum ValueType {VALUE_NULL, VALUE_LIST, VALUE_STRING};

protected:

    uint32_t ruleName;

    ValueType valueType;

    union {
        ListDescriptor listValue;
        StringDescriptor stringValue;
    };

public:


    GdlNode(): ruleName(0), stringValue("")
    {
        this->valueType = VALUE_NULL;
    }

    GdlNode(uint32_t ruleName): ruleName(ruleName), stringValue("")
    {
        this->valueType = VALUE_STRING;
    }

    GdlNode(uint32_t ruleName, ListDescriptor listValue): ruleName(ruleName), listValue(listValue)
    {
        this->valueType = VALUE_LIST;
    }

    GdlNode(uint32_t ruleName, StringDescriptor stringValue): ruleName(ruleName), stringValue(stringValue)
    {
        this->valueType = VALUE_STRING;
    }

    uint32_t getRuleName()
    {
        return this->ruleName;
    }

    uint32_t getValueType()
    {
        return this->valueType;
    }

    ListDescriptor& getListValue()
    {
        return this->listValue;
    }

    StringDescriptor& getStringValue()
    {
        return this->stringValue;
    }

    void setListValue(ListDescriptor listValue)
    {
        this->valueType = VALUE_LIST;
        this->listValue = listValue;
    }

    void setStringValue(StringDescriptor stringValue)
    {
        this->valueType = VALUE_STRING;
        this->stringValue = stringValue;
    }

    GdlNode* getFirst()
    {
        return this + 1;
    }

    GdlNode* get(uint32_t name)
    {
        if (this->valueType != VALUE_LIST) {
            return NULL;
        }

        auto it = this->getFirst();
        for (size_t i = 0; i < this->listValue.count; i++) {
            if (it->ruleName == name) {
                return it;
            }

            auto diff = ((*it).valueType == VALUE_LIST ? (*it).listValue.totalCount : 1);
            it += diff;
        }

        return NULL;
    }

    std::vector<GdlNode*> getArray(uint32_t name)
    {
        std::vector<GdlNode*> arr;
        if (this->valueType != VALUE_LIST) {
            return arr;
        }

        auto it = this->getFirst();
        for (size_t i = 0; i < this->listValue.count; i++) {
            if (it->ruleName == name) {
                arr.push_back(it);
            }

            auto diff = ((*it).valueType == VALUE_LIST ? (*it).listValue.totalCount : 1);
            it += diff;
        }

        return arr;
    }
};
