/*
 * File:	parser.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the recursive-descent parser for
 *		Simple C.
 */

# include <cstdlib>
# include <iostream>
# include "checker.h"
# include "tokens.h"
# include "lexer.h"
# include <vector>

using namespace std;

static int lookahead;
static string lexbuf;
Type returnType;


static Type expression(bool &lvalue);
//static void statement(const Type &ret_type);
static void statement();


/*
 * Function:	error
 *
 * Description:	Report a syntax error to standard error.
 */

static void error()
{
    if (lookahead == DONE)
	report("syntax error at end of file");
    else
	report("syntax error at '%s'", lexbuf);

    exit(EXIT_FAILURE);
}


/*
 * Function:	match
 *
 * Description:	Match the next token against the specified token.  A
 *		failure indicates a syntax error and will terminate the
 *		program since our parser does not do error recovery.
 */

static void match(int t)
{
    if (lookahead != t)
	error();

    lookahead = yylex();
    lexbuf = yytext;
}


/*
 * Function:	isSpecifier
 *
 * Description:	Return whether the given token is a type specifier.  Simple
 *		C has only char, int, and double.
 */

static bool isSpecifier(int token)
{
    return token == CHAR || token == INT || token == DOUBLE;
}


/*
 * Function:	integer
 *
 * Description:	Match the next token as a number and return its value.
 */

static unsigned integer()
{
    string buf;


    buf = lexbuf;
    match(INTEGER);
    return strtoul(buf.c_str(), NULL, 0);
}


/*
 * Function:	identifier
 *
 * Description:	Match the next token as an identifier and return its name.
 */

static string identifier()
{
    string buf;


    buf = lexbuf;
    match(ID);
    return buf;
}


/*
 * Function:	specifier
 *
 * Description:	Parse a type specifier.
 *
 *		specifier:
 *		  char
 *		  int
 *		  double
 */

static int specifier()
{
    int typespec = lookahead;

    if (isSpecifier(lookahead))
	match(lookahead);
    else
	error();

    return typespec;
}


/*
 * Function:	pointers
 *
 * Description:	Parse pointer declarators (i.e., zero or more asterisks).
 *
 *		pointers:
 *		  empty
 *		  * pointers
 */

static unsigned pointers()
{
    unsigned count = 0;

    while (lookahead == '*') {
	match('*');
	count ++;
    }

    return count;
}


/*
 * Function:	declarator
 *
 * Description:	Parse a declarator, which in Simple C is either a scalar
 *		variable or an array, with optional pointer declarators.
 *
 *		declarator:
 *		  pointers identifier
 *		  pointers identifier [ integer ]
 */

static void declarator(int typespec)
{
    string name;
    unsigned indirection;


    indirection = pointers();
    name = identifier();

    if (lookahead == '[') {
	match('[');
	declareVariable(name, Type(typespec, indirection, integer()));
	match(']');

    } else
	declareVariable(name, Type(typespec, indirection));
}


/*
 * Function:	declaration
 *
 * Description:	Parse a local variable declaration.  Global declarations
 *		are handled separately since we need to detect a function
 *		as a special case.
 *
 *		declaration:
 *		  specifier declarator-list ';'
 *
 *		declarator-list:
 *		  declarator
 *		  declarator , declarator-list
 */

static void declaration()
{
    int typespec;


    typespec = specifier();
    declarator(typespec);

    while (lookahead == ',') {
	match(',');
	declarator(typespec);
    }

    match(';');
}


/*
 * Function:	declarations
 *
 * Description:	Parse a possibly empty sequence of declarations.
 *
 *		declarations:
 *		  empty
 *		  declaration declarations
 */

static void declarations()
{
    while (isSpecifier(lookahead))
	declaration();
}


/*
 * Function:	primaryExpression
 *
 * Description:	Parse a primary expression.
 *
 *		primary-expression:
 *		  ( expression )
 *		  identifier ( expression-list )
 *		  identifier ( )
 *		  identifier
 *		  real
 *		  integer
 *		  character
 *		  string
 *
 *		expression-list:
 *		  expression
 *		  expression , expression-list
 */
//

