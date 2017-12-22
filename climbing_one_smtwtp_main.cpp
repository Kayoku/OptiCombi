#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <math.h>
#include <iomanip>
#include <map>
#include <memory>

#include "Instance.h"
#include "InstanceGenerator100.h"
#include "InstanceGenerator1000.h"
#include "SMTWTP.h"
#include "SMTWTP_climbing.h"
#include "SMTWTP_initializer.h"
#include "SMTWTP_vnd.h"
#include "SMTWTP_ILS.h"
#include "SMTWTP_vns.h"

////////////////////////////////////////////////////////////////////////////
void usage()
////////////////////////////////////////////////////////////////////////////
{
 std::cerr << "usage: ./climbing_one_smtwtp_main.cpp <instance_size> <instance_file> <id_instance>\n"
           << "  ex: ./climbing_one_smtwtp_main 100 1\n"
           << "      ./climbing_one_smtwtp_main 1000 10\n";
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
 std::cout << std::setw(25) << mode
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
void solve_problem
////////////////////////////////////////////////////////////////////////////
(
 SMTWTP &problems,
 Instance &inst,
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
  deviation = (((float)cost - (float)inst.get_best_sol()) / (float)inst.get_instance_size());

  time_inter += time;
  deviation_inter += deviation;
  cost_cpt_inter += problems.get_compute_cpt();
  found_cost_inter += cost;
  if (deviation == 0)
   has_best_inter = true;
 }

 display_result(problems.get_name(),
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

 if (argc != 3)
 {
  usage();
  return -1;
 }

 int instance_size = std::stoi(argv[1]);
 int id_instance = std::stoi(argv[2]);
 int nb_while = 10;
 std::cout << "Nombre de boucle: " << nb_while << std::endl;

 std::vector<std::unique_ptr<SMTWTP>> problems;

 /************** Main Program *************/

 Instance inst;
 std::vector<std::unique_ptr<InstanceGenerator>> generators;

 if (instance_size == 100)
 {
  if (id_instance < 1 || id_instance > 125)
  {
   std::cerr << "Pas d'instance à cette id." << std::endl;
   return -1;
  }
  std::string filename = "../../instances/wt100.txt";
  std::ifstream file(filename);
  if (!file.is_open())
  {
   std::cerr << "Pas de fichier" << std::endl;
   return -1;
  }
  generators.push_back(std::unique_ptr<InstanceGenerator>(new InstanceGenerator100(file)));
  inst = (*generators.back()).get_new_instance(instance_size);
  while (inst.get_id() != id_instance)
   inst = (*generators.back()).get_new_instance(instance_size);
 }
 else if (instance_size == 1000)
 {
  if (id_instance < 1 || id_instance > 25)
  {
   std::cerr << "Pas d'instance à cette id." << std::endl;
   return -1;
  }
  std::vector<std::string> filenames = {"../../instances/wt_1000_"+std::to_string(id_instance)+".txt"};
  generators.push_back(std::unique_ptr<InstanceGenerator>(new InstanceGenerator1000(filenames)));
  inst = (*generators.back()).get_new_instance(instance_size);
  while (inst.get_id() != id_instance)
   inst = (*generators.back()).get_new_instance(instance_size);
 }
 else
 {
  std::cerr << "Pas la bonne taille." << std::endl;
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
 std::cout << "best result: " << inst.get_best_sol() << '\n';
 display_result("Mode", "Time (us)", "Deviation", "Cost cpt", "Found cost", "Best");

 /* Basic Algos */

 for (auto init : inits)
  problems.push_back(std::unique_ptr<SMTWTP>(new SMTWTP_initializer(instance_size, init)));

 /* 18 Algos  */

 if (instance_size != 1000)
 {
  for (auto select : selects)
   for (auto neighbour : neighbours)
    for (auto init : inits)
     problems.push_back(std::unique_ptr<SMTWTP>(new SMTWTP_climbing(instance_size, select, neighbour, init)));
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

 Init_Mode init = Init_Mode::MDD;
 for (auto conf : configs)
  problems.push_back(std::unique_ptr<SMTWTP>(new SMTWTP_vnd(instance_size, init, conf)));

 /* ILS */

 problems.push_back(std::unique_ptr<SMTWTP>(new SMTWTP_ILS(instance_size, init, configs[0], 2, 3)));

 /* VNS */

 problems.push_back(std::unique_ptr<SMTWTP>(new SMTWTP_vns(instance_size, init, 10)));

 // On lance les algos

 for (auto &p : problems)
  solve_problem(*p, inst, nb_while);

 return 0;
}
