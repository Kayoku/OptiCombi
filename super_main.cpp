#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <map>
#include <memory>
#include <algorithm>

#include "Instance.h"
#include "InstanceGenerator100.h"
#include "InstanceGenerator1000.h"
#include "SMTWTP.h"
#include "SMTWTP_climbing.h"
#include "SMTWTP_initializer.h"
#include "SMTWTP_vnd.h"
#include "SMTWTP_ILS.h"
#include "SMTWTP_population.h"


/*
  Nom de l'algo1
    Temps
    Deviation
    Cost
    Taux de bon résultat
  Nom de l'algo2
*/

std::map<std::string, std::array<float, 5>> values;

struct ErrorProxy {
 ErrorProxy(const char* file, int line)
 {}

 ~ErrorProxy()
 {
  std::cout << std::endl;
  exit(EXIT_FAILURE);
 }

 template<typename T>
 ErrorProxy& operator<<(T&& t)
 {
  std::cout << t;
  return *this;
 }
};

#define LOG_ERROR ErrorProxy{__FILE__, __LINE__}

/* USEFUL FUNCTIONS */

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
 std::cout << std::setw(20) << mode
           << "   "
           << std::setw(14) << time
           << "   "
           << std::setw(14) << deviation
           << "   "
           << std::setw(14) << cost_cpt
           << "   "
           << std::setw(14) << found_cost
           << "   "
           << std::setw(4) << has_best
           << std::endl;
}

////////////////////////////////////////////////////////////////////////////
void init_values(std::string name)
////////////////////////////////////////////////////////////////////////////
{
 for (int i = 0 ; i < 5 ; i++)
  values[name][i] = 0;
}

////////////////////////////////////////////////////////////////////////////
void usage()
////////////////////////////////////////////////////////////////////////////
{
 std::cerr << "usage: ./oc <instance> <id> <algos> <iterations>" << std::endl;
 std::cerr << "- instance   : 100 or 1000." << std::endl;
 std::cerr << "- id         : all for all, id (number) for just one instance." << std::endl;
 std::cerr << "- algos      : all/const/climbing/vnd/vnd-plus/ILS/VNS/GA." << std::endl;
 std::cerr << "- iterations : nb of iterations (> 1)." << std::endl;
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
 float has_best_inter = 0.0;

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
  deviation = 100 * ((((float)cost) - ((float)inst.get_best_sol())) / (std::max<float>(1, inst.get_best_sol())));

  time_inter += time;
  deviation_inter += deviation;
  cost_cpt_inter += problems.get_compute_cpt() - cost_cpt_inter;
  found_cost_inter += cost;
  if (inst.get_best_sol() == cost)
   has_best_inter += 1;
 }

 problems.reset_compute_cpt();

 values[problems.get_name()][0] += time_inter/nb_while;
 values[problems.get_name()][1] += deviation_inter/nb_while;
 values[problems.get_name()][2] += cost_cpt_inter/nb_while;

 if (has_best_inter > 0)
 {
  values[problems.get_name()][3] += (float)has_best_inter/(float)nb_while * 100.0;
  values[problems.get_name()][4]++;
 }

 display_result(problems.get_name(),
                float_to_string(time_inter/nb_while),
                float_to_string(deviation_inter/nb_while),
                float_to_string(cost_cpt_inter/nb_while),
                float_to_string(found_cost_inter/nb_while),
                float_to_string(has_best_inter/nb_while * 100));
}

////////////////////////////////////////////////////////////////////////////
std::vector<std::unique_ptr<SMTWTP>> choice_algos
////////////////////////////////////////////////////////////////////////////
(
 int instance_size,
 std::string str_algo
)
{
 std::vector<std::unique_ptr<SMTWTP>> algos;

 std::vector<Select_Mode> selects = {Select_Mode::FIRST,
                                     Select_Mode::BEST};
 std::vector<Neighbour_Mode> neighbours = {Neighbour_Mode::INSERT,
                                           Neighbour_Mode::SWAP,
                                           Neighbour_Mode::EXCHANGE};
 std::vector<Init_Mode> inits = {Init_Mode::RND,
                                 Init_Mode::EDD,
                                 Init_Mode::MDD};

 if (str_algo == "all" || str_algo == "const")
 {
  for (auto init : inits)
   algos.push_back(std::unique_ptr<SMTWTP>(new SMTWTP_initializer(instance_size, init)));
 }

 if (str_algo == "all" || str_algo == "climbing")
 {
  for (auto select : selects)
   for (auto neighbour : neighbours)
    for (auto init : inits)
     algos.push_back(std::unique_ptr<SMTWTP>(new SMTWTP_climbing(instance_size, select, neighbour, init)));
 }

 std::vector<std::vector<Neighbour_Mode>> configs =
 {
  {
   Neighbour_Mode::EXCHANGE,
   Neighbour_Mode::SWAP,
   Neighbour_Mode::INSERT
  },
  {
   Neighbour_Mode::EXCHANGE,
   Neighbour_Mode::INSERT,
   Neighbour_Mode::SWAP
  }
 };

 if (str_algo == "all" || str_algo == "vnd")
  for (auto init : inits)
   for (auto select : selects)
    for (auto conf : configs)
     algos.push_back(std::unique_ptr<SMTWTP>(new SMTWTP_vnd(instance_size, init, select, conf)));

 if (str_algo == "all" || str_algo == "vnd-plus")
  for (auto init : inits)
   algos.push_back(std::unique_ptr<SMTWTP>(new SMTWTP_vnd(instance_size, init, 1)));

 if (str_algo == "all" || str_algo == "ILS")
 {
  for (auto swap : {true, false})
  {
   algos.push_back(std::unique_ptr<SMTWTP>(new SMTWTP_ILS(instance_size, Init_Mode::MDD, Select_Mode::FIRST, configs[1], 5, 30, swap)));
   algos.push_back(std::unique_ptr<SMTWTP>(new SMTWTP_ILS(instance_size, Init_Mode::EDD, Select_Mode::BEST, configs[1], 5, 30, swap)));
  }
 }

 if (str_algo == "all" || str_algo == "GA")
 {
  algos.push_back(std::unique_ptr<SMTWTP>(new SMTWTP_population(instance_size, 10, 1000, 50, 1)));
  algos.push_back(std::unique_ptr<SMTWTP>(new SMTWTP_population(instance_size, 100, 1000, 50, 1)));
  algos.push_back(std::unique_ptr<SMTWTP>(new SMTWTP_population(instance_size, 10, 10000, 50, 1)));
  algos.push_back(std::unique_ptr<SMTWTP>(new SMTWTP_population(instance_size, 10, 1000, 300, 1)));
  algos.push_back(std::unique_ptr<SMTWTP>(new SMTWTP_population(instance_size, 10, 1000, 50, 4)));
 }

 return algos;
}

