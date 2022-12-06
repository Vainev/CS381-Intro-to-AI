/**
 * @file csp.cpp
 * @author Joanna Li (li.j@digipen.edu)
 * @brief 
 * 	This file contains the function implementattions for the csp class. 
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifdef INLINE_CSP
	//#warning "INFO - inlining CSP methods"
	#define INLINE inline
#else   
	//#warning "INFO - NOT inlining CSP methods"
	#define INLINE 
#endif

/**
 * @brief Construct a new CSP<T>::CSP object
 * 
 * @tparam T 
 * @param cg 
 */
template <typename T> 
CSP<T>::CSP(T &cg) :
	arc_consistency(),
	cg(cg),
	solution_counter(0),
	recursive_call_counter(0),
	iteration_counter(0) 
{
}

/**
 * @brief CSP solver brute force, no forward checking, backtracking
 * 
 * @param level 
 * @return true, solution found
 * @return false, solution not found
 */
template <typename T> 
bool CSP<T>::SolveDFS(unsigned level) 
{
	++recursive_call_counter;

	// Will happen when we call solve for a non existing level or
	// when all variables are assigned without any issues (have solution to the problem)
	if(cg.AllVariablesAssigned())
	{
		return true;
	}

	Variable* varToAssign = NULL;
	varToAssign = MinRemVal();

	// get all constraints of the variable
	const std::vector<const Constraint*> & allConstraints = cg.GetConstraints(varToAssign);
	typename std::vector<const Constraint*>::const_iterator bConst = allConstraints.begin();
	typename std::vector<const Constraint*>::const_iterator eConst = allConstraints.end();

	// get all the domains of the variable
	const std::set<Value>& varDomain = varToAssign->GetDomain();
	typename std::set<Value> originalDomain = varDomain;

	// while there is stil values left for this variable (domain != 0)
	while(!varToAssign->IsImpossible())
	{
		++iteration_counter;

		// assign random value from domain
		varToAssign->Assign();

		bool allSatisfied = true;

		// check if assignment satisfies all constraint
		bConst = allConstraints.begin();
		for(; (bConst != eConst) && allSatisfied; ++bConst)
		{
			allSatisfied &= (*bConst)->Satisfiable();
		}

		// check to see if its not satisfied or next level cant be solved (recusrive call)
		if(!allSatisfied || !SolveDFS(level + 1))
		{
			// remove the value assigned (delete from original)
			// and try with new value on next iteration
			varToAssign->RemoveValue(varToAssign->GetValue());
			varToAssign->UnAssign();
			continue;
		}
		
		return true;
	}

	// if lvl = 0 then that means the first varibale took all the values and children 
	// checked all values
	if(level != 0)
	{
		// reset domains (rolling back)
		varToAssign->SetDomain(originalDomain);
	}

	return false;
}

/**
 * @brief 
 * 	CSP solver, uses forward checking
 * @param level 
 * @return true, solution found
 * @return false, solution not found
 */
template <typename T> 
bool CSP<T>::SolveFC(unsigned level) 
{	++recursive_call_counter;

	// Will happen when we call solve for a non existing level or
	// when all variables are assigned without any issues (have solution to the problem)
	if(cg.AllVariablesAssigned())
	{
		return true;
	}

	Variable* varToAssign = NULL;
	varToAssign = MinRemVal();

	// get all constraints of the variable
	const std::vector<const Constraint*> & allConstraints = cg.GetConstraints(varToAssign);
	typename std::vector<const Constraint*>::const_iterator bConst = allConstraints.begin();
	typename std::vector<const Constraint*>::const_iterator eConst = allConstraints.end();

	// get all the domains of the variable
	const std::set<Value>& varDomain = varToAssign->GetDomain();
	typename std::set<Value> originalDomain = varDomain;


	// while there is stil values left for this variable (domain != 0)
	while(!varToAssign->IsImpossible())
	{
		++iteration_counter;

		// assign random value from domain
		varToAssign->Assign();

		bool allSatisfied = true;

		// check if assignment satifies all constraint
		bConst = allConstraints.begin();
		for(; (bConst != eConst) && allSatisfied; ++bConst)
		{
			allSatisfied &= (*bConst)->Satisfiable();
		}

		// if not satisfied, remove the value that was assigned to the variable
		// and try with new value on next iteration
		if(!allSatisfied)
		{
			varToAssign->RemoveValue(varToAssign->GetValue());
			varToAssign->UnAssign();
			continue;
		}

		// save the current state
		std::map<Variable*, std::set<typename Variable::Value> > savedState = SaveState(varToAssign);

		bool isImpossible = ForwardChecking(varToAssign);

		// if its possible and next level can be solved with forward checking (recursive call)
		// then return true
		if(!isImpossible && SolveFC(level + 1))
		{
			return true;
		}
		else
		{
			// load state
			LoadState(savedState);

			// remove value from the original
			varToAssign->RemoveValue(varToAssign->GetValue());
			varToAssign->UnAssign();
			continue;
		}
	}

	// reset domains (rolling back)
	varToAssign->SetDomain(originalDomain);
	return false;
}

