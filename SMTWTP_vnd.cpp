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
  
 return best_solution;
}

////////////////////////////////////////////////////////////////////////////
int SMTWTP_vnd::get_full_compute_cpt()
////////////////////////////////////////////////////////////////////////////
{
 int cpt = 0;

 for (auto v: vnd)
  cpt += v.get_compute_cpt();

 return cpt + get_compute_cpt();
}
