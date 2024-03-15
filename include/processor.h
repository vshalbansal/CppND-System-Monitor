#ifndef PROCESSOR_H
#define PROCESSOR_H
#include<vector>
#include<string>
class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  Processor(std::string id);
  

  // TODO: Declare any necessary private members
 private:
 std::string processorID_{};
 long nonIdle_;
 long idle_;
};

#endif