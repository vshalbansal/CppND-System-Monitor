#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;



//System constructor to store initial CPU Timers
System::System() {
    vector<std::string> cpuIds = LinuxParser::CpuIds();
    for(int i =0;i<int(cpuIds.size());i++) {
        Processor processor(cpuIds[i]);
        cpu_.push_back(processor);
    }

    vector<int> PID_list = LinuxParser::Pids();
    for(auto pid:PID_list ) {
        Process newProcess(pid);
        if(newProcess.Command().size()!=0)
            processes_.push_back(newProcess);
    }
    std::sort(processes_.begin(),processes_.end());


}
// TODO: Return the system's CPU
vector<Processor>& System::Cpu() { return cpu_; }

// Done: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> PID_list = LinuxParser::Pids();
    // std::unordered_map<int,Process> processUmap;
    // for(auto process:processes_) {
    //     processUmap[process.Pid()] = process;
    // }
    processes_.clear();
    for(auto pid:PID_list ) {
        Process newProcess(pid);
        if(newProcess.Command().size()!=0)
            processes_.push_back(newProcess);
    }
    std::sort(processes_.begin(),processes_.end());
    return processes_; 
}

// Done: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel();}

// Done: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Done: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem();}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }