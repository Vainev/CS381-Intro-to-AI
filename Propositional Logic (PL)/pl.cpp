/**
 * @file pl.cpp
 * @author Joanna Li (li.j@digipen.edu)
 * @brief 
 *  This file contains the funciton implementations for all the functions in the 
 *  .h file.
 * @date 2022-11-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "pl.h"

CNF const operator|( Literal const& op1, Literal const& op2 ) { return CNF(op1)|CNF(op2); }
CNF const operator|( Literal const& op1, CNF     const& op2 ) { return CNF(op1)|op2; }
CNF const operator&( Literal const& op1, Literal const& op2 ) { return CNF(op1)&CNF(op2); }
CNF const operator&( Literal const& op1, CNF     const& op2 ) { return CNF(op1)&op2; }
CNF const operator>( Literal const& op1, Literal const& op2 ) { return CNF(op1)>CNF(op2); }
CNF const operator>( Literal const& op1, CNF     const& op2 ) { return CNF(op1)>op2; }

/**
 * @brief Negates all the literals in the clause
 * 
 * @return ClauseSet , set of all the negated clauses
 */
ClauseSet Clause::operator~() const
{
    ClauseSet negatedSet;
    LiteralSet::const_iterator it = literals.begin();
    LiteralSet::const_iterator end = literals.end();

    // negate all the literals in the clause set
    while(it != end)
    {
        negatedSet.insert(~(*(it)));
        it++;
    }

    return negatedSet;
}

/**
 * @brief Or
 * 
 * @param rhs 
 * @return Clause const 
 */
Clause const Clause::operator|(Clause const & rhs) const
{
    // if we have no literals in our clause, return rhs
    if(literals.size() == 0)
    {
        return rhs;
    }

    // if clause given to us is empty then return our clause
    if(rhs.size() == 0)
    {
        return *this;
    }

    // copy our clauses
    Clause result(*this);

    // insert the clauses given to us
    result.literals.insert(rhs.begin(), rhs.end());

    LiteralSet::iterator current = result.begin();
    LiteralSet::iterator end = result.end();

    LiteralSet::iterator it;
    bool removed = false;
    while(current != end)
    {
        LiteralSet::iterator it = current;
        ++it;

        while(it != end)
        {
            // check if its complementary
            if(current->Complementary(*it))
            {
                result.literals.erase(it);
                LiteralSet::iterator toRemove = current++;
                result.literals.erase(toRemove);
                removed = true;
                break;
            }
            else
            {
                ++it;
            }
        }

        // check if something was removed or not
        if(!removed)
        {
            ++current;
        }
        else
        {
            removed = false;
        }
    }

    return result;
}

KnowledgeBase::KnowledgeBase() : clauses() {}
////////////////////////////////////////////////////////////////////////////

/**
 * @brief Operator += overload to take cnf
 * 
 * @param cnf 
 *  CNF to insert into knowledge base
 * @return KnowledgeBase& 
 */
KnowledgeBase& KnowledgeBase::operator+=( CNF const& cnf ) 
{
    clauses.insert(cnf.begin(), cnf.end());
    return *this;
}

/**
 * @brief Operator += overload to take a literal
 * 
 * @param literal 
 *  Literal to insert into knowlegde base
 * @return KnowledgeBase& 
 */
KnowledgeBase& KnowledgeBase::operator+=(Literal const& literal)
{
    CNF lit = literal;
    return(*this += lit);
}

/**
 * @brief Operator += overload to take another knowledge base
 * 
 * @param kb 
 *  Knowledge base to insert into current base
 * @return KnowledgeBase& 
 */
KnowledgeBase& KnowledgeBase::operator+=(KnowledgeBase const& kb)
{
    clauses.insert(kb.begin(), kb.end());
    return *this;
}

/**
 * @brief Operator += overload to take a clause
 * 
 * @param clause 
 *  Clause to insert into knowledge base
 * @return KnowledgeBase& 
 */
KnowledgeBase& KnowledgeBase::operator+=(Clause const& clause)
{
    clauses.insert(clause);
    return *this;
}

////////////////////////////////////////////////////////////////////////
std::set< Clause >::const_iterator KnowledgeBase::begin() const { return clauses.begin(); }
std::set< Clause >::const_iterator KnowledgeBase::end()   const { return clauses.end(); }
unsigned                           KnowledgeBase::size()  const { return clauses.size(); }

////////////////////////////////////////////////////////////////////////////

/**
 * @brief Proof by contraduction using resolution 
 * 
 * @param alpha 
 *  CNF to prove
 * @return true, if proven
 * @return false, not proven
 */
bool KnowledgeBase::ProveByRefutation( CNF const& alpha ) const 
{
    // negate the cnf given
    CNF negated = ~alpha;

    // if size == 0 then every value is complimntary, so negate is false
    if(negated.size() == 0)
    {
        return true;
    }

    KnowledgeBase current(*this);

    current += negated;
    KnowledgeBase result;
    bool res = Resolve(result, current);
    return res;
}

/**
 * @brief The actual proof part
 * 
 * @param result 
 *  Result of the proof
 * @param newKB 
 *  New knowledge base
 * @return true, proven
 * @return false, not probven
 */
