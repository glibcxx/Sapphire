#pragma once

#include <string>
#include <Windows.h>
#include <sddl.h>

namespace sapphire::ipc::backend {

    class Pipe {
    public:
        ~Pipe() {
            disconnect();
        }

        bool isOpen() const {
            return hPipe != INVALID_HANDLE_VALUE;
        }

        bool connect(const std::wstring &pipeName, int timeout = 20000) {
            if (isOpen()) return true;

            while (true) {
                hPipe = CreateFileW(
                    pipeName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr
                );

                if (isOpen()) return true;

                if (GetLastError() != ERROR_PIPE_BUSY) {
                    hPipe = INVALID_HANDLE_VALUE;
                    return false;
                }

                if (!WaitNamedPipeW(pipeName.c_str(), timeout)) {
                    hPipe = INVALID_HANDLE_VALUE;
                    return false;
                }
            }
        }

        bool create(const std::wstring &pipeName) {
            if (isOpen()) return true;

            PSECURITY_DESCRIPTOR p_sd = nullptr;
            SECURITY_ATTRIBUTES  sa = {};
            sa.nLength = sizeof(SECURITY_ATTRIBUTES);
            sa.bInheritHandle = FALSE;
            ConvertStringSecurityDescriptorToSecurityDescriptorW(
                L"D:(A;;GA;;;WD)(A;;GA;;;AC)",
                SDDL_REVISION_1,
                &p_sd,
                nullptr
            );
            sa.lpSecurityDescriptor = p_sd;
            if (p_sd) LocalFree(p_sd);

            hPipe = CreateNamedPipeW(
                pipeName.c_str(),
                PIPE_ACCESS_DUPLEX,
                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                PIPE_UNLIMITED_INSTANCES,
                4096,
                4096,
                0,
                &sa
            );

            return isOpen();
        }

        bool listen() {
            if (!isOpen()) return false;

            BOOL connected = ConnectNamedPipe(hPipe, nullptr);
            if (!connected && GetLastError() != ERROR_PIPE_CONNECTED) {
                return false;
            }
            return true;
        }

        void disconnect() {
            if (isOpen()) {
                DisconnectNamedPipe(hPipe);
                CloseHandle(hPipe);
                hPipe = INVALID_HANDLE_VALUE;
            }
        }

        bool read(char *buffer, uint32_t bytesToRead) {
            if (!isOpen()) return false;

            DWORD totalBytesRead = 0;
            while (totalBytesRead < bytesToRead) {
                DWORD bytesRead = 0;
                BOOL  result = ReadFile(
                    hPipe, buffer + totalBytesRead, bytesToRead - totalBytesRead, &bytesRead, nullptr
                );

                if (!result || bytesRead == 0) {
                    disconnect(); // Assume connection is lost
                    return false;
                }
                totalBytesRead += bytesRead;
            }
            return true;
        }

        bool write(const char *buffer, uint32_t bytesToWrite) {
            if (!isOpen()) return false;

            DWORD bytesWritten = 0;
            BOOL  result = WriteFile(hPipe, buffer, bytesToWrite, &bytesWritten, nullptr);

            if (!result || bytesWritten != bytesToWrite) {
                disconnect(); // Assume connection is lost
                return false;
            }
            return true;
        }

    private:
        HANDLE hPipe = INVALID_HANDLE_VALUE;
    };

} // namespace sapphire::ipc::backend