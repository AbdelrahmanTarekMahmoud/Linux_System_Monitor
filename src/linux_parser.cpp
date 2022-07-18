#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::stol;
// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      // To deal with spaces and quos 
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          //To make things back like it was 
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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    //Linux(os)version(version) 5.15.0-41-generic(kernel)
    linestream >> os >> version >> kernel;
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
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  string mtotal;
  string mfree;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          mtotal = value;
        }
        if (key == "MemFree:") {
          mfree = value;
        }
      }
    }
  }
  return (stof(mtotal)-stof(mfree)) / stof(mtotal);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  string uptime;
  std::string line;
  std::ifstream stream(kProcDirectory+kUptimeFilename);
  if(stream.is_open())
  {
    std::getline(stream,line);
    std::istringstream streamline(line);
    streamline >> uptime;
  }
  return stol(uptime);
}

// TODO: Read and return the number of jiffies for the system
//holds the number of ticks that have occurred since the system booted
long LinuxParser::Jiffies() 
{ 
  long Total=0;
  vector<std::string> CpuUtil =LinuxParser::CpuUtilization(); 
  // all non guest
  for (int i = kUser_;i <kSteal_;i++)
  {
    Total+=stol(CpuUtil[i]);
  }
  return Total;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) 
{ 
  std::string line,value;
  std::vector<std::string> Values;
  std::ifstream stream(kProcDirectory +std::to_string(pid)+kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream,line);
    std::istringstream streamline(line);
    while (streamline >> value)
    {
     Values.push_back(value);   
    }
  }
  return stol(Values[13]+Values[14]);

}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{ 
return LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{  
  long total=0;
  std::vector<std::string> values =LinuxParser::CpuUtilization();
  for (int i =kIdle_;i < kIOwait_;i++)
  {
    total += stol(values[i]);
  }
  return total;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{ 
std::string line,description;
std::string value;
std::vector<std::string> Result;
std::ifstream stream(kProcDirectory + kStatFilename);
if(stream.is_open())
{
  std::getline(stream,line);
  std::istringstream streamline(line);
  //to ignore CPU: and take the values only
  streamline >> description;
  while(streamline >> value)
  {
    Result.push_back(value);
  }
}
return Result;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{ 
  std::string line,description;
  std::string value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    while(std::getline(stream , line))
    {
      std::istringstream streamline(line);
      while (streamline >> description >> value)
      {
        if(description=="processes")
        {
          return stoi(value);
        }
      }

    }
  }
return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  std::string line,description;
  std::string value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    while(std::getline(stream , line))
    {
      std::istringstream streamline(line);
      while (streamline >> description >> value)
      {
        if(description=="procs_running")
        {
          return stoi(value);
        }
      }

    }
  }
return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{
std::string line;
std::ifstream stream(kProcDirectory+to_string(pid)+kCmdlineFilename);
if(stream.is_open())
{
std::getline(stream,line);
}
return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{ 
  std::string line,description,value;
  std::ifstream stream(kProcDirectory+to_string(pid)+kStatusFilename);
  if(stream.is_open())
  {
    while(std::getline(stream , line))
    {
      std::istringstream streamline(line);
      while (streamline >> description >> value)
      {
        if(description=="VmSize:")
        {
          return to_string(stoi(value) / 1024);
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
  std::string line;
  std::string description , value;
  std::ifstream stream(kProcDirectory+to_string(pid)+kStatusFilename);
  if(stream.is_open())
  {
    while(std::getline(stream,line))
    {
      std::istringstream streamline(line);
      streamline >> description >> value;
      {
        if(description == "Uid:")
        {
          return value;
        }
      }
    }
  }
  return "0";
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  string line, key, x, value;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
   while (std::getline(stream, line)) {
     std::replace(line.begin(), line.end(), ':', ' ');
     std::istringstream linestream(line);
     linestream >> key >> x >> value;
     if (value == uid) {
       return key;
     }
   }
  }
  return string();
}
// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
	int i = 0;
    while (linestream >> value) {
      if (i == 21) {
        return stol(value) / sysconf(_SC_CLK_TCK);
      }
      i++;
    }
  }
  return 0;
}
//1