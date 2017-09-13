#include <iostream>
#include <fstream>

#include "Instance.h"
#include "InstanceGenerator.h"
#include "SMTWTP_simple.h"

int main (int argc, char *argv[])
{
 /* Récupération des arguments */
 if (argc != 2)
 {
  std::cout << "usage: ./simple_smtwtp_main.cpp <instance_size> < <instance_file>\n";
  return -1;
 }

 int instance_size = std::stoi(argv[1]);

 SMTWTP_simple problems(instance_size);
 InstanceGenerator generator(std::cin);

 while(true)
 {
  Instance inst = generator.get_new_instance(instance_size); 

  if (!generator.previous_good())
   break;
  
  auto rand_solution = problems.random_solution();
  auto edd_solution = problems.EDD_solution(inst);
  auto mdd_solution = problems.MDD_solution(inst);

  std::cout << "i: " << inst.get_id(); 
  std::cout << " rand: " << problems.compute_cost(rand_solution, inst);
  std::cout << " edd: " << problems.compute_cost(edd_solution, inst);
  std::cout << " mdd: " << problems.compute_cost(mdd_solution, inst);
  std::cout << std::endl;
 }

 return 0;
}
