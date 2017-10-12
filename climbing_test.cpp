#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <math.h>
#include <iomanip>
#include <map>

#include "Instance.h"
#include "InstanceGenerator100.h"
#include "InstanceGenerator1000.h"
#include "SMTWTP_climbing.h"
#include "SMTWTP_initializer.h"
#include "SMTWTP_vnd.h"

////////////////////////////////////////////////////////////////////////////
void usage()
////////////////////////////////////////////////////////////////////////////
{
 std::cerr << "usage: ./climbing_one_smtwtp_main.cpp <instance_size> <instance_file> <id_instance>\n"
           << "  ex: ./climbing_one_smtwtp_main 100 ../../instances/wt100.txt 1\n"
           << "      ./climbing_one_smtwtp_main 100 ../../instances/wt100.txt 10\n";
}

////////////////////////////////////////////////////////////////////////////
std::string which_mode(Select_Mode select, Neighbour_Mode neighbour, Init_Mode init)
////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////
std::string float_to_string(float f)
////////////////////////////////////////////////////////////////////////////
{
 std::ostringstream s;
 s << std::fixed << std::setprecision(2) << f;
 return s.str();
}

////////////////////////////////////////////////////////////////////////////
void display_result
////////////////////////////////////////////////////////////////////////////
(
 std::string mode,
 std::string time,
 std::string deviation,
 std::string cost_cpt,
 std::string found_cost,
 std::string has_best
)
{
 std::cout << std::setw(18) << mode 
           << "   "
           << std::setw(14) << std::fixed << std::setprecision(2) << time
           << "   "
           << std::setw(14) << deviation
           << "   "
           << std::setw(14) << cost_cpt
           << "   "
           << std::setw(14) << found_cost
           << "   "
           << std::setw(3) << has_best 
           << std::endl;
}

////////////////////////////////////////////////////////////////////////////
void solve_problem()
////////////////////////////////////////////////////////////////////////////
(
 SMTWTP problems,
 Instance inst,
 int nb_while
)
{
 float time_inter = 0.0;
 float deviation_inter = 0.0;
 float cost_cpt_inter = 0.0;
 float found_cost_inter = 0.0;
 bool has_best_inter = false;

 for (int i = 0 ; i < nb_while ; i++)
 {
  float deviation = 0.0;
  int time = 0;
  long cost = 0;
  std::chrono::high_resolution_clock::time_point t1;
  std::chrono::high_resolution_clock::time_point t2;
  std::vector<long> solution;

  t1 = std::chrono::high_resolution_clock::now();
  solution = problems.get_solution(inst);
  t2 = std::chrono::high_resolution_clock::now();

  time = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count();
  cost = problems.compute_cost(solution, inst);
  deviation = ((float)cost - (float)best_cost[inst.get_id()-1]) / (float)instance_size;

  time_inter += time;
  deviation_inter += deviation;
  cost_cpt_inter += problems.get_compute_cpt();
  found_cost_inter += cost;
  if (deviation == 0)
   has_best_inter = true;
 }

 display_result(mode_str[init],
                float_to_string(time_inter/nb_while),
                float_to_string(deviation_inter/nb_while),
                float_to_string(cost_cpt_inter/nb_while),
                float_to_string(found_cost_inter/nb_while),
                std::string((has_best_inter) ? "Y" : "N"));
}