/**
 * @brief 
 * 	CSP solver, uses arc consistency, decided to not implement
 * @param level 
 * @return true, solution found
 * @return false, solution not found
 */
template <typename T> 
bool CSP<T>::SolveARC(unsigned level) 
{
	++recursive_call_counter;
	std::cout << "entering SolveArc (level " << level << ")\n";
	return false;
}

/**
 * @brief Forward checking
 * 
 * @param x 
 * 	Variable to check from
 * @return true, if its impossible to solve
 * @return false, if its possible to solve
 */
template <typename T> 
INLINE
bool CSP<T>::ForwardChecking(Variable *x) 
{
	// get all neighbors of x
	const std::set<Variable*> & neighbors = cg.GetNeighbors(x);
	typename std::set<Variable*>::const_iterator bNeigh = neighbors.begin();
	typename std::set<Variable*>::const_iterator eNeigh = neighbors.end();

	while(bNeigh != eNeigh)
	{
		// if a neighbor has already been assigned, skip
		Variable * currNeigh = *(bNeigh++);
		if(currNeigh->IsAssigned())
		{
			continue;
		}

		// get the constraints that connect the current neighbor and x 
		const std::set<const Constraint*> & connectingCon = cg.GetConnectingConstraints(x, currNeigh);
		typename std::set<const Constraint*>::const_iterator bConst = connectingCon.begin();
		typename std::set<const Constraint*>::const_iterator eConst = connectingCon.end();

		// get all the values of the neighbor
		const std::set<Value>& allVal = currNeigh->GetDomain();
		typename std::set<Value>::const_iterator bVal = allVal.begin();
		typename std::set<Value>::const_iterator eVal = allVal.end();

		// test with all the values
		while(bVal != eVal)
		{
			// assign to current value we are checking to neighbor
			currNeigh->Assign(*(bVal++));

			// check with all the contraints for this value
			bConst = connectingCon.begin();
			while(bConst != eConst)
			{
				Constraint const * currConst = *(bConst++);

				// check if current value satifies all the constraints
				if(!currConst->Satisfiable())
				{
					// remove the value assigned (delete from original)
					// and try with new value on next iteration
					currNeigh->RemoveValue(currNeigh->GetValue());
					currNeigh->UnAssign();

					// check to make sure its not the last value
					// if it was, then everything is wrong
					if(currNeigh->SizeDomain() == 0)
					{
						return true;
					}

					break;
				}
			}
		}

		if(currNeigh->IsAssigned())
		{
			currNeigh->UnAssign();
		}
	}

	return false;
}

/**
 * @brief 
 * 	Load states (available values) of all unassigned variables 
 * @param saved 
 */
template <typename T> 
void CSP<T>::LoadState(std::map<Variable*, std::set<typename CSP<T>::Variable::Value> >& saved) const 
{
	typename std::map<Variable*, std::set<typename Variable::Value> >::iterator b_result = saved.begin();
	typename std::map<Variable*, std::set<typename Variable::Value> >::iterator e_result = saved.end();

	for ( ; b_result != e_result; ++b_result ) 
	{
		//std::cout << "loading state for " 
		//<< b_result->first->Name() << std::endl;
		(*b_result).first->SetDomain( (*b_result).second );
	}
}

