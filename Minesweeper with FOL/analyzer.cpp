/**
 * @file analyzer.cpp
 * @author Joanna Li (li.j@digipen.edu)
 * @brief 
 * 	This file contains the function implementatinns for the Solver and Analyzer classes.
 * @date 2022-11-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "analyzer.h"
#include "field.h"
#include <set>
#include <iostream>
#include <tuple>

/**
 * @brief Construct a new Analyzer:: Analyzer object
 * 
 * @param field 
 */
Analyzer::Analyzer(MSfieldPart1 & field) : field(field),temp(field) 
{
}

/**
 * @brief 
 * 	Find the set difference
 * 
 * @param mine 
 * 	Mine coordinates
 * @param safe 
 * 	Safe coordinates
 * @param ulX 
 * 	Original coordinates X
 * @param ulY 
 * 	Original coordinates Y
 */
void Analyzer::FindSetDiff(Coordinates &mine, Coordinates &safe, const Coordinates &ulX, const Coordinates &ulY)
{
	// ulx - uly
	CooIter found;
	for(CooIter it = ulY.begin(); it != ulY.end(); ++it)
	{
		found = mine.find(std::make_pair(it->first, it->second));

		if(found != mine.end())
		{
			mine.erase(found);
		}
	}

	// uly - ulx
	for(CooIter it = ulX.begin(); it != ulX.end(); ++it)
	{
		found = safe.find(std::make_pair(it->first, it->second));

		if(found != safe.end())
		{
			safe.erase(found);
		}
	}
}

/**
 * @brief Applies rule 1 and 2 
 * 
 * @param f 
 * 	Mine field
 * @param x 
 * 	X coord
 * @param y 
 * 	Y coord
 * @param open 
 * 	Can open location or not
 */
void Analyzer::ApplyRule12( MSfieldPart1 & f, int x, int y, bool open)
{
	/*  Rule 1:
	======
	if some location X satisfies
	Val(X) - KM(X) = | UL(X) |
	where |.| is the cardinality - number of elements in the set
	then all locations in UL(X) are mines*/

	// ul(x)
	Coordinates ul = f.UnKnownLocations(x, y);

	// val(x)
	int val = f.GetMineCount(x, y);

	int km = f.KnownMines(x, y);
	int size = ul.size();

	// all loc in ul are mines
	if(val - km == size)
	{
		// mark the locations as mines
		for(CooIter it = ul.begin(); it != ul.end(); ++it)
		{
			f.MarkAsMine((*it).first, (*it).second);
			++totalMarkedMines;
		}
	}

	// 	Rule 2:
	// ======
	// if some location X satisfies
	// Val(X) = KM(X) 
	// then all locations in UL(X) are safe

	// all locs in ul are safe
	if(val == km)
	{
		// mark the locations as mines
		for(CooIter it = ul.begin(); it != ul.end(); ++it)
		{
			f.MarkAsSafe((*it).first, (*it).second);
		}

		if(open)
		{
			// open the safe locations
			for (CooIter it = ul.begin(); it != ul.end(); ++it)
			{
				f.OpenLocation(it->first, it->second);
			}
		}
	}
}

/**
 * @brief Apply rule 3
 * 
 * @param f 
 * 	Mine field
 * @param x 
 * 	X coord
 * @param y 
 * 	Y coord
 * @param x2 
 * 	2nd location's x coord
 * @param y2 
 *  2nd location's y coord
 * @param open 
 * 	Can open location or not
 */
