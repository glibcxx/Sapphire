#pragma once

#include "ResourceEvent.h"

namespace rendergraph {

    class APIResourcePool;
    class BindInterface;
    class ResourceViewInterface;
    class ResourceTargetInterface;

    enum class ValidationResult : int {};

    // size: 8
    class Resource {
    public:
        // vtb+0
        virtual ~Resource() = default;

        // vtb+1
        virtual optional_ref<const ResourceViewInterface> getViewInterface() const noexcept = 0;

        // vtb+2
        virtual optional_ref<const ResourceTargetInterface> getTargetInterface() const noexcept = 0;

        // vtb+3
        virtual ValidationResult acquireAPIResources(APIResourcePool &pool) = 0;

        // vtb+4
        virtual ValidationResult enterRenderScope() noexcept = 0;

        // vtb+5
        virtual ValidationResult exitRenderScope() noexcept = 0;

        // vtb+6
        virtual void bind(BindInterface &binder) const = 0;

        // vtb+7
        virtual ResourceBinding::ResourceType getBindingType() const = 0;
    };

} // namespace rendergraph