static Type primaryExpression(bool &lvalue)
{
	Type left;
    if (lookahead == LPAREN) {
	match(LPAREN);
	
	left = expression(lvalue);
	match(RPAREN);

    } else if (lookahead == STRING) {
	match(STRING);
	lvalue = false; 
	unsigned string_length = lexbuf.length() -2;
	return Type(CHAR, 0, string_length);

    } else if (lookahead == CHARACTER) { 
	match(CHARACTER);
	lvalue = false;
	return Type(INT, 0);

    } else if (lookahead == INTEGER) {
	match(INTEGER);
	lvalue = false;
	return Type(INT, 0);

    } else if (lookahead == REAL) {
	match(REAL);
	lvalue = false;
	return Type(DOUBLE, 0);

    } else if (lookahead == ID) {
	Symbol *symbol = checkIdentifier(identifier());
	left = symbol ->type();
	lvalue = left.isScalar();
 
	if (lookahead == '(') {
	    match('(');
		vector <Type> argument_types;
	
	    if (lookahead != ')') {
		argument_types.push_back(expression(lvalue));

		while (lookahead == ',') {
		    match(',');
		    argument_types.push_back(expression(lvalue));
		}
	    }

	    match(')');
		 
	}

    } else{
		error();
	}

	return left;
}


/*
 * Function:	postfixExpression
 *
 * Description:	Parse a postfix expression.
 *
 *		postfix-expression:
 *		  primary-expression
 *		  postfix-expression [ expression ]
 */

static Type postfixExpression(bool &lvalue)
{
    Type left = primaryExpression(lvalue);

    while (lookahead == '[') {
	match('[');
	Type right = expression(lvalue);
	match(']');
	left = checkIndex(left, right);
	//cout << "index" << endl;
	lvalue = true;
    }

	return left;
}


/*
 * Function:	prefixExpression
 *
 * Description:	Parse a prefix expression.
 *
 *		prefix-expression:
 *		  postfix-expression
 *		  ! prefix-expression
 *		  - prefix-expression
 *		  * prefix-expression
 *		  & prefix-expression
 *		  sizeof prefix-expression
 *		  sizeof ( specifier pointers )
 *		  ( specifier pointers ) prefix-expression
 *
 *		This grammar is still ambiguous since "sizeof(type) * n"
 *		could be interpreted as a multiplicative expression or as a
 *		cast of a dereference.  The correct interpretation is the
 *		former, as the latter makes little sense semantically.  We
 *		resolve the ambiguityprefixExprehere by always consuming the "(type)"
 *		as part of the sizeof expression.
 */

static Type  prefixExpression(bool &lvalue)
{
	//Type type;

    if (lookahead == '!') {
	match('!');
	Type left = prefixExpression(lvalue);
	//cout << "check !" << endl;
	left = checkNot(left);
	lvalue = false;
	return left;

    } else if (lookahead == '-') {
	match('-');
	Type left = prefixExpression(lvalue);
	//cout << "check -" << endl;
	left = checkNeg(left);
	lvalue = false;
	return left;

    } else if (lookahead == '*') {
	match('*');
	Type left = prefixExpression(lvalue);
	//cout << "check *" << endl;
	left = checkDeref(left);
	lvalue = true;
	return left;

    } else if (lookahead == '&') {
	match('&');
	Type left = prefixExpression(lvalue);
	//cout << "check &" << endl;
	left = checkAddr(left, lvalue);
	lvalue = false;
	return left;

    } else if (lookahead == SIZEOF) {
	match(SIZEOF);

	if (lookahead == '(') {
	    match('(');

	    if (isSpecifier(lookahead)) {
		specifier();
		pointers();
		match(')');
	    } else {
		Type left = postfixExpression(lvalue);
		left = checkSizeof(left);
		lvalue = false;
		return left;
		}
	} else {
	    Type left = prefixExpression(lvalue);
		lvalue = false;
		return left;
	}
	cout << "check sizeof" << endl;

    } else if (lookahead == '(') {
	match('(');

	if (isSpecifier(lookahead)) {
	    // specifier();
	    int typespec = specifier();
		unsigned indirection = pointers();
		// Type left = specifier();
		// pointers();
		
		match(')');
		
	    Type left = prefixExpression(lvalue);
		//left = checkSizeof(left);
	    left = checkCast(left, typespec, indirection);
		//cout << "cast" << endl;
		lvalue = false;
		//return left;
		return Type(INT);

	} else{
	    Type left = postfixExpression(lvalue);
		return left;
	}
    } 
	else
	    postfixExpression(lvalue);
}


