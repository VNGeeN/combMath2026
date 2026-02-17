#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../team.hpp"

TEST_CASE("Minimal team sample", "[team]")
{
    TeamProblem p;
    p.requiredSkills = {1, 2, 7};
    p.employees = {
        {1, 4},
        {2},
        {4, 7},
        {7, 8},
        {3, 5}
    };

    auto r = solveMinimalTeamNextComb(p);

    REQUIRE(r.feasible == true);
    REQUIRE(r.best_team_1based.size() == 2);

    // Возможный минимальный ответ: сотрудники (2,3) в 1-based: {2,3} => {1,2}? нет; лучше {2,3}? depends.
    // Проверим корректность по покрытию:
    // В тесте проще проверить, что решение покрывает все навыки:
    std::vector<std::size_t> state0based;
    for (auto x : r.best_team_1based) state0based.push_back(x - 1);
    REQUIRE(CoversAllSkills_ByState(state0based, p.employees, p.requiredSkills) == true);
}
