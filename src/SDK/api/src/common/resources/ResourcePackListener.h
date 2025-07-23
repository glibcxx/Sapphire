#pragma once

class ResourcePackManager;

// size: 8
class ResourcePackListener {
public:
    // vtb+0
    virtual ~ResourcePackListener() = default;

    // vtb+1
    virtual void onActiveResourcePacksChanged(ResourcePackManager &mgr) = 0;

    // vtb+2
    virtual void onFullPackStackInvalid();

    // vtb+3
    virtual void onBaseGamePackDownloadComplete();

    // vtb+4
    virtual void onLanguageSubpacksChanged();

    // vtb+5
    virtual void onResourceManagerDestroyed(ResourcePackManager &mgr);
};