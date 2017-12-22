#include "SMTWTP_population.h"
#include <algorithm>

////////////////////////////////////////////////////////////////////////////
std::vector<long> SMTWTP_population::get_solution
////////////////////////////////////////////////////////////////////////////
(
 Instance &instance
)
{
 std::vector<std::vector<long>> sols;
 sols.push_back(random_solution());
 sols.push_back(EDD_solution(instance));
 sols.push_back(MDD_solution(instance));

 for (auto s: sols)
  populations.push_back(std::pair<std::vector<long>, float>(s, compute_cost(s, instance)));

}

////////////////////////////////////////////////////////////////////////////
std::string SMTWTP_population::get_name()
////////////////////////////////////////////////////////////////////////////
{
 return "Population";
}
