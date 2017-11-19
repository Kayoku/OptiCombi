#include "InstanceGenerator1000.h"
#include <fstream>
#include <string>

////////////////////////////////////////////////////////////////////////////
Instance InstanceGenerator1000::get_new_instance()
////////////////////////////////////////////////////////////////////////////
{
 // Ouverture du premier fichier
 std::ifstream file(files[0]); 
 if (!file)
 {
  std::cerr << "Impossible d'ouvrir le fichier " << files[0] << '\n';
  exit(-1);
 }

 char c = files[0][files[0].size()-5];
 std::string nb;
 int ic = 0;
 while (c != '_')
 {
  nb = c + nb;
  ic++;
  c = files[0][files[0].size()-5-ic];
 }

 int id_inst = std::stoi(nb);
 files.erase(files.begin());

 // Création des variables utiles
 long inter;
 char comma;
 cpt_instance++;

 std::vector<long> durations;
 std::vector<long> weights;
 std::vector<long> deadlines;
 
 for (int i = 0 ; i < instance_size ; i++)
 {
  file >> inter;
  durations.push_back(inter);
  file >> comma;
  file >> inter;
  weights.push_back(inter);
  file >> comma;
  file >> inter;
  deadlines.push_back(inter);
 }
 
 Instance inst(id_inst, instance_size, best_sol[id_inst-1], durations, weights, deadlines);
 return inst;
}

////////////////////////////////////////////////////////////////////////////
bool InstanceGenerator1000::previous_good()
////////////////////////////////////////////////////////////////////////////
{
 if (cpt_instance >= 25)
  return false;
 return true;
}
