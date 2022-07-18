#include <string>
using std::string;
using std::stringstream;
#include "format.h"
#include <iomanip>
using std::string;


// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
// Function to maintain The proper format of Time which will be displayed
string Format::ElapsedTime(long seconds) 
{
     int h , m , s;
stringstream value;
h= ((seconds / 3600) % 100); // To repeat when hours hit 99
m =((seconds/ 60) % 60); // to repeat when minutes hit 59
s =(seconds % 60); // TO repeat when seconds hit 59
 value << std::setfill('0') << std::setw(2) << h << ":"
         << std::setfill('0') << std::setw(2) << m<< ":"
         << std::setfill('0') << std::setw(2) << s;
  return value.str(); 
}