#include "SMTWTP_ILS.h"
#include <random>

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
 int cpt_better_sol = 0;
 int intensity = perturbation_intensity;

 for (int i = 0 ; i < amplification ; i++)
 {
  while (cpt_better_sol < 10)
  {
   // Local search
 
   new_solution = do_vnd(instance, pertub_solution);
   if (compute_cost(new_solution, instance) <
       compute_cost(best_solution, instance))
   {
    cpt_better_sol = 0;
    best_solution = new_solution;
   }
   else
    cpt_better_sol++;
 
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
  }
  intensity *= 2;
  cpt_better_sol = 0;
 }

 return best_solution;
}

////////////////////////////////////////////////////////////////////////////
std::string SMTWTP_ILS::get_name()
////////////////////////////////////////////////////////////////////////////
{
 std::string name = "ILS-" + std::to_string(perturbation_intensity) + "-" +
                     std::to_string(amplification) + "-";
 for (auto v : vnd)
  name += v.get_name() + '-';
 name.pop_back();
 return name;
}