////////////////////////////////////////////////////////////////////////////
std::vector<Instance> choice_instances
////////////////////////////////////////////////////////////////////////////
(
 int instance_size,
 int instance_id
)
{
 std::vector<Instance> instances;

 if (instance_size == 100)
 {
  std::string filename = "../../instances/wt100.txt";
  std::ifstream file(filename);
  if (!file.is_open())
   LOG_ERROR << "File " << filename << " doesn't exist.";
  std::unique_ptr<InstanceGenerator> generator(new InstanceGenerator100(file));

  // Récupération de toutes les instances
  if (!instance_id)
   for (int i = 0 ; i < 125 ; i++)
    instances.push_back(generator->get_new_instance());
  // Récupération d'une instance
  else
  {
   Instance inter;
   inter = generator->get_new_instance();
   while (inter.get_id() != instance_id)
    inter = generator->get_new_instance();
   if (inter.get_id() != instance_id)
    LOG_ERROR << "No instance with id " << instance_id;
   instances.push_back(inter);
  }
 }
 else if (instance_size == 1000)
 {
  // Récupérations de toutes les instances
  if (!instance_id)
  {
   std::vector<std::string> filenames;
   for (int i = 1 ; i < 26 ; i++)
    filenames.push_back("../../instances/wt_1000_"+std::to_string(i)+".txt");
   std::unique_ptr<InstanceGenerator> generator(new InstanceGenerator1000(filenames));
   for (int i = 0 ; i < 25 ; i++)
    instances.push_back(generator->get_new_instance());
  }
  // Récupération d'une instance
  else
  {
   if (instance_id > 25)
    LOG_ERROR << "No instance with id " << instance_id;
   std::vector<std::string> filenames = {"../../instances/wt_1000_"+std::to_string(instance_id)+".txt"};
   std::unique_ptr<InstanceGenerator> generator(new InstanceGenerator1000(filenames));
   instances.push_back(generator->get_new_instance());
  }
 }
 else
  LOG_ERROR << "This instance size " << instance_size << " doesn't exist.";

 return instances;
}

/* MAIN */

////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
////////////////////////////////////////////////////////////////////////////
{
 // Récupération des arguments
 if (argc != 5)
 {
  usage();
  return -1;
 }

 int instance_size = std::stoi(argv[1]);
 std::string str_instance = std::string(argv[2]);
 int instance_id = 0;
 std::string str_algo = std::string(argv[3]);
 int nb_while = std::stoi(argv[4]);
 if (str_instance != "all")
  instance_id = std::stoi(str_instance);

 // Génération des instances
 auto instances = choice_instances(instance_size, instance_id);

 // Sélection des algos testés
 auto algos = choice_algos(instance_size, str_algo);

 // Lancement des algos
 int cpt_inst = 0;
 for (auto &algo : algos)
  init_values(algo->get_name());

 for (auto &inst : instances)
 {
  cpt_inst++;
  std::cout << "id: " << inst.get_id() << std::endl;
  std::cout << "best result: " << inst.get_best_sol() << std::endl;
  display_result("Mode", "Time (us)", "Deviation", "Cost cpt", "Found cost", "Best");
  for (auto &algo : algos)
   solve_problem(*algo, inst, nb_while);
 }

 // On divise par le nb d'instance
 for (auto v : values)
 {
  for (int i = 0 ; i < 3 ; i++)
   values[v.first][i] /= cpt_inst;
  values[v.first][3] /= std::max<float>(1, values[v.first][4]);
 }

 // On écrit 
 std::ofstream csv_file("test.csv");
 csv_file << ",Temps,Deviation,Cost,Taux,Trouve" << std::endl;
 for (auto v : values)
 {
  csv_file << v.first << ","
           << values[v.first][0] << ","
           << values[v.first][1] << ","
           << values[v.first][2] << ","
           << values[v.first][3] << ","
           << values[v.first][4] << std::endl;
 }

 return 0;
}
