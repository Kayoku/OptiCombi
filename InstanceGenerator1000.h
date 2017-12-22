#ifndef INSTANCE_GENERATOR_1000_DEFINE
#define INSTANCE_GENERATOR_1000_DEFINE

#include <iostream>
#include <fstream>

#include "InstanceGenerator.h"

class InstanceGenerator1000 : public InstanceGenerator
{
 private:
  std::vector<std::string> files;
  const int instance_size = 1000;

 public:
  InstanceGenerator1000(std::vector<std::string> files):
    files(files),
    InstanceGenerator()
   {
    std::ifstream best_file("../../instances/optimal_results.txt");
    if (!best_file.is_open())
    {
     std::cerr << "Fichier des optimaux non trouvé." << std::endl;
     exit(-1);
    }

    long inter = 0;
    while (best_file >> inter)
     best_sol.push_back(inter);
   }  

  Instance get_new_instance() override;
  bool previous_good() override;
};

#endif
