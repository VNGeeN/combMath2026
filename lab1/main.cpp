#include "salesman.hpp"
#include <iostream>
#include <string>

static void usage(const char* exe) {
    std::cerr
        << "Usage:\n"
        << "  " << exe << " <input_matrix.txt> [output_result.txt]\n"
        << "Example:\n"
        << "  " << exe << " matrix.txt result.txt\n";
}

int main(int argc, char** argv) {
    try {
        if (argc < 2) { usage(argv[0]); return 2; }

        const std::string input  = argv[1];
        const std::string output = (argc >= 3) ? argv[2] : "result.txt";

        auto A = readAdjacencyMatrix(input);
        auto r = solveSalesmanBruteforceAllPermutationsNarayan(A);

        // For batch automation: print time only (seconds) to stdout
        std::cout << r.elapsed_seconds << "\n";

        writeSalesmanResult(output, r);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
