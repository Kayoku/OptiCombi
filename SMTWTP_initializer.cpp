#include "SMTWTP_initializer.h"
#include <algorithm>

////////////////////////////////////////////////////////////////////////////
std::vector<int> SMTWTP_initializer::get_initializer
////////////////////////////////////////////////////////////////////////////
(
 Instance &instance
)
{
 std::vector<int> init_solution;

 switch(init)
 {
  case Init_Mode::RND:
   init_solution = random_solution();
   break;
  case Init_Mode::EDD:
   init_solution = EDD_solution(instance);
   break;
  case Init_Mode::MDD:
   init_solution = MDD_solution(instance);
   break;
 }

 return init_solution;
}

// POUR LES SOLUTIONS : 
// Index du tableau = Ordre
// Valeur = Numéro de l'instance

////////////////////////////////////////////////////////////////////////////
std::vector<int> SMTWTP_initializer::random_solution()
////////////////////////////////////////////////////////////////////////////
{
 std::vector<int> solutions;

 for (int i = 0 ; i < get_instance_size() ; i++)
  solutions.push_back(i);

 std::random_device rd;
 std::mt19937 g(rd());

 std::shuffle(solutions.begin(), solutions.end(), g);

 return solutions;
}

////////////////////////////////////////////////////////////////////////////
std::vector<int> SMTWTP_initializer::EDD_solution
////////////////////////////////////////////////////////////////////////////
(
 Instance &instance
)
{
 std::vector<int> solutions;
 std::vector<std::pair<int, int>> sol_and_deadline;

 for (int i = 0 ; i < get_instance_size() ; i++)
  sol_and_deadline.push_back(std::pair<int, int>(i, instance.get_deadline(i)));
 
 // tri croissant
 std::sort(sol_and_deadline.begin(),
           sol_and_deadline.end(),
           [](std::pair<int, int> &p1, std::pair<int, int> &p2)
             { return p1.second < p2.second; });
 
 for (int i = 0 ; i < get_instance_size() ; i++)
  solutions.push_back(sol_and_deadline[i].first);
 
 return solutions;
}

////////////////////////////////////////////////////////////////////////////
std::vector<int> SMTWTP_initializer::MDD_solution
////////////////////////////////////////////////////////////////////////////
(
 Instance &instance
)
{
 std::vector<int> solutions;
 std::vector<int> removable;
 int current_date = 0;

 // Ajout des indexs
 for (int i = 0 ; i < get_instance_size() ; i++)
  removable.push_back(i);

 // Algo MDD
 for (int i = 0 ; i < get_instance_size() ; i++)
 {
  std::vector<std::pair<int, int>> mdd_values;

  // On recalcule pour les index restants, le MDD
  for (auto index: removable)
   mdd_values.push_back(std::pair<int, int>
                            (index,
                             std::max((current_date +
                                     instance.get_duration(index)),
                                     instance.get_deadline(index))));

  // On trie par ordre croissant
  std::sort(mdd_values.begin(),
            mdd_values.end(),
            [](std::pair<int, int> &p1, std::pair<int, int> &p2)
              { return p1.second < p2.second; });

  // On ajoute le premier et on fait les modifications générales
  current_date += instance.get_duration(mdd_values[0].first);
  solutions.push_back(mdd_values[0].first);
  removable.erase(std::remove(removable.begin(),
                              removable.end(),
                              mdd_values[0].first),
                  removable.end());
 }

 return solutions;
}

////////////////////////////////////////////////////////////////////////////
std::vector<int> SMTWTP_initializer::get_solution
////////////////////////////////////////////////////////////////////////////
(
 Instance &instance
)
{
 return get_initializer(instance);
}