/*
 * File:	checker.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the semantic checker for Simple C.
 *
 *		If a symbol is redeclared, the redeclaration is discarded
 *		and the original declaration is retained.
 *
 *		Extra functionality:
 *		- inserting an undeclared symbol with the error type
 *		- optionally deleting the symbols when closing a scope
 */

# include <set>
# include <iostream>
# include "lexer.h"
# include "tokens.h"
# include "checker.h"
# include "Type.h"
# include "Scope.h"
# include "Symbol.h"
# include <vector>
# include <string>


using namespace std;

static set<string> defined;
static Scope *current, *global;
static const Type error, integer(INT), real(DOUBLE);

static string redefined = "redefinition of '%s'";
static string redeclared = "redeclaration of '%s'";
static string conflicting = "conflicting types for '%s'";
static string undeclared = "'%s' undeclared";


static string E1 = "invalid return type";
static string E2 = "scalar type required in statement";
static string E3 = "lvalue required in expression";
static string E4 = "invalid operands to binary %s";
static string E5 = "invalid operand to unary %s";
static string E6 = "invalid operand in sizeof expression";
static string E7 = "invalid operand in cast expression";
static string E8 = "called object is not a function";
static string E9 = "invalid arguments to called function";

/*
 * Function:	openScope
 *
 * Description:	Create a scope and make it the new top-level scope.
 */

Scope *openScope()
{
    current = new Scope(current);

    if (global == nullptr)
	global = current;

    return current;
}


/*
 * Function:	closeScope
 *
 * Description:	Remove the top-level scope, and make its enclosing scope
 *		the new top-level scope.
 */

Scope *closeScope(bool cleanup)
{
    Scope *old = current;
    current = current->enclosing();

    if (!cleanup)
	return old;

    for (auto symbol : old->symbols()) {
	if (symbol->type().isFunction())
	    delete symbol->type().parameters();

	delete symbol;
    }

    delete old;
    return nullptr;
}


/*
 * Function:	defineFunction
 *
 * Description:	Define a function with the specified NAME and TYPE.  A
 *		function is always defined in the outermost scope.  We
 *		simply rely on declareFunction() to do most of the actual
 *		work.
 */

Symbol *defineFunction(const string &name, const Type &type)
{
    if (defined.count(name) > 0)
	report(redefined, name);

    defined.insert(name);
    return declareFunction(name, type);
}


/*
 * Function:	declareFunction
 *
 * Description:	Declare a function with the specified NAME and TYPE.  A
 *		function is always declared in the outermost scope.  Any
 *		redeclaration is discarded.
 */

Symbol *declareFunction(const string &name, const Type &type)
{
    Symbol *symbol;


    cout << name << ": " << type << endl;
    symbol = global->find(name);

    if (symbol == nullptr) {
	symbol = new Symbol(name, type);
	global->insert(symbol);

    } else {
	if (symbol->type() != type)
	    report(conflicting, name);

	delete type.parameters();
    }

    return symbol;
}


/*
 * Function:	declareVariable
 *
 * Description:	Declare a variable with the specified NAME and TYPE.  Any
 *		redeclaration is discarded.
 */

Symbol *declareVariable(const string &name, const Type &type)
{
    Symbol *symbol;


    cout << name << ": " << type << endl;
    symbol = current->find(name);

    if (symbol == nullptr) {
	symbol = new Symbol(name, type);
	current->insert(symbol);

    } else {
	if (current != global)
	    report(redeclared, name);

	else if (symbol->type() != type)
	    report(conflicting, name);
    }

    return symbol;
}


/*
 * Function:	checkIdentifier
 *
 * Description:	Check if NAME is declared.  If it is undeclared, then
 *		declare it as having the error type in order to eliminate
 *		future error messages.
 */

Symbol *checkIdentifier(const string &name)
{
    Symbol *symbol;


    symbol = current->lookup(name);

    if (symbol == nullptr) {
	report(undeclared, name);
	symbol = new Symbol(name, error);
	current->insert(symbol);
    }

    return symbol;
}

Type checkIf(Type &left)
{
    if(left == error) {
      return error;
  }
    Type left_ = left.promote().decay();
    if(!left_.isScalar())
    {
        report(E2, "IF");
    }

    return left;
}

Type checkWhile(Type &left)
{
    if(left == error) {
      return error;
    }
    Type left_ = left.promote().decay();
    if(!left_.isScalar())
    {
        report(E2, "WHILE");
        return error;
    }
    return left;
}

Type checkFor(Type &left)
{
    if(left == error) {
      return error;
    }
    Type left_ = left.promote().decay();
    if(!left_.isScalar())
    {
        report(E2, "FOR");
        return error;
    }
    return left;
}


