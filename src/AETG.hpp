#include <cstdint>
#include <vector>
#include <set>
#include <future>
#include <random>
#include <algorithm>

#include "UpdatableHeap.hpp"
#include "TestCase.hpp"

class AETG{
public:
    AETG(std::vector<uint32_t>, uint32_t, uint32_t);
    std::vector<TestCase> generate_test_plan();
private:
    TestCase generate_test_case(const std::set<std::set<uint32_t>>&);
    std::set<uint32_t> get_partial_test_case(const std::set<std::set<uint32_t>>&);
    TestCase generate_random_test_case(const std::set<std::set<uint32_t>>&, const std::set<uint32_t>&);
    std::vector<uint32_t> get_random_seq(const std::set<uint32_t>&);
    void update_unincluded(std::set<std::set<uint32_t>>&, const std::vector<uint32_t>&);
    uint32_t get_param_type(uint32_t);

    std::vector<uint32_t> param_spaces_;
    std::vector<std::vector<uint32_t>> param_values_;
    uint32_t num_of_ways_;
    uint32_t candidate_pool_size_;
    std::vector<TestCase> candidate_pool_;
};

template<typename T>
void set_union(std::set<T>&, const std::set<T>&);

std::set<std::set<uint32_t>> get_combinations(const std::vector<std::vector<uint32_t>>&, uint32_t);
std::set<std::set<uint32_t>> get_combinations(const std::vector<uint32_t>&, uint32_t);
std::set<std::set<uint32_t>> get_combinations(const std::set<uint32_t>&, uint32_t);

std::set<std::set<uint32_t>> fill_combination(const std::set<uint32_t>&, const std::vector<std::vector<uint32_t>>&);
void emit_combination(std::set<std::set<uint32_t>>&, std::vector<uint32_t>&, uint32_t, const std::vector<uint32_t>&, const std::vector<std::vector<uint32_t>>&);

template<typename T>
void set_union(std::set<T>& base_set, const std::set<T>& other_set)
{
    // Reference: // Reference: https://stackoverflow.com/questions/5211806/whats-the-difference-between-stdmerge-and-stdset-union
    std::set<T> result;
    std::set_union(base_set.begin(), base_set.end(),
        other_set.begin(), other_set.end(),                  
        std::inserter(result, result.begin()));
    base_set.swap(result);
}