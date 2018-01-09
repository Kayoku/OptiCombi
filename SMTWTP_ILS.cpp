#include "SMTWTP_ILS.h"
#include <random>
#include <chrono>

////////////////////////////////////////////////////////////////////////////
std::vector<long> SMTWTP_ILS::get_solution 
////////////////////////////////////////////////////////////////////////////
(
 Instance &instance
)
{
 std::random_device rd;
 std::mt19937 g(rd());
 std::vector<long> best_solution = get_initializer(instance);
 std::vector<long> pertub_solution = best_solution;
 std::vector<long> new_solution;
 int compute_best_sol = compute_cost(best_solution, instance);
 std::chrono::high_resolution_clock::time_point init_time, inter_time;

// for (int i = 0 ; i < amplification ; i++)
// {
 init_time = std::chrono::high_resolution_clock::now();
 do
 {
  // Local search
  new_solution = do_vnd(instance, pertub_solution);
  if (new_solution != best_solution)
  {
   best_solution = new_solution;
   compute_best_sol = compute_cost(new_solution, instance);
  }
 
  // Perturbation
  if (swap)
   pertub_solution = swap_perturb(best_solution);
  else
   pertub_solution = insert_perturb(best_solution);

  inter_time = std::chrono::high_resolution_clock::now();
 } while (std::chrono::duration_cast<std::chrono::microseconds>(inter_time-init_time).count() < seconds * 1000000);
//  intensity *= 2;
// }

 return best_solution;
}

////////////////////////////////////////////////////////////////////////////
std::vector<long> SMTWTP_ILS::swap_perturb
////////////////////////////////////////////////////////////////////////////
(
 std::vector<long> parent
)
{
 int size = parent.size();
 for (int i = 0 ; i < (int)(g()%intensity)+1 ; i++)
 {
  int id1 = g()%size;
  int id2 = g()%size;
  while (id1 == id2)
   id2 = (g()+1)%size;
  
  std::iter_swap(parent.begin()+id1, parent.begin()+id2);
 }
 
 return parent;
}


////////////////////////////////////////////////////////////////////////////
std::vector<long> SMTWTP_ILS::insert_perturb
////////////////////////////////////////////////////////////////////////////
(
 std::vector<long> parent
)
{
 int id1, id2;
 long erased_value;
 for (int i = 0 ; i < (int)(g()%intensity)+1 ; i++)
 {
  id1 = g()%parent.size();
  id2 = g()%(parent.size()-1);
  while (id2 == id1)
   id2 = (g()+1)%(parent.size()-1);
  erased_value = parent[id1];
  parent.erase(parent.begin()+id1);
  parent.insert(parent.begin()+id2, erased_value);
 }
 return parent;
}

////////////////////////////////////////////////////////////////////////////
std::string SMTWTP_ILS::get_name()
////////////////////////////////////////////////////////////////////////////
{
 std::string name = "ILS-s" + std::to_string(seconds) + "-i" + 
                    std::to_string(intensity); 
 for (auto v : vnd)
 {
  auto vname = v.get_name();
  std::string vname_cut(vname.begin()+1, vname.end());
  name += vname_cut + '-';
 }
 name.pop_back();
 return name;
}
