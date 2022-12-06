/**
 * @file analyzer.h
 * @author Joanna Li (li.j@digipen.edu)
 * @brief 
 * 	This file contains the class Solver and Analyzer and their funciton decalrations to solve
 *  minesweeper.
 * @date 2022-11-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef ANALYZER_H
#define ANALYZER_H
#include "field.h"
#include <iostream>

using Coordinates = std::set<std::pair<int,int> >;
using CooIter = std::set<std::pair<int, int>>::iterator;

////////////////////////////////////////////////////////////////////////////////
class Analyzer 
{
	protected:
		void ApplyRule12( MSfieldPart1 & f, int x, int y, bool open);
		void ApplyRule3 ( MSfieldPart1 & f,  int x, int y, int x2, int y2, bool open);
		int ApplyRulesAux();

		MSfieldPart1 & field;           //actual field
		MSfieldPart1 temp;              //temporary field for proof by contradiction
		int totalMarkedMines = 0;
	public:
		Analyzer(MSfieldPart1 & field);
		virtual ~Analyzer() {}
		bool IsMine(int x,int y);  //resolution proof by contradiction
		bool IsSafe(int x, int y); //resolution proof by contradiction


	private:
		void FindSetDiff(Coordinates &mine, Coordinates &safe, const Coordinates &ulX, const Coordinates &ulY);
		bool ApplyRules(MSfieldPart1 & f, bool open);
		std::tuple<bool, bool> Rule45(MSfieldPart1 & f, int x , int y);
};

////////////////////////////////////////////////////////////////////////////////
class Solver : public Analyzer 
{
	private:
		int SolveAux();
	public:
		Solver(MSfieldPart2 & field);
        void Solve( );
};
#endif
