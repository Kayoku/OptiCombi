#ifndef model_instance
#define model_instance

#include <iostream>
#include <fstream>
#include <vector>

class Instance {
 private:
  std::vector<long> durations;
  std::vector<long> weights;
  std::vector<long> deadlines; 
  
  int id;
  int instance_size;
  long best_sol;

 public:
  Instance
  (
   int id,
   int instance_size,
   long best_sol,
   std::vector<long> durations,
   std::vector<long> weights,
   std::vector<long> deadlines
  ):
   durations(durations),
   weights(weights),
   deadlines(deadlines),
   id(id),
   instance_size(instance_size),
   best_sol(best_sol)
  {}

  Instance():
   id(0),
   instance_size(0),
   best_sol(0)
  {}

  int get_id() { return id; }
  int get_instance_size() { return instance_size; }
  long get_best_sol() { return best_sol; }
  long get_duration(int i) { return durations[i]; }
  long get_weight(int i) { return weights[i]; }
  long get_deadline(int i) { return deadlines[i]; }
  
  std::vector<long> get_durations() { return durations; }
  std::vector<long> get_weights() { return weights; }
  std::vector<long> get_deadlines() { return deadlines; }

  void display();
};

#endif
