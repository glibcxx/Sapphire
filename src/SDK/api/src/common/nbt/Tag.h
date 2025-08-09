#pragma once

#include "SDK/api/src/common/CommonTypes.h"
#include <string>

class IDataOutput;
class IDataInput;
class PrintStream;

// size: 8
class Tag {
public:
    // vtb+0
    virtual ~Tag() = default;

    // vtb+1
    virtual void deleteChildren() {}

    enum class Type : uint8_t {
        End = 0,
        Byte = 1,
        Short = 2,
        Int = 3,
        Int64 = 4,
        Float = 5,
        Double = 6,
        ByteArray = 7,
        String = 8,
        List = 9,
        Compound = 10,
        IntArray = 11,
    };

    inline static const std::string NullString = "";

    // vtb+2
    virtual void write(IDataOutput &) const = 0;

    // vtb+3
    virtual void load(IDataInput &) = 0;

    // vtb+4
    virtual std::string toString() const = 0;

    // vtb+5
    virtual Type getId() const = 0;

    // vtb+6
    virtual bool equals(const Tag &rhs) const;

    // vtb+7
    virtual void print(PrintStream &out) const;

    // vtb+8
    virtual void print(const std::string &prefix, PrintStream &out) const;

    static Unique<Tag> readNamedTag(IDataInput &dis, std::string &name);

    static void writeNamedTag(const std::string &name, const Tag &tag, IDataOutput &dos);

    static Unique<Tag> newTag(Tag::Type type);

    static std::string getTagName(Type type);

    // vtb+9
    virtual Unique<Tag> copy() const = 0;

    // vtb+10
    virtual size_t hash() const = 0;

    static constexpr int TAGERR_OUT_OF_BOUNDS = 1;
    static constexpr int TAGERR_BAD_TYPE = 2;

    // protected:

    Tag() = default;
};
