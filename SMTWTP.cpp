#include "SMTWTP.h"

////////////////////////////////////////////////////////////////////////////
long SMTWTP::compute_cost
////////////////////////////////////////////////////////////////////////////
(
 std::vector<long> &solution,
 Instance &instance
)
{
 long cost = 0;
 long date = 0;

 compute_cpt++;

 for (int i = 0 ; i < instance.get_instance_size() ; i++)
 {
  date += instance.get_duration(solution[i]);
  cost += instance.get_weight(solution[i]) * 
          std::max(date - instance.get_deadline(solution[i]), (long)0); 
 }

 return cost;
}

////////////////////////////////////////////////////////////////////////////
int SMTWTP::get_compute_cpt()
////////////////////////////////////////////////////////////////////////////
{
 return compute_cpt;
}
