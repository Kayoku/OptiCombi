#include "SMTWTP_climbing.h"
#include <algorithm>
#include <cmath>

////////////////////////////////////////////////////////////////////////////
std::vector<long> SMTWTP_climbing::get_process
////////////////////////////////////////////////////////////////////////////
(
 std::vector<long> init_solution,
 Instance &instance
)
{
 std::vector<long> solution;

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
std::vector<long> SMTWTP_climbing::get_solution
////////////////////////////////////////////////////////////////////////////
(
 Instance &instance
)
{
 reset_compute_cpt();
 std::vector<long> init_solution = get_initializer(instance);
 return get_process(init_solution, instance);
}

////////////////////////////////////////////////////////////////////////////
std::vector<int> help_index
////////////////////////////////////////////////////////////////////////////
(
 int instance_size,
 int depth,
 long value
)
{
 std::vector<int> indexs;

 for (int i = 0 ; i < depth ; i++)
 {
  indexs.push_back(value % instance_size);
  value = (int) (value/instance_size);
 }
 
 return indexs; 
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// INSERT
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
std::vector<long> SMTWTP_climbing::insert_process 
////////////////////////////////////////////////////////////////////////////
(
 std::vector<long> solution,
 Instance &instance
)
{
 best_cost = compute_cost(solution, instance); 
 best_solution = solution;

 //
 // Pour chaque combinaison possibles
 //
 bool finish = false;
 int max = (int)std::pow(instance_size, depth);
 std::vector<long> order;
 for (int i = 0 ; i < max ; i++)
  order.push_back(i); 
 std::random_shuffle(order.begin(), order.end());

 for (auto o : order)
 {
  auto indexs = help_index(best_solution.size(), depth, o);
  finish = insert_branch(solution, instance, indexs);

  if (finish)
   break;
 }
 return best_solution;
}

////////////////////////////////////////////////////////////////////////////
bool SMTWTP_climbing::insert_branch
////////////////////////////////////////////////////////////////////////////
(
 std::vector<long> new_sol,
 Instance &inst,
 std::vector<int> indexs
)
{
 bool finish = false;

 long erased_value = new_sol[indexs[0]];
 new_sol.erase(new_sol.begin()+indexs[0]);

 // Dans le cas ou il ne reste qu'un de profondeur
 // On le traite test tous les cas de figure
 if (indexs.size() == 1)
  return insert_all(new_sol, inst, erased_value);

 // Sinon, on fait une boucle, puis on réitère sur 
 // cette fonction récursivement
 for (size_t j = 0 ; j < new_sol.size() ; j++)
 {
  new_sol.insert(new_sol.begin()+j, erased_value);
  finish = insert_branch(new_sol, inst, std::vector<int>(indexs.begin()+1, indexs.end()));

  if (finish)
   return true;
  new_sol.erase(new_sol.begin()+j);
 }

 return false;
}

////////////////////////////////////////////////////////////////////////////
bool SMTWTP_climbing::insert_all
////////////////////////////////////////////////////////////////////////////
(
 std::vector<long> new_sol,
 Instance &inst,
 long erased_value
)
{
 for (int j = 0 ; j < (int)new_sol.size()-1 ; j++)
 {
  new_sol.insert(new_sol.begin()+j, erased_value); 
  long new_cost = compute_cost(new_sol, inst);
  if (best_cost > new_cost)
  {
   best_solution = new_sol;
   best_cost = new_cost;

   if (select == Select_Mode::FIRST)
    return true;
  }
  new_sol.erase(new_sol.begin()+j);
 }

 return false;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// SWAP 
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
std::vector<long> SMTWTP_climbing::swap_process 
////////////////////////////////////////////////////////////////////////////
(
 std::vector<long> solution,
 Instance &instance
)
{
 best_cost = compute_cost(solution, instance);
 best_solution = solution;
 std::vector<long> new_sol;

 //
 // Pour chaque combinaison possibles
 //
 bool finish = false;
 int max = (int)std::pow(instance_size, depth);
 std::vector<long> order; 
 for (int i = 0 ; i < max ; i++)
  order.push_back(i); 
 std::random_shuffle(order.begin(), order.end());

 for (auto o : order)
 {
  auto indexs = help_index(best_solution.size(), depth, o);
  finish = swap_branch(solution, instance, indexs);
  
  if (finish)
   break;
 }

 return best_solution;
} 

////////////////////////////////////////////////////////////////////////////
bool SMTWTP_climbing::swap_branch
////////////////////////////////////////////////////////////////////////////
(
 std::vector<long> new_sol,
 Instance &inst,
 std::vector<int> indexs
)
{
 bool finish = false;

 // Dans le cas ou il ne reste qu'un de profondeur
 // On le traite test tous les cas de figure
 if (indexs.size() == 1)
  return swap_all(new_sol, inst, indexs[0]);

 // Sinon, on fait une boucle, puis on réitère sur 
 // cette fonction récursivement
 for (size_t j = indexs[0]+1 ; j < new_sol.size() ; j++)
 {
  std::iter_swap(new_sol.begin()+indexs[0], new_sol.begin()+j);
  finish = swap_branch(new_sol, inst, std::vector<int>(indexs.begin()+1, indexs.end()));

  if (finish)
   return true;
 }

 return false;
}


////////////////////////////////////////////////////////////////////////////
bool SMTWTP_climbing::swap_all
////////////////////////////////////////////////////////////////////////////
(
 std::vector<long> new_sol,
 Instance &inst,
 long offset
)
{
 auto solution = new_sol;
 for (int j = offset+1 ; j < (int)solution.size() ; j++)
 {
  new_sol = solution;
  std::iter_swap(new_sol.begin()+offset, new_sol.begin()+j);
  long new_cost = compute_cost(new_sol, inst);
  if (best_cost > new_cost)
  {
   best_solution = new_sol;
   best_cost = new_cost;
 
   if (select == Select_Mode::FIRST)
    return true;
  }
 } 

 return false;
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// EXCHANGE 
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
std::vector<long> SMTWTP_climbing::exchange_process 
////////////////////////////////////////////////////////////////////////////
(
 std::vector<long> solution,
 Instance &instance
)
{
 best_cost = compute_cost(solution, instance);
 best_solution = solution;
 std::vector<long> new_sol;

 //
 // Pour chaque combinaison possibles
 //
 bool finish = false;
 int max = (int)std::pow(instance_size, depth);
 std::vector<long> order;
 for (int i = 0 ; i < max ; i++)
  order.push_back(i);
 std::random_shuffle(order.begin(), order.end());

 for (auto o : order)
 {
  auto indexs = help_index(best_solution.size(), depth, o);
  finish = exchange_branch(solution, instance, indexs);

  if (finish)
   break;
 }

 return best_solution;
}

////////////////////////////////////////////////////////////////////////////
bool SMTWTP_climbing::exchange_branch
////////////////////////////////////////////////////////////////////////////
(
 std::vector<long> new_sol,
 Instance &inst,
 std::vector<int> indexs
)
{
 bool finish = false;

 // Dans le cas ou il ne reste qu'un de profondeur
 // On le traite test tous les cas de figure
 if (indexs.size() == 1)
  return exchange_all(new_sol, inst, indexs[0]);

 // Sinon, on fait une boucle, puis on réitère sur 
 // cette fonction récursivement 
 for (size_t j = indexs[0] ; j < new_sol.size()-1 ; j++)
 {
  std::iter_swap(new_sol.begin()+j, new_sol.begin()+j+1);
  finish = exchange_branch(new_sol, inst, std::vector<int>(indexs.begin()+1, indexs.end()));

  if (finish)
   return true;
 }

 return false;
}


////////////////////////////////////////////////////////////////////////////
bool SMTWTP_climbing::exchange_all
////////////////////////////////////////////////////////////////////////////
(
 std::vector<long> new_sol,
 Instance &inst,
 long offset
)
{
 auto solution = new_sol;

// for (size_t j = offset ; j < new_sol.size()-1 ; j++)
// {
 new_sol = solution;

 if (offset+1 >= (int)new_sol.size())
  return false;

 std::iter_swap(new_sol.begin()+offset, new_sol.begin()+offset+1);
 long new_cost = compute_cost(new_sol, inst);
 if (best_cost > new_cost)
 {
  best_solution = new_sol;
  best_cost = new_cost;
  
  if (select == Select_Mode::FIRST)
   return true;
 // }
 }

 return false;
}


////////////////////////////////////////////////////////////////////////////
std::string SMTWTP_climbing::get_name()
////////////////////////////////////////////////////////////////////////////
{
 std::string s, n, i;

 if (Select_Mode::FIRST == select)
  s = "f";
 else
  s = "b";

 if (Neighbour_Mode::INSERT == neighbour)
  n = "i";
 else if (Neighbour_Mode::SWAP == neighbour)
  n = "s";
 else
  n = "e";

 if (Init_Mode::RND == init)
  i = "r";
 else if (Init_Mode::EDD == init)
  i = "e";
 else
  i = "m";

 return i+s+n;
}

////////////////////////////////////////////////////////////////////////////
std::string SMTWTP_climbing::get_spec()
////////////////////////////////////////////////////////////////////////////
{
 std::string n, s;

 if (Select_Mode::FIRST == select)
  s = "f";
 else
  s = "b";
 
 if (Neighbour_Mode::INSERT == neighbour)
  n = "i";
 else if (Neighbour_Mode::SWAP == neighbour)
  n = "s";
 else
  n = "e";

 return s+n;
}

