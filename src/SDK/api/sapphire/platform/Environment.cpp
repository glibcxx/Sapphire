#include "Environment.h"

sapphire::platform::Environment &sapphire::platform::Environment::getInstance() {
    static sapphire::platform::Environment env;
    return env;
}

sapphire::platform::Environment::Environment() {
    mGameModule = GetModuleHandle(nullptr);
    mMainWindow = FindWindow(0, L"Minecraft");
    GetModuleHandleEx(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCWSTR)&Environment::getInstance,
        &mSapphireModule
    );
    GetModuleInformation(GetCurrentProcess(), this->mSapphireModule, &mSapphireModuleInfo, sizeof(MODULEINFO));
    wchar_t modulePathBuf[MAX_PATH] = {0};
    if (GetModuleFileName(mSapphireModule, modulePathBuf, MAX_PATH) != 0) {
        mSapphireCorePath = modulePathBuf;
        mSapphireBinPath = mSapphireCorePath.parent_path();
        mSapphireHomePath = mSapphireBinPath.parent_path() / SPHR_HOME_FOLDER_NAME;
    }
}