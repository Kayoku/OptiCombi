#ifndef model_instance
#define model_instance

#include <iostream>
#include <fstream>
#include <vector>

class Instance {
 private:
  std::vector<int> durations;
  std::vector<int> weights;
  std::vector<int> deadlines; 
  
  int id;
  int instance_size;

 public:
  Instance
  (
   int id,
   int instance_size,
   std::vector<int> durations,
   std::vector<int> weights,
   std::vector<int> deadlines
  ):
   id(id),
   instance_size(instance_size),
   durations(durations),
   weights(weights),
   deadlines(deadlines)
  {}

  int get_id() { return id; }
  int get_instance_size() { return instance_size; }
  int get_duration(int i) { return durations[i]; }
  int get_weight(int i) { return weights[i]; }
  int get_deadline(int i) { return deadlines[i]; }
  
  std::vector<int> get_durations() { return durations; }
  std::vector<int> get_weights() { return weights; }
  std::vector<int> get_deadlines() { return deadlines; }

  void display();
};

#endif
