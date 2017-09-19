#include "SMTWTP_climbing.h"
#include <algorithm>

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

void vec_erase(std::vector<int> &solution, int index)
{
 for (int i = index ; i < solution.size()-1 ; i++)
  solution[i] = solution[i+1]; 
}

void vec_insert(std::vector<int> &solution, int index, int value)
{
 for (int i = solution.size()-1 ; i > index ; i--)
  solution[i] = solution[i-1];
 solution[index] = value;
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
 std::vector<int> new_sol;

 std::vector<int> order;
 for (int i = 0 ; i < solution.size() ; i++)
  order.push_back(i); 
 std::random_shuffle(order.begin(), order.end());

 // Pour chaque element possible
 for (auto o : order)
 {
  new_sol = solution;
  int erased_value = new_sol[o];
  new_sol.erase(new_sol.begin()+o);
  // Pour chaque endroit possible à l'insertion 
  for (int j = 0 ; j < new_sol.size()-1 ; j++)
  {
   new_sol.insert(new_sol.begin()+j, erased_value); 
   int new_cost = compute_cost(new_sol, instance);
   if (best_cost > new_cost)
   {
    if (select == Select_Mode::FIRST)
     return new_sol;

    best_solution = new_sol;
    best_cost = new_cost;
   }
   new_sol.erase(new_sol.begin()+j);
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
 std::vector<int> new_sol;

 std::vector<int> order;
 for (int i = 0 ; i < solution.size() ; i++)
  order.push_back(i); 
 std::random_shuffle(order.begin(), order.end());

 // Pour chaque element possible
 for (auto o : order)
 {
  for (int j = o+1 ; j < solution.size() ; j++)
  {
   new_sol = solution;
   std::iter_swap(new_sol.begin()+o, new_sol.begin()+j);
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
 std::vector<int> new_sol;

 std::vector<int> order;
 for (int i = 0 ; i < solution.size()-1 ; i++)
  order.push_back(i); 
 std::random_shuffle(order.begin(), order.end());

 for (auto o : order)
 {
  new_sol = solution;
  std::iter_swap(new_sol.begin()+o, new_sol.begin()+o+1);
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
