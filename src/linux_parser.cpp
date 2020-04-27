#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
{
  double free;
  double total;
  std::ifstream ifs (kProcDirectory + kMeminfoFilename);

  if (ifs.is_open())
  {
    string line;

    while (getline(ifs, line))
    {
      std::replace(line.begin(), line.end(), ":", " ");
      std::istringstream iss (line);
      string k;
      double v;

      while (iss >> k >> v)
      {
        if (k == "MemFree")
        {
          free = v;
        }
        else if (k == "MemTotal")
        {
          total = v;
        }
      }
    }
  }

  return (total - free) / total;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime()
{
  std::ifstream ifs (kProcDirectory + kUptimeFilename);
  long uptime;
  
  if (ifs.is_open())
  {
    string line;
    while (getline(ifs, line))
    {
      //line exists
      std::istringstream ls (line);
      ls >> uptime;
    }
  }

  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies()
{
  auto cpu = CpuUtilization();
  auto jiffies = 0;

  for (int i = kUser_; i <= kSteal_; i++)
  {
    jiffies += stof(cpu[i]);
  }

  return jiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid)
{
  std::ifstream ifs (kProcDirectory + to_string(pid) + kStatFilename);
  long jiffies = 0;

  if (ifs.is_open())
  {
    std::string line;

    while (getline(ifs, line))
    {
      std::istringstream iss (line);
      std::istream_iterator<string> begin(iss), end;
      vector<string> segments (begin, end);

      jiffies += stol(segments[13]) + stol(segments[14]) + stol(segments[15]) + stol(segments[16]);
    }
  }

  return jiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{
  auto totalActive = Jiffies() - IdleJiffies();
  return totalActive;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies()
{
  auto cpu = CpuUtilization();
  auto idleJiffies = 0;

  for (int i = kIdle_; i <= kIOwait_; i++)
  {
    auto current = cpu[i];
    idleJiffies += stol(current);
  }

  return idleJiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization()
{
  std::ifstream ifs (kProcDirectory + kStatFilename);
  vector<string> list;

  if (ifs.is_open())
  {
    string line;
    getline(ifs, line);
    std::istringstream ls (line);

    while (getline(ls, line, ' '))
    {
      if (line != "cpu")
      {
        list.push_back(line);
      }
    }
  }

  return list;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
  std::ifstream ifs (kProcDirectory + kStatFilename);
  int processes = 0;

  if (ifs.is_open())
  {
    string line;
    while (getline(ifs, line))
    {
      std::istringstream iss (line);
      string k, v;

      while (iss >> k >> v)
      {
        if (k == "processes")
        {
          processes = stoi(v);
          break;
        }
      }
    }
  }

  return processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses()
{
  std::ifstream ifs (kProcDirectory + kStatFilename);
  int processes = 0;

  if (ifs.is_open())
  {
    string line;
    while (getline(ifs, line))
    {
      std::istringstream iss (line);
      string k, v;

      while (iss >> k >> v)
      {
        if (k == "procs_running")
        {
          processes = stoi(v);
          break;
        }
      }
    }
  }

  return processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid)
{
  std::ifstream ifs (kProcDirectory + to_string(pid) + kCmdlineFilename);
  string line;

  if (ifs.is_open())
  {
    getline(ifs, line);
  }

  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]])
{
  std::ifstream ifs (kProcDirectory + to_string(pid) + kStatusFilename);

  if (ifs.is_open())
  {
    string line;
    while (getline(ifs, line))
    {
      std::istringstream iss (line);
      string k, v;

      while (iss >> k >> v)
      {
        if (k == "VmSize:")
        {
          auto ramInKb = v;
          auto ramInMb = stof(ramInKb) / 1024.0;
          return to_string(ramInMb);
        }
      }
    }
  }

  return "0";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid)
{
  std::ifstream ifs (kProcDirectory + to_string(pid) + kStatusFilename);
  string uidValue;

  if (ifs.is_open())
  {
    string line;
    while (getline(ifs, line))
    {
      std::istringstream iss (line);
      string k, v;

      while (iss >> k >> v)
      {
        if (k == "Uid:")
        {
          uidValue = v;
        }
      }
    }
  }

  return uidValue;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid)
{
  string uidForPid = Uid(pid);
  string user;

  std::ifstream ifs (kPasswordPath);
  if (ifs.is_open())
  {
    string line;
    while (getline(ifs, line))
    {
      std::replace(line.begin(), line.end(), ":", " ");
      std::istringstream iss (line);
      string k, v;
      string uid;

      while (iss >> k >> v >> uid)
      {
        if (uidForPid == uid)
        {
          user = k;
        }
      }
    }
  }

  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid)
{
  std::ifstream ifs (kProcDirectory + to_string(pid) + kStatFilename);
  long timeResult = 0;

  if (ifs.is_open())
  {
    string line;
    while (getline(ifs, line))
    {
      std::istringstream iss (line);
      std::istream_iterator<string> begin(iss), end;
      vector<string> segments (begin, end);

      auto time = stol(segments[21]);
      timeResult = time / sysconf(_SC_CLK_TCK);
    }
  }

  return timeResult;
}