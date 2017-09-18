#include "SMTWTP.h"

////////////////////////////////////////////////////////////////////////////
int SMTWTP::compute_cost
////////////////////////////////////////////////////////////////////////////
(
 std::vector<int> solution,
 Instance &instance
)
{
 int cost = 0;
 int date = 0;

 compute_cpt++;

 for (int i = 0 ; i < instance.get_instance_size() ; i++)
 {
  date += instance.get_duration(solution[i]);
  cost += instance.get_weight(solution[i]) * 
          std::max(date - instance.get_deadline(solution[i]), 0); 
 }

 return cost;
}
