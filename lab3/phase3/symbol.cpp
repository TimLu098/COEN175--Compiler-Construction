/*
file: Symbol.cpp
*/
#include "symbol.h"
#include "type.h"

using namespace std;


Symbol::Symbol(const std::string &name, const Type &type)
    :_name(name), _type(type) 
    {
        // this-> name = name;
        // this ->type = type;
    }



