#include "processor.h"
#include "linux_parser.h"
#include<vector>
#include<string>


#include<iostream>

using std::vector;
// Done: Return the CPU utilization
float Processor::Utilization() {     
    
    long currentIdle = LinuxParser::IdleJiffies(processorID_);
    long currentNonIdle = LinuxParser::ActiveJiffies(processorID_);

    long prevTotal = idle_+nonIdle_;
    long currentTotal = currentIdle + currentNonIdle;

    long total = currentTotal - prevTotal;
    long idle = currentIdle - idle_;

    float cpuPercentage;
    if(total==0)
        cpuPercentage = 0.0;
    else
        cpuPercentage = (float(total)-float(idle))/float(total);
    
    // store latest cpu stats 
    idle_ = currentIdle;
    nonIdle_ = currentNonIdle;
    return cpuPercentage;
}

Processor::Processor(std::string id) {
    processorID_ = id;
    nonIdle_ = LinuxParser::ActiveJiffies(id);
    idle_ = LinuxParser::IdleJiffies(id);
}