#include "field.h"
#include <cstring>
#include <iostream>

#ifndef INLINE_FIELD
	#include "field.inl"
#endif

MSfieldPart1::MSfieldPart1(const char * filename) : x_max(0), y_max(0), data(NULL), bust_counter(0), num_unknown_locations(0), num_unknown_mines(0) {
	const int MAX_LINE_SIZE =255;
	std::ifstream field_file(filename,std::ifstream::in);

	/* read - dimensions*/
	/* figure_out x_max */
	field_file.ignore(MAX_LINE_SIZE,'+');//skip till +
	char  s[MAX_LINE_SIZE];
	field_file.getline(s,MAX_LINE_SIZE,'+');
	x_max = std::strlen(s);
	//std::cout << "x_max = " << x_max << std::endl;
	field_file.ignore(MAX_LINE_SIZE,'\n');//skip till new line

	std::vector< char* > raw_data; //temporary storage for rows
	/* figure_out y_max */
	field_file.ignore(MAX_LINE_SIZE,'|');//skip to past "|"
	while ( !field_file.eof() ) {
		raw_data.push_back( new char[x_max+1] );
		field_file.getline(raw_data.back(),x_max+1,'|');
		field_file.ignore(MAX_LINE_SIZE,'|');//skip to next row
	}
	field_file.close();
	y_max = raw_data.size();
	//std::cout << "y_max = " << y_max << std::endl;

	//allocate real storage
	data = new int* [x_max];
	for (int x=0;x<x_max;++x) {
		data[x] = new int[y_max]; 
		for (int y=0;y<y_max;++y) data[x][y] = -10;  /*init*/
	}

	//read data from temporary storage into real
	for (int y=0;y<y_max;++y) { /*rows*/
		for (int x=0;x<x_max;++x) {
			switch (raw_data[y][x]) {
				case 'm': /*known mine*/         
					data[x][y] = -1; 
					break;
				case 'M': /*mine unknown to AI*/ 
					data[x][y] = -5; 
                    ++num_unknown_mines;
                    ++num_unknown_locations;
					break;
				case ' ': /*unknown content*/    
					data[x][y] = -10; 
                    ++num_unknown_locations;
					break;
				default : /* should be a 0-8 character */
					data[x][y] = raw_data[y][x] - 48;
					if ( data[x][y] <0 || data[x][y]>8) {
						std::cerr << "Illegal character in input " 
							<< raw_data[y][x] << " changing to unknown\n";
						data[x][y] = -10;
					}
					break;
			}
		}
	}
	std::vector< char*>::iterator b = raw_data.begin();
	std::vector< char*>::iterator e = raw_data.end();
	for ( ; b != e; ++b) { delete [] *b; }

    Check();
}


////////////////////////////////////////////////////////////////////////////////
void MSfieldPart1::Check() const {
    bool allright = true;
    for (int x=0;x<x_max;++x) {
        for (int y=0;y<y_max;++y) {
            if ( data[x][y] >=0 ) {
                int count = 0;
                for (int i=-1;i<2;++i) {
                    for (int j=-1;j<2;++j) { //8 neighbors around x,y
                        if ( (i || j) && CheckIsInside(x+i,y+j) ) {
                            if ( IsUnmarkedMine(x+i,y+j) || IsMarkedMine(x+i,y+j) ) {
                                ++count; 
                            }
                        }
                    }
                }
                if ( data[x][y] != count ) {
                    std::cerr << "Wrong count in ("<<x<<","<<y<<") is " << data[x][y] << " should be " << count << std::endl;
                    allright = false;
                }
            }
        }
    }
    if ( allright == false ) throw MSexception("Errors in the input");
}

////////////////////////////////////////////////////////////////////////////////
MSfieldPart1::~MSfieldPart1() {
	for (int x=0;x<x_max;++x) { delete [] data[x]; }
	delete [] data;
}

