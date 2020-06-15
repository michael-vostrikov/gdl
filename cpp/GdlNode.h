#include <vector>

class GdlNode
{
protected:

    uint32_t name;

    enum {VALUE_LIST, VALUE_STRING} valueType;

    std::vector<GdlNode*> listValue;
    StringDescriptor stringValue;

public:

    GdlNode(uint32_t name): name(name), stringValue(""), listValue()
    {
        this->valueType = GdlNode::VALUE_LIST;
    }

    GdlNode(uint32_t name, StringDescriptor value): name(name), stringValue(value), listValue()
    {
        this->valueType = GdlNode::VALUE_STRING;
    }

    GdlNode(uint32_t name, std::vector<GdlNode*> value): name(name), stringValue(""), listValue(value)
    {
        this->valueType = GdlNode::VALUE_LIST;
    }

    GdlNode(const GdlNode& node): name(node.name), listValue(node.listValue), stringValue(node.stringValue)
    {
        this->valueType = node.valueType;
    }

    uint32_t& getName()
    {
        return this->name;
    }

    void setName(uint32_t name)
    {
        this->name = name;
    }

    int getValueType()
    {
        return this->valueType;
    }

    StringDescriptor& getStringValue()
    {
        return this->stringValue;
    }

    std::vector<GdlNode*>& getListValue()
    {
        return this->listValue;
    }

    void addToList(GdlNode* element)
    {
        this->listValue.push_back(element);
    }

    void setStringValue(StringDescriptor value)
    {
        this->valueType = GdlNode::VALUE_STRING;
        this->stringValue = value;
        this->listValue = {};
    }

    void setListValue(std::vector<GdlNode*> value)
    {
        this->valueType = GdlNode::VALUE_LIST;
        this->stringValue = "";
        this->listValue = value;
    }

    GdlNode* getFirst()
    {
        auto it = this->listValue.begin();
        return *it;
    }

    GdlNode* get(uint32_t name)
    {
        for (auto it = this->listValue.begin(); it != this->listValue.end(); ++it) {
            GdlNode* pNode = *it;
            if (pNode->name == name) {
                return pNode;
            }
        }

        return NULL;
    }

    std::vector<GdlNode*> getArray(uint32_t name)
    {
        std::vector<GdlNode*> arr;
        for (auto it = this->listValue.begin(); it != this->listValue.end(); ++it) {
            GdlNode* pNode = *it;
            if (pNode->name == name) {
                arr.push_back(pNode);
            }
        }

        return arr;
    }

    size_t getToStringSize()
    {
        if (this->valueType == VALUE_STRING) {
            return this->stringValue.getSize();
        }

        size_t size = 0;
        for (auto it = this->listValue.begin(); it != this->listValue.end(); ++it) {
            GdlNode* pNode = *it;
            size += pNode->getToStringSize();
        }

        return size;
    }

    StringDescriptor toString()
    {
        size_t size = this->getToStringSize();
        char* data = new char[size];

        if (this->valueType == VALUE_STRING) {
            memcpy(data, this->stringValue.getDataPtr(), size);
        }
        else {
            size_t pos = 0;
            for (auto it = this->listValue.begin(); it != this->listValue.end(); ++it) {
                GdlNode* pNode = *it;

                StringDescriptor innerStr = pNode->toString();
                memcpy(data + pos, innerStr.getDataPtr(), innerStr.getSize());
                pos += innerStr.getSize();

                delete[] innerStr.getDataPtr();
            }
        }

        return StringDescriptor(data, size);
    }
};
