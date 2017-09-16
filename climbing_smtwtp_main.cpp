#include <iostream>
#include <fstream>

#include "Instance.h"
#include "InstanceGenerator.h"
#include "SMTWTP_climbing.h"

void usage()
{
 std::cout << "usage: ./climbing_smtwtp_main.cpp <instance_size> [first, best] [insert, swap, exchange] [rnd, edd, mdd] < <instance_file>\n";
}

int main (int argc, char *argv[])
{
 /******** Récupération des arguments ***********/

 if (argc != 5)
 {
  usage();
  return -1;
 }

 int instance_size = std::stoi(argv[1]);
 std::string select_str(argv[2]);
 std::string neighbour_str(argv[3]);
 std::string init_str(argv[4]);

 if (!((select_str == "first" || select_str == "best") &&
     (neighbour_str == "insert" || neighbour_str == "swap" || neighbour_str == "exchange") &&
     (init_str == "rnd" || init_str == "edd" || init_str == "mdd")))
 {
  std::cout << select_str << " " << neighbour_str << " " << init_str << '\n';
  usage();
  return -1;
 }

 Select_Mode select;
 Neighbour_Mode neighbour;
 Init_Mode init;

 if (select_str == "first")
  select = Select_Mode::FIRST;
 else
  select = Select_Mode::BEST;

 if (neighbour_str == "insert")
  neighbour = Neighbour_Mode::INSERT;
 else if (neighbour_str == "swap")
  neighbour = Neighbour_Mode::SWAP;
 else
  neighbour = Neighbour_Mode::EXCHANGE;

 if (init_str == "rnd")
  init = Init_Mode::RND;
 else if (init_str == "edd")
  init = Init_Mode::EDD;
 else
  init = Init_Mode::MDD; 

 /************** Main Program *************/

 SMTWTP_climbing problems(instance_size, select, neighbour, init);
 InstanceGenerator generator(std::cin);

 while(true)
 {
  Instance inst = generator.get_new_instance(instance_size); 

  if (!generator.previous_good())
   break;
  
  auto solution = problems.get_solution(inst);

  std::cout << "i: " << inst.get_id(); 
  std::cout << " score: " << problems.compute_cost(solution, inst);
  std::cout << std::endl;
 }

 return 0;
}