/*
 * Function:	multiplicativeExpression
 *
 * Description:	Parse a multiplicative expression.
 *
 *		multiplicative-expression:
 *		  prefix-expression
 *		  multiplicative-expression * prefix-expression
 *		  multiplicative-expression / prefix-expression
 *		  multiplicative-expression % prefix-expression
 */

static Type multiplicativeExpression(bool &lvalue)
{
    
	Type left = prefixExpression(lvalue);

    while (1) {
	if (lookahead == '*') {
	    match('*');
	    Type right = prefixExpression(lvalue);
	    //cout << "check *" << endl;
		left = checkMul(left, right);
		lvalue = false;
		//return left;

	} else if (lookahead == '/') {
	    match('/');
	    Type right = prefixExpression(lvalue);
	    //cout << "check /" << endl;
		left = checkDiv(left, right);
		lvalue = false;
		//return left;

	} else if (lookahead == '%') {
	    match('%');
	    Type right = prefixExpression(lvalue);
	    //cout << "check %" << endl;
		left = checkRem(left, right);
		lvalue = false;
		//return left;

	} else
	    break;
    }

	return left;
}


/*
 * Function:	additiveExpression
 *
 * Description:	Parse an additive expression.
 *
 *		additive-expression:
 *		  multiplicative-expression
 *		  additive-expression + multiplicative-expression
 *		  additive-expression - multiplicative-expression
 */

static Type additiveExpression(bool &lvalue)
{
    Type left = multiplicativeExpression(lvalue);

    while (1) {
	if (lookahead == '+') {
	    match('+');
	    Type right = multiplicativeExpression(lvalue);
	    //cout << "check +" << endl;
		left = checkAdd(left, right);
		lvalue = false;
		//return left;

	} else if (lookahead == '-') {
	    match('-');
	    Type right = multiplicativeExpression(lvalue);
	    //cout << "check -" << endl;
		left = checkSub(left, right);
		lvalue = false;
		//return left;

	} else
	    break;
    }
	return left;
}


/*
 * Function:	relationalExpression
 *
 * Description:	Parse a relational expression.  Note that Simple C does not
 *		have shift operators, so we go immediately to additive
 *		expressions.
 *
 *		relational-expression:
 *		  additive-expression
 *		  relational-expression < additive-expression
 *		  relational-expression > additive-expression
 *		  relational-expression <= additive-expression
 *		  relational-expression >= additive-expression
 */

static Type relationalExpression(bool &lvalue)
{
    Type left = additiveExpression(lvalue);

    while (1) {
	if (lookahead == '<') {
	    match('<');
	    Type right = additiveExpression(lvalue);
	    //cout << "check <" << endl;
		left = checkLtn(left, right);
		lvalue = false;
		//return left;

	} else if (lookahead == '>') {
	    match('>');
	    Type right = additiveExpression(lvalue);
	    //cout << "check >" << endl;
		left = checkGtn(left, right);
		lvalue = false;
		//return left;

	} else if (lookahead == LEQ) {
	    match(LEQ);
	    Type right = additiveExpression(lvalue);
	    //cout << "check leq" << endl;
		left = checkLeq(left, right);
		lvalue = false;
		//return left;

	} else if (lookahead == GEQ) {
	    match(GEQ);
	    Type right = additiveExpression(lvalue);
	    //cout << "check geq" << endl;
		left = checkGeq(left, right);
		lvalue = false;
		//return left;

	} else
	    break;
    }
	return left;
}


/*
 * Function:	equalityExpression
 *
 * Description:	Parse an equality expression.
 *
 *		equality-expression:
 *		  relational-expression
 *		  equality-expression == relational-expression
 *		  equality-expression != relational-expression
 */

