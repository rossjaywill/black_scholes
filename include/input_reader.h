#ifndef INPUT_READER_H
#define INPUT_READER_H

namespace bsm
{

enum class Format
{
    CSV
};

class InputReader
{
    explicit InputReader(Format fmt) : fmt_(fmt) {}

private:
    Format fmt_;
}

}

#endif
