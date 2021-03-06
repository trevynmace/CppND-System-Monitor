#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid), command_(LinuxParser::Command(pid)), user_(LinuxParser::User(pid)) { };

// TODO: Return this process's ID
int Process::Pid()
{
    return this -> pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const
{
    float totalSystemUptimeInSeconds = (float) LinuxParser::UpTime();
    float totalProcessUptimeInSeconds = (float) LinuxParser::UpTime(this -> pid_);
    float totalActiveJiffies = (float) LinuxParser::ActiveJiffies(this -> pid_);
    float totalActiveSeconds = totalSystemUptimeInSeconds - (totalProcessUptimeInSeconds / (float) sysconf(_SC_CLK_TCK));

    float result = (totalActiveJiffies / (float) sysconf(_SC_CLK_TCK)) / totalActiveSeconds;
    return result;
}

// TODO: Return the command that generated this process
string Process::Command()
{
    return this -> command_;
}

// TODO: Return this process's memory utilization
string Process::Ram()
{
    return LinuxParser::Ram(this -> pid_);
}

// TODO: Return the user (name) that generated this process
string Process::User()
{
    return this -> user_;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime()
{
    return LinuxParser::UpTime(this -> pid_);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const
{
    auto thisCpu = CpuUtilization();
    auto otherCpu = a.CpuUtilization();
    return thisCpu < otherCpu;
}