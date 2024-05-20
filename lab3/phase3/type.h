/*
file: type.h
*/

#ifndef TYPE_H
#define  TYPE_H
#include <vector>
#include <ostream>
#include <string>


typedef  std::vector <class Type> Types;

enum {ARRAY, FUNCTION, SCALAR};

struct Parameters{
    bool variadic;
    Types types;
};

class Type{

    int _kind, _specifier;
    unsigned _indirection;
    unsigned _length;  // for fucntions
    Parameters *_parameters;
     

    public: 

    Type();
    Type(int specifier, unsigned indirection = 0);//scalar
    Type(int specifier, unsigned indirection, unsigned length); //built array
    Type(int specifier, unsigned indirection, Parameters * parameters); //function

    int kind() const {return _kind;}
    int specifier() const {return _specifier;}
    unsigned indirection() const {return _indirection;}
    Parameters *parameters() const { return _parameters; }

    unsigned length() const;

    bool operator == (const Type &that) const;
    bool operator != (const Type &that) const;

};

std:: ostream &operator << (std::ostream &ostr, const Type &type);

#endif