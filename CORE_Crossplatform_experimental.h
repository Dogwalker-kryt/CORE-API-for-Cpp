// this is the Crossplatform variant thisone only needs this file teh nromal version for linux/unix is in the src folder
#pragma once

#include <string>
#include <array>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <fstream>
#include <vector>
#include <memory>
#include <filesystem>

#ifdef _WIN32
  #include <windows.h>
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "Ws2_32.lib")
#else
  #include <sys/utsname.h>
  #include <sys/types.h>
  #include <sys/sysinfo.h>
  #include <unistd.h>
  #include <ifaddrs.h>
  #include <arpa/inet.h>
  #include <net/if.h>
  #include <netdb.h>
  #include <sys/stat.h>
  #include <dirent.h>
#endif

#include <openssl/sha.h>
#include <uuid/uuid.h>

namespace CoreUtils {

class Trmexec {
public:
    static std::string execCommand(const std::string& cmd, size_t bufferSize = 512) {
        std::array<char, 1024> buffer; // max buffer
        std::string result;
        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe) throw std::runtime_error("popen() failed");
        while (fgets(buffer.data(), std::min(bufferSize, buffer.size()), pipe) != nullptr) {
            result += buffer.data();
        }
        int ret = pclose(pipe);
        if (ret != 0) throw std::runtime_error("Command failed with return code " + std::to_string(ret));
        while (!result.empty() && (result.back() == '\n' || result.back() == '\r'))
            result.pop_back();
        return result;
    }
};

class TimeDate {
public:
    static std::string getTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        tm local;
#ifdef _WIN32
        localtime_s(&local, &t);
#else
        localtime_r(&t, &local);
#endif
        std::ostringstream oss;
        oss << std::put_time(&local, "%H:%M:%S");
        return oss.str();
    }
    static std::string getDate() {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        tm local;
#ifdef _WIN32
        localtime_s(&local, &t);
#else
        localtime_r(&t, &local);
#endif
        std::ostringstream oss;
        oss << std::put_time(&local, "%Y-%m-%d");
        return oss.str();
    }
    static std::string getTimeDate() {
        return getDate() + " " + getTime();
    }
};

class SystemInfo {
public:
    static std::string getHostname() {
#ifdef _WIN32
        char name[256];
        WSADATA wsa;
        WSAStartup(MAKEWORD(2,2), &wsa);
        if (gethostname(name, sizeof(name)) != 0) {
            WSACleanup();
            return "Unknown";
        }
        WSACleanup();
        return name;
#else
        char name[HOST_NAME_MAX];
        if (gethostname(name, sizeof(name)) != 0)
            return "Unknown";
        return name;
#endif
    }
    static std::string getUptime() {
#ifdef _WIN32
        ULONGLONG ms = GetTickCount64();
        ms /= 1000;
        unsigned long h = ms / 3600;
        unsigned long m = (ms / 60) % 60;
        unsigned long s = ms % 60;
        std::ostringstream oss;
        oss << h << "h " << m << "m " << s << "s";
        return oss.str();
#else
        struct sysinfo info;
        if (sysinfo(&info) != 0) return "Unavailable";
        long h = info.uptime / 3600;
        long m = (info.uptime % 3600) / 60;
        long s = info.uptime % 60;
        std::ostringstream oss;
        oss << h << "h " << m << "m " << s << "s";
        return oss.str();
#endif
    }
    static std::string getOSVersion() {
#ifdef _WIN32
        OSVERSIONINFOEX info;
        ZeroMemory(&info, sizeof(info));
        info.dwOSVersionInfoSize = sizeof(info);
        if (!GetVersionEx((OSVERSIONINFO*)&info)) return "Unknown";
        std::ostringstream oss;
        oss << "Windows " << info.dwMajorVersion << "." << info.dwMinorVersion;
        return oss.str();
#else
        struct utsname buf;
        if (uname(&buf) == 0)
            return std::string(buf.sysname) + " " + buf.release;
        return "Unknown";
#endif
    }
};

class FileUtil {
public:
    static bool fileExists(const std::string& path) {
        return std::filesystem::exists(path);
    }
    static bool writeFile(const std::string& path, const std::string& data, bool append = false) {
        std::ofstream ofs(path, append ? std::ios::app : std::ios::trunc);
        if (!ofs) return false;
        ofs << data;
        return true;
    }
    static std::string readFile(const std::string& path) {
        std::ifstream ifs(path);
        if (!ifs) return "";
        std::ostringstream oss;
        oss << ifs.rdbuf();
        return oss.str();
    }
    static std::vector<std::string> listDirectory(const std::string& path) {
        std::vector<std::string> names;
        for (const auto& entry : std::filesystem::directory_iterator(path))
            names.push_back(entry.path().filename().string());
        return names;
    }
};

class NetUtils {
public:
    static bool pingHost(const std::string& host) {
#ifdef _WIN32
        std::string cmd = "ping -n 1 " + host + " >nul";
#else
        std::string cmd = "ping -c 1 " + host + " > /dev/null 2>&1";
#endif
        return system(cmd.c_str()) == 0;
    }
    static std::string getLocalIPAddress() {
#ifdef _WIN32
        char hostname[256];
        gethostname(hostname, sizeof(hostname));
        addrinfo hints = {}, *res = nullptr;
        hints.ai_family = AF_INET;
        getaddrinfo(hostname, nullptr, &hints, &res);
        if (!res) return "Unavailable";
        sockaddr_in* addr = (sockaddr_in*)res->ai_addr;
        char buf[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr->sin_addr, buf, sizeof(buf));
        freeaddrinfo(res);
        return buf;
#else
        struct ifaddrs* ifaddr;
        if (getifaddrs(&ifaddr) == -1) return "Unavailable";
        std::string ip = "Unavailable";
        for (auto* ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
            if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_INET) continue;
            if (strcmp(ifa->ifa_name, "lo") == 0) continue;
            char buf[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr, buf, sizeof(buf));
            ip = buf;
            break;
        }
        freeifaddrs(ifaddr);
        return ip;
#endif
    }
};

class HashUtils {
public:
    static std::string sha256(const std::string& input) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((const unsigned char*)input.c_str(), input.size(), hash);
        std::ostringstream oss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
            oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        return oss.str();
    }
    static std::string generateUUID() {
        uuid_t uuid;
        char str[37];
        uuid_generate(uuid);
        uuid_unparse(uuid, str);
        return str;
    }
};

class Logger {
public:
    enum class Level { Info, Warning, Error };
    static void log(const std::string& msg, Level level = Level::Info, const std::string* path = nullptr) {
        std::string lvlStr = (level == Level::Info ? "[Info] " : level == Level::Warning ? "[Warning] " : "[Error] ");
        std::string full = TimeDate::getTimeDate() + " " + lvlStr + msg + "\n";
        if (path) {
            FileUtil::writeFile(*path, full, true);
        } else {
            std::cout << full;
        }
    }
    static void info(const std::string& msg, const std::string* path = nullptr) {
        log(msg, Level::Info, path);
    }
    static void warning(const std::string& msg, const std::string* path = nullptr) {
        log(msg, Level::Warning, path);
    }
    static void error(const std::string& msg, const std::string* path = nullptr) {
        log(msg, Level::Error, path);
    }
};

}
