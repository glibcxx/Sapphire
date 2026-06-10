#pragma once

#include "pch.h" // IWYU pragma: keep

#include "ITransactionContainer.h"
#include "AsyncExecutionPolicy.h"

namespace mce {

    template <typename TPayload, typename TService, typename TExecutionPolicy = AsyncExecutionPolicy>
    class TransactionContainer
        : public ITransactionContainer,
          protected TExecutionPolicy {
    public:
        using ResourceService = TService;
        using ResourceServiceContext = typename ResourceService::ResourceServiceContext;
        using ResourcePtr = typename ResourceService::ResourcePtr;
        using ResourceTransactionPointer = typename ResourceService::FactoryTrackerPtr;
        using Payload = TPayload;

        ResourceTransactionPointer     mResourceTransactionPointer; // off+8
        std::function<void(Payload &)> mDeferredPayload;            // off+24
        Payload                        mPayload;                    // off+88

        gsl::not_null<ResourceServiceContext *> mResourceServiceContext;
    };

} // namespace mce