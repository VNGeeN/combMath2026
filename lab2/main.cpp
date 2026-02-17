#include "team.hpp"
#include <iostream>
#include <string>

static void usage(const char* exe) {
    std::cerr
        << "Usage:\n"
        << "  " << exe << " <input_skills.txt> [output_result.txt]\n"
        << "Example:\n"
        << "  " << exe << " skills.txt result.txt\n";
}

int main(int argc, char** argv) {
    try {
        if (argc < 2) { usage(argv[0]); return 2; }

        const std::string input  = argv[1];
        const std::string output = (argc >= 3) ? argv[2] : "result.txt";

        auto p = readTeamProblem(input);
        auto r = solveMinimalTeamNextComb(p);

        std::cout << r.elapsed_seconds << "\n";
        writeTeamResult(output, r);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
