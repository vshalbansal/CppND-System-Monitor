#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include<unordered_map>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;




// DONE: An example of how to read data from the filesystem
// Read and return OS name and version
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
// Read and return Kernel version
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
// Done: Read and return list of process IDs 
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

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  std::string key;
  std::string value;
  string line;
  std::unordered_map<string,float> memInfo;
  std::ifstream meminfoStream(kProcDirectory + kMeminfoFilename);
  while(std::getline(meminfoStream,line)) {
    std::istringstream linestream(line);
    linestream>> key >> value;
    memInfo[key] = stof(value);

  }
  return (memInfo["MemTotal:"]-memInfo["MemFree:"])/memInfo["MemTotal:"]; 
}

// Done: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::ifstream uptimeFile(kProcDirectory+kUptimeFilename);
  std::string line;
  long uptime;
  if(uptimeFile.is_open()) {
    getline(uptimeFile,line);
    std::istringstream linestream(line);
    linestream>>uptime;
  }
  return uptime;
  
}

// Done: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return sysconf(_SC_CLK_TCK);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  std::ifstream procStatFile(kProcDirectory+std::to_string(pid)+kStatFilename);
  long activeJiffies = 0;
  if(procStatFile.is_open()) {
    std::string line;
    getline(procStatFile,line);
    std::istringstream linestream(line);
    std::string statVal;
    for(int i = 1;i<=17;i++) {
      linestream>>statVal;
      if(i>=14) {
        activeJiffies += std::stol(statVal);
      }
    }
  }
  return activeJiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies(std::string cpuId) {
  vector<long long int > currentCpuStat = LinuxParser::CpuUtilization(cpuId);
  long nonIdleJiffies = currentCpuStat[kUser_] + currentCpuStat[kNice_] + currentCpuStat[kSystem_] + currentCpuStat[kIRQ_] 
                                + currentCpuStat[kSoftIRQ_] + currentCpuStat[kSteal_];
  return nonIdleJiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies(std::string cpuId) {
  vector<long long int > currentCpuStat = LinuxParser::CpuUtilization(cpuId);
  long idleJiffies = currentCpuStat[kIdle_] + currentCpuStat[kIOwait_];
  return idleJiffies;

}


// Done: Read and return CPU utilization 
vector<long long> LinuxParser::CpuUtilization(std::string cpuId) { 
  vector<long long int> cpuStat;
  std::ifstream statFile(kProcDirectory+ kStatFilename);
  std::string line;
  if(statFile.is_open()) {
    while(getline(statFile,line)) {
      std::istringstream linestream(line);
      std::string key;
      linestream>>key;
      if(key==cpuId) {
        long long int value;
        while(linestream>>value) {
          cpuStat.push_back(value);
        }
        break;
      }
    }
  }
  return cpuStat; 
}

std::vector<std::string> LinuxParser::CpuIds() {
  vector<string> cpuIds;
  std::ifstream cpuStatFile(kProcDirectory+kStatFilename);
  if(cpuStatFile.is_open()) {
    std::string line;
    while(getline(cpuStatFile,line)) {
      std::istringstream linestream(line);
      std::string key;
      linestream>>key;
      if(key.substr(0,3)=="cpu") {
        cpuIds.push_back(key);
      }
    }
  }
  return cpuIds;

}

// Done: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream statFile(kProcDirectory+kStatFilename);
  int numProcesses = 0;
  if(statFile.is_open()) {
    std::string line;
    while(getline(statFile,line)) {
      std::istringstream linestream(line);
      std::string key;
      std::string value;
      getline(linestream,key,' ');
      if(key=="processes") {
        getline(linestream,value);
        numProcesses = stoi(value);
        break;
      }
    }
  }
  return numProcesses;
}

// Done: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream statFile(kProcDirectory+kStatFilename);
  int numRunProcesses = 0;
  if(statFile.is_open()) {
    std::string line;
    while(getline(statFile,line)) {
      std::istringstream linestream(line);
      std::string key;
      std::string value;
      getline(linestream,key,' ');
      if(key=="procs_running") {
        getline(linestream,value);
        numRunProcesses = stoi(value);
        break;
      }
    }
  }
  return numRunProcesses;
}

// Done: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::string processCommand;
  std::ifstream fileStream(kProcDirectory+to_string(pid)+kCmdlineFilename);
  if(fileStream.is_open()) {
    getline(fileStream,processCommand);
  }
  return processCommand; 
  }

// Done: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
long int LinuxParser::Ram(int pid) { 
  std::ifstream processStatusFile(kProcDirectory+to_string(pid)+kStatusFilename);
  std::string line;
  long memUsed = 0;
  if(processStatusFile.is_open()) {
    while(getline(processStatusFile,line)) {
      std::istringstream linestream(line);
      std::string key;
      linestream>>key;
      if(key == "VmSize:") {
        linestream>>memUsed;
        break;
      }
    }
  }
  return memUsed/1024;
}



// Done: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::ifstream processStatusFile(kProcDirectory+to_string(pid)+kStatusFilename);
  std::string line;
  std::string uid;
  if(processStatusFile.is_open()) {
    while(getline(processStatusFile,line)) {
      std::istringstream linestream(line);
      std::string key;
      std::string value;
      linestream>> key;
      if(key=="Uid:") {
        linestream>>value;
        uid = value;
        break;
      }
    }
  }
  return uid; }

// Done: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  std::string uid = LinuxParser::Uid(pid);
  std::ifstream filePasswd(kPasswordPath);
  std::string line;
  std::string username,passwd, usrid;
  if(filePasswd.is_open()) {
    while(getline(filePasswd,line)) {
      std::istringstream linestream(line);
      getline(linestream,username,':');
      getline(linestream,passwd,':');
      getline(linestream,usrid,':');
      if(usrid==uid) 
        break;
    }
  }
  return username;


}

// Done: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  std::ifstream processStatFile(kProcDirectory + to_string(pid) + kStatFilename);
  long procUptime = 0;
  if(processStatFile.is_open()) {
    std::string line;
    getline(processStatFile,line);
    std::istringstream linestream(line);
    std::string statValue;
    for(int itr = 1;itr<=22;itr++) {
      linestream>>statValue;
      if(itr==22) {
        procUptime = std::stol(statValue);
        break;
      }
    }
    procUptime/= sysconf(_SC_CLK_TCK);
  }
  return UpTime()-procUptime;

}
