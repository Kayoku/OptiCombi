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
 int intensity = perturbation_intensity;
 std::chrono::high_resolution_clock::time_point init_time, inter_time;

 // on calcul une fois hors du temps
 new_solution = do_vnd(instance, pertub_solution);
 if (new_solution != pertub_solution)
 {
  best_solution = new_solution;
  compute_best_sol = compute_cost(new_solution, instance);
 }

 for (int i = 0 ; i < amplification ; i++)
 {
  init_time = std::chrono::high_resolution_clock::now();
  do
  {
   // Local search
 
   new_solution = do_vnd(instance, pertub_solution);
   if (new_solution != pertub_solution)
   {
    best_solution = new_solution;
    compute_best_sol = compute_cost(new_solution, instance);
   }
 
   // Perturbation
 
   pertub_solution = best_solution;
   int id1 = 0, id2 = 0; 
   for (int j = 0 ; j < intensity ; j++)
   {
    do
    {
     id1 = g()%100;
     id2 = g()%100;
    } while (id1 == id2);
 
    std::iter_swap(pertub_solution.begin()+id1,
                   pertub_solution.begin()+id2);
   }
   inter_time = std::chrono::high_resolution_clock::now();
  } while (std::chrono::duration_cast<std::chrono::microseconds>(inter_time-init_time).count() < seconds * 1000000);
  intensity *= 2;
 }

 return best_solution;
}

////////////////////////////////////////////////////////////////////////////
std::string SMTWTP_ILS::get_name()
////////////////////////////////////////////////////////////////////////////
{
 std::string name = "ILS-s" + std::to_string(seconds) + "-i" + 
                    std::to_string(perturbation_intensity) + "-a" +
                    std::to_string(amplification) + "-";
 for (auto v : vnd)
 {
  auto vname = v.get_name();
  std::string vname_cut(vname.begin()+1, vname.end());
  name += vname_cut + '-';
 }
 name.pop_back();
 return name;
}
