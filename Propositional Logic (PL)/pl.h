/**
 * @file pl.h
 * @author Joanna Li (li.j@digipen.edu)
 * @brief 
 *  This file contains the function declarations for the literal, clause, cnf and 
 *  knowlegde base class to implement conversion to CNF and prrof by contradiction
 *  using resolution.
 * @date 2022-11-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef PL4_H
#define PL4_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

class Literal 
{
    public:
        Literal( std::string const& _name ) : name(_name), negated(false) 
        { }

        Literal( ) : name(""), negated(false) 
        { } // just for map.operator[]
        ////////////////////////////////////////////////////////////////////////
        Literal& Negate() 
        {
            negated = !negated; 
            return *this;
        }

        bool IsPositive() const 
        { 
            return !negated; 
        }
        ////////////////////////////////////////////////////////////////////////
        bool operator==( Literal const& op2 ) const 
        {
            Literal const& op1 = *this;
            return ( op1.negated == op2.negated ) && ( op1.name == op2.name );
        }
        ////////////////////////////////////////////////////////////////////////
        bool operator<( Literal const& op2 ) const 
        {
            Literal const& op1 = *this;
            //negated infront
            if ( op1.negated && !op2.negated ) 
            {
                return true;
            }
            
            if ( !op1.negated && op2.negated ) 
            {
                return false;
            }
            return ( op1.name < op2.name );
        }
        ////////////////////////////////////////////////////////////////////////
        Literal operator~() const 
        { 
            Literal result( *this );
            result.Negate();
            return result;
        }
        ////////////////////////////////////////////////////////////////////////
        bool Complementary( Literal const& op2 ) const 
        {
            return ( name == op2.name ) && ( negated != op2.negated );
        }
        ////////////////////////////////////////////////////////////////////////
        friend std::ostream& operator<<( std::ostream& os, Literal const& literal ) 
        {
            os << (literal.negated?"~":"") << literal.name;
            return os;
        }
        
    private:
        std::string name;
        bool negated;
};

typedef std::set<Literal> LiteralSet;

class Clause 
{
    public:

     std::string ToString() const;

        /**
         * @brief Construct a new Clause object
         * 
         * @param l 
         *  Set of literals 
         */
        Clause(LiteralSet const & l): literals(l)
        {}

        /**
         * @brief Construct a new Clause object
         * 
         * @param single 
         *  Single literal
         */
        Clause(Literal const & single)
        {
            literals.emplace(single);
        }

        /**
         * @brief Implication operator
         * 
         * @param rh 
         * @return true 
         * @return false 
         */
        bool operator<(Clause const & rh) const 
        {
            return literals < rh.literals;
        }

        /**
         * @brief Returns size of the set of literal
         * 
         * @return unsigned 
         */
        unsigned size() const
        {
            return literals.size();
        }

        /**
         * @brief Returns iterator to beginning of literal set
         * 
         * @return LiteralSet::const_iterator 
         */
        LiteralSet::const_iterator begin() const 
        { 
            return literals.begin(); 
        }

        /**
         * @brief Returns end iterator of the literal set
         * 
         * @return LiteralSet::const_iterator 
         */
		LiteralSet::const_iterator end() const 
        { 
            return literals.end(); 
        }

        std::set<Clause> operator~() const;

        Clause const operator|(Clause const & rhs) const;

        ////////////////////////////////////////////////////////////////////////
        friend std::ostream& operator<<( std::ostream& os, Clause const& clause ) 
        {
            unsigned size = clause.literals.size();

            if ( clause.size() == 0 ) 
            {
                os << " FALSE ";
            } 
            else 
            {
                std::set< Literal >::const_iterator it = clause.literals.begin();
                os << "( " << *it;
                ++it;

                for ( ; it!=clause.literals.end(); ++it ) 
                {
                    os << " | " << *it;
                }
                os << " ) ";
            }
            return os;
        }

    private:
        LiteralSet literals;
};


typedef std::set<Clause> ClauseSet;

class CNF 
{
    public:
        /**
         * @brief Construct a new CNF object
         * 
         */
        CNF()
        {}

        /**
         * @brief Construct a new CNF object
         * 
         * @param c 
         *  Clause set to do operations on
         */
        CNF(ClauseSet const & c): clauses(c)
        {}

        /**
         * @brief Construct a new CNF object
         * 
         * @param single 
         *  Literal to do operations on
         */
        CNF(Literal const & single)
        {
            clauses.emplace(single);
        }

        ////////////////////////////////////////////////////////////////////////
        // not
        CNF const operator~() const;
        ////////////////////////////////////////////////////////////////////////
        // =>
        CNF const operator>( CNF const& op2 ) const 
        {
            CNF const& op1 = *this;
            return ~(op1)|op2;
        }
        ////////////////////////////////////////////////////////////////////////
        // and
        CNF const operator&( CNF const& op2 ) const;
        ///////////////////////////////////////////////////////////////////////
        // or
        CNF const operator|( CNF const& op2 ) const;
        /////////////////////////////////////////////////////////////////////////////////
        CNF const operator>( Literal const& op2 ) const { return operator>( CNF(op2) ); }
        CNF const operator&( Literal const& op2 ) const { return operator&( CNF(op2) ); }
        CNF const operator|( Literal const& op2 ) const { return operator|( CNF(op2) ); }

        ////////////////////////////////////////////////////////////////////////
        bool Empty() const 
        { 
            return clauses.size() == 0;
        }

        ////////////////////////////////////////////////////////////////////////
        std::set< Clause >::const_iterator begin() const { return clauses.begin(); }
        std::set< Clause >::const_iterator end()   const { return clauses.end(); }
        unsigned                           size()  const { return clauses.size(); }
        ////////////////////////////////////////////////////////////////////////
        friend std::ostream& operator<<( std::ostream& os, CNF const& cnf ) 
        {
            unsigned size = cnf.clauses.size();
            for( std::set< Clause >::const_iterator it1 = cnf.clauses.begin(); it1 != cnf.clauses.end(); ++it1) 
            { 
                os << *it1 << ", ";
            }
            return os;
        }
    private:
        ClauseSet clauses;
};

CNF const operator|( Literal const& op1, Literal const& op2 );
CNF const operator|( Literal const& op1, CNF     const& op2 );
CNF const operator&( Literal const& op1, Literal const& op2 );
CNF const operator&( Literal const& op1, CNF     const& op2 );
CNF const operator>( Literal const& op1, Literal const& op2 );
CNF const operator>( Literal const& op1, CNF     const& op2 );

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class KnowledgeBase 
{
    public:
        ////////////////////////////////////////////////////////////////////////////
        KnowledgeBase();
        ////////////////////////////////////////////////////////////////////////////
        KnowledgeBase& operator+=( CNF const& cnf );
        
        KnowledgeBase& operator+=(Literal const& literal);
        KnowledgeBase& operator+=(KnowledgeBase const& kb);
        KnowledgeBase& operator+=(Clause const& clause);


        ////////////////////////////////////////////////////////////////////////
        std::set< Clause >::const_iterator begin() const;
        std::set< Clause >::const_iterator end()   const;
        unsigned                           size()  const;
        ////////////////////////////////////////////////////////////////////////////
        bool ProveByRefutation( CNF const& alpha ) const;
        ////////////////////////////////////////////////////////////////////////////
        friend std::ostream& operator<<( std::ostream& os, KnowledgeBase const& kb );
    private:
        ClauseSet clauses;
        bool Resolve(KnowledgeBase & result, KnowledgeBase & newKB) const;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
