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

 public:
  Instance
  (
   int id,
   int instance_size,
   std::vector<long> durations,
   std::vector<long> weights,
   std::vector<long> deadlines
  ):
   id(id),
   instance_size(instance_size),
   durations(durations),
   weights(weights),
   deadlines(deadlines)
  {}

  int get_id() { return id; }
  int get_instance_size() { return instance_size; }
  long get_duration(int i) { return durations[i]; }
  long get_weight(int i) { return weights[i]; }
  long get_deadline(int i) { return deadlines[i]; }
  
  std::vector<long> get_durations() { return durations; }
  std::vector<long> get_weights() { return weights; }
  std::vector<long> get_deadlines() { return deadlines; }

  void display();
};

#endif
