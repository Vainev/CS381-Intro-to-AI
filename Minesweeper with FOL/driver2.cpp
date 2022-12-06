#include <iostream>
#include <set>
#include <ctime>
#include <cstdlib>
#include "field.h"
#include "analyzer.h"

int main() try {
	std::srand ( time (NULL) ) ;

	MSfieldPart2 f1   ("ms1");
    MSfieldPart2 copy1("ms1");
	Solver solver1(f1);
	solver1.Solve();
    std::cout << f1 << std::endl;
    if ( f1.Validate( copy1 ) ) { std::cout << "Solved OK\n"; } 
    else { std::cout << "Solved with errors or incompletely\n"; }

	MSfieldPart2 f2   ("ms2");
    MSfieldPart2 copy2("ms2");
	Solver solver2(f2);
	solver2.Solve();
    std::cout << f2 << std::endl;
    if ( f2.Validate( copy2 ) ) { std::cout << "Solved OK\n"; } 
    else { std::cout << "Solved with errors or incompletely\n"; }

	MSfieldPart2 f3   ("ms3");
    MSfieldPart2 copy3("ms3");
	Solver solver3(f3);
	solver3.Solve();
    std::cout << f3 << std::endl;
    if ( f3.Validate( copy3 ) ) { std::cout << "Solved OK\n"; } 
    else { std::cout << "Solved with errors or incompletely\n"; }
	
	MSfieldPart2 f4   ("ms4");
    MSfieldPart2 copy4("ms4");
	Solver solver4(f4);
	solver4.Solve();
    std::cout << f4 << std::endl;
    if ( f4.Validate( copy4 ) ) { std::cout << "Solved OK\n"; } 
    else { std::cout << "Solved with errors or incompletely\n"; }
	
	MSfieldPart2 f5   ("ms5");
    MSfieldPart2 copy5("ms5");
	Solver solver5(f5);
	solver5.Solve();
    std::cout << f5 << std::endl;
    if ( f5.Validate( copy5 ) ) { std::cout << "Solved OK\n"; } 
    else { std::cout << "Solved with errors or incompletely\n"; }
} catch ( MSexception& e) {
	std::cerr << e.what() << std::endl;
}
