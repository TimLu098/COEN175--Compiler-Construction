/*
file: scope.cpp
*/

#include "scope.h"
#include "symbol.h"
#include <cassert>



void Scope::insert(Symbol *symbol)
{
    symb.push_back(symbol);
}

Symbol* Scope::find(const std::string &name) const
{
    for(int i=0; i< (int)symb.size(); ++i)
    {
        if(symb[i] -> name() == name)
        {
            return symb[i];
        }
    }

    return nullptr;
}

Scope::Scope(Scope * next)
    : next(next)
{
	
}
 



Symbol* Scope::lookup(const std::string &name)const
{

    Symbol *current = find(name);

    if(current != nullptr)
    {
        return current;
    } 
    else 
    {
        if(next!= nullptr){
            return next->lookup(name);
        }
        else
        {
            return nullptr;
        }
    }
}
