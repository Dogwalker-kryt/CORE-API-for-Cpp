#include "CORE.h"

class Trmexec {
    public:
        static std::string execTerminal(const char* cmd) {
            std::array<char, 128> buffer;
            std::string result;
            auto deleter = [](FILE* f) { if (f) pclose(f); };
            std::unique_ptr<FILE, decltype(deleter)> pipe(popen(cmd, "r"), deleter);
            if (!pipe) throw std::runtime_error("popen() failed!");
            while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
                result += buffer.data();
            }
            return result;
        }
        //v2 for a bigger in/output to catch
        static std::string execTerminalv2(const std::string &command) {
            std::array<char, 128> buffer;
            std::string result;
            std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
            if (!pipe) return "";
            while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
                result += buffer.data();
            }
            return result;
        }
        //v3 for a even bigger in/output to catch
        static std::string execTerminalv3(const std::string& cmd) {
            std::array<char, 512> buffer;
            std::string result;
            FILE* pipe = popen(cmd.c_str(), "r");
            if (!pipe) {
                throw std::runtime_error("popen() failed");
            }
            while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
                result += buffer.data();
            }
            int returnCode = pclose(pipe);
            if (returnCode != 0) {
                return ""; 
            }
            while (!result.empty() && (result.back() == '\n' || result.back() == '\r')) {
                result.pop_back();
            }
            return result;
        }
};

class TimeDatefunc {
public: 
    // only time 
    static std::string getTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm* local_time = std::localtime(&now_time);
        std::ostringstream oss;
        oss << std::put_time(local_time, "%H:%M:%S");
        return oss.str();
    }
    // only date
    static std::string getDate() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_date = std::chrono::system_clock::to_time_t(now);
        std::tm* local_date = std::localtime(&now_date);

        std::ostringstream oss;
        oss << std::put_time(local_date, "%Y-%m-%d");
        return oss.str();
    }
    // if you need both 
    static std::string getTimeAndDate() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_all = std::chrono::system_clock::to_time_t(now);
        std::tm* local_all = std::localtime(&now_all);

        std::ostringstream oss;
        oss << std::put_time(local_all, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
};

class SystemInfo {
public:
    static std::string getHostname() {
        char hostname[1024];
        if (gethostname(hostname, sizeof(hostname)) == 0) {
            return std::string(hostname);
        }
        return "Unknown";
    }

    static std::string getCurrentUser() {
        const char* user = getenv("USER");
        if (user) return std::string(user);
        return "Unknown";
    }

    static std::string getSystemUptime() {
        struct sysinfo info;
        if (sysinfo(&info) != 0) return "Unavailable";

        long hours = info.uptime / 3600;
        long minutes = (info.uptime % 3600) / 60;
        long seconds = info.uptime % 60;

        std::ostringstream oss;
        oss << hours << "h " << minutes << "m " << seconds << "s";
        return oss.str();
    }

    static std::string getOSVersion() {
        struct utsname buffer;
        if (uname(&buffer) == 0) {
            return std::string(buffer.sysname) + " " + buffer.release;
        }
        return "Unknown";
    }
};


class FileUtil {
public:
    static bool fileExists(const std::string& path) {
        struct stat buffer;
        return (stat(path.c_str(), &buffer) == 0);
    }

    static std::vector<std::string> listDirectory(const std::string& path) {
        std::vector<std::string> files;
        DIR* dir = opendir(path.c_str());
        if (!dir) return files;

        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            files.emplace_back(entry->d_name);
        }

        closedir(dir);
        return files;
    }

    static std::string readFile(const std::string& path) {
        std::ifstream file(path);
        if (!file) return "";

        std::ostringstream oss;
        oss << file.rdbuf();
        return oss.str();
    }

    static bool writeFile(const std::string& path, const std::string& data) {
        std::ofstream file(path);
        if (!file) return false;

        file << data;
        return true;
    }
};


class NetUtils {
public:
    static bool pingHost(const std::string& host) {
        std::string command = "ping -c 1 " + host + " > /dev/null 2>&1";
        return system(command.c_str()) == 0;
    }

    static std::string getLocalIPAddress() {
        struct ifaddrs* ifaddr;
        if (getifaddrs(&ifaddr) == -1) return "Unavailable";

        std::string ipAddress = "Unavailable";
        for (struct ifaddrs* ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
                void* addr_ptr = &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;
                char buffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, addr_ptr, buffer, INET_ADDRSTRLEN);
                if (strcmp(ifa->ifa_name, "lo") != 0) {
                    ipAddress = buffer;
                    break;
                }
            }
        }

        freeifaddrs(ifaddr);
        return ipAddress;
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
        char uuidStr[37];
        uuid_generate(uuid);
        uuid_unparse(uuid, uuidStr);
        return std::string(uuidStr);
    }
};