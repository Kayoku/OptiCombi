#include "SMTWTP_vnd.h"
#include <random>

////////////////////////////////////////////////////////////////////////////
std::vector<long> SMTWTP_vnd::get_solution
////////////////////////////////////////////////////////////////////////////
(
 Instance &instance
)
{
 return do_vnd(instance, get_initializer(instance));
}

////////////////////////////////////////////////////////////////////////////
std::vector<long> SMTWTP_vnd::do_vnd
////////////////////////////////////////////////////////////////////////////
(
 Instance &instance,
 std::vector<long> init_sol
)
{
 reset_compute_cpt();
 for (auto &v: vnd)
  v.reset_compute_cpt();

 std::vector<long> best_solution = init_sol;
 std::vector<long> new_solution;
 bool finish = true;
 std::random_device rd;
 std::mt19937 g(rd());

 // VND Aléatoire
 if (random)
 {
  do
  {
   for (int i = 0 ; i < (int)vnd.size() ; i++)
   {
    auto &algo = vnd[g()%vnd.size()];
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
  } while (!finish);
 }
 // VND Basique
 else
 {
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
 }

 for (auto &v: vnd)
  compute_cpt += v.get_compute_cpt();
  
 return best_solution;
}

////////////////////////////////////////////////////////////////////////////
std::string SMTWTP_vnd::get_name()
////////////////////////////////////////////////////////////////////////////
{
 // Dans l'idéal : 
 // init + alea / fe fi fs be bi bs
 std::string name = "vnd-";
 if (init == Init_Mode::RND)
  name += "rnd-";
 else if (init == Init_Mode::EDD)
  name += "edd-";
 else
  name += "mdd-";

 if (random)
  name += "alea-" + std::to_string(depth);
 else 
 {
  for (auto v : vnd)
   name += v.get_spec() + '-';
  name.pop_back();
 }

 return name;
}
