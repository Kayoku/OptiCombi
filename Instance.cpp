#include "Instance.h"

//////////////////////////////////////////////////////////////////////////
void Instance::display()
////////////////////////////////////////////////////////////////////////////
{
 std::cout << "Instance " << id << std::endl
           << "Duration | Weight | Deadline" << std::endl;

 for (int i = 0 ; i < instance_size ; i++)
 {
  std::cout << durations[i] << " ";
  std::cout << weights[i] << " ";
  std::cout << deadlines[i] << std::endl;
 }
}
