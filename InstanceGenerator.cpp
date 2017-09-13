#include "InstanceGenerator.h"

////////////////////////////////////////////////////////////////////////////
Instance InstanceGenerator::get_new_instance(int instance_size)
////////////////////////////////////////////////////////////////////////////
{
 int inter;
 cpt_instance++;

 std::vector<int> durations;
 std::vector<int> weights;
 std::vector<int> deadlines;
 
 for (int i = 0 ; i < instance_size ; i++)
 {
  file >> inter;
  durations.push_back(inter);
 }

 for (int i = 0 ; i < instance_size ; i++)
 {
  file >> inter;
  weights.push_back(inter);
 }
 
 for (int i = 0 ; i < instance_size ; i++)
 {
  file >> inter;
  deadlines.push_back(inter);
 }
 
 Instance inst(cpt_instance, instance_size, durations, weights, deadlines);
 return inst;
}

////////////////////////////////////////////////////////////////////////////
bool InstanceGenerator::previous_good()
////////////////////////////////////////////////////////////////////////////
{
 if (file.eof())
  return false;
 return true;
}
