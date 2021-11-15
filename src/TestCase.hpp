#include <cstdint>
#include <vector>
#include <iostream>

class TestCase{
public:
    TestCase(std::vector<uint32_t>&&, uint32_t);
    TestCase(TestCase&&);
    TestCase& operator=(TestCase&&);
    bool operator<(const TestCase&) const;
    const std::vector<uint32_t>& values();
    uint32_t num_of_new_combinations();
    friend std::ostream& operator<<(std::ostream& out, const TestCase&);
private:
    std::vector<uint32_t> values_;
    uint32_t num_of_new_combinations_;
};

// Reference: https://stackoverflow.com/questions/1380463/sorting-a-vector-of-custom-objects

// Reference: https://stackoverflow.com/questions/66763732/cpp-use-of-deleted-function
// Reference: https://docs.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-170