//30 × 16 field with 99 mines

#include <iostream>
#include <ctime>
#include <cstdlib>

int main() {
    int const v=30;
    int const h=16;
    int const mines_max=99;
    bool f[h][v] = { {false} };

    //mines
    int m = 0;
    std::srand ( std::time( NULL ) );
    while ( m<mines_max ) {
        int rand_h = std::rand()%h;
        int rand_v = std::rand()%v;
        if ( f[rand_h][rand_v] == false ) {
            f[rand_h][rand_v] = true;
            ++m;
        }
    }

    //print
    std::cout << '+';
    for ( int j=0;j<v; ++j ) { std::cout << '-'; }
    std::cout << '+' << std::endl;

    for ( int i=0;i<h; ++i ) {
        std::cout << '|';
        for ( int j=0;j<v; ++j ) {
            std::cout << (f[i][j]?'M':' ');
        }
        std::cout << '|' << std::endl;
    }

    std::cout << '+';
    for ( int j=0;j<v; ++j ) { std::cout << '-'; }
    std::cout << '+' << std::endl;
}