static Type equalityExpression(bool &lvalue)
{
    Type left = relationalExpression(lvalue);

    while (1) {
	if (lookahead == EQL) {
	    match(EQL);
	    Type right = relationalExpression(lvalue);
	    //cout << "check eql" << endl;
		left = checkEql(left, right);
		lvalue = false;
		//return left;

	} else if (lookahead == NEQ) {
	    match(NEQ);
	    Type right = relationalExpression(lvalue);
	    //cout << "check neq" << endl;
		left = checkNeq(left, right);
		lvalue = false;
		//return left;

	} else
	    break;
    }
	return left;
}


/*
 * Function:	logicalAndExpression
 *
 * Description:	Parse a logical-and expression.  Note that Simple C does
 *		not have bitwise-and expressions.
 *
 *		logical-and-expression:
 *		  equality-expression
 *		  logical-and-expression && equality-expression
 */

static Type logicalAndExpression(bool &lvalue)
{
    Type left = equalityExpression(lvalue);

    while (lookahead == AND) {
	match(AND);
	Type right = equalityExpression(lvalue);
	left = checklogicalAnd(left, right);
	//cout << "check and" << endl;
	lvalue = false;
    }

	return left;
}


/*
 * Function:	expression
 *
 * Description:	Parse an expression, or more specifically, a logical-or
 *		expression, since Simple C does not allow comma or
 *		assignment as an expression operator.
 *
 *		expression:
 *		  logical-and-expression
 *		  expression || logical-and-expression
 */

static Type expression(bool &lvalue)
{
	Type left = logicalAndExpression(lvalue);
    //logicalAndExpression();

    while (lookahead == OR) {
	match(OR);
	Type right = logicalAndExpression(lvalue);
	//cout << "check ||" << endl;
	left = checklogicalOr(left, right);
	lvalue= false;
    }
	return left;
}


/*
 * Function:	statements
 *
 * Description:	Parse a possibly empty sequence of statements.  Rather than
 *		checking if the next token starts a statement, we check if
 *		the next token ends the sequence, since a sequence of
 *		statements is always terminated by a closing brace.
 *
 *		statements:
 *		  empty
 *		  statement statements
 */
//static void statements(const Type &ret_type)
static void statements()
{
    while (lookahead != '}')
	//statement(ret_type);
	statement();
}


/*
 * Function:	Assignment
 *
 * Description:	Parse an assignment statement.
 *
 *		assignment:
 *		  expression = expression
 *		  expression
 */

// static void assignment()
// {
// 	Type left = false;
// 	Type right = false;
//     if (lookahead == '=') {
// 	match('=');
// 	bool  lvalue = false;
   
//    	left = expression(lvalue);

// 	Type right = expression(lvalue);
// 	checkAssignment(left, right, lvalue);
//     }
// }
static void assignment(bool &lvalue)
{
    Type left = expression(lvalue);

        if (lookahead == '=') {
        match('=');
        
        bool temp = lvalue;
        Type right = expression(lvalue);
        left = checkAssignment(left, right, temp);
    }
}


/*
 * Function:	statement
 *
 * Description:	Parse a statement.  Note that Simple C has so few
 *		statements that we handle them all in this one function.
 *
 *		statement:
 *		  { declarations statements }
 *		  return expression ;
 *		  while ( expression ) statement
 *		  for ( assignment ; expression ; assignment ) statement
 *		  if ( expression ) statement
 *		  if ( expression ) statement else statement
 *		  assignment ;
 */
//static void statement(const Type &type)
static void statement()
{
	bool lvalue = false; 
    if (lookahead == '{') {
	match('{');
	openScope();
	declarations();
	statements();
	closeScope();
	match('}');

    } else if (lookahead == RETURN) {
	match(RETURN);
	Type left = expression(lvalue);
	left= checkReturn(left, returnType);
	match(';');

    } else if (lookahead == WHILE) {
	match(WHILE);
	match('(');
	Type left = expression(lvalue);
	left = checkWhile(left);
	match(')');
	statement();

    } else if (lookahead == FOR) {
	match(FOR);
	match('(');
	assignment(lvalue);
	match(';');
	Type left = expression(lvalue);
	left = checkFor(left);
	match(';');
	assignment(lvalue);
	match(')');
	statement();

    } else if (lookahead == IF) {
	match(IF);
	match('(');
	Type left = expression(lvalue);
	left = checkIf(left);
	match(')');
	statement();

	if (lookahead == ELSE) {
	    match(ELSE);
	    statement();  //all statement had "type"
	}

    } else {
	assignment(lvalue);
	match(';');
    }
}


