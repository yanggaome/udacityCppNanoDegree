#include "processor.h"
#include "linux_parser.h"
// YG DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
    return (float)LinuxParser::Jiffies() / (float)LinuxParser::ActiveJiffies();
}