#pragma once

enum class ResourceFileSystem : int
{
    UserPackage = 0,
    AppPackage = 1,
    Raw = 2,
    RawPersistent = 3,
    SettingsDir = 4,
    ExternalDir = 5,
    ServerPackage = 6,
    DataDir = 7,
    UserDir = 8,
    ScreenshotsDir = 9,
    StoreCache = 10,
    Invalid = 11,
};

class ResourceUtil {};
