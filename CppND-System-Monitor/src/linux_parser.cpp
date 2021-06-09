#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
// local utility function to find a value by key from a file
template <typename T>
T findValueByKey(std::string const &keyTarget, std::string const &fileName) {
  T value;
  std::string line, key;

  std::ifstream fileStream(fileName);
  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      std::istringstream lineStream(line);
      while (lineStream >> key >> value) {
        if (key == keyTarget) {
          fileStream.close();
          return value;
        }
      }
    }
  }

  fileStream.close();
  return value;
}

// get value from one line file
template <typename T>
T getValueOfFile(std::string const &fileName) {
  T value;
  std::string line;
  std::ifstream fileStream(fileName);
  if (fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream lineStream(line);
    lineStream >> value;
  }
  fileStream.close();
  return value;
}

// local utility function to process a file and push all lines to a vector of string
std::vector<std::string> pushStringToVector(std::string const &filename) {
  std::vector<string> result;
  std::string line, value;
  std::ifstream fileStream(filename);

  if (fileStream) {
    std::getline(fileStream, line);

    std::istringstream lineStream(line);
    while (lineStream >> value) result.push_back(value);
  }
  fileStream.close();

  return result;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream fileStream(kOSPath);
  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream lineStream(line);
      while (lineStream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return "Linux"; // set default to Linux
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream lineStream(line);
    lineStream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// YG DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::string memTotal = "MemTotal:";
  std::string memFree = "MemFree:";
  float totalMem = findValueByKey<float>(memTotal, kProcDirectory + kMeminfoFilename);
  float freeMem = findValueByKey<float>(memFree, kProcDirectory + kMeminfoFilename);
  return (totalMem - freeMem) / totalMem;
 }

// YG DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  return getValueOfFile<long>(kProcDirectory + kUptimeFilename);
}

// YG DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies() - IdleJiffies(); }

// YG DONE: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  std::string filename = kProcDirectory + std::to_string(pid) + kStatFilename;
  std::vector<string> strVec = pushStringToVector(filename);

  if (strVec.size() >= 21)
    return stol(strVec[13]) + stol(strVec[14]) + stol(strVec[15]) + stol(strVec[16]);

  return 0;
}

// YG DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  const auto cpu_stats = LinuxParser::CpuUtilization();
  const auto user = stol(cpu_stats[LinuxParser::kUser_]);
  const auto nice = stol(cpu_stats[LinuxParser::kNice_]);
  const auto system = stol(cpu_stats[LinuxParser::kSystem_]);
  const auto idle = stol(cpu_stats[LinuxParser::kIdle_]);
  const auto iowait = stol(cpu_stats[LinuxParser::kIOwait_]);
  const auto irq = stol(cpu_stats[LinuxParser::kIRQ_]);
  const auto softirq = stol(cpu_stats[LinuxParser::kSoftIRQ_]);
  const auto steal = stol(cpu_stats[LinuxParser::kSteal_]);

  return user + nice + system + idle + iowait + irq + softirq + steal;
}

// YG DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  const auto cpu_stats = LinuxParser::CpuUtilization();
  const auto idle = stol(cpu_stats[LinuxParser::kIdle_]);
  const auto iowait = stol(cpu_stats[LinuxParser::kIOwait_]);

  return (idle + iowait);
}

// YG DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::vector<string> result;
  std::string line, value;
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  if (fileStream.is_open()) {
    std::getline(fileStream, line); // only first line is needed, no need to do while loop
    std::istringstream lineStream(line);
    lineStream >> value; // dummy "cpu" string
    
    // process values
    for (int i = 0; i < CPUStates::kGuestNice_; i++) {
      lineStream >> value;
      result.push_back(value);
    }
  }

  fileStream.close();
  return result;
}

// YG DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::string keyTarget = "processes";
  return findValueByKey<int>(keyTarget, kProcDirectory + kStatFilename);
}

// YG DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::string keyTarget = "procs_running";
  return findValueByKey<int>(keyTarget, kProcDirectory + kStatFilename);
}

// YG DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  return getValueOfFile<std::string>(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
}

// YG DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  //VmRSS is the measure of how much RAM the process is actually using.
  //VmSize includes RSS, plus things like shared libraries and memory mapped files (which don't actually use RAM), as well as allocated, but unused, memory.
  std::string keyTarget = "VmRSS:";
  long memKB = findValueByKey<long>(keyTarget, kProcDirectory + std::to_string(pid) + kStatusFilename);
  return std::to_string(memKB / 1024);
}

// YG DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::string keyTarget = "Uid:";
  return findValueByKey<std::string>(keyTarget, kProcDirectory + std::to_string(pid) + kStatusFilename);
}

// YG DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  std::string key = LinuxParser::Uid(pid);
  std::string line, uid, user;

  std::ifstream fileStream(kPasswordPath);

  if(fileStream.is_open()) {
    while(std::getline(fileStream, line)) {
      std::replace(line.begin(), line.end(), 'x', ' ');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream(line);
      while(lineStream >> user >> uid) {
        if (uid == key)
          return user;
      }
    }
  }
  fileStream.close();
  return user;
}

// YG DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  std::vector<string> result = pushStringToVector(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (result.size() >= 21) {
    return UpTime() - std::stol(result[21]) / sysconf(_SC_CLK_TCK);
  }
  return 0;
}
