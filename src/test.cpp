#include <iostream>
#include "AETG.hpp"

const uint32_t CANDIDATE_POOL_SIZE = 50;

int main(){

    uint32_t num_of_param_types, num_of_ways;

    std::cin >> num_of_param_types;

    std::vector<uint32_t> param_spaces(num_of_param_types);

    for(uint32_t i = 0; i < num_of_param_types; i++)
    {
        std::cin >> param_spaces[i];
    }

    std::cin >> num_of_ways;

    AETG aetg(param_spaces, num_of_ways, CANDIDATE_POOL_SIZE);

    std::vector<TestCase> test_plan(std::move(aetg.generate_test_plan()));

    for(uint32_t i = 0; i < test_plan.size(); i++)
    {
        std::cout << test_plan[i] << std::endl;
    }

    return 0;
}