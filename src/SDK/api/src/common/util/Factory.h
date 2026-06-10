#pragma once

#include "pch.h" // IWYU pragma: keep

template <typename Type, typename... Args>
class OwnerPtrFactory {
public:
    using TypeCreator = std::function<std::unique_ptr<Type>(Args...)>;

    void registerFactory(const std::string &name, TypeCreator creator);

    // private:

    using FactoryMap = std::unordered_map<std::string, TypeCreator>;

    FactoryMap mFactoryMap;
};