#include "field.h"
#include "analyzer.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

void test0() {
    MSfieldPart1 f2("ms2");
    //  012345678            012345678
    // +---------+          +---------+
    //0|121212121|         0|121212121|
    //1|         |         1|msmsmsmsm|
    // +---------+          +---------+
    std::cout << f2 << std::endl;

    //part 1
    Analyzer a2(f2);

    std::cout << ( ( a2.IsMine(0,1) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a2.IsMine(2,1) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a2.IsMine(4,1) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a2.IsMine(6,1) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a2.IsMine(8,1) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a2.IsSafe(1,1) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a2.IsSafe(3,1) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a2.IsSafe(5,1) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a2.IsSafe(7,1) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a2.IsSafe(0,1) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a2.IsSafe(2,1) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a2.IsSafe(4,1) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a2.IsSafe(6,1) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a2.IsSafe(8,1) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a2.IsMine(1,1) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a2.IsMine(3,1) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a2.IsMine(5,1) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a2.IsMine(7,1) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
}

void test1() {
    MSfieldPart1 f3("ms3");
    //            01234
    //+-----+    +-----+
    //|     |   0|smsms|
    //| 242 |   1|s242s|
    //|     |   2|smsms|
    //+-----+    +-----+
    std::cout << f3 << std::endl;

    //part 1
    Analyzer a3(f3);

    std::cout << ( ( a3.IsMine(1,0) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsMine(3,0) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsMine(1,2) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsMine(3,2) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsSafe(0,0) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsSafe(0,1) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsSafe(0,2) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsSafe(2,0) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsSafe(2,2) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsSafe(4,0) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsSafe(4,1) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsSafe(4,2) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsSafe(1,0) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsSafe(3,0) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsSafe(1,2) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsSafe(3,2) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsMine(0,0) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsMine(0,1) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsMine(0,2) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsMine(2,0) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsMine(2,2) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsMine(4,0) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsMine(4,1) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsMine(4,2) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
}

void test2() {
    MSfieldPart1 f5("ms5");
    //   012345
    //  +------+
    // 0|m     |
    // 1|2 MM  |
    // 2|2M M  |
    // 3|m 4M  |
    // 4|223M  |
    // 5|1M2   |
    // 6|3 3   |
    // 7|mm MMM|
    // 8|mm4m4 |
    // 9|m4323 |
    //10|2m11mM|
    //  +------+
    std::cout << f5 << std::endl;

    //part 1
    Analyzer a5(f5);

    //cannot prove neither mine nor safe for 3,2
    std::cout << ( ( a5.IsMine(3,2) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << ( ( a5.IsSafe(3,2) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    //can prove mine for 3,3
    std::cout << ( ( a5.IsMine(3,3) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << ( ( a5.IsSafe(3,3) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    //can prove mine for 3,4
    std::cout << ( ( a5.IsMine(3,4) ) ? "OK" : "Wrong" ) << std::endl;
    std::cout << ( ( a5.IsSafe(3,4) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
}


void test3() {
    MSfieldPart2 f1   ("ms1");
    MSfieldPart2 copy1("ms1");
    Solver solver1(f1);
    solver1.Solve();
    std::cout << f1 << std::endl;
    std::cout << "GetBustCounter() = " << f1.GetBustCounter() << std::endl;
    if ( f1.Validate( copy1 ) ) { std::cout << "Solved OK\n"; } 
    else { std::cout << "Solved with errors or incompletely\n"; }
}

void test4() {
    MSfieldPart2 f2   ("ms2");
    MSfieldPart2 copy2("ms2");
    Solver solver2(f2);
    solver2.Solve();
    std::cout << f2 << std::endl;
    std::cout << "GetBustCounter() = " << f2.GetBustCounter() << std::endl;
    if ( f2.Validate( copy2 ) ) { std::cout << "Solved OK\n"; } 
    else { std::cout << "Solved with errors or incompletely\n"; }
}

void test5() {
    MSfieldPart2 f3   ("ms3");
    MSfieldPart2 copy3("ms3");
    Solver solver3(f3);
    solver3.Solve();
    std::cout << f3 << std::endl;
    std::cout << "GetBustCounter() = " << f3.GetBustCounter() << std::endl;
    if ( f3.Validate( copy3 ) ) { std::cout << "Solved OK\n"; } 
    else { std::cout << "Solved with errors or incompletely\n"; }
}

void test6() {
    MSfieldPart2 f4   ("ms4");
    MSfieldPart2 copy4("ms4");
    Solver solver4(f4);
    solver4.Solve();
    std::cout << f4 << std::endl;
    std::cout << "GetBustCounter() = " << f4.GetBustCounter() << std::endl;
    if ( f4.Validate( copy4 ) ) { std::cout << "Solved OK\n"; } 
    else { std::cout << "Solved with errors or incompletely\n"; }
}

void test7() {
    MSfieldPart2 f5   ("ms5");
    MSfieldPart2 copy5("ms5");
    Solver solver5(f5);
    solver5.Solve();
    std::cout << f5 << std::endl;
    std::cout << "GetBustCounter() = " << f5.GetBustCounter() << std::endl;
    if ( f5.Validate( copy5 ) ) { std::cout << "Solved OK\n"; } 
    else { std::cout << "Solved with errors or incompletely\n"; }
}

void (*pTests[])(void) = { 
	test0, test1, test2, test3, test4, test5, test6, test7
};

#include <cstdio> /* sscanf */
int main(int argc, char *argv[] ) {
    std::srand ( time (NULL) ) ;
    if (argc >1) {
        int test = 0;
        std::sscanf(argv[1],"%i",&test);
        try {
            pTests[test]();
        } catch( const char* msg) {
            std::cerr << msg << std::endl;
        }
    }


    return 0;
}
