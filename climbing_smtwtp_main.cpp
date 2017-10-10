#include <iostream>
#include <fstream>
#include <chrono>
#include <math.h>

#include "Instance.h"
#include "InstanceGenerator100.h"
#include "SMTWTP_climbing.h"

void usage()
{
 std::cerr << "usage: ./climbing_smtwtp_main.cpp <instance_size> <instance_file> <nb_read> <out_file> [first, best] [insert, swap, exchange] [rnd, edd, mdd]\n"
           << "  ex: ./climbing_smtwtp_main 100 ../../instances/wt100.txt 1 out best exchange edd\n"
           << "      ./climbing_smtwtp_main 100 ../../instances/wt100.txt 10 out.csv best exchange mdd\n";
}

int main (int argc, char *argv[])
{
 /******** Récupération des arguments ***********/

 if (argc != 8)
 {
  usage();
  return -1;
 }

 int instance_size = std::stoi(argv[1]);
 std::string instance_file(argv[2]);
 int nb_read = std::stoi(argv[3]);
 std::string out_file(argv[4]);
 std::string select_str(argv[5]);
 std::string neighbour_str(argv[6]);
 std::string init_str(argv[7]);

 if (!((select_str == "first" || select_str == "best") &&
     (neighbour_str == "insert" || neighbour_str == "swap" || neighbour_str == "exchange") &&
     (init_str == "rnd" || init_str == "edd" || init_str == "mdd")))
 {
  std::cerr << select_str << " " << neighbour_str << " " << init_str << '\n';
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

 bool live = false;
 if (out_file == "out")
  live = true;

 /************** Main Program *************/

 int cpt = 1;
 SMTWTP_climbing problems(instance_size, select, neighbour, init);

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

 std::vector<std::vector<float>> deviation_table;
 std::vector<std::vector<int>> time_table;
 deviation_table.resize(best_cost.size());
 time_table.resize(best_cost.size());

 do
 {
  std::ifstream file(instance_file);
  if (!file.is_open())
  {
   std::cerr << "Fichier non trouvé.\n";
   return -1;
  }
 
  InstanceGenerator100 generator(file);

  while(true)
  {
   Instance inst = generator.get_new_instance(instance_size); 
   if (!generator.previous_good())
    break;

   int time_solution = 0.0;
   float deviation_solution = 0.0;

   std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now(); 
   auto solution = problems.get_solution(inst);
   std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

   auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
   time_solution = duration;

   int current_cost = problems.compute_cost(solution, inst);
   int current_best = best_cost[inst.get_id()-1];

   deviation_solution = ((float)current_cost - (float)current_best) / 100.0;

   // Affiche les résultats
   std::cout << "id: " << inst.get_id()
             << " time: " << time_solution
             << " deviation: " << deviation_solution
             << " (" << current_cost << ")" << std::endl;

   // Enregistre les résultats
   if (!live) 
   {
    time_table[inst.get_id()-1].push_back(time_solution);
    deviation_table[inst.get_id()-1].push_back(deviation_solution);
   }
  }
  file.close();
 } while (cpt++ < nb_read && !live);

 if (!live)
 {
  std::ofstream out_stream(out_file);

  for (int i = 0 ; i < time_table.size() ; i++)
  {
   std::string str;
   for (int j = 0 ; j < time_table[i].size() ; j++)
    str += std::to_string(time_table[i][j]) + ",";
   str.pop_back();
   str += '\n';
   out_stream << str;
  }

  for (int i = 0 ; i < deviation_table.size() ; i++)
  {
   std::string str;
   for (int j = 0 ; j < deviation_table[i].size() ; j++)
    str += std::to_string(deviation_table[i][j]) + ",";
   str.pop_back();
   str += '\n';
   out_stream << str;
  }
 }
 
 return 0;
}
