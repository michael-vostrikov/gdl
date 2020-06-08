#include <vector>

class GdlNode
{
protected:

    StringDescriptor name;

    enum {VALUE_LIST, VALUE_STRING} valueType;

    std::vector<GdlNode*> listValue;
    StringDescriptor stringValue;

public:

    GdlNode(StringDescriptor name, StringDescriptor value): name(name), stringValue(value), listValue()
    {
        this->valueType = GdlNode::VALUE_STRING;
    }

    GdlNode(StringDescriptor name, std::vector<GdlNode*> value): name(name), stringValue(""), listValue(value)
    {
        this->valueType = GdlNode::VALUE_LIST;
    }

    GdlNode(const GdlNode& node): name(node.name), listValue(node.listValue), stringValue(node.stringValue)
    {
        this->valueType = node.valueType;
    }

    StringDescriptor& getName()
    {
        return this->name;
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

    GdlNode* get(StringDescriptor name)
    {
        for (auto it = this->listValue.begin(); it != this->listValue.end(); ++it) {
            GdlNode* pNode = *it;
            if (pNode->name == name) {
                return pNode;
            }
        }

        return NULL;
    }

    std::vector<GdlNode*> getArray(StringDescriptor name)
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

void printGdlNode(GdlNode* rule, int level = 1)
{
    std::string indent = "";
    for (int i = 1; i < level; i++) {
        indent += "    ";
    }

    std::cout << indent << "GdlNode: {"<< std::endl;
    std::cout << indent << "    " << "name: " << rule->getName().getStdString() << std::endl;
    std::cout << indent << "    " << "stringValue: '" << rule->getStringValue().getStdString() << "'" << std::endl;
    std::cout << indent << "    " << "valueType: " << std::to_string((int)rule->getValueType()) << std::endl;
    std::cout << indent << "    " << "listValue: " << std::to_string(rule->getListValue().size()) << ": [" << std::endl;
    for (auto it = rule->getListValue().begin(); it != rule->getListValue().end(); ++it) {
        printGdlNode(*it, level + 2);
    }
    std::cout << indent << "    " << "]" << std::endl;
    std::cout << indent << "}" << std::endl;
}
