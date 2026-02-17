#pragma once
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

struct SalesmanResult
{
    long long min_cost = std::numeric_limits<long long>::max();
    std::vector<size_t> best_perm;
    double elapsed_seconds = 0.0;
    std::size_t permutations_checked = 0;
};

using SalesmanMatrix = std::vector<std::vector<size_t>>;

inline SalesmanMatrix readAdjacencyMatrix(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open input file: " + filename);

    SalesmanMatrix matrix;
    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::vector<size_t> row;
        long long value;
        while (ss >> value)
        {
            row.push_back(static_cast<size_t>(value));
        }
        if (!row.empty())
            matrix.push_back(std::move(row));
    }

    if (matrix.empty())
        throw std::runtime_error("Input matrix is empty: " + filename);

    const size_t n = matrix.size();
    for (const auto &row : matrix)
    {
        if (row.size() != n)
            throw std::runtime_error("Input matrix must be square (n x n): " + filename);
    }

    return matrix;
}

inline void writeSalesmanResult(const std::string &filename, const SalesmanResult &r)
{
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open output file: " + filename);

    file << "Minimum cost: " << r.min_cost << "\n";
    file << "Best Hamiltonian cycle: ";
    for (size_t i = 0; i < r.best_perm.size(); ++i)
    {
        file << r.best_perm[i];
        if (i + 1 < r.best_perm.size())
            file << " -> ";
    }
    if (!r.best_perm.empty())
        file << " -> " << r.best_perm.front();
    file << "\n";
    file << "Permutations checked: " << r.permutations_checked << "\n";
    file << "Elapsed time (s): " << r.elapsed_seconds << "\n";
}

inline SalesmanResult solveSalesmanNarayan(
    const SalesmanMatrix &A,
    size_t INF = (std::numeric_limits<size_t>::max() / 4))
{
    const size_t n = A.size();
    if (n == 0)
        throw std::runtime_error("Matrix size is 0.");
    for (const auto &row : A)
        if (row.size() != n)
            throw std::runtime_error("Matrix must be square (n x n).");

    std::vector<size_t> perm;
    perm.reserve(n);
    for (size_t i = 0; i < n; ++i)
        perm.push_back(i);

    // std::sort(perm.begin(), perm.end());

    SalesmanResult best;
    best.min_cost = INF;
    best.best_perm.clear();
    best.permutations_checked = 0;

    auto t0 = std::chrono::high_resolution_clock::now();

    do
    {
        ++best.permutations_checked;

        size_t cost = 0;
        bool ok = true;

        for (size_t i = 0; i + 1 < n; ++i)
        {
            const size_t from = perm[i];
            const size_t to = perm[i + 1];
            const size_t w = A[from][to];
            // if (w >= INF) { ok = false; break; }
            cost += w;
            if (cost >= static_cast<size_t>(best.min_cost))
            {
                ok = false;
                break;
            }
        }

        if (ok)
        {
            const size_t w = A[perm.back()][perm.front()];
            if (w >= INF)
            {
                ok = false;
            }
            else
            {
                cost += w;
            }

            if (ok && cost < static_cast<size_t>(best.min_cost))
            {
                best.min_cost = static_cast<long long>(cost);
                best.best_perm.resize(n);
                for (size_t i = 0; i < n; ++i)
                {
                    best.best_perm[i] = perm[i] + 1;
                }
            }
        }

    } while (std::next_permutation(perm.begin(), perm.end()));

    auto t1 = std::chrono::high_resolution_clock::now();
    best.elapsed_seconds = std::chrono::duration<double>(t1 - t0).count();

    return best;
}