////////////////////////////////////////////////////////////
MSfieldPart1::MSfieldPart1(const MSfieldPart1& rhs) 
	: x_max(rhs.x_max), y_max(rhs.y_max),
    data(new int* [rhs.x_max]),
    bust_counter(rhs.bust_counter),
    num_unknown_locations(rhs.num_unknown_locations),
    num_unknown_mines(rhs.num_unknown_mines)
{
	for (int x=0;x<x_max;++x) {
		data[x] = new int[y_max]; 
		for (int y=0;y<y_max;++y) data[x][y] = rhs.data[x][y];  /*init*/
	}
}

////////////////////////////////////////////////////////////
MSfieldPart1& MSfieldPart1::operator=(const MSfieldPart1& rhs) {
	if (this == &rhs) return *this;

	for (int x=0;x<x_max;++x) { delete [] data[x]; }
	delete [] data;

	x_max = rhs.x_max;
	y_max = rhs.y_max;
	bust_counter = rhs.bust_counter;
    num_unknown_locations = rhs.num_unknown_locations;
    num_unknown_mines     = rhs.num_unknown_mines;

	data = new int* [x_max];
	for (int x=0;x<x_max;++x) {
		data[x] = new int[y_max]; 
		for (int y=0;y<y_max;++y) { 
            data[x][y] = rhs.data[x][y]; 
        }
	}
	return *this;
}

////////////////////////////////////////////////////////////
int MSfieldPart1::GetBustCounter() const { return bust_counter; }

////////////////////////////////////////////////////////////
int MSfieldPart1::OpenLocation_aux(int x, int y) {
//    if ( data[x][y] != -2 || data[x][y] != -10 ) {
//	    std::cout << "DEBUG OpenLocation_aux (" << x << "," << y << ") which is " << data[x][y] << "\n";
//    }
	
    if ( IsUnmarkedMine(x,y) ) { //mine
		++bust_counter;
		return -100; //busted
    }
   
    if ( IsUnknown( x,y ) || data[x][y] == -2 ) {
        --num_unknown_locations; 
    } else {
    }

	int count = 0;
	for (int i=-1;i<2;++i) {
		for (int j=-1;j<2;++j) { //8 neighbors around x,y
			if ( (i || j) && CheckIsInside(x+i,y+j) ) {
				if ( IsUnmarkedMine(x+i,y+j) || IsMarkedMine(x+i,y+j) ) {
					++count; 
				}
			}
		}
		data[x][y] = count;
	}
	return data[x][y];
}

////////////////////////////////////////////////////////////
void MSfieldPart1::MarkAsSafe(int x, int y) { //mark safe but do not open the location
	//std::cout << "Mark as Safe (" << x << "," << y << ")\n";
//    if ( data[x][y] != -10 ) {
//	    std::cout << "DEBUG Mark as Safe (" << x << "," << y << ") which is " << data[x][y] << "\n";
//    }
	data[x][y] = -2;
}

////////////////////////////////////////////////////////////
void MSfieldPart1::MarkAsMine(int x, int y) {
	//std::cout << "Mark as Mine (" << x << "," << y << ")\n";

//    if ( data[x][y] != -5 ) {
//	    std::cout << "DEBUG Mark as Mine (" << x << "," << y << ") which is " << data[x][y] << "\n";
//    }

    if ( IsUnknown( x,y ) ) {
        --num_unknown_mines;
        --num_unknown_locations;
    }
    data[x][y] = -1;
}

////////////////////////////////////////////////////////////
//prints a little bit more info compared to operator<<
void MSfieldPart1::DebugPrint(std::ostream& os) const {
	os << "Print in debug mode" << std::endl;
	os << "+";
	for (int x=0;x<4*x_max-1;++x) os << "-";
	os << "+\n";

	for (int y=0;y<y_max;++y) { /*rows*/
		for (int x=0;x<x_max;++x) {
			switch (data[x][y]) {
				case -1:  os << "| m ";                   break;
				case -2:  os << "| s ";                   break;
				case -5:  os << "| M ";                   break; 
				case -10: os << "|   ";                   break;
				default:  os << "| "<< data[x][y] << " "; break;
			}
		}
		os << "|\n";
	}
	os << "+";
	for (int x=0;x<4*x_max-1;++x) os << "-";
	os << "+\n";
    os << "Unknown locations " << num_unknown_locations
       << " unknown mines " << num_unknown_mines << std::endl;
}

