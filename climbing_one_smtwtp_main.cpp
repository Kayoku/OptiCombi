#include <iostream>
#include <fstream>
#include <chrono>
#include <math.h>
#include <iomanip>

#include "Instance.h"
#include "InstanceGenerator.h"
#include "SMTWTP_climbing.h"

void usage()
{
 std::cerr << "usage: ./climbing_one_smtwtp_main.cpp <instance_size> <instance_file> <id_instance>\n"
           << "  ex: ./climbing_one_smtwtp_main 100 ../../instances/wt100.txt 1\n"
           << "      ./climbing_one_smtwtp_main 100 ../../instances/wt100.txt 10\n";
}

std::string which_mode(Select_Mode select, Neighbour_Mode neighbour, Init_Mode init)
{
 std::string s, n, i;

 if (Select_Mode::FIRST == select)
  s = "first";
 else
  s = "best";

 if (Neighbour_Mode::INSERT == neighbour)
  n = "insert";
 else if (Neighbour_Mode::SWAP == neighbour)
  n = "swap";
 else
  n = "exchange"; 

 if (Init_Mode::RND == init)
  i = "rnd";
 else if (Init_Mode::EDD == init)
  i = "edd";
 else
  i = "mdd";

 return s+"-"+n+"-"+i;
}

int main (int argc, char *argv[])
{
 /******** Récupération des arguments ***********/

 if (argc != 4)
 {
  usage();
  return -1;
 }

 int instance_size = std::stoi(argv[1]);
 std::string instance_file(argv[2]);
 int id_instance = std::stoi(argv[3]);

 /************** Main Program *************/

 std::vector<int> best_cost;

 std::ifstream best_file("../../instances/wtbest100b.txt");
 if (!best_file.is_open())
 {
  std::cerr << "Fichier des optimaux non trouvé.\n";
  return -1;
 }

 int inter = 0;
 while(best_file >> inter)
  best_cost.push_back(inter);

 std::ifstream file(instance_file);
 if (!file.is_open())
 {
  std::cerr << "Fichier non trouvé.\n";
  return -1;
 }

 InstanceGenerator generator(file);
 Instance inst = generator.get_new_instance(instance_size);
 while (inst.get_id() != id_instance)
  inst = generator.get_new_instance(instance_size);

 if (inst.get_id() != id_instance)
 {
  std::cerr << "Pas d'instance à cette id.\n";
  return -1;
 }

 // Pour les 18 Algos possibles, on fait
 std::vector<Select_Mode> selects = {Select_Mode::FIRST,
                                    Select_Mode::BEST};
 std::vector<Neighbour_Mode> neighbours = {Neighbour_Mode::INSERT,
                                          Neighbour_Mode::SWAP,
                                          Neighbour_Mode::EXCHANGE};
 std::vector<Init_Mode> inits = {Init_Mode::RND,
                                Init_Mode::EDD,
                                Init_Mode::MDD};

 std::cout << "id: " << inst.get_id() << '\n';
 std::cout << "best result: " << best_cost[inst.get_id()-1] << '\n';
 std::cout << std::setw(18) << "mode" 
           << "   "
           << std::setw(10) << "time (us)"
           << "   "
           << std::setw(10) << "deviation"
           << "   "
           << std::setw(10) << "cost cpt"
           << "   "
           << std::setw(10) << "found cost" << std::endl;

 for (auto select : selects)
 {
  for (auto neighbour : neighbours)
  {
   for (auto init : inits)
   {
    SMTWTP_climbing problems(instance_size, select, neighbour, init);
    
    int time_solution = 0.0;
    float deviation_solution = 0.0;
   
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now(); 
    auto solution = problems.get_solution(inst);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
   
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    time_solution = duration;
   
    int current_cost = problems.compute_cost(solution, inst);
    int current_best = best_cost[inst.get_id()-1];
   
    deviation_solution = ((float)current_cost - (float)current_best) / 100.0;
   
    // Affiche les résultats
    std::cout << std::setw(18) << which_mode(select, neighbour, init)
              << "   "
              << std::setw(10) << time_solution
              << "   "
              << std::setw(10) << deviation_solution
              << "   "
              << std::setw(10) << problems.get_compute_cpt()
              << "   " 
              << std::setw(10) << current_cost
              << (deviation_solution == 0 ? " Y" : " N") << std::endl;
   }
  }
 }
   
 return 0;
}