bool KnowledgeBase::Resolve(KnowledgeBase & result, KnowledgeBase & newKB) const
{
	// KB to be added to the original one
	KnowledgeBase additionalKB;

	// Process old data with new ones
	ClauseSet::const_iterator currentKB = result.clauses.begin();
	ClauseSet::const_iterator currentKBEnd = result.clauses.end();

	ClauseSet::const_iterator newKBEnd = newKB.clauses.end();
    ClauseSet::const_iterator newKBCurrent;

	while (currentKB != currentKBEnd) 
    {
        newKBCurrent = newKB.clauses.begin();

        while(newKBCurrent != newKBEnd)
        {
            Clause newClause = *currentKB | *(newKBCurrent++);

            // check if its a contradiction
            if(newClause.size() == 0)
            {
                return true;
            }
            else if(result.clauses.find(newClause) == result.clauses.end()
                    && newKB.clauses.find(newClause) == newKB.clauses.end())
            {
                // check the new clause doesn't exist in both KB before adding
                additionalKB += newClause;
            }
        }

        ++currentKB;
	}

    // process new data
	newKBCurrent = newKB.clauses.begin();
    while(newKBCurrent != newKBEnd)
    {
        ClauseSet::const_iterator it = newKBCurrent;

        while(it != newKBEnd)
        {
            Clause newClause = *newKBCurrent | *(it++);

            // check if its a contradiction
            if(newClause.size() == 0)
            {
                return true;
            }
            else if(result.clauses.find(newClause) == result.clauses.end()
                    && newKB.clauses.find(newClause) == newKB.clauses.end())
            {
                // check the new caluse doesn't exist in both KB before adding
                additionalKB += newClause;
            }
        }

        ++newKBCurrent;
    }

	// Nothing is added and no empty clause is found = meaning no contradiction
    // original clause doesn't follow from the axioms
	if (additionalKB.size() == 0) 
    { 
		return false;
	}

	// continue proof
	result += newKB;
	return Resolve(result, additionalKB);
}

////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& os, KnowledgeBase const& kb ) 
{
    unsigned size = kb.clauses.size();

    for( std::set< Clause >::const_iterator it1 = kb.clauses.begin(); it1 != kb.clauses.end(); ++it1) 
    { 
        os << *it1 << ", ";
    }

    return os;
}

/**
 * @brief "not"
 *  if CNF is made of a single clause: A | B | ~C,
 *    negating it gives ~A & ~B & C (3 clauses)
 *    otherwise
 *   CNF = clause1 & clause2 & clause3,
 *   ~CNF = ~clause1 | ~clause2 | ~clause3 
 *  "or" is defined later
 * 
 * @return CNF const 
 */
CNF const CNF::operator~() const
{
    // nothing to do
    if(clauses.size() == 0)
    {
        return CNF();
    }

    ClauseSet::const_iterator it = clauses.begin();
    ClauseSet::const_iterator end = clauses.end();

     CNF result = ~(*(it));
    ++it;

    // negate all the clauses
    while(it != end)
    {
        result = result | ~(*(it));
        ++it;
    }

    return result;            
}

/**
 * @brief "and"
 *   CNF1 = clause1 & clause2 & clause3,
 *   CNF2 = clause4 & clause5 & clause6,
 *   CNF1 & CNF2 = clause1 & clause2 & clause3 & clause4 & clause5 & clause6
 * @param op2 
 * @return CNF const 
 */
CNF const CNF::operator&( CNF const& op2 ) const 
{            
    // return op2 if there is no clauses for us
    // to combine
    if(clauses.size() == 0)
    {
        return op2;
    }
    // if there are no clauses given to us
    // just return our clauses
    else if(op2.size() == 0)
    {
        return *this;
    }

    // copy our clauses
    CNF result (*this);

    // add op2 clauses to ours
    result.clauses.insert(op2.begin(), op2.end());
    return result;
}

/**
 * @brief "or"
 * 
 *  CNF1 = clause1 & clause2 & clause3,
 *  CNF2 = clause4 & clause5 & clause6,
 *  CNF1 | CNF2 = 
 *                c1|c4 & c1|c5 & c1|c6    &
 *                c2|c4 & c2|c5 & c2|c6    &
 *                c3|c4 & c3|c5 & c3|c6
 * @param op2 
 * @return CNF const 
 */
CNF const CNF::operator|( CNF const& op2 ) const 
{            
    // return op2 if there is no clauses for us
    // to combine
    if(clauses.size() == 0)
    {
        return op2;
    }
    // if there are no clauses given to us
    // just return our clauses
    else if(op2.size() == 0)
    {
        return *this;
    }

    CNF result;

    ClauseSet::const_iterator it1 = clauses.begin();
    ClauseSet::const_iterator end1 = clauses.end();

    ClauseSet::const_iterator it2;
    ClauseSet::const_iterator end2 = op2.end();

    // combine the clauses together
    while(it1 != end1)
    {
        it2 = op2.begin();

        while(it2 != end2)
        {
            // or the clauses
            result.clauses.insert(*it1 | *(it2));
            ++it2;
        }

        ++it1;
     }

    return result;
}