////////////////////////////////////////////////////////////
int MSfieldPart1::OpenLocation(int , int ) {
	throw MSexception("OpenLocation - illegal to use in part 1");
}

#include <iomanip>
////////////////////////////////////////////////////////////
std::ostream & operator<<(std::ostream& os, const MSfieldPart1& msf) {
    unsigned witdh = 3;
    os << std::setw(witdh) << "y\\x";
    for (int x=0;x<msf.x_max;++x) {
        os << std::setw(witdh) << x;
    }
    os << std::endl;

	os << std::setw(witdh) << "+"; 
    for (unsigned x=0;x<witdh*msf.x_max;++x) os << "-"; 
	os << "+\n"; 

	for (int y=0;y<msf.y_max;++y) { /*rows*/
        os << std::setw(witdh-1) << y << "|";
		for (int x=0;x<msf.x_max;++x) {
			switch (msf.data[x][y]) {
                case -1:  os << std::setw(witdh) << " m"; break;
				case -2:  os << std::setw(witdh) << " s"; break;
				case -5:  os << std::setw(witdh) << "  "; break;
				case -10: os << std::setw(witdh) << "  "; break;
				default:  os << std::setw(witdh) << msf.data[x][y]; break;
			}
		}
		os << "|\n";
	}
	os << std::setw(witdh) << "+"; 
    for (unsigned x=0;x<witdh*msf.x_max;++x) os << "-"; 
	os << "+\n"; 

    os << "Unknown locations " << msf.num_unknown_locations
       << " unknown mines " << msf.num_unknown_mines << std::endl;
	return os;
}


////////////////////////////////////////////////////////////
MSfieldPart2::MSfieldPart2(const char * filename) : MSfieldPart1(filename) { }

////////////////////////////////////////////////////////////////////////////////
int MSfieldPart2::OpenLocation(int x, int y) {
	return OpenLocation_aux(x,y);
}

////////////////////////////////////////////////////////////////////////////////
bool MSfieldPart2::Validate( MSfieldPart2 const& original ) const { 
    //compare mines, check unassigned 
    if ( x_max != original.x_max || y_max != original.y_max ) {
        std::cerr << "Different sizes\n";
        return false;
    }

    bool allright = true;
    for (int x=0;x<x_max;++x) {
        for (int y=0;y<y_max;++y) {
            if ( data[x][y] >=0 ) {
                int count = 0;
                for (int i=-1;i<2;++i) {
                    for (int j=-1;j<2;++j) { //8 neighbors around x,y
                        if ( (i || j) && CheckIsInside(x+i,y+j) ) {
                            if ( IsUnmarkedMine(x+i,y+j) || IsMarkedMine(x+i,y+j) ) {
                                ++count; 
                            }
                        }
                    }
                }
                if ( data[x][y] != count ) {
                    std::cerr << "Wrong count in ("<<x<<","<<y<<") is " << data[x][y] << " should be " << count << std::endl;
                    allright = false;
                }
            } else if ( data[x][y] == -1 ) { //assigned mine
                if ( original.data[x][y] != -5 && original.data[x][y] != -1 ) {
                    std::cerr << "Wrong mine in ("<<x<<","<<y<<")" << std::endl;
                    allright = false;
                }
            } else if ( data[x][y] == -10 ) { //unassigned safe
                std::cerr << "Unassigned safe in ("<<x<<","<<y<<")" << std::endl;
                allright = false;
            } else if ( data[x][y] == -5 ) { //unassigned mine
                std::cerr << "Unassigned mine in ("<<x<<","<<y<<")" << std::endl;
                allright = false;
            }
        }
    }
    return allright;
}
