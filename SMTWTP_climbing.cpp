#include "SMTWTP_climbing.h"

////////////////////////////////////////////////////////////////////////////
std::vector<int> SMTWTP_climbing::get_solution
////////////////////////////////////////////////////////////////////////////
(
 Instance &instance
)
{
 std::vector<int> solutions;

 switch(init)
 {
  case Init_Mode::RND:
   solutions = random_solution(); 
   break;
  case Init_Mode::EDD:
   solutions = EDD_solution(instance);
   break;
  case Init_Mode::MDD:
   solutions = MDD_solution(instance);
   break;
 }

 bool is_best = false;
 if (select == Select_Mode::BEST)
  is_best = true;

 switch(neighbour)
 {
  case Neighbour_Mode::INSERT:
    solutions = insert_process(is_best, solutions, instance);
   break;
  case Neighbour_Mode::SWAP:
    solutions = swap_process(is_best, solutions, instance);
   break;
  case Neighbour_Mode::EXCHANGE:
    solutions = exchange_process(is_best, solutions, instance);
   break;
 }

 return solutions;
}

////////////////////////////////////////////////////////////////////////////
std::vector<int> SMTWTP_climbing::insert_process
////////////////////////////////////////////////////////////////////////////
(
 bool is_best,
 std::vector<int> solutions,
 Instance &instance
)
{
 int best_cost = compute_cost(solutions, instance); 
 std::vector<int> best_solution = solutions;
 
 if (is_best)
 {
  // Pour chaque element possible
  for (int i = 0 ; i < solutions.size() ; i++)
  {
   std::vector<int> new_sol = solutions;
   int erased_value = solutions[i];
   new_sol.erase(solutions.begin()+i);
   std::vector<int> partial_sol;
   // Pour chaque endroit possible à l'insertion
   for (int j = 0 ; j < new_sol.size() ; j++)
   {
    partial_sol = new_sol;
    partial_sol.insert(partial_sol.begin()+j, erased_value); 
    int new_cost = compute_cost(partial_sol, instance);
    if (best_cost < new_cost)
    {
     best_cost = new_cost;
     best_solution = partial_sol;
    }
   }
  } 
 }
 else
 {
  bool find_better = false;
  // Pour chaque element possible
  for (int i = 0 ; i < solutions.size() && !find_better ; i++)
  {
   std::vector<int> new_sol = solutions;
   int erased_value = solutions[i];
   new_sol.erase(solutions.begin()+i);
   std::vector<int> partial_sol;
   // Pour chaque endroit possible à l'insertion
   for (int j = 0 ; j < new_sol.size() && !find_better ; j++)
   {
    partial_sol = new_sol;
    partial_sol.insert(partial_sol.begin()+j, erased_value);
    int new_cost = compute_cost(partial_sol, instance);
    if (best_cost < new_cost)
    {
     best_cost = new_cost;
     best_solution = partial_sol;
     find_better = true;
    }
   }
  }
 }

 return best_solution;
}

////////////////////////////////////////////////////////////////////////////
std::vector<int> SMTWTP_climbing::swap_process
////////////////////////////////////////////////////////////////////////////
(
 bool is_best,
 std::vector<int> solutions,
 Instance &instance
)
{
 return solutions;
} 

////////////////////////////////////////////////////////////////////////////
std::vector<int> SMTWTP_climbing::exchange_process
////////////////////////////////////////////////////////////////////////////
(
 bool is_best,
 std::vector<int> solutions,
 Instance &instance
)
{
 return solutions;
}
