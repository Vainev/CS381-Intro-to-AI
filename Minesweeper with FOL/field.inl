#ifdef INLINE_FIELD
	#define INLINE inline
#else   
	#define INLINE 
#endif

////////////////////////////////////////////////////////////
INLINE int MSfieldPart1::TotalUnknownMines() const { return num_unknown_mines; }
////////////////////////////////////////////////////////////
INLINE int MSfieldPart1::TotalUnknownLocations() const { return num_unknown_locations; }
////////////////////////////////////////////////////////////
INLINE int MSfieldPart1::GetMaxX() const { return x_max; }

////////////////////////////////////////////////////////////
INLINE int MSfieldPart1::GetMaxY() const { return y_max; }

////////////////////////////////////////////////////////////
INLINE bool MSfieldPart1::IsMarkedMine(int x, int y) const {
    if ( !CheckIsInside(x,y) )
		throw MSexception("IsMarkedMine - location is outside the grid");
	return data[x][y] == -1; 
}

////////////////////////////////////////////////////////////
//this function may be used by class and derivatives ONLY
//return true if there is a mine (even if it's unknown to AI)
INLINE bool MSfieldPart1::IsUnmarkedMine(int x, int y) const { 
	// own
//std::cout << x << ", " << y << " = " << data[x][y] << std::endl;
	return data[x][y] == -1 || data[x][y] == -5; 
}

////////////////////////////////////////////////////////////
//false iff location is open or marked as mine
//true otherwise
INLINE bool MSfieldPart1::IsUnknown(int x, int y) const { 
	
// own
//std::cout << x << ", " << y << " = " << data[x][y] << std::endl;

	return ( data[x][y] == -5 || data[x][y] == -10 ); 
}

////////////////////////////////////////////////////////////
INLINE bool MSfieldPart1::IsClicked(int x, int y) const { 
	return data[x][y] >= 0; 
}

////////////////////////////////////////////////////////////
INLINE int MSfieldPart1::GetMineCount(int x, int y) const {
	if ( data[x][y] >= 0 ) return data[x][y];
	else { //this square wasn't open or is a mine, 
		//so this is an illegal question
		throw 
			MSexception("GetMineCount - no mine count for unknown and/or mine locations");
	}
}

////////////////////////////////////////////////////////////
INLINE std::set<std::pair<int,int> > MSfieldPart1::UnKnownLocations(int x, int y) const {
	std::set<std::pair<int,int> > result;
	for (int i=-1;i<2;++i) {
		for (int j=-1;j<2;++j) {
			if ( (i || j) && x+i<x_max && x+i>-1 && y+j<y_max && y+j>-1 ) {
				if ( IsUnknown( x+i,y+j ) ) {
					result.insert (std::make_pair<int,int>(x+i,y+j));
				}
			}
		} 
	}
	return result;
}


////////////////////////////////////////////////////////////
INLINE bool MSfieldPart1::CheckIsInside(int x,int y) const {
	return ( x<x_max && x>-1 && y<y_max && y>-1 );
}

////////////////////////////////////////////////////////////
INLINE int MSfieldPart1::KnownMines(int x, int y) const {
	if ( !CheckIsInside(x,y) )
		throw MSexception("GetKnownMines - location is outside the grid");
	int count = 0;
	for (int i=-1;i<2;++i) {
		for (int j=-1;j<2;++j) {
			if ( (i || j) 
					&& x+i<x_max && x+i>-1 
					&& y+j<y_max && y+j>-1 ) 
			{
				if ( IsMarkedMine( x+i,y+j ) ) { ++count; }
			}
		} 
	}
	return count;
}
