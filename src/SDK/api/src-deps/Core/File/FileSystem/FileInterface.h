#pragma once

#include "../Path.h"
#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"

typedef void *FilePtr;

enum class FileType : int {
    Invalid = 0,
    Zip = 1,
    EncryptedZip = 2,
    Other = 3,
};

// size: 8
class IFileReadAccess {
public:
    // vtb+0
    virtual ~IFileReadAccess();

    // vtb+1
    virtual size_t fread(void *, size_t, size_t, FilePtr) const = 0;
};

// size: 8
class IFileWriteAccess {
public:
    // vtb+0
    virtual ~IFileWriteAccess();

    // vtb+1
    virtual size_t fwrite(const void *, size_t, size_t, FilePtr) = 0;
};

// size: 24
class IFileAccess : public Bedrock::EnableNonOwnerReferences {
public:
    // vtb+0
    virtual ~IFileAccess();

    // vtb+1
    virtual FilePtr fopen(const Core::Path &, const std::string &) = 0;

    // vtb+2
    virtual int fclose(FilePtr) = 0;

    // vtb+3
    virtual int fseek(FilePtr, int64_t, int) = 0;

    // vtb+4
    virtual int64_t ftell(FilePtr) = 0;

    // vtb+5
    virtual const IFileReadAccess *getReadInterface() const = 0;

    // vtb+6
    virtual IFileWriteAccess *getWriteInterface() = 0;

    // vtb+7
    virtual void unload() = 0;
};