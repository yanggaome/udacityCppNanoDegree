#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
// YG nothing done here
Processor& System::Cpu() { return cpu_; }

// YG DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    processes_.clear();
    std::vector<int> pids = LinuxParser::Pids();
    // create processes using pids
    for (const int p : pids) {
        //Process proc(p);
        //processes_.push_back(proc);
        processes_.emplace_back(p); // emplace_back will call the constructor
    }
    // sort processes based on customerized "<" or ">" operator
    // now it is just based on process cpu utilization
    std::sort(processes_.begin(), processes_.end(), [](Process& a, Process& b) {return a > b; });
    return processes_;
}

// YG DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// YG DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// YG DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// YG DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// YG DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// YG DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