void Analyzer::ApplyRule3(MSfieldPart1 & f,  int x, int y, int x2, int y2, bool open)
{
	// 	Rule 3:
	// ======
	// if for 2 locations X and Y:
	// (Val(X) - KM(X) ) - ( Val(Y) - KM(Y) ) = | UL(X) \ UL(Y) |
	// where "\" is set difference
	// then
	// A) all locations in UL(X)\UL(Y) are mines
	// B) all locations in UL(Y)\UL(X) are safe

	// ul(x)
	Coordinates ulX = f.UnKnownLocations(x, y);

	// val(x)
	int valX = f.GetMineCount(x, y);
	int kmX = f.KnownMines(x, y);

	// ul(y)
	Coordinates ulY = f.UnKnownLocations(x2, y2);

	// val(y)
	int valY = f.GetMineCount(x2, y2);
	int kmY = f.KnownMines(x2, y2);

	// find the set differences
	Coordinates mine = ulX;
	Coordinates safe = ulY;

	FindSetDiff(mine, safe, ulX, ulY);

	size_t mineSize = mine.size();
	size_t safeSize = safe.size();
	size_t calc = (valX - kmX) - (valY - kmY);
	size_t calc2 = (valY - kmY) - (valX - kmX);

	if(calc == mineSize)
	{
		for(CooIter it = mine.begin(); it != mine.end(); ++it)
		{
			f.MarkAsMine(it->first, it->second);
			++totalMarkedMines;
		}

		for(CooIter it = safe.begin(); it != safe.end(); ++it)
		{
			f.MarkAsSafe(it->first, it->second);
		}

		if(open)
		{
			for (CooIter it = safe.begin(); it != safe.end(); ++it)
			{
				f.OpenLocation(it->first, it->second);
			}
		}
	}
	else if(calc2 == safeSize)
	{
		for(CooIter it = safe.begin(); it != safe.end(); ++it)
		{
			f.MarkAsMine(it->first, it->second);
			++totalMarkedMines;
		}

		for(CooIter it = mine.begin(); it != mine.end(); ++it)
		{
			f.MarkAsSafe(it->first, it->second);
		}

		if(open)
		{
			for (CooIter it = mine.begin(); it != mine.end(); ++it)
			{
				f.OpenLocation(it->first, it->second);
			}
		}
	}
}

/**
 * @brief Check for contradictions
 * 
 * @param f 
 * 	Mine field
 * @param x 
 * 	X coord
 * @param y 
 * 	Y coord
 * @return std::tuple<bool, bool> 
 */
std::tuple<bool, bool> Analyzer::Rule45(MSfieldPart1 & f, int x , int y)
{
	// Rule 4 (too many mines):
	// ======
	// Val(X) < KM(X) then assignment is impossible

	// Rule 5 (not enough mines):
	// ======
	// Val(X) > KM(X) + | UL(X) | then assignment is impossible
	
	Coordinates ul = f.UnKnownLocations(x, y);
	int val = f.GetMineCount(x, y);
	int km = f.KnownMines(x, y);

	int calc = km + static_cast<int>(ul.size());
	return std::make_tuple(val < km, val > calc);
}

/**
 * @brief Applies all the rules to mine field
 * 
 * @param f 
 * 	Mine field
 * @param open 
 * 	Can open loc or not
 * @return true 
 * @return false 
 */
bool Analyzer::ApplyRules(MSfieldPart1 & f, bool open)
{
	// get all the locs that were clicked
	std::vector<std::pair<int, int>> clickedPositions;
	for (int y = 0; y < f.GetMaxY(); ++y) 
	{
		for (int x = 0; x < f.GetMaxX(); ++x) 
		{
			if(f.IsClicked(x, y))
			{
				clickedPositions.push_back(std::make_pair(x, y));
			}
		}
	}

	int x1, y1;

	// apply rule 3
	for(int i = 0; i < static_cast<int>(clickedPositions.size()); ++i)
	{
		x1 = clickedPositions[i].first;
		y1 = clickedPositions[i].second;

		for(int j = -2; j < 3; ++j)
		{
			for(int k = -2; k < 3; ++k)
			{
				if((k || j) && f.CheckIsInside(x1 + j, y1 + k))
				{
					if(!f.IsUnknown(x1 + j, y1 + k) && !f.IsMarkedMine(x1 + j, y1 + k) && f.IsClicked(x1 + j, y1 + k))
					{
						ApplyRule3(f, x1, y1, x1 + j, y1 + k, open);
					}
				}
			}
		}


		// apply rule 1 and 2, then check for contradictions
		for(int j = 0; j < static_cast<int>(clickedPositions.size()); ++j)
		{
			ApplyRule12(f, clickedPositions[j].first, clickedPositions[j].second, open);
			std::tuple<bool, bool> ans = Rule45(f, clickedPositions[j].first, clickedPositions[j].second);

			if(std::get<0>(ans) || std::get<1>(ans))
			{
				return true;
			}
		}
	}

	return false;
}

/**
 * @brief 
 * 	Apply rules without checking for contradictions for part 2
 * 
 * @return int 
 * 	Number of unknown locations
 */
