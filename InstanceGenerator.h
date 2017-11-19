#ifndef INSTANCE_GENERATOR_DEFINE
#define INSTANCE_GENERATOR_DEFINE

#include <iostream>
#include <fstream>

#include "Instance.h"

class InstanceGenerator
{
 protected:
  int cpt_instance;
  std::vector<long> best_sol;

 public:
  InstanceGenerator():
      cpt_instance(0)
   {}  

  virtual Instance get_new_instance() = 0;
  virtual bool previous_good() = 0;
};

#endif
