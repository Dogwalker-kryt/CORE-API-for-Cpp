# CORE - Centralized Operations & Resource Engine

CORE is a lightweight, C++ library that provides essential system utilities, including command execution, system information retrieval, file manipulation, networking helpers, cryptographic utilities and more. Designed to serve as a centralized engine for common operations and resource management, CORE aims to simplify and standardize interactions with the system.

Its primiarly made for Linux, but with plans to make it cross platform, but soem function are nativly cross platform.


---

## Features

- Execute terminal or shell commands with ease
- Retrieve current system time, date, and uptime
- Access system information such as hostname, current user, and OS version
- Perform basic file operations: check existence, list directory contents, read and write files
- Perform simple network tasks like pinging hosts and retrieving local IP addresses
- Generate SHA-256 hashes and UUIDs for secure identifiers
- **more will be added**

---

## Installation

first you need to clone/download the repo
```sh
git clone https://github.com/Dogwalker-kryt/CORE-API-for-Cpp
```
after this you can put teh header in the usr/include folder or just include in you project dir.

if you put it in usr/include you can call it with:
```cpp
#include <CORE.h>
```
**OR**

if you put it in your project directory you can call it with 
```cpp
#include "CORE.h"
```
or if it is burried in some folders
```cpp
#include "your_folder_where_its_located/CORE.h"
```
or
```cpp
#include "../CORE.h"
```
---

## Usage

Below are examples demonstrating how to use various CORE components.

### Time/Date function
will run on Windows, Linux, MacOs and other Unix systems

```cpp
#include "CORE.h"
int main() {
  std::string currentTime = TimeDatefunc::getTime();
  std::string currentDate = TimeDatefunc::getDate();
  std::string timeAndDate = TimeDatefunc::getTimeAndDate();
  
  std::cout << "Time: " << currentTime << "\nDate: " << currentDate << "\nTime & Date: " << timeAndDate << "\n";
}
```
---

### Terminal excute fcuntion
will run on Linux, Unix like systems and MacOs

**v1**

for basic commands andoutput catching
```cpp
// v1
int main() {
  Trmexec::execTerminal("clear");
  return 0;
// you can also use the v1 for longer command and/or outputs to catch 
}
```
**v2**

for more advanced comamnds wiht larger/compliated outputs
```cpp
// v2
int main() {
  std::string cmd = "sudo dd if=" + drive + " of=" + imagePath + " bs=4M status=progress && sync";
  std::string output = Trmexec::execTerminalv2(cmd.c_str());
  return 0;
}
```
**v3**

for even more advanced command and outputs wiht more thanone word for one info 
```cpp
// v3
int main() {
  std::string cmd = "lsblk -J -o NAME,SIZE,MODEL,SERIAL,TYPE,MOUNTPOINT,VENDOR,FSTYPE,UUID -p " + drive;
  std::string json = Terminalexec::execTerminalv3(cmd);
  return 0;
  // here we need to catch all the outputs in the same order. so you use the v3, because we dont know if the produckt name is somehting like Samsung SSD or just nvme.
  // you use it so the outputs dont scramble
  // in the other types like v1 and v2 th output will vary and display fasle info beause they can only read one output word no 2, 3 or more
}
```
---

### System information
runs on Linux, Unix like and MacOs
```cpp
std::string nameofhost = SystemInfo::the_function()
```
---

### File Utils
foreasier read, write, list dir and file existace check
runs on all systems

check existance
```cpp
bool exiteance = FileUtil::fileExists("/path/to/your/file");
```
read file
```cpp
std::string content = FileUtil::readFile("path/to/your/file.txt");
```

write to a file
```cpp
bool success = FileUtil::writeFile("path/to/your/file.txt", "Your content goes here");
```

listfiles in a directory
```cpp
std::vector<std::string> files = FileUtil::listDirectory("path/to/your/directory");
```
---

### Hashutils
runs on all sytems

sha256
```cpp
std::srting hash = HashUtils::saha256("Hello world");
std::cout << hash << "\n";
```

UUID
```cpp
std::string generatedId = HashUtils::generateUUID();
std::cout << generatedId << "\n";
```
---

### NetworkUtils
get local ip
```cpp
std::string localIP = NetUtils::getLocalIPAddress();
std::cout << "Local IP Address: " << localIP << "\n";
```
ping
```cpp
std::string host = "google.com";
if (NetUtils::pingHost(host)) {
    std::cout << host << " is reachable.\n";
  } else {
    std::cout << host << " is not reachable.\n";
  }
```
