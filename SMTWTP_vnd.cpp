#include "SMTWTP_vnd.h"

////////////////////////////////////////////////////////////////////////////
std::vector<long> SMTWTP_vnd::get_solution 
////////////////////////////////////////////////////////////////////////////
(
 Instance &instance
)
{
 std::vector<long> best_solution = get_initializer(instance);
 std::vector<long> new_solution;
 bool finish = true;

 do
 {
  for (auto &algo: vnd)
  {
   new_solution = algo.get_process(best_solution, instance); 
   if (new_solution != best_solution)
   {
    best_solution = new_solution;
    finish = false;
    break;
   }
   else
    finish = true;
  }
 } while(!finish);

 for (auto &v: vnd)
  compute_cpt += v.get_compute_cpt();
  
 return best_solution;
}

////////////////////////////////////////////////////////////////////////////
std::string SMTWTP_vnd::get_name()
////////////////////////////////////////////////////////////////////////////
{
 std::string name;
 for (auto v : vnd)
  name += v.get_name() + '-';
 name.pop_back();
 return name;
}
