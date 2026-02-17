#pragma once
#include <chrono>
#include <cstddef>
#include <fstream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <unordered_set>

struct TeamProblem
{
    std::vector<int> requiredSkills;
    std::vector<std::vector<int>> employees; // employees[i] = список навыков сотрудника i
};

struct TeamResult
{
    bool feasible = false;
    std::vector<std::size_t> best_team_1based; // как в образце: выводим 1-based
    std::size_t combinations_checked = 0;
    double elapsed_seconds = 0.0;
};

// Формат:
// n m
// r1..rm
// затем n строк: ti s1..sti
inline TeamProblem readTeamProblem(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open input file: " + filename);

    std::size_t n = 0, m = 0;
    if (!(file >> n >> m))
        throw std::runtime_error("Bad header (expected: n m): " + filename);

    TeamProblem p;
    p.requiredSkills.resize(m);
    for (std::size_t i = 0; i < m; ++i)
    {
        if (!(file >> p.requiredSkills[i]))
            throw std::runtime_error("Bad required skills line (not enough skills): " + filename);
    }

    p.employees.resize(n);
    for (std::size_t i = 0; i < n; ++i)
    {
        std::size_t t = 0;
        if (!(file >> t))
            throw std::runtime_error("Bad employee line (expected skill count): " + filename);

        p.employees[i].resize(t);
        for (std::size_t j = 0; j < t; ++j)
        {
            if (!(file >> p.employees[i][j]))
                throw std::runtime_error("Bad employee skills (not enough numbers): " + filename);
        }
    }

    return p;
}

inline void writeTeamResult(const std::string& filename, const TeamResult& r)
{
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open output file: " + filename);

    if (!r.feasible)
    {
        file << "No feasible team\n";
        file << "Combinations checked: " << r.combinations_checked << "\n";
        file << "Elapsed time (s): " << r.elapsed_seconds << "\n";
        return;
    }

    file << "Minimum team size: " << r.best_team_1based.size() << "\n";
    file << "Team (1-based): ";
    for (std::size_t i = 0; i < r.best_team_1based.size(); ++i)
    {
        file << r.best_team_1based[i];
        if (i + 1 < r.best_team_1based.size())
            file << " ";
    }
    file << "\n";
    file << "Combinations checked: " << r.combinations_checked << "\n";
    file << "Elapsed time (s): " << r.elapsed_seconds << "\n";
}

inline bool NextCombinations(std::size_t dim, std::vector<std::size_t>& state)
{
    const std::size_t k = state.size();
    if (k == 0 || k > dim) return false;

    for (std::size_t i = k; i-- > 0; )
    {
        const std::size_t maxAllowed = dim - k + i;
        if (state[i] < maxAllowed)
        {
            ++state[i];
            for (std::size_t j = i + 1; j < k; ++j)
                state[j] = state[j - 1] + 1;
            return true;
        }
    }
    return false;
}

inline bool CoversAllSkills_ByState(
    const std::vector<std::size_t>& state,
    const std::vector<std::vector<int>>& employees,
    const std::vector<int>& requiredSkills)
{
    if (requiredSkills.empty()) return true;

    std::unordered_set<int> remaining(requiredSkills.begin(), requiredSkills.end());

    for (std::size_t empIdx : state)
    {
        for (int skill : employees[empIdx])
        {
            remaining.erase(skill);
            if (remaining.empty())
                return true;
        }
    }
    return remaining.empty();
}

inline TeamResult solveMinimalTeamNextComb(const TeamProblem& p)
{
    const std::size_t n = p.employees.size();

    TeamResult res;
    res.feasible = false;
    res.best_team_1based.clear();
    res.combinations_checked = 0;

    auto t0 = std::chrono::high_resolution_clock::now();

    // Если требований нет — считаем, что пустая команда допустима
    if (p.requiredSkills.empty())
    {
        res.feasible = true;
        // best_team_1based пустой
        auto t1 = std::chrono::high_resolution_clock::now();
        res.elapsed_seconds = std::chrono::duration<double>(t1 - t0).count();
        return res;
    }

    // Перебор размеров команды
    for (std::size_t k = 1; k <= n; ++k)
    {
        std::vector<std::size_t> state(k);
        for (std::size_t i = 0; i < k; ++i)
            state[i] = i;

        do
        {
            ++res.combinations_checked;

            if (CoversAllSkills_ByState(state, p.employees, p.requiredSkills))
            {
                res.feasible = true;
                res.best_team_1based.resize(k);
                for (std::size_t i = 0; i < k; ++i)
                    res.best_team_1based[i] = state[i] + 1; 

                auto t1 = std::chrono::high_resolution_clock::now();
                res.elapsed_seconds = std::chrono::duration<double>(t1 - t0).count();
                return res; 
            }
        }
        while (NextCombinations(n, state));
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    res.elapsed_seconds = std::chrono::duration<double>(t1 - t0).count();
    return res;
}
