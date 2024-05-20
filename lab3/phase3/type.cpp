
/*
file: type.cpp
*/
#include "type.h"
#include "tokens.h"
#include <cassert>


Type::Type(int specifier, unsigned indirection) 
    : _kind(SCALAR), _specifier(specifier), _indirection(indirection) 
{
    
}

Type :: Type (int specifier, unsigned indirection, unsigned length)
    :_kind(ARRAY),_specifier(specifier),_indirection(indirection), _length(length)
{

}

Type::Type(int specifier, unsigned indirection, Parameters *parameters) 
    : _kind(FUNCTION), _specifier(specifier), _indirection(indirection), _parameters(parameters) 
{
    
}

Type::Type()
    :_kind(ERROR)
{

}


bool Type:: operator != (const Type &that)const
{
    return !operator == (that);
}

unsigned Type::length() const
{
    assert(_kind == ARRAY);
    return _length;
}


bool Type:: operator== (const Type &that) const
{
    if(_kind != that._kind)
        return false;
    if(_kind == ERROR)
        return true;
    if(_specifier != that._specifier)
        return false;
    if(_indirection != that._indirection)
        return false;
    if(_kind == SCALAR)
        return true;
    if(_kind == ARRAY)
        return _length == that._length;
    
    assert(_kind == FUNCTION);

    if(_parameters == nullptr || that._parameters == nullptr)
        return false;
    
    if(_parameters -> variadic != that._parameters -> variadic)
        return false;

    return _parameters -> types == that._parameters -> types;
}

// add instructors and ostream

std::ostream &operator<<(std::ostream &ostr, const Type &type)
{

    return ostr;
}