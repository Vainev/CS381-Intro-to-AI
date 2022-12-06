#ifndef FIELD_H
#define FIELD_H

#include <fstream>
#include <iostream>
#include <set>
#include <vector>

class MSexception : public std::exception {
	std::string msg;
	public:
	MSexception(const std::string & _msg) : msg(_msg) {}
	const char * what() const noexcept { return msg.c_str(); }
	virtual ~MSexception() {}
};
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

/* 
 *  UNKNOWN
 * -10 - unknown square 
 * -5 - mine in this square, but unknown to the AI
 *  KNOWN
 * -2 - safe location, but not open yet
 * -1 - mine in this square
 *  0,...,8 - open and visited
 *  */

/* Definition:
 * Safe location - you know there is no mine, but you haven't clicked on it yet
 */

/*
 * Part 1 is a static analyzer, so you may mark safe and mine, but not allowed to 
 * open locations (OpenLocation(...) will throw if you call it)
 */
class MSfieldPart1 {
    protected:
        int x_max, y_max;
        int ** data;
        int bust_counter; //number of times user steps on a mine (part2)
        int num_unknown_locations; 
        int num_unknown_mines;      //mines HAVE to be marked on the input field
    public:

        //the usual stuff
        MSfieldPart1(const char * filename);
        virtual ~MSfieldPart1();
        MSfieldPart1(const MSfieldPart1& rhs);
        MSfieldPart1& operator=(const MSfieldPart1& rhs);

        //simple getters
        int GetBustCounter() const;
        int GetMaxX() const;
        int GetMaxY() const;
        int TotalUnknownMines() const;
        int TotalUnknownLocations() const;

    public:
        //solving interface - read-only (part 1)
        //location is unknown to the client - unknown safe or unknown mine
        bool IsUnknown(int x, int y) const;
        //client already clicked on the square (so it contain the number of mines around it)
        bool IsClicked(int x, int y) const;
        //mine is marked by the user (or fiven as input)
        bool IsMarkedMine(int x, int y) const;

        //these are methods needed by Rules
        //UL(X)
        std::set<std::pair<int,int> > UnKnownLocations( int x, int y ) const;
        //Val(x)
        int  GetMineCount    ( int x, int y ) const;
        //KM(X)
        int  KnownMines      ( int x, int y ) const;
        bool CheckIsInside   ( int x, int y ) const;

        //methods to modify data (write) - part2 
        void MarkAsSafe(int x, int y);
        void MarkAsMine(int x, int y);
        //required only for part 2, in part 1 always throws
        virtual int OpenLocation(int x, int y);

        //prints a little bit more info compared to operator<<
        void DebugPrint(std::ostream& os) const;

        friend 
            std::ostream & operator<<(std::ostream& os, const MSfieldPart1& msf);

    protected:
        int  OpenLocation_aux(int x, int y);
        bool IsUnmarkedMine(int x, int y) const; 
        void Check() const; //input sanity check

};

std::ostream & operator<<(std::ostream& os, const MSfieldPart1& msf);

class MSfieldPart2 : public MSfieldPart1 {
	private:
		//MSfieldPart2 is NOT copiable/assignable
		//no cheating!!!
		MSfieldPart2(const MSfieldPart2 & rhs);
		MSfieldPart2& operator=(const MSfieldPart2 & rhs);
	public:
		MSfieldPart2(const char * filename);
		//returns a number of adjacent mines
		int OpenLocation(int x, int y);
        bool Validate( MSfieldPart2 const& original ) const;
};

#ifdef INLINE_FIELD
	#include "field.inl"
#endif

#undef INLINE
#endif
