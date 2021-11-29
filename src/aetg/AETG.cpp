#include "AETG.hpp"

AETG::AETG(std::vector<uint32_t> param_spaces, uint32_t num_of_ways, uint32_t candidate_pool_size) :
    param_spaces_(param_spaces),
    num_of_ways_(num_of_ways),
    candidate_pool_size_(candidate_pool_size)
{
    if(param_spaces_.size() < num_of_ways_) throw std::exception();
    
    candidate_pool_.reserve(candidate_pool_size_);

    param_values_.reserve(param_spaces_.size());
    for(uint32_t i = 0, counter = 0; i < param_spaces_.size(); i++)
    {
        std::vector<uint32_t> values;
        values.reserve(param_spaces_[i]);
        for(uint32_t j = 0; j < param_spaces_[i]; j++)
        {
            values.emplace_back(counter++);
        }
        param_values_.emplace_back(std::move(values));
    }
}

std::vector<TestCase> AETG::generate_test_plan()
{
    std::vector<TestCase> result;
    std::set<std::set<uint32_t>> unincluded(std::move(get_combinations(param_values_, num_of_ways_)));
    
    while(unincluded.size() > 0)
    {
        TestCase test_case(std::move(generate_test_case(unincluded)));
        update_unincluded(unincluded, test_case.values());
        result.emplace_back(std::move(test_case));
    }
    return result;
}

TestCase AETG::generate_test_case(const std::set<std::set<uint32_t>>& unincluded)
{
    candidate_pool_.clear();
    std::set<uint32_t> partial_test_case = get_partial_test_case(unincluded);

    std::vector<std::future<TestCase>> futures;
    futures.reserve(candidate_pool_size_);

    for(uint32_t i = 0; i < candidate_pool_size_; i++)
    {
        auto future = std::async(std::launch::async, &AETG::generate_random_test_case, this, unincluded, partial_test_case);
        futures.emplace_back(std::move(future));
    }

    // Reference: https://stackoverflow.com/questions/36596398/vector-of-future-in-c11
    for(auto& future : futures)
    {
        candidate_pool_.emplace_back(std::move(future.get()));
    }

    std::sort(candidate_pool_.begin(), candidate_pool_.end());

    // std::cout << "[CONSOLE] Test case generated: " << candidate_pool_[0] << std::endl;

    return std::move(candidate_pool_[0]);
}

std::set<uint32_t> AETG::get_partial_test_case(const std::set<std::set<uint32_t>>& unincluded)
{
    UpdatableHeap<std::set<uint32_t>> heap;
    for(const auto& value_set : unincluded)
    {
        std::set<std::set<uint32_t>> combinations(std::move(get_combinations(value_set, num_of_ways_ - 1)));

        for(auto& combination : combinations)
        {
            heap.push_or_update(combination, 1);
        }
    }
    return heap.top();
}

TestCase AETG::generate_random_test_case(
    const std::set<std::set<uint32_t>>& unincluded, 
    const std::set<uint32_t>& partial_test_case)
{
    std::set<uint32_t> test_case(partial_test_case);
    uint32_t num_of_new_combinations = 0;

    std::vector<uint32_t> seq = std::move(get_random_seq(partial_test_case));

    for(uint32_t i = 0; i < seq.size(); i++)
    {
        std::set<std::set<uint32_t>> partial_test_cases(std::move(get_combinations(test_case, num_of_ways_ - 1)));

        std::vector<uint32_t> new_combinations_counts(param_spaces_[seq[i]], 0);
        for(uint32_t j = 0; j < param_spaces_[seq[i]]; j++)
        {
            for(auto& partial_combination : partial_test_cases)
            {
                std::set<uint32_t> combination(partial_combination);
                combination.insert(param_values_[seq[i]][j]);

                if(unincluded.find(combination) != unincluded.end())
                {
                    new_combinations_counts[j]++;
                }
            }
        }

        // Reference: https://stackoverflow.com/questions/2953491/finding-the-position-of-the-maximum-element
        uint32_t max_idx = std::distance(
            new_combinations_counts.begin(), 
            std::max_element(new_combinations_counts.begin(), new_combinations_counts.end()));

        num_of_new_combinations += new_combinations_counts[max_idx];
        test_case.insert(param_values_[seq[i]][max_idx]);
    }

    std::vector<uint32_t> test_case_vec(test_case.begin(), test_case.end());
    std::sort(test_case_vec.begin(), test_case_vec.end());

    return std::move(TestCase(std::move(test_case_vec), num_of_new_combinations));
}

