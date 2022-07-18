/*
Responsiple to describe the process by its :
1-pid
2-user "Name"
3-command
4-CPU utiliazation
5-RAM "used"
6-UPTIME "for how much time it was opended"
And using Linux_parser function to insert on it the chosen PID_
*/
#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int Pid);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
bool operator<(Process const& a) const;

  // TODO: Declare any necessary private members
 private:
 int pid_;
};

#endif