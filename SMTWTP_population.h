#ifndef SMTWTP_population_define
#define SMTWTP_population_define

#include <iostream>
#include <vector>
#include <random>

#include "SMTWTP_initializer.h"
#include "Instance.h"

typedef std::vector<std::pair<std::vector<long>, float>> Population;
typedef std::pair<std::vector<long>, float> Person;

class SMTWTP_population : public SMTWTP_initializer
{
 private:
  Population populations;
  int size_population;
  int nb_generation;
  int rate_mutation;
  int intensity_mutation;
  std::random_device rd;
  std::mt19937 g;

 public:
  SMTWTP_population(int instance_size,
                    int size_population,
                    int nb_generation,
                    int rate_mutation,
                    int intensity_mutation):
     SMTWTP_initializer(instance_size, Init_Mode::RND),
     size_population(size_population),
     nb_generation(nb_generation),
     rate_mutation(rate_mutation),
     intensity_mutation(intensity_mutation),
     rd(),
     g(rd())
   {}

  std::vector<long> order_crossover
   (std::vector<long> parent1,
    std::vector<long> parent2);
  Population elitist(Population parents,
                     Population childs);
  std::vector<long> swap_mutation(std::vector<long> parent);
  std::vector<long> insert_mutation(std::vector<long> parent);
  std::vector<long> get_solution(Instance &instance) override;
  std::string get_name() override;
};

#endif
