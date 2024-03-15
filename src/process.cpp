#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

#include "linux_parser.h"


Process::Process(int pid):pid_(pid) {};

// Done: Return this process's ID
int Process::Pid() { return pid_;}

// Done: Return this process's CPU utilization
float Process::CpuUtilization() const { 
  long processUptime = LinuxParser::UpTime(pid_);   
  long procActiveJiffies = LinuxParser::ActiveJiffies(pid_); 
  return (float(procActiveJiffies)/float(LinuxParser::Jiffies()))/float(processUptime);
  
}

// Done: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Done: Return this process's memory utilization
long int Process::Ram() const { return LinuxParser::Ram(pid_); }

// Done: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Done: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    // return a.Ram() < Ram(); 
    return a.CpuUtilization() < CpuUtilization();
}