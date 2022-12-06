
`proof by refutation = proof by contradiction using resolition`


## Literal

- name, negated bool

### Functions:
- operator <, bool
- operatoe ~ negation, returns literal
- complemenetary, bool
- operator ==, bool
- negate (to negate it) operator calls it, return literal reference

## Clause
- some data structure, need size func
- constructor needed
- is an "or" of literals (A | B | ~C) = 1 clause
- ~A & ~B & C (3 clauses)
- has set of literals



 ### CNF
- set of clauses
- not and or already implemented apparently
- storage for cnf
- collection of clauses
- Implement "and" and "or" of CNFs 
    1) "and"

        CNF1 = 
        
            clause1 & clause2 & clause3,

        CNF2 = 
            
            clause4 & clause5 & clause6,

        CNF1 & CNF2 = 
        
            clause1 & clause2 & clause3 & clause4 & clause5 & clause6
    2) "or

        CNF1 = 
        
            clause1 & clause2 & clause3,

        CNF2 = 

            clause4 & clause5 & clause6

        CNF1 | CNF2 = 

                    c1|c4 & c1|c5 & c1|c6    &
                    c2|c4 & c2|c5 & c2|c6    &
                    c3|c4 & c3|c5 & c3|c6

    3) negation

        a) if CNF is made of a single clause: A | B | ~C,

          negating it gives ~A & ~B & C (3 clauses)

        b) otherwise 
            CNF = 

                clause1 & clause2 & clause3
        ~CNF = 

                ~clause1 | ~clause2 | ~clause3 
            "or" is already implemented and "~" is on a single clause, 
            so 3a) takes care of it

    4) Implication
        convert using 

            A=>B
            is equivalent to
            ~A|B
            "not" and "or" are already implemented.
### Functions to be implemented:
- operator ~ not (negation)
- opertaor & and
- operator | or
- constructor that takes a literal



