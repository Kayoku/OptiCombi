#include "SMTWTP_population.h"
#include <algorithm>
#include <random>

////////////////////////////////////////////////////////////////////////////
std::vector<long> SMTWTP_population::get_solution
////////////////////////////////////////////////////////////////////////////
(
 Instance &instance
)
{
 int cpt_generation = 0;

 // Initialisation des populations
 std::vector<std::vector<long>> sols;
 for (int i = 0 ; i < size_population-2 ; i++)
  sols.push_back(random_solution());
 sols.push_back(EDD_solution(instance));
 sols.push_back(MDD_solution(instance));

 populations.clear();
 for (auto s: sols)
  populations.push_back(Person(s, compute_cost(s, instance)));

 // On démarre les générations
 while (cpt_generation < nb_generation)
 {
  Population childs;

  // Reproduction
  for (int i = 0 ; i <(int) (g()%(size_population*2))+size_population/2 ; i++)
  {
   // On prend deux parents aléatoirement
   int parent1 = g()%populations.size();
   int parent2 = g()%populations.size();
   while (parent1 == parent2)
    parent2 = (g()+1)%populations.size();
 
   auto new_child = order_crossover(populations[parent1].first, populations[parent2].first);
   childs.push_back(Person(new_child, compute_cost(new_child, instance)));
  } 

  // Mutation
  for (int i = 0 ; i < rate_mutation ; i++) 
  {
   int parent = g()%populations.size();
   auto new_mutant = insert_mutation(populations[parent].first);
   childs.push_back(Person(new_mutant, compute_cost(new_mutant, instance))); 
  }
  
  // Elitiste pour l'évolution de la population
  populations = elitist(populations, childs);

  cpt_generation++;
 } 

 std::sort(populations.begin(), populations.end(), [&](const std::pair<std::vector<long>, float>& first,
                                               const std::pair<std::vector<long>, float>& second)
                                               { return first.second < second.second; });

 return populations[0].first;
}

////////////////////////////////////////////////////////////////////////////
Population SMTWTP_population::elitist
////////////////////////////////////////////////////////////////////////////
(
 Population parents,
 Population childs
)
{
 for (auto child : childs)
  parents.push_back(child);

 std::sort(parents.begin(), parents.end(), [&](const std::pair<std::vector<long>, float>& first,
                                               const std::pair<std::vector<long>, float>& second)
                                               { return first.second < second.second; });

 parents.resize(size_population);

 return parents;
}

////////////////////////////////////////////////////////////////////////////
std::vector<long> SMTWTP_population::swap_mutation
////////////////////////////////////////////////////////////////////////////
(
 std::vector<long> parent
)
{
 int size = parent.size();
 for (int i = 0 ; i < (int)(g()%intensity_mutation)+1 ; i++)
 {
  int id1 = g()%size;
  int id2 = g()%size;
  while (id1 == id2)
   id2 = (g()+1)%size;

  std::iter_swap(parent.begin()+id1, parent.begin()+id2);
 }

 return parent;
}

////////////////////////////////////////////////////////////////////////////
std::vector<long> SMTWTP_population::insert_mutation
////////////////////////////////////////////////////////////////////////////
(
 std::vector<long> parent
)
{
 int id1, id2;
 long erased_value;
 for (int i = 0 ; i < (int)(g()%intensity_mutation)+1 ; i++)
 {
  id1 = g()%parent.size();
  id2 = g()%(parent.size()-1);
  while (id2 == id1)
   id2 = (g()+1)%(parent.size()-1);
  erased_value = parent[id1];
  parent.erase(parent.begin()+id1);
  parent.insert(parent.begin()+id2, erased_value);
 }
 return parent;
}

////////////////////////////////////////////////////////////////////////////
std::vector<long> SMTWTP_population::order_crossover
////////////////////////////////////////////////////////////////////////////
(
 std::vector<long> parent1,
 std::vector<long> parent2
)
{
 std::vector<long> new_child(parent1.size(), 0);
 int offset = g()%25; 
 int window = g()%75;

 // 1. On copie la suite génétique du premier parent
 std::copy(parent1.begin()+offset, parent1.begin()+offset+window, new_child.begin()+offset); 

 // 2. On comble avec la suite de l'autre parent
 // 
 std::vector<long> gen_missing(parent1.size()-window, 0);
 std::copy(parent1.begin(), parent1.begin()+offset, gen_missing.begin());
 std::copy(parent1.begin()+offset+window, parent1.end(), gen_missing.begin()+offset);

 std::vector<std::pair<long, int>> indexed_missing;
 for (auto gen: gen_missing)
  indexed_missing.push_back(std::pair<long, int>(
                    gen, 
                    std::find(parent2.begin(), parent2.end(), gen)-parent2.begin()));

 // On trie puis on ajoute les gens manquant
 std::sort(indexed_missing.begin(), indexed_missing.end(),
           [&](const std::pair<long, int>& first, const std::pair<long, int>& second)
           { return first.second < second.second; });

 int ii = 0;
 for (int i = 0 ; i < (int)new_child.size() ; i++)
 {
  if (i == offset)
   i += window;

  new_child[i] = indexed_missing[ii].first;
  ii++;
 }

 return new_child;
}

////////////////////////////////////////////////////////////////////////////
std::string SMTWTP_population::get_name()
////////////////////////////////////////////////////////////////////////////
{
 std::string name = "GA-";
 name += "s" + std::to_string(size_population) + "-";
 name += "g" + std::to_string(nb_generation) + "-";
 name += "m" + std::to_string(rate_mutation) + "-";
 name += "i" + std::to_string(intensity_mutation);
 return name;
}
