#ifndef SMTWTP_define
#define SMTWTP_define

#include <iostream>
#include <vector>

#include "Instance.h"

class SMTWTP
{
 private:
  int instance_size;
  int compute_cpt;

 public:
  SMTWTP(int instance_size):
      instance_size(instance_size),
      compute_cpt(0)
    {}

  int get_instance_size() { return instance_size; }
  int get_compute_cpt() { return compute_cpt; }

  long compute_cost (std::vector<long> &solution,
                    Instance &instance);

  virtual std::vector<long> get_solution(Instance &instance) = 0;
};

#endif
