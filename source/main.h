// Copyright (c) 2025 Siltrez (Via discord: antis1213)
// This file is part of AntisLib and is licensed under the MIT License.
// See the LICENSE file in the project root for full license text.

// AntisLib.cpp
#define _WIN32_WINNT 0x0600
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4996)
#include <winsock2.h>
#include <Windows.h>
#include <Lmcons.h>
#include <WinInet.h>
#include <shellapi.h>
#include <mmsystem.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <string>
#include <sstream>
#include <vector>

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "Iphlpapi.lib")

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "iphlpapi.lib")

extern "C" {

    // --- Basic Info ---
    __declspec(dllexport) const char* GetUserNameC()
    {
        static char name[UNLEN + 1];
        DWORD size = UNLEN + 1;
        if (GetUserNameA(name, &size))
            return name;
        return "Unknown";
    }

    __declspec(dllexport) const char* GetHostNameC()
    {
        static char name[MAX_COMPUTERNAME_LENGTH + 1];
        DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
        if (GetComputerNameA(name, &size))
            return name;
        return "Unknown";
    }

    __declspec(dllexport) const char* GetOSVersionC()
    {
        static char version[128];
        OSVERSIONINFOA osvi = { sizeof(OSVERSIONINFOA) };
        if (GetVersionExA(&osvi))
        {
            sprintf_s(version, "Windows %lu.%lu (Build %lu)",
                osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber);
            return version;
        }
        return "Unknown OS";
    }

    __declspec(dllexport) int GetCPUCountC()
    {
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        return (int)sysinfo.dwNumberOfProcessors;
    }

    __declspec(dllexport) const char* GetCurrentDirectoryC()
    {
        static char dir[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, dir);
        return dir;
    }

    // --- Network ---
    __declspec(dllexport) const char* GetIPAddressesC()
    {
        static std::string output;
        output.clear();

        char hostname[256];
        if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
            return "Error getting hostname";

        addrinfo hints = { 0 }, * info = nullptr;
        hints.ai_family = AF_UNSPEC;
        if (getaddrinfo(hostname, NULL, &hints, &info) != 0)
            return "Error resolving IPs";

        for (addrinfo* p = info; p; p = p->ai_next)
        {
            char ip[INET6_ADDRSTRLEN];
            void* addr = (p->ai_family == AF_INET)
                ? (void*)&((sockaddr_in*)p->ai_addr)->sin_addr
                : (void*)&((sockaddr_in6*)p->ai_addr)->sin6_addr;
            inet_ntop(p->ai_family, addr, ip, sizeof(ip));
            if (!output.empty()) output += ", ";
            output += ip;
        }
        freeaddrinfo(info);
        return output.c_str();
    }

    // --- HTTP (simple blocking) ---
    std::string HttpRequest(const char* url, const char* method, const char* data)
    {
        HINTERNET hInternet = InternetOpenA("AntisLib", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (!hInternet) return "InternetOpen failed";

        HINTERNET hConnect = InternetOpenUrlA(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (!hConnect)
        {
            InternetCloseHandle(hInternet);
            return "InternetOpenUrl failed";
        }

        std::string response;
        char buffer[1024];
        DWORD bytesRead;
        while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0)
            response.append(buffer, bytesRead);

        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return response;
    }

    __declspec(dllexport) const char* HttpGetC(const char* url)
    {
        static std::string result;
        result = HttpRequest(url, "GET", nullptr);
        return result.c_str();
    }

    // --- Clipboard ---
    __declspec(dllexport) const char* GetClipboardTextC()
    {
        static std::string text;
        text.clear();

        if (!OpenClipboard(NULL)) return "";
        HANDLE hData = GetClipboardData(CF_TEXT);
        if (hData)
        {
            char* pszText = static_cast<char*>(GlobalLock(hData));
            if (pszText)
            {
                text = pszText;
                GlobalUnlock(hData);
            }
        }
        CloseClipboard();
        return text.c_str();
    }

    __declspec(dllexport) void SetClipboardTextC(const char* text)
    {
        if (!OpenClipboard(NULL)) return;
        EmptyClipboard();
        size_t len = strlen(text) + 1;
        HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, len);
        if (hGlob)
        {
            memcpy(GlobalLock(hGlob), text, len);
            GlobalUnlock(hGlob);
            SetClipboardData(CF_TEXT, hGlob);
        }
        CloseClipboard();
    }

    // --- Notifications ---
    __declspec(dllexport) void ShowNotificationC(const char* title, const char* message)
    {
        MessageBoxA(NULL, message, title, MB_OK | MB_ICONINFORMATION);
    }

    // --- Sound ---
    __declspec(dllexport) void PlaySystemBeepC()
    {
        MessageBeep(MB_OK);
    }

    __declspec(dllexport) void PlayWavFileC(const char* path)
    {
        PlaySoundA(path, NULL, SND_ASYNC | SND_FILENAME);
    }

    // --- Utilities ---
    __declspec(dllexport) void OpenURLC(const char* url)
    {
        ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
    }

} // extern "C"
