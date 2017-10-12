#ifndef INSTANCE_GENERATOR_100_DEFINE
#define INSTANCE_GENERATOR_100_DEFINE

#include <iostream>
#include <fstream>

#include "InstanceGenerator.h"

class InstanceGenerator100: public InstanceGenerator
{
 private:
  std::istream &file;

 public:
  InstanceGenerator100(std::istream &file):
    InstanceGenerator(),
    file(file)
   {
    std::ifstream best_file("../../instances/wtbest100b.txt");
    if (!best_file.is_open())
    {
     std::cerr << "Fichier des optimaux non trouvé." << std::endl;
     exit(-1);
    }

    long inter = 0;
    while (best_file >> inter)
     best_sol.push_back(inter);
   }  

  Instance get_new_instance(int instance_size) override;
  bool previous_good() override;
};

#endif
