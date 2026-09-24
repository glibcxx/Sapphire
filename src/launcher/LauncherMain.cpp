#include "DebuggerApp.h"
#include "LauncherApp.h"
#include "common/sys/MiniWindows.h"

int main(int argc, char **argv) {
    sapphire::sys::win::dword_t dwProcessId = 0;
    sapphire::sys::win::dword_t dwTargetTid = 0;
    for (int i = 1; i < argc; i += 2) {
        std::string_view arg(argv[i]);
        if (arg == "-p")
            dwProcessId = atoi(argv[i + 1]);
        else if (arg == "-tid")
            dwTargetTid = atoi(argv[i + 1]);
    }

    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    if (!dwProcessId) {
        sapphire::launcher::LauncherApp app;
        return app.run();
    } else {
        sapphire::launcher::DebuggerApp app{dwProcessId};
        return app.run();
    }
}