Type checklogicalOr(Type &left, Type &right)
{
    if(left == error || right == error) {
      return error;
    }
    Type left_ = left.promote().decay();
    Type right_ = right.promote().decay();
    
    if(!left_.isScalar() || !right_.isScalar()) 
    {
        report(E4, "||");
        return error;
    }
    return integer;
}

Type checklogicalAnd(Type &left, Type &right)
{
    if(left == error || right == error) {
      return error;
    }
    Type left_ = left.promote().decay();
    Type right_ = right.promote().decay();
    
    if(!left_.isScalar() || !right_.isScalar()) 
    {
        report(E4, "&&");
        return error;
    }
    return integer;
}

Type checkNot(Type &left){
    if(left == error) {
      return error;
    }
    Type left_ = left.promote().decay();

    if(!left_.isScalar())
    {
        report(E5, "!");
        return error;
    }
    return integer;

}

Type checkMul(Type &left, Type &right)
{
   if(left == error || right == error) {
      return error;
    } 
    Type left_ = left.promote().decay();
    Type right_ = right.promote().decay();

   if(!left_.isNumeric() || !right_.isNumeric())
   {
       report(E4, "*");
       return error;
   }

   if (left_ == real || right_ == real)
	   return real;
   else
	   return integer;
}


Type checkDiv(Type &left, Type &right)
{
   if(left == error || right == error) {
      return error;
    } 

    Type left_ = left.promote().decay();
    Type right_ = right.promote().decay();

   if(!left_.isNumeric() || !right_.isNumeric())
   {
       report(E4, "/");
       return error;
   }

   if (left_ == real || right_ == real)
	   return real;
   else
	   return integer;
}

Type checkRem(Type &left, Type &right)  // Question
{
    if(left == error || right == error) {
      return error;
    }
   Type left_ = left.promote().decay();
   Type right_ = right.promote().decay();

   if (left_ != integer || right_ != integer)
   {
       report(E4, "%");
       return error;
   }

   return integer;
}

Type checkNeg(Type &left)
{
    if(left == error) {
      return error;
    }
	Type left_ = left.promote().decay();
	if(!left_.isNumeric())
	{
		report(E5, "-");
		return error;
	}
	return left_;
}

Type checkNeq(Type &left, Type &right)
{
    if(left == error || right == error) {
      return error;
    }
	Type left_ = left.promote().decay();
   	Type right_ = right.promote().decay();
	if(!left_.Compatible(right) || !right_.Compatible(left))
	{
		report(E4, "!=");
		return error;
	}
	return integer;
}

Type checkEql(Type &left, Type &right)
{
    if(left == error || right == error) {
      return error;
        }
    Type left_ = left.promote().decay();
    Type right_ = right.promote().decay();
    if(!left_.Compatible(right) || !right_.Compatible(left))
    {
        report(E4, "==");
        return error;
    }
    return integer;
}

Type checkGtn(Type &left, Type &right)
{
        if(left == error || right == error) {
            return error;
        }
        Type left_ = left.promote().decay();
        Type right_ = right.promote().decay();
        if(!left_.Compatible(right) || !right_.Compatible(left))
        {
                report(E4, ">");
                return error;
        }
        return integer;
}

Type checkLtn(Type &left, Type &right)
{
        if(left == error || right == error) {
            return error;
        }
        Type left_ = left.promote().decay();
        Type right_ = right.promote().decay();
        if(!left_.Compatible(right) || !right_.Compatible(left))
        {
                report(E4, "<");
                return error;
        }
        return integer;
}


Type checkGeq(Type &left, Type &right)
{
        if(left == error || right == error) {
            return error;
        }
        Type left_ = left.promote().decay();
        Type right_ = right.promote().decay();
        if(!left_.Compatible(right) || !right_.Compatible(left))
        {
		report(E4, ">=");
		return error;
        }
        return integer;
}


Type checkLeq(Type &left, Type &right)
{
        if(left == error || right == error) {
            return error;
        }
        Type left_ = left.promote().decay();
        Type right_ = right.promote().decay();
        if(!left_.Compatible(right) || !right_.Compatible(left))
        {
                report(E4, "<=");
                return error;
        }
        return integer;
}

Type checkAdd(Type &left, Type &right)
{
    if(left == error || right == error) {
      return error;
    }
	Type left_ = left.promote().decay();
	Type right_ = right.promote().decay();

	if(left_.isNumeric()|| right_.isNumeric())
	{
		
		if(left_.specifier() == DOUBLE || right_.specifier() == DOUBLE)
		{	
			return real;
		}
		else{ 
			return integer;
		}
	}
	
	if(left_.isPointer() && right_.specifier()== INT )
	{
		return left_;
	}

	if(left_.specifier()==INT && right_.isPointer())
	{
		return right_;
	}
}


