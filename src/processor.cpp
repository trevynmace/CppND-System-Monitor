#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <iostream>

using std::string;
using std::stol;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization()
{
    std::vector<string> cpu = LinuxParser::CpuUtilization();
    long idle = stol(cpu[3]) + stol(cpu[4]);
    long active = stol(cpu[1]) + stol(cpu[2]) + stol(cpu[5]) + stol(cpu[6]) + stol(cpu[7]);
    long total = idle + active;
    return (float) active / (float) total;
}