/*
File: scope.h
*/

#ifndef SCOPE_H
#define SCOPE_H

#include <vector>
#include "symbol.h"


//vector
// head->next
class Scope{
     

    public:
        Scope(Scope *next);
        Symbol *find(const std::string &name) const;
        Symbol *lookup(const std::string &name)const;

        void insert(Symbol *symbol);


std::vector <Symbol *>  symb;
Scope* next = nullptr;
        //Scope* next() const {return _next;}
    //Symbol * _symbols;
   
        
};

#endif 