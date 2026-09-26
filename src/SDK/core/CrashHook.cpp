#include "SDK/api/sapphire/hook/Hook.h"

namespace {

    HOOK_STATIC(
        DiscardSetUnhandledExceptionFilterHook,
        sapphire::HookPriority::Normal,
        SetUnhandledExceptionFilter,
        LPTOP_LEVEL_EXCEPTION_FILTER,
        LPTOP_LEVEL_EXCEPTION_FILTER
    ) {
        return nullptr;
    }

    HOOK_STATIC(
        DiscardSetTerminateHook,
        sapphire::HookPriority::Normal,
        set_terminate,
        terminate_handler,
        terminate_handler
    ) {
        return nullptr;
    }

    class AutoInstallUninstall {
    public:
        AutoInstallUninstall() {
            DiscardSetUnhandledExceptionFilterHook::hook();
            DiscardSetTerminateHook::hook();
        }
        ~AutoInstallUninstall() {
            DiscardSetUnhandledExceptionFilterHook::unhook();
            DiscardSetTerminateHook::unhook();
        }
    } autoInstallUninstall;

} // namespace
