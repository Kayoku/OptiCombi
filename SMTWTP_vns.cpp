#include "SMTWTP_vns.h"
#include <random>

////////////////////////////////////////////////////////////////////////////
std::vector<long> SMTWTP_vns::get_solution
////////////////////////////////////////////////////////////////////////////
(
 Instance &instance
)
{
 std::vector<long> best_solution = get_initializer(instance);
 std::vector<long> new_solution;
 int cpt_better_sol = 0;
 std::random_device rd;
 std::mt19937 g(rd());

 do
 { 
  auto &algo = vns[g()%vns.size()];
  new_solution = algo.get_process(best_solution, instance); 
  if (compute_cost(new_solution, instance) < compute_cost(best_solution, instance))
  {
   best_solution = new_solution;
   cpt_better_sol = 0;
   break;
  }
  else
   cpt_better_sol++;
 } while(cpt_better_sol < max_no_evolution);

 for (auto &v: vns)
  compute_cpt += v.get_compute_cpt();
  
 return best_solution;
}

////////////////////////////////////////////////////////////////////////////
std::string SMTWTP_vns::get_name()
////////////////////////////////////////////////////////////////////////////
{
 std::string name = "VNS-";
 for (auto v : vns)
  name += v.get_name() + '-';
 name.pop_back();
 return name;
}
