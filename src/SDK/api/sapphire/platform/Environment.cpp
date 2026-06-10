#include "Environment.h"

#include <Windows.h>
#include <Psapi.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.ApplicationModel.h>

namespace sapphire::platform {

    class EnvironmentImpl : public Environment {
    public:
        EnvironmentImpl() {
            mGameModule = GetModuleHandle(nullptr);
            mMainWindow = FindWindow(0, L"Minecraft");
            GetModuleHandleEx(
                GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                (LPCWSTR)&Environment::getInstance,
                &mSapphireModule
            );
            mSapphireVersion = {SAPPHIRE_VERSION_MAJOR, SAPPHIRE_VERSION_MINOR, SAPPHIRE_VERSION_PATCH};
            GetModuleInformation(GetCurrentProcess(), this->mSapphireModule, &mSapphireModuleInfo, sizeof(MODULEINFO));
            wchar_t modulePathBuf[MAX_PATH] = {0};
            if (GetModuleFileName(mSapphireModule, modulePathBuf, MAX_PATH) != 0) {
                mSapphireCorePath = modulePathBuf;
                mSapphireBinPath = mSapphireCorePath.parent_path();
                mSapphireHomePath = mSapphireBinPath.parent_path() / SPHR_HOME_FOLDER_NAME;
            }
            auto v = winrt::Windows::ApplicationModel::Package::Current().Id().Version();
            mGameVersion = {v.Major, v.Minor, v.Build / 100};
            mGameVersionStr = mGameVersion.toString();
        }
        ~EnvironmentImpl() = default;

    private:
        MODULEINFO mSapphireModuleInfo;
    };

} // namespace sapphire::platform

sapphire::platform::Environment &sapphire::platform::Environment::getInstance() {
    static sapphire::platform::EnvironmentImpl env;
    return env;
}

std::string_view sapphire::platform::Environment::getSapphireVersionStr() const noexcept {
    return SAPPHIRE_VERSION_STRING;
}

void sapphire::platform::Environment::setMainWindow(HWND hwnd) {
    mMainWindow = hwnd;
}