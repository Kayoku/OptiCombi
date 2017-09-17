#include "SMTWTP_climbing.h"

////////////////////////////////////////////////////////////////////////////
std::vector<int> SMTWTP_climbing::get_solution
////////////////////////////////////////////////////////////////////////////
(
 Instance &instance
)
{
 std::vector<int> init_solution;
 switch(init)
 {
  case Init_Mode::RND:
   init_solution = random_solution(); 
   break;
  case Init_Mode::EDD:
   init_solution = EDD_solution(instance);
   break;
  case Init_Mode::MDD:
   init_solution = MDD_solution(instance);
   break;
 }

 std::vector<int> solution;
 switch(neighbour)
 {
  case Neighbour_Mode::INSERT:
    solution = insert_process(init_solution, instance); 
    while (solution != init_solution)
    {
     init_solution = solution;
     solution = insert_process(init_solution, instance); 
    }
   break;
  case Neighbour_Mode::SWAP:
    solution = swap_process(init_solution, instance);
    while (solution != init_solution)
    {
     init_solution = solution;
     solution = swap_process(init_solution, instance);
    }
   break;
  case Neighbour_Mode::EXCHANGE:
    solution = exchange_process(init_solution, instance);
    while (solution != init_solution)
    {
     init_solution = solution;
     solution = exchange_process(init_solution, instance);
    }
   break;
 }

 return solution;
}

////////////////////////////////////////////////////////////////////////////
std::vector<int> SMTWTP_climbing::insert_process 
////////////////////////////////////////////////////////////////////////////
(
 std::vector<int> solution,
 Instance &instance
)
{
 int best_cost = compute_cost(solution, instance); 
 std::vector<int> best_solution(solution);
 
 // Pour chaque element possible
 int i, j;
 for (i = 0 ; i < solution.size() ; i++)
 {
  std::vector<int> new_sol(solution);
  int erased_value = solution[i];
  new_sol.erase(new_sol.begin()+i);
  // Pour chaque endroit possible à l'insertion
  for (j = 0 ; j < new_sol.size() ; j++)
  {
   std::vector<int> partial_sol(new_sol);
   partial_sol.insert(partial_sol.begin()+j, erased_value); 
   int new_cost = compute_cost(partial_sol, instance);
   if (best_cost > new_cost)
   {
    if (select == Select_Mode::FIRST)
     return partial_sol;

    best_solution = partial_sol;
    best_cost = new_cost;
   }
  }
 }
 
 return best_solution;
}

////////////////////////////////////////////////////////////////////////////
std::vector<int> SMTWTP_climbing::swap_process 
////////////////////////////////////////////////////////////////////////////
(
 std::vector<int> solution,
 Instance &instance
)
{
 int best_cost = compute_cost(solution, instance);
 std::vector<int> best_solution(solution);

 // Pour chaque element possible
 for (int i = 0 ; i < solution.size() ; i++)
 {
  for (int j = i+1 ; j < solution.size() ; j++)
  {
   std::vector<int> new_sol(solution);
   std::iter_swap(new_sol.begin()+i, new_sol.begin()+j);
   int new_cost = compute_cost(new_sol, instance);
   if (best_cost > new_cost)
   {
    if (select == Select_Mode::FIRST)
     return new_sol;

    best_solution = new_sol;
    best_cost = new_cost;
   } 
  }
 }

 return best_solution;
} 

////////////////////////////////////////////////////////////////////////////
std::vector<int> SMTWTP_climbing::exchange_process 
////////////////////////////////////////////////////////////////////////////
(
 std::vector<int> solution,
 Instance &instance
)
{
 int best_cost = compute_cost(solution, instance);
 std::vector<int> best_solution(solution);

 for (int i = 0 ; i < solution.size()-1 ; i++)
 {
  std::vector<int> new_sol(solution);
  std::iter_swap(new_sol.begin()+i, new_sol.begin()+i+1);
  int new_cost = compute_cost(new_sol, instance);

  if (best_cost > new_cost)
  {
   if (select == Select_Mode::FIRST)
    return new_sol;

   best_solution = new_sol;
   best_cost = new_cost;
  }
 }

 return best_solution;
}
