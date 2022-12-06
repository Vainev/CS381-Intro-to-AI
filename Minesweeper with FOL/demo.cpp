#include <iostream>
#include <set>
#include "field.h"

int main() try {
	MSfieldPart1 f("ms3");
	std::cout << "original:\n" << f << std::endl;

	//mark mines - magically
	f.MarkAsMine(1,0);
	f.MarkAsMine(1,2);
	f.MarkAsSafe(2,0);
	f.MarkAsSafe(2,2);

	std::cout << "after marking \n" << f << std::endl;

	//checks
	int x=0, y=0;
	if ( f.CheckIsInside(x,y) )
		std::cout << "known mines around (" << x << "," << y << ") = " 
			<< f.KnownMines(x,y) << std::endl;
	x=0; y=1;
	if ( f.CheckIsInside(x,y) )
		std::cout << "known mines around (" << x << "," << y << ") = " 
			<< f.KnownMines(x,y) << std::endl;
	x=4; y=2;
	if ( f.CheckIsInside(x,y) )
		std::cout << "known mines around (" << x << "," << y << ") = " 
			<< f.KnownMines(x,y) << std::endl;

	x=0; y=0;
	if ( f.CheckIsInside(x,y) ) {
		std::set<std::pair<int,int> > UL = f.UnKnownLocations(x,y);
		std::set<std::pair<int,int> >::const_iterator b = UL.begin();
		std::set<std::pair<int,int> >::const_iterator e = UL.end();
		std::cout << "unknown location around (" << x << "," << y << ") =  { ";
		for( ; b!=e; ++b) {
			std::cout << "(" << b->first << "," << b->second << ")  ";
		}
		std::cout << "} " << std::endl;
	}

	x=4; y=1;
	if ( f.CheckIsInside(x,y) ) {
		std::set<std::pair<int,int> > UL = f.UnKnownLocations(x,y);
		std::set<std::pair<int,int> >::const_iterator b = UL.begin();
		std::set<std::pair<int,int> >::const_iterator e = UL.end();
		std::cout << "unknown location around (" << x << "," << y << ") =  { ";
		for( ; b!=e; ++b) {
			std::cout << "(" << b->first << "," << b->second << ")  ";
		}
		std::cout << "} " << std::endl;
	}

	//test part 2 
	MSfieldPart2 f3("ms3");
	std::cout << "original:\n" << f3 << std::endl;
	f3.DebugPrint(std::cout);
	std::cout << std::endl;
	x=0; y=2;
	if ( f3.CheckIsInside(x,y) ) {
		std::cout << "result of OpenLocation (" << x << "," << y << ") is "
			<< f3.OpenLocation(x,y) << std::endl;
	}
	x=1; y=0;
	if ( f3.CheckIsInside(x,y) ) {
		std::cout << "result of OpenLocation (" << x << "," << y << ") is "
			<< f3.OpenLocation(x,y) << std::endl;
	}
	x=2; y=0;
	if ( f3.CheckIsInside(x,y) ) {
		std::cout << "result of OpenLocation (" << x << "," << y << ") is "
			<< f3.OpenLocation(x,y) << std::endl;
	}
	std::cout << "after open \n" << f3 << std::endl;
	f3.DebugPrint(std::cout);
	std::cout << std::endl;
	
    //checks
    std::cout << "TotalUnknownLocations() = " << f3.TotalUnknownLocations() << std::endl;
    std::cout << "TotalUnknownMines() = " << f3.TotalUnknownMines() << std::endl;
	
} catch ( MSexception& e) {
	std::cerr << e.what() << std::endl;
}
