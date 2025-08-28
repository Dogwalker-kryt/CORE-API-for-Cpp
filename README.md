# CORE - Centralized Operations & Resource Engine

CORE is a lightweight, C++ library that provides essential system utilities, including command execution, system information retrieval, file manipulation, networking helpers, cryptographic utilities and more. Designed to serve as a centralized engine for common operations and resource management, CORE aims to simplify and standardize interactions with the system.

Its primiarly made for Linux, but with plans to make it cross platform

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
#include "../your_folder_where_its_located/CORE.h"
```
---

## Usage

Below are examples demonstrating how to use various CORE components.

### Time/Date function
```cpp
#include "CORE.h"
int main() {
  std::string currentTime = TimeDatefunc::getTime();
  std::string currentDate = TimeDatefunc::getDate();
  std::string timeAndDate = TimeDatefunc::getTimeAndDate();
  
  std::cout << "Time: " << currentTime << "\nDate: " << currentDate << "\nTime & Date: " << timeAndDate << "\n";
}
```

### Terminal excute fcuntion
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

### System information

for this one it easy for all funciton i this section
```cpp
std::string nameofhost = SystemInfo::the_function()
```

### File Utils
foreasier read, write, list dir and file existace check
```cpp

```
