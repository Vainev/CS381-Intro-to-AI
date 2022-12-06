#include <iostream>
#include <set>
#include <ctime>
#include <cstdlib> //srand
#include "field.h"
#include "analyzer.h"

int main(int argc, char** argv) try {
    if (argc < 2) { std::cerr << "Need a single argument\n"; return -1; }

    MSfieldPart2 f(argv[1]);
    MSfieldPart2 copy(argv[1]);

    Solver solver(f);
    solver.Solve();
    std::cout << f;
    std::cout << "GetBustCounter " << f.GetBustCounter() << std::endl;

    if ( f.Validate( copy ) ) { std::cout << "Solved OK\n"; } 
    else { std::cout << "Solved with errors or incompletely\n"; }

} catch ( MSexception& e) {
	std::cerr << e.what() << std::endl;
}
