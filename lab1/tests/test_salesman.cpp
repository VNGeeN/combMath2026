#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../salesman.hpp"

TEST_CASE("Salesman: 4-node sample has min cost 80", "[salesman]") {
    std::vector<std::vector<long long>> A = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };

    auto r = solveSalesmanBruteforceAllPermutationsNarayan(A);

    REQUIRE(r.min_cost == 80);
    REQUIRE(r.best_perm.size() == 4);
    REQUIRE(r.permutations_checked == 24); // 4! = 24
}
