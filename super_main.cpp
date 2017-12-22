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
void usage()
////////////////////////////////////////////////////////////////////////////
{
 std::cerr << "usage: ./oc <instance> <id> <algos> <iterations>" << std::endl;
 std::cerr << "- instance   : 100 or 1000." << std::endl;
 std::cerr << "- id         : all for all, id (number) for just one instance." << std::endl;
 std::cerr << "- algos      : all/const/climbing/vnd/ILS/VNS/GA." << std::endl;
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
  deviation = 100 * (((float)cost - (float)inst.get_best_sol()) / (float)inst.get_best_sol());

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

 if (str_algo == "all" || str_algo == "vnd")
 {
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
   algos.push_back(std::unique_ptr<SMTWTP>(new SMTWTP_vnd(instance_size, Init_Mode::MDD, conf)));

  if (str_algo == "all" || str_algo == "ils")
   algos.push_back(std::unique_ptr<SMTWTP>(new SMTWTP_ILS(instance_size, Init_Mode::MDD, configs[0], 2, 3)));

  if (str_algo == "all" || str_algo == "vns")
   algos.push_back(std::unique_ptr<SMTWTP>(new SMTWTP_vns(instance_size, Init_Mode::MDD, 10)));
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
 for (auto &inst : instances)
 {
  std::cout << "id: " << inst.get_id() << std::endl;
  std::cout << "best result: " << inst.get_best_sol() << std::endl;
  display_result("Mode", "Time (us)", "Deviation", "Cost cpt", "Found cost", "Best");
  for (auto &algo : algos)
   solve_problem(*algo, inst, nb_while);
 }

 return 0;
}