int main (int argc, char *argv[])
////////////////////////////////////////////////////////////////////////////
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
 int nb_while = 1;
 float time_inter = 0.0;
 float deviation_inter = 0.0;
 float cost_cpt_inter = 0.0;
 float found_cost_inter = 0.0;
 bool has_best_inter = false;

 /************** Main Program *************/

 std::vector<long> best_cost;

 std::ifstream best_file("../../instances/optimal_results.txt");
 if (!best_file.is_open())
 {
  std::cerr << "Fichier non trouvé.\n";
  return -1;
 }

 long inter;
 while (best_file >> inter)
  best_cost.push_back(inter);

 InstanceGenerator1000 generator({"../../instances/wt_1000_1.txt"});
 Instance inst = generator.get_new_instance(instance_size);

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
 display_result("Mode", "Time (us)", "Deviation", "Cost cpt", "Found cost", "Best");

 std::vector<SMTWTP> algos;

 /* Basic Algos */

 {
  std::map<Init_Mode, std::string> mode_str =
  {
   {Init_Mode::RND, "basic rnd"},
   {Init_Mode::EDD, "basic edd"},
   {Init_Mode::MDD, "basic mdd"}
  };

  for (auto init : inits)
   algos.push_back(SMTWTP_initializer problems(instance_size, init));
 }

 /* 18 Algos  */

 for (auto select : selects)
 {
  for (auto neighbour : neighbours)
  {
   for (auto init : inits)
   {
    time_inter = 0; 
    deviation_inter = 0;
    cost_cpt_inter = 0;
    found_cost_inter = 0;
    has_best_inter = false;

    for (int i = 0 ; i < nb_while ; i++)
    {
     SMTWTP_climbing problems(instance_size, select, neighbour, init);
     
     float time = 0.0;
     float deviation = 0.0;
    
     std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now(); 
     auto solution = problems.get_solution(inst);
     std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    
     time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    
     long cost = problems.compute_cost(solution, inst);
     long current_best = best_cost[inst.get_id()-1];
    
     deviation = ((float)cost - (float)current_best) / (float)instance_size;

     time_inter += time;
     deviation_inter += deviation;
     cost_cpt_inter += problems.get_compute_cpt();
     found_cost_inter += cost;
     if (deviation == 0)
      has_best_inter = true;
    }
    display_result(which_mode(select, neighbour, init),
                   float_to_string(time_inter/nb_while),
                   float_to_string(deviation_inter/nb_while),
                   float_to_string(cost_cpt_inter/nb_while),
                   float_to_string(found_cost_inter/nb_while),
                   std::string((has_best_inter) ? "Y" : "N"));
   }
  }
 }

 /* VND  */ 

 std::vector<std::vector<Config_VND>> configs =
 {
  {
   Config_VND(Select_Mode::FIRST,
              Neighbour_Mode::EXCHANGE),
   Config_VND(Select_Mode::FIRST,
              Neighbour_Mode::SWAP),
   Config_VND(Select_Mode::FIRST,
              Neighbour_Mode::INSERT)
  },
  {
   Config_VND(Select_Mode::FIRST,
              Neighbour_Mode::EXCHANGE),
   Config_VND(Select_Mode::FIRST,
              Neighbour_Mode::INSERT),
   Config_VND(Select_Mode::FIRST,
              Neighbour_Mode::SWAP)
  }
 };

 for (auto conf : configs)
 {
  Init_Mode init = Init_Mode::MDD;
  time_inter = 0;
  deviation_inter = 0;
  cost_cpt_inter = 0;
  found_cost_inter = 0;
  has_best_inter = false;

  for (int i = 0 ; i < nb_while ; i++)
  {
   SMTWTP_vnd problems(instance_size, init, conf);
  
   float time = 0.0;
   float deviation = 0.0;

   std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
   auto solution = problems.get_solution(inst);
   std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

   time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

   long cost = problems.compute_cost(solution, inst);
   long current_best = best_cost[inst.get_id()-1];

   deviation = ((float)cost - (float)current_best) / (float)instance_size;

   time_inter += time;
   deviation_inter += deviation;
   cost_cpt_inter += problems.get_full_compute_cpt();
   found_cost_inter += cost;
   if (deviation == 0)
    has_best_inter = true;
  }

  std::string vnd_string;
  for (auto v : conf)
   vnd_string += v.get_title() + "-";
  vnd_string.pop_back();

  // Affiche les résultats
  display_result(vnd_string,
                 float_to_string(time_inter/nb_while),
                 float_to_string(deviation_inter/nb_while),
                 float_to_string(cost_cpt_inter/nb_while),
                 float_to_string(found_cost_inter/nb_while),
                 std::string((has_best_inter) ? "Y" : "N"));
 }

 return 0;
}