/*
 * Function:	parameter
 *
 * Description:	Parse a parameter, which in Simple C is always a scalar
 *		variable with optional pointer declarators.
 *
 *		parameter:
 *		  specifier pointers identifier
 */

static Type parameter()
{
    string name;
    int typespec;
    unsigned indirection;


    typespec = specifier();
    indirection = pointers();
    name = identifier();

    declareVariable(name, Type(typespec, indirection));
    return Type(typespec, indirection);
}


/*
 * Function:	parameters
 *
 * Description:	Parse the parameters of a function, but not the opening or
 *		closing parentheses.
 *
 *		parameters:
 *		  void
 *		  parameter-list
 *		  parameter-list , ...
 */

static Parameters *parameters()
{
    Parameters *params;


    params = new Parameters();
    params->variadic = false;

    if (lookahead == VOID)
	match(VOID);

    else {
	params->types.push_back(parameter());

	while (lookahead == ',') {
	    match(',');

	    if (lookahead == ELLIPSIS) {
		params->variadic = true;
		match(ELLIPSIS);
		break;
	    }

	    params->types.push_back(parameter());
	}
    }

    return params;
}


/*
 * Function:	globalDeclarator
 *
 * Description:	Parse a declarator, which in Simple C is either a scalar
 *		variable, an array, or a function, with optional pointer
 *		declarators.
 *
 *		global-declarator:
 *		  pointers identifier
 *		  pointers identifier [ integer ]
 *		  pointers identifier ( parameters )
 */

static void globalDeclarator(int typespec)
{
    string name;
    unsigned indirection;


    indirection = pointers();
    name = identifier();

    if (lookahead == '(') {
	match('(');
	openScope();
	declareFunction(name, Type(typespec, indirection, parameters()));
	closeScope(true);
	match(')');

    } else if (lookahead == '[') {
	match('[');
	declareVariable(name, Type(typespec, indirection, integer()));
	match(']');

    } else
	declareVariable(name, Type(typespec, indirection));
}


/*
 * Function:	functionOrGlobal
 *
 * Description:	Parse a function definition or global declaration.
 *
 * 		function-or-global:
 * 		  specifier function-declarator { declarations statements }
 * 		  specifier global-declarator , global-declarator-list ;
 * 		  specifier global-declarator ;
 *
 * 		global-declarator-list:
 * 		  global-declarator
 * 		  global-declarator , global-declarator-list
 */

static void functionOrGlobal()
{
    string name;
    int typespec;
    unsigned indirection;
    Parameters *params;
	
    typespec = specifier();
    indirection = pointers();
    name = identifier();

    if (lookahead == '(') {
	match('(');
	openScope();
	params = parameters();
	match(')');

	if (lookahead == '{') {
		returnType = Type(typespec, indirection);
	   //Symbol *temp = defineFunction(name, Type(typespec, indirection, params));
	    defineFunction(name, Type(typespec, indirection, params));
		match('{');
	    declarations();
		//statements(temp -> type());
	    //statements(returnType); 
	    statements();
		closeScope();
	    match('}');
	    return;
	}

	declareFunction(name, Type(typespec, indirection, params));
	closeScope(true);

    } else if (lookahead == '[') {
	match('[');
	declareVariable(name, Type(typespec, indirection, integer()));
	match(']');

    } else
	declareVariable(name, Type(typespec, indirection));

    while (lookahead == ',') {
	match(',');
	globalDeclarator(typespec);
    }

    match(';');
}


/*
 * Function:	main
 *
 * Description:	Analyze the standard input stream.
 *
 *		translation-unit:
 *		  empty
 *		  function-or-global translation-unit
 */

int main()
{
    openScope();
    lookahead = yylex();
    lexbuf = yytext;

    while (lookahead != DONE)
	functionOrGlobal();

    closeScope();
    exit(EXIT_SUCCESS);
}


// if sclar , return, else, send out error message