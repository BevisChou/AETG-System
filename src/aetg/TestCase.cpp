#include "TestCase.hpp"

TestCase::TestCase(std::vector<uint32_t>&& vals, uint32_t num) : 
    values_(vals),
    num_of_new_combinations_(num) {}

TestCase::TestCase(TestCase&& test_case) :
    values_(std::move(test_case.values_)),
    num_of_new_combinations_(test_case.num_of_new_combinations_) {}

TestCase& TestCase::operator=(TestCase&& other)
{
    if(this != &other)
    {
        values_ = other.values_;
        num_of_new_combinations_ = other.num_of_new_combinations_;
    }
    return *this;
}

bool TestCase::operator<(const TestCase& test_case) const
{
    return num_of_new_combinations_ > test_case.num_of_new_combinations_;
}

const std::vector<uint32_t>& TestCase::values()
{
    return values_;
}

uint32_t TestCase::num_of_new_combinations()
{
    return num_of_new_combinations_;
}

std::ostream& operator<<(std::ostream& out, const TestCase& test_case)
{
    for(uint32_t i = 0; i < test_case.values_.size(); i++)
    {
        out << test_case.values_[i] << " ";
    }
    return out << test_case.num_of_new_combinations_;
}