int Analyzer::ApplyRulesAux()
{
	// get all the locs that were clicked
	std::vector<std::pair<int, int>> clickedPositions;
	for (int y = 0; y < field.GetMaxY(); ++y) 
	{
		for (int x = 0; x < field.GetMaxX(); ++x) 
		{
			if(field.IsClicked(x, y))
			{
				clickedPositions.push_back(std::make_pair(x, y));
			}
		}
	}

	int x1, y1;
	for(int i = 0; i < static_cast<int>(clickedPositions.size()); ++i)
	{
		x1 = clickedPositions[i].first;
		y1 = clickedPositions[i].second;

		for(int j = -2; j < 3; ++j)
		{
			for(int k = -2; k < 3; ++k)
			{
				if((k || j) && field.CheckIsInside(x1 + j, y1 + k))
				{
					if(!field.IsUnknown(x1 + j, y1 + k) && !field.IsMarkedMine(x1 + j, y1 + k) && field.IsClicked(x1 + j, y1 + k))
					{
						ApplyRule3(field, x1, y1, x1 + j, y1 + k, true);
					}
				}
			}
		}

		ApplyRule12(field, clickedPositions[i].first, clickedPositions[i].second, true);
	}

	return field.TotalUnknownLocations();
}

/**
 * @brief Checks if loc given is a mine or not
 * 
 * @param x 
 * @param y 
 * @return true 
 * @return false 
 */
bool Analyzer::IsMine(int x,int y) 
{
	temp = field;
	temp.MarkAsSafe(x,y);
	return ApplyRules(temp, false);
}

/**
 * @brief Checks if loc given is safe or not
 * 
 * @param x 
 * @param y 
 * @return true 
 * @return false 
 */
bool Analyzer::IsSafe(int x, int y) 
{ 
	temp = field;
	temp.MarkAsMine(x,y);
	return ApplyRules(temp, false);
}

/**
 * @brief Construct a new Solver:: Solver object
 * 
 * @param field 
 */
Solver::Solver(MSfieldPart2 & field) : Analyzer(field) {}

/**
 * @brief 
 * 	Solves the minefield given
 * 
 */
void Solver::Solve() 
{
    //part 2
	int unknownCount = field.TotalUnknownLocations();
	int prevCount = -1;

	while(unknownCount != prevCount && unknownCount > 0)
	{
		prevCount = unknownCount;
		unknownCount  = ApplyRulesAux();

		if(unknownCount > 0 && prevCount == unknownCount)
		{
			unknownCount = SolveAux();
		}
	}
	
	if(field.TotalUnknownLocations() != 0 && field.TotalUnknownMines() == totalMarkedMines)
	{
		for(int i = 0; i < field.GetMaxX(); ++i)
		{
			for(int j = 0; j < field. GetMaxY(); ++j)
			{
				if(field.IsUnknown(i, j))
				{
					field.OpenLocation(i, j);
				}
			}
		}

		unknownCount = 0;
	}
}

/**
 * @brief 
 * 	Helps in solving the mine field
 * 
 * @return int 
 * 	Number of unknown locations
 */
int Solver::SolveAux() 
{
	for (int i = 0; i < field.GetMaxX(); ++i) 
	{
		for (int j = 0; j < field.GetMaxY(); ++j) 
		{
			if (field.IsUnknown(i, j)) 
			{
				Coordinates ul = field.UnKnownLocations(i, j);
				bool xCheck = (i == 0 || j == field.GetMaxY() - 1);
				bool yCheck = (j == 0 || i == field.GetMaxX() - 1);

				int size = ul.size();
				bool check = true;
				
				// determine if loc needs to be checked or not
				if(xCheck) 
				{
					if((yCheck) && size == 3)
					{
						check = false;
					}
					else if(size == 5)	
					{
						check = false;
					}
				}
				else if(yCheck) 
				{
					if ((xCheck) && size == 3)	
					{
						check = false;
					}
					else if(size == 5)
					{
						check = false;
					}
				}
				else 
				{
					if(size == 8)
					{
						check = false;
					}
				}

				if(check)
				{
					if(IsMine(i, j)) 
					{
						field.MarkAsMine(i, j);
						++totalMarkedMines;
						return field.TotalUnknownLocations();
					}
					if(IsSafe(i, j)) 
					{
						field.OpenLocation(i, j);
						return field.TotalUnknownLocations();
					}
				}
			}
		}
	}

	return field.TotalUnknownLocations();
}

