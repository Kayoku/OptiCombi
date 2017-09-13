#ifndef INSTANCE_GENERATOR_DEFINE
#define INSTANCE_GENERATOR_DEFINe

#include <iostream>
#include <fstream>

#include "Instance.h"

class InstanceGenerator
{
 private:
  std::istream &file;
  int cpt_instance;

 public:
  InstanceGenerator(std::istream &file):
      file(file),
      cpt_instance(0)
   {}  

  Instance get_new_instance(int instance_size);
  bool previous_good();
};

#endif
