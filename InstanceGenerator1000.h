#ifndef INSTANCE_GENERATOR_1000_DEFINE
#define INSTANCE_GENERATOR_1000_DEFINE

#include <iostream>
#include <fstream>

#include "InstanceGenerator.h"

class InstanceGenerator1000 : public InstanceGenerator
{
 private:
  std::vector<std::string> files;

 public:
  InstanceGenerator1000(std::vector<std::string> files):
    files(files),
    InstanceGenerator()
   {}  

  Instance get_new_instance(int instance_size) override;
  bool previous_good() override;
};

#endif
