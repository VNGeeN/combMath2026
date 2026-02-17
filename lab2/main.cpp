#include <iostream>
#include <vector>
#include <unordered_set>
#include <limits>

// Генерация следующего сочетания в лексикографическом порядке.
// state — строго возрастающие индексы из [0..dim-1].
// Возвращает true, если следующее сочетание существует.
bool NextCombinations(std::size_t dim, std::vector<std::size_t>& state)
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

// Проверка: покрывает ли выбранная подгруппа (state) все требуемые навыки.
bool CoversAllSkills_ByState(
    const std::vector<std::size_t>& state,
    const std::vector<std::vector<int>>& employees,
    const std::vector<int>& requiredSkills)
{
    if (requiredSkills.empty()) return true;

    std::unordered_set<int> remaining(requiredSkills.begin(), requiredSkills.end());

    for (std::size_t empIdx : state)
    {
        // empIdx — индекс сотрудника
        for (int skill : employees[empIdx])
        {
            remaining.erase(skill);
            if (remaining.empty())
                return true; // ранний выход
        }
    }
    return remaining.empty();
}

// Возвращает минимальную команду индексов сотрудников.
// Если решения нет — возвращает пустой вектор.
std::vector<std::size_t> FindMinimalTeam(
    const std::vector<std::vector<int>>& employees,
    const std::vector<int>& requiredSkills)
{
    const std::size_t n = employees.size();
    if (requiredSkills.empty()) return {}; // "ничего не требуется" -> можно трактовать как пустая команда

    // Перебираем размер команды k от 1 до n
    for (std::size_t k = 1; k <= n; ++k)
    {
        // Начальное сочетание: [0,1,2,...,k-1]
        std::vector<std::size_t> state(k);
        for (std::size_t i = 0; i < k; ++i)
            state[i] = i;

        do
        {
            if (CoversAllSkills_ByState(state, employees, requiredSkills))
                return state; // это минимальный k, значит решение оптимально по размеру
        }
        while (NextCombinations(n, state));
    }

    return {}; // решения нет
}

int main()
{
    // ====== Пример данных ======
    // Требуемые навыки проекта:
    std::vector<int> requiredSkills = {1, 2, 7};

    // employees[i] — навыки i-го сотрудника
    std::vector<std::vector<int>> employees = {
        {1, 4},    // сотрудник 0
        {2},       // сотрудник 1
        {4, 7},    // сотрудник 2
        {7, 8},    // сотрудник 3
        {3, 5}     // сотрудник 4
    };

    // ====== Решение ======
    auto team = FindMinimalTeam(employees, requiredSkills);

    if (team.empty())
    {
        std::cout << "Решение не найдено: требуемые навыки нельзя покрыть имеющимися сотрудниками.\n";
        return 0;
    }

    std::cout << "Минимальная команда (0-based индексы): ";
    for (std::size_t idx : team) std::cout << idx << " ";
    std::cout << "\n";

    std::cout << "Минимальная команда (1-based индексы): ";
    for (std::size_t idx : team) std::cout << (idx + 1) << " ";
    std::cout << "\n";

    return 0;
}