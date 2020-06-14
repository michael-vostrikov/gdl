#include <cstring>
#include <algorithm>

class Stream
{
    StringDescriptor content;
    size_t pos;

public:

    Stream(StringDescriptor content): content(content)
    {
        this->reset();
    }

    void reset()
    {
        this->pos = 0;
    }

    StringDescriptor getContent()
    {
        return this->content;
    }

    size_t getPos()
    {
        return this->pos;
    }

    void setPos(size_t pos)
    {
        this->pos = this->normalizePos(pos);
    }

    size_t normalizePos(size_t pos)
    {
        return std::max(0lu, std::min(pos, this->content.getSize()));
    }

    char* getCurrentDataPtr()
    {
        return this->content.getDataPtr() + this->pos;
    }

    char getCurrentSymbol()
    {
        return this->content.getDataPtr()[this->pos];
    }

    char readSymbol()
    {
        char symbol = '\0';
        if (this->pos < this->content.getSize()) {
            symbol = this->content.getDataPtr()[this->pos];
            this->pos++;
        }

        return symbol;
    }

    /* Note that you should check on eof before read */
    bool eof()
    {
        return (this->pos == this->content.getSize());
    }

    std::string getLineAndColumn()
    {
        return this->getLineAndColumn(this->pos);
    }

    std::string getLineAndColumn(size_t pos)
    {
        size_t line = 1;
        size_t column = 1;

        pos = this->normalizePos(pos);
        for (size_t i = 0; i < pos; i++) {
            if (this->content.getDataPtr()[i] == '\n') {
                line++;
                column = 1;
            }
            else if (this->content.getDataPtr()[i] == '\t') {
                column += 4;
            }
            else {
                column++;
            }
        }

        return std::string(std::to_string(line)) + ":" + std::to_string(column);
    }
};