Type checkSub(Type &left, Type &right)
{
        if(left == error || right == error) {
            return error;
        }
        Type left_ = left.promote().decay();
        Type right_ = right.promote().decay();

        if(left_.isNumeric()|| right_.isNumeric())
        {

                if(left_.specifier() == DOUBLE || right_.specifier() == DOUBLE)
                {
                        return real;
                }
                else{
                        return integer;
                }
        }

        if(left_.isPointer() && right_.isPointer())
        {
                return integer;
        }
	else{
		report(E4,"-");
		return error;
	}


}

Type checkDeref(Type &left)
{
    if(left == error) {
      return error;
    }
    Type left_ = left.decay();

    if(!left_.isPointer())
    {
        report(E5, "*");
        return error;
    }

    return Type(left_.specifier(), left_.indirection()-1);
}

Type checkAddr(Type &left, bool &lvalue)
{
    if(left == error) {
      return error;
    }
    if(!lvalue)
    {
        report(E3, "&");
        return error;
    }
    return Type(left.specifier(), left.indirection()+1);

}

Type checkSizeof(Type &left)
{
    if(left == error) {
      return error;
    }

    if(left.isFunction())
    {
        report(E6, "SIZEOF");
        return error;
    }
    return integer;


}

Type checkIndex(Type &left, Type &right)
{
    if(left == error || right == error) {
      return error;
    }
    Type left_ = left.promote().decay();
    Type right_ = right.promote().decay();

    if(!left_.isPointer() || right_.specifier() != INT )
    {
        report(E4, "[]");
        return error;
    }
    if(right_.isPointer() || right_.specifier() != INT) //New 
    {
        report(E4, "[]");
        return error;
    }

    return Type(left_.specifier(), left_.indirection()-1);


}

Type checkAssignment(Type &left, Type &right, bool &lvalue)
{
    if(left == error || right == error) {
      return error;
    }
    if(!lvalue)
    {
        report(E3,"");
        return error;

    }

    if(!left.Compatible(right))
    {
        report(E4, "=");
        return error;
    }
    return left;
}

Type checkReturn(const Type &left, const Type &returned) 
{
    if(left == error) {
      return error;
    }    
    if(!left.Compatible(returned)) {
        report(E1, "");
        return error;
    }

    return returned;
}


Type checkCast(Type &left, int& typespec, unsigned& indirection)
{
    if(left == error) {
      return error;
    }
    left = Type(left.specifier(), typespec, indirection);
    Type left_ = left.decay();

    if((left.isNumeric() && left_.isNumeric()) || (left.indirection() > 0 && left_.indirection() > 0))
    {
        return left_;
    }
    report(E7, "");
    return error;

}



Type checkFunctionCall(Type &left, std::vector<Type> *args)
{
    if(left == error) {
      return error;
    }
    if(!left.isFunction())
    {
        report(E8, "FUNCTION");
        return error;

    }

    
    // if the parameters list ends with an ellipsis,then the number of argument 
    // must exceed or or equal the number of parameters
   
        // If function object OR any of the args are of type ERROR, return "error"
    if (left.kind() == ERROR) {
        report(E8, ""); // E8? not sure
        return error;
    }
    for (size_t i = 0; i < args->size(); ++i) {
        if (args->at(i).kind() == ERROR) {
            report(E9, ""); // E9? not sure
            return error;
        }
    }

    // Check variadic and if the number of arguments matches the number of parameters (non-variadic case)
    if (!left.parameters()-> variadic && args->size() != left.parameters()->types.size()) {
        report(E9, "");
        return error;
    }

    // Variadic case, if the parameters list ends with an ellipsis then the number of arguments must exceed or or equal the number of parameters
    if (left.parameters()->variadic && args->size() < left.parameters()->types.size()) {
        report(E9, "");
        return error;
    }

    // Check compatibility of arguments with parameters
    for (size_t i = 0; i < args->size() && i < left.parameters()->types.size(); ++i) {
        Type argument_Type = args->at(i).decay();
        Type paramType = left.parameters()->types.at(i).decay();
        //(*left.parameters)[i].decay()
        // Check if the argument type is compatible with the parameter type
        if (!argument_Type.Compatible(paramType)) {
            report(E9, "");
            return error;
        }
    }

    // Check any remaining arguments for scalar types
    for (size_t i = left.parameters()->types.size(); i < args->size(); ++i) {
        if (!(*args)[i].isScalar()) {
            report(E9, "");
            return error;
        }
    }

    
    return left.specifier();
}



