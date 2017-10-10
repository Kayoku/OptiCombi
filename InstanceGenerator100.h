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
   {}  

  Instance get_new_instance(int instance_size) override;
  bool previous_good() override;
};

#endif