std::vector<uint32_t> AETG::get_random_seq(const std::set<uint32_t>& partial_test_case)
{
    std::set<uint32_t> params;
    for(uint32_t i = 0; i < param_spaces_.size(); i++)
    {
        params.insert(i);
    }
    for(uint32_t value : partial_test_case)
    {
        params.erase(get_param_type(value));
    }

    // Reference: https://stackoverflow.com/questions/5034211/c-copy-set-to-vector
    std::vector<uint32_t> result(params.begin(), params.end());

    // Reference: https://stackoverflow.com/questions/6926433/how-to-shuffle-a-stdvector
    std::random_device rd; 
    std::default_random_engine rand(rd());
    std::shuffle(result.begin(), result.end(), rand);

    return result;
}

void AETG::update_unincluded(std::set<std::set<uint32_t>>& unincluded, const std::vector<uint32_t>& test_case)
{
    auto values = get_combinations(test_case, num_of_ways_);

    // Reference: https://stackoverflow.com/questions/16079362/how-to-compute-in-place-set-difference-of-two-multisets
    std::set<std::set<uint32_t>> result;

    std::set_difference(
        std::make_move_iterator(unincluded.begin()), 
        std::make_move_iterator(unincluded.end()), 
        values.begin(),
        values.end(), 
        std::inserter(result, result.begin()));
    unincluded.swap(result);
}

uint32_t AETG::get_param_type(uint32_t value)
{
    uint32_t result = 0;
    while(value >= param_spaces_[result])
    {
        value -= param_spaces_[result++];
    }
    return result;
}

std::set<std::set<uint32_t>> get_combinations(const std::vector<std::vector<uint32_t>>& values, uint32_t num_of_ways)
{
    if(values.size() < num_of_ways) throw std::exception();

    std::vector<uint32_t> types;
    types.reserve(values.size());
    for(uint32_t i = 0; i < values.size(); i++)
    {
        types.emplace_back(i);
    }
    auto combinations = get_combinations(types, num_of_ways);
    
    std::set<std::set<uint32_t>> result;
    for(auto& combination : combinations)
    {
        set_union(result, fill_combination(combination, values));
    }
    return result;
}

std::set<std::set<uint32_t>> get_combinations(const std::vector<uint32_t>& values, uint32_t num_of_ways)
{
    if(values.size() < num_of_ways) throw std::exception();

    std::set<std::set<uint32_t>> result;

    // Reference: https://stackoverflow.com/questions/9430568/generating-combinations-in-c

    std::vector<bool> selection_array(values.size());
    std::fill(selection_array.begin(), selection_array.begin() + num_of_ways, true);

    // Reference: https://stackoverflow.com/questions/20052674/how-to-convert-vector-to-set
    std::vector<uint32_t> combination_vec;
    combination_vec.reserve(num_of_ways);
    do 
    {
        combination_vec.clear();
        for (int i = 0; i < values.size(); i++)
        {
            if(selection_array[i])
            {
                combination_vec.emplace_back(values[i]);
            }     
        }
        result.insert(std::move(std::set<uint32_t>(
            std::make_move_iterator(combination_vec.begin()),
            std::make_move_iterator(combination_vec.end()))));

    } 
    while (std::prev_permutation(selection_array.begin(), selection_array.end()));

    return result;
}

std::set<std::set<uint32_t>> get_combinations(const std::set<uint32_t>& values, uint32_t num_of_ways)
{
    try
    {
        return get_combinations(std::vector<uint32_t>(values.begin(), values.end()), num_of_ways);
    }
    catch(const std::exception& e)
    {
        throw e;
    }
}

std::set<std::set<uint32_t>> fill_combination(
    const std::set<uint32_t>& type_combination, 
    const std::vector<std::vector<uint32_t>>& type_candidates)
{
    std::set<std::set<uint32_t>> result;
    std::vector<uint32_t> type_seq(type_combination.begin(), type_combination.end());
    std::vector<uint32_t> selection_arr(type_combination.size());

    emit_combination(result, selection_arr, 0, type_seq, type_candidates);
    return result;
}

void emit_combination(
    std::set<std::set<uint32_t>>& result_set, 
    std::vector<uint32_t>& selection_array, 
    uint32_t current_index, 
    const std::vector<uint32_t>& type_sequence,
    const std::vector<std::vector<uint32_t>>& candidates)
{
    if(current_index == selection_array.size())
    {
        std::vector<uint32_t> combination_vec;
        combination_vec.reserve(selection_array.size());
        for(uint32_t i = 0; i < selection_array.size(); i++)
        {
            combination_vec.emplace_back(candidates[type_sequence[i]][selection_array[i]]);
        }
        result_set.insert(std::move(std::set<uint32_t>(
            std::make_move_iterator(combination_vec.begin()),
            std::make_move_iterator(combination_vec.end()))));
    }
    else
    {
        for(uint32_t i = 0; i < candidates[type_sequence[current_index]].size(); i++)
        {
            selection_array[current_index] = i;
            emit_combination(result_set, selection_array, current_index + 1, type_sequence, candidates);
        }
    }
}