/**
 * @brief 
 * 	Save states (available values) of all unassigned variables except the current
 *  
 * @param x 
 */
template <typename T> 
INLINE
std::map< typename CSP<T>::Variable*, std::set<typename CSP<T>::Variable::Value> > 
CSP<T>::SaveState(typename CSP<T>::Variable* x) const 
{
	std::map<Variable*, std::set<typename Variable::Value> > result;

	const std::vector<Variable*>& all_vars = cg.GetAllVariables();
	typename std::vector<Variable*>::const_iterator b_all_vars = all_vars.begin();

	typename std::vector<Variable*>::const_iterator e_all_vars = all_vars.end();

	for ( ; b_all_vars!=e_all_vars; ++b_all_vars) 
	{
		if ( !(*b_all_vars)->IsAssigned() && *b_all_vars!=x ) 
		{
			//std::cout << "saving state for " 
			//<< (*b_all_vars)->Name() << std::endl;
			result[ *b_all_vars ] = (*b_all_vars)->GetDomain();
		}
	}
	return result;
}

/**
 * @brief 
 * 	Check the current (incomplete) assignment for satisfiability
 * 
 * @param p_var 
 */
template <typename T> 
INLINE
bool CSP<T>::AssignmentIsConsistent( Variable* p_var ) const 
{
	return false;
}

/**
 * @brief (Not implemented)
 * 	Insert pair 
 *	(neighbors of the current variable, the current variable)
 *	current variable is the variable that just lost some values
 *	for all y~x insert (y,x)
 *	into arc-consistency queue 
 * @param cv 
 */
template <typename T> 
INLINE
void CSP<T>::InsertAllArcsTo( Variable* cv )
{

}

/**
 * @brief (Not Implemented)
 * 	AIMA p.146 AC-3 algorithm
 * 
 * @param x 
 */
template <typename T> 
INLINE
bool CSP<T>::CheckArcConsistency(Variable* x) 
{
	return false;
}

/**
 * @brief (Not implemented)
 * CHECK that for each value of x there is a value of y 
 * which makes all constraints involving x and y satisfiable
 * 
 * @param x 
 * @param y 
 * @param c 
 */
template <typename T> 
INLINE
bool CSP<T>::RemoveInconsistentValues(Variable* x,Variable* y,const Constraint* c) 
{
	return false;
}

/**
 * @brief 
 * 	Choose next variable for assignment
 *  choose the one with minimum remaining values
 * @tparam T 
 * @return CSP<T>::Variable* 
 */
template <typename T> 
INLINE
typename CSP<T>::Variable* CSP<T>::MinRemVal() 
{
	// get all variables
	const std::vector<Variable*> &allVar = cg.GetAllVariables();
	typename std::vector<Variable*>::const_iterator bVar = allVar.begin();
	typename std::vector<Variable*>::const_iterator eVar = allVar.end();

	// set inital min and maxSize to null and max, will be used to find the minimun
	Variable* min = NULL;
	unsigned maxSize = std::numeric_limits<unsigned>::max();

	for(; bVar != eVar; ++bVar)
	{
		// get the domain of the current variable we are checking
		Variable* temp = *bVar;
		const std::set<Value>& currDomains = temp->GetDomain();

		// check if variable is not assigned and its domain is smaller than the current min
		// we have
		if(!temp->IsAssigned() && currDomains.size() < maxSize)
		{
			min = temp;
			maxSize = currDomains.size();
		}
	}

	return min;
}

/**
 * @brief (Not Implemented)
 * 	Choose next variable for assignment
 *  choose the one with max degree
 * 
 * @tparam T 
 * @return CSP<T>::Variable* 
 */
template <typename T> 
typename CSP<T>::Variable* CSP<T>::MaxDegreeHeuristic() 
{
	return NULL;
}

#undef INLINE
