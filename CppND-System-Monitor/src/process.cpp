#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
    computeCpuUtilization(); // can be used for sorting
    computeRam(); // can be used for sorting
};
// YG DONE: Return this process's ID
int Process::Pid() {
    return pid_;
}

// YG: compute and store cpu utilization when process is created
void Process::computeCpuUtilization() {
    float result = LinuxParser::ActiveJiffies(pid_) / sysconf(_SC_CLK_TCK);
    cpu_utilization_ = result / UpTime();
}
// YG DONE: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_utilization_;}

// YG DONE: Return the command that generated this process
// if command is longer than 50, append ...
string Process::Command() {
    string cmd = LinuxParser::Command(pid_);
    string result;
    if (cmd.size() > 50)
        result = cmd.substr(0, 47) + "...";
    else
        result = cmd;
    
    return result;
}

// YG: compute and store ram (MB)
// can be used for sorting procs
void Process::computeRam() {
    ramStr_ = LinuxParser::Ram(pid_);
    ramInt_ = std::stol(ramStr_);
}
// YG DONE: Return this process's memory utilization
string Process::Ram() { return ramStr_; }

// YG DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// YG DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// sorting is done in system.cpp
// suports sorting using cpu utilization or RAM
// YG DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return cpu_utilization_ < a.cpu_utilization_;
    //return ramInt_ < a.ramInt_;
}
// overload "greater than" comparison operator for Process objects
bool Process::operator>(Process const& a) const {
    return cpu_utilization_ > a.cpu_utilization_;
    //return ramInt_ > a.ramInt_;
}
