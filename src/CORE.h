#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <memory>
#include <stdexcept>
#include <array>
#include <limits>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <cstring>
#include <fstream>
#include <regex>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <stdexcept>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/sha.h>
#include <uuid/uuid.h>

class Trmexec {
    public:
        static std::string execTerminal(const char*);
        static std::string execTerminalv2(const std::string);
        static std::string execTerminalv3(const std::string);
};

class TimeDatefunc {
    public:
        static void Time(const std::string timestr);
};

class SystemInfo {
    public:
        static std::string getHostname();
        static std::string getCurrentUser();
        static std::string getSystemUptime();
        static std::string getOSVersion();
        
};

class FileUtil {
    public:
        static bool fileExists(const std::string& path);
        static std::vector<std::string> listDirectory(const std::string& path);
        static std::string readFile(const std::string& path);
        static bool writeFile(const std::string& path, const std::string& data);
};

class Logger {
public:
    static void LogWarning(const std::string& message);
    static void LogWarningFile(const std::string& message, const std::string path);
    static void LogError(const std::string& message);
    static void LogErrorFile(const std::string& message, const std::string path);
    static void LogInfo(const std::string& message);
    static void LogInfoFile(const std::string& message, const std::string path);
};
#endif 
