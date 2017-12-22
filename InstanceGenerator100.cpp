#include "InstanceGenerator100.h"

////////////////////////////////////////////////////////////////////////////
Instance InstanceGenerator100::get_new_instance()
////////////////////////////////////////////////////////////////////////////
{
 long inter;
 cpt_instance++;

 std::vector<long> durations;
 std::vector<long> weights;
 std::vector<long> deadlines;
 
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
 
 Instance inst(cpt_instance, instance_size, best_sol[cpt_instance-1], durations, weights, deadlines);
 return inst;
}

////////////////////////////////////////////////////////////////////////////
bool InstanceGenerator100::previous_good()
////////////////////////////////////////////////////////////////////////////
{
 if (file.eof())
  return false;
 return true;
}
