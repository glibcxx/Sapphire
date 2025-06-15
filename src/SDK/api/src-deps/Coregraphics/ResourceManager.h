#pragma once

namespace cg {

    template <typename Res, typename Loc, typename Tracker, typename Handle, template <typename K, typename V> class Container>
    class ResourceManager {
    public:
        using ResourceType = Res;
        using ResourceHandle = Handle;
        using ResourceLocation = Loc;
        using ResourceTracker = Tracker;
        using ResourceManagerBase = ResourceManager;

        Container<Loc, Res> mContainer; // off+0
    };

} // namespace cg