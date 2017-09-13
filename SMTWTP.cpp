#include "SMTWTP.h"

////////////////////////////////////////////////////////////////////////////
int SMTWTP::compute_cost
////////////////////////////////////////////////////////////////////////////
(
 std::vector<int> &solutions,
 Instance &instance
)
{
 int cost = 0;
 int date = 0;

 for (int i = 0 ; i < instance.get_instance_size() ; i++)
 {
  date += instance.get_duration(solutions[i]);
  cost += instance.get_weight(solutions[i]) * 
          std::max(date - instance.get_deadline(solutions[i]), 0); 
 }

 return cost;
}
