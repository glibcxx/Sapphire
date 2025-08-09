#pragma once

#include <vector>
#include <functional>

#include "ResourceUtil.h"
#include "SDK/api/src-deps/Core/File/Path.h"
#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "SDK/api/src-deps/Core/Utility/UUID.h"
#include "SDK/api/src-deps/Core/Utility/SemVersion.h"

// size: 56
class ResourceLocation {
public:
    ResourceFileSystem   mFileSystem;
    Core::HeapPathBuffer mPath;
    uint64_t             mPathHash;
    size_t               mFullHash;
};
static_assert(sizeof(ResourceLocation) == 56);

class ResourceInformation {};

// size: 88
class ResourceLoader : public Bedrock::EnableNonOwnerReferences {
public:
    virtual ~ResourceLoader() = default;

    std::function<Core::HeapPathBuffer()> mGetPath;
};

enum class ResourcePackStackType : int {
    LEVEL = 0,
    ADDON = 1,
    GLOBAL = 2,
    TREATMENT = 3,
    BASE_GAME = 4,
    SIZE = 5,
};

enum class PackType : char {
    Invalid = 0,
    Addon = 1,
    Cached = 2,
    CopyProtected = 3,
    Behavior = 4,
    PersonaPiece = 5,
    Resources = 6,
    Skins = 7,
    WorldTemplate = 8,
    Count = 9,
};

struct PackIdVersion {
    mce::UUID  mId;
    SemVersion mVersion;
    PackType   mPackType;
};

class ResourceLocationPair {
public:
    ResourceLocation mResourceLocation;
    PackIdVersion    mPackId;
    int              mPackPosition;
};