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
    solutions = insert_process(is_best, solutions);
   break;
  case Neighbour_Mode::SWAP:
    solutions = swap_process(is_best, solutions);
   break;
  case Neighbour_Mode::EXCHANGE:
    solutions = exchange_process(is_best, solutions);
   break;
 }

 return solutions;
}

////////////////////////////////////////////////////////////////////////////
std::vector<int> SMTWTP_climbing::insert_process
////////////////////////////////////////////////////////////////////////////
(
 bool is_best,
 std::vector<int> solutions
)
{
 std::vector<int> new_solutions;

 return new_solutions;
}

////////////////////////////////////////////////////////////////////////////
std::vector<int> SMTWTP_climbing::swap_process
////////////////////////////////////////////////////////////////////////////
(
 bool is_best,
 std::vector<int> solutions
)
{
 std::vector<int> new_solutions;

 return new_solutions;
} 

////////////////////////////////////////////////////////////////////////////
std::vector<int> SMTWTP_climbing::exchange_process
////////////////////////////////////////////////////////////////////////////
(
 bool is_best,
 std::vector<int> solutions
)
{
 std::vector<int> new_solutions;

 return new_solutions;
}
