/*
 * File:	parser.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the recursive-descent parser for
 *		Simple C.
 */

# include <cstdlib>
# include <iostream>
# include "tokens.h"
# include "lexer.h"
# include "checker.h"
# include "type.h"

using namespace std;

static int lookahead;
static string lexbuf;

static void expression();
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
 * Function:	specifier
 *
 * Description:	Parse a type specifier.
 *
 *		specifier:
 *		  char
 *		  int
 *		  double
 */
int specifier()
{
	int typespec = lookahead;

    if (isSpecifier(lookahead)){
		openScope();
		string temp = lexbuf;
		match(lookahead);
		cout << "Specifier: " << temp << endl;
		closeScope();
	}
    else {
		openScope();
		error();
		closeScope();
	}
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
int  pointers()
{
	unsigned count = 0;
    while (lookahead == '*'){
		match('*');
		count++;

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
    unsigned direct = pointers();
    string idName = lexbuf;
	match(ID);

    if (lookahead == '[') {
	//cout<< "open scope" << endl;
	openScope();
	match('[');

	unsigned long arrayLength;
	arrayLength = stoul(lexbuf);
	match(INTEGER);
	declareVariable(idName, Type(typespec, direct, arrayLength));
	match(']');
	//cout << "(" << typespec... ", ARRAY)" << endl;
	closeScope();
	}

	else{
		openScope();

		declareVariable(idName, Type(typespec, direct));
		//cout << "(" << typespec... ", SCALAR)" << endl;
		closeScope();
	}

}


/*
 * Function:	declaration
 *`
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
	int typespec = specifier();
    //specifier();
    declarator(typespec);

    while (lookahead == ',') {
	//cout<< "open scope" << endl;
	openScope();
	match(',');
	declarator(typespec);
	//cout<< "close scope" << endl;
    closeScope();
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

static void primaryExpression(bool lparen)
{
	
    if (lparen) {
		//cout<< "open scope" << endl;
		openScope();
		expression();
		match(')');
		//cout<< "close scope" << endl;
		closeScope();
    }  
	
	
	else if (lookahead == STRING) {
		//cout<< "open scope" << endl;
		openScope();
		match(STRING);
		//cout<< "close scope" << endl;
		closeScope();
    } 
	
	else if (lookahead == CHARACTER) {
		//cout<< "open scope" << endl;
		openScope();
		match(CHARACTER);
		//cout<< "close scope" << endl;
		closeScope();
    } 
	
	else if (lookahead == INTEGER) {
		//cout<< "open scope" << endl;
		openScope();
		match(INTEGER);
		closeScope();
		//cout<< "close scope" << endl;

    } 
	
	else if (lookahead == REAL) {
		//cout<< "open scope" << endl;
		openScope();
		match(REAL);
		//cout<< "close scope" << endl;
		closeScope();
    } 
	
	else if (lookahead == ID) {
		//cout<< "open scope" << endl;
		string here = lexbuf;
		openScope();
		match(ID);
		checkIdentifier(here);

	if (lookahead == '(') {
		//cout<< "open scope" << endl;
	    openScope();
		match('(');
		
		if (lookahead != ')') {
			//cout<< "open scope" << endl;
			openScope();
			expression();
			
		while (lookahead == ',') {
			//cout<< "open scope" << endl;
		    openScope();
			match(',');
		    expression();
			closeScope();
			//cout<< "close scope" << endl;
		}
		closeScope();
		//cout<< "close scope" << endl;
	    }

	    match(')');
		closeScope();
		//cout<< "close scope" << endl;
	}
	//cout<< "close scope" << endl;
	closeScope();
    } 
	
	else
	error();
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

static void postfixExpression(bool lparen)
{
    primaryExpression(lparen);

    while (lookahead == '[') {
	//cout<< "open scope" << endl;
	openScope();
	match('[');
	expression();
	match(']');
	cout << "index" << endl;
	//cout<< "close scope" << endl;
    closeScope();
	}
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
 *		resolve the ambiguity here by always consuming the "(type)"
 *		as part of the sizeof expression.
 */

static void prefixExpression()
{
	
    if (lookahead == '!') {
	//cout<< "open scope" << endl;
	openScope();
	match('!');
	prefixExpression();
	cout << "not" << endl;
	//cout<< "close scope" << endl;
    closeScope();
	
	}
	
	else if (lookahead == '-') {
	//cout<< "open scope" << endl;
	openScope();
	match('-');
	prefixExpression();
	cout << "neg" << endl;
	//cout<< "close scope" << endl;
	closeScope();
    }
	
	 else if (lookahead == '*') {
		//cout<< "open scope" << endl;
		openScope();
		match('*');
		prefixExpression();
		cout << "deref" << endl;
		//cout<< "close scope" << endl;
		closeScope();
    }
	
	 else if (lookahead == '&') {
		//cout<< "open scope" << endl;
		openScope();
		match('&');
		prefixExpression();
		cout << "addr" << endl;
		//cout<< "close scope" << endl;
		closeScope();
	} 
	
	else if (lookahead == SIZEOF) {
		//cout<< "open scope" << endl;
		openScope();
		match(SIZEOF);

		if (lookahead == '(') {
			openScope();//cout<< "open scope" << endl;
			
			match('(');

			if (isSpecifier(lookahead)) {
				//cout<< "open scope" << endl;
				openScope();
				specifier();
				pointers();
				match(')');
				//cout<< "close scope" << endl;
				closeScope();
			} 

			
			else
				{
				postfixExpression(true);
				}
			//cout<< "close scope" << endl;
			closeScope();

		} 
		
		else{
			prefixExpression();
			
			cout << "sizeof" << endl;
		}
	} 
	
	else if (lookahead == '(') {
		//cout<< "open scope" << endl;
		openScope();
		match('(');
	
		if (isSpecifier(lookahead)) {
			openScope();
			//cout<< "open scope" << endl;
			specifier();
			pointers();
			match(')');
			prefixExpression();
			cout << "cast" << endl;
			closeScope();
			//cout<< "close scope" << endl;

		}
	
	 else
	    postfixExpression(true);
	
	closeScope();
	//cout<< "close scope" << endl;
    } 
	
	else{
		postfixExpression(false);
	}

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

static void multiplicativeExpression()
{
    prefixExpression();

    while (1) {
	//cout<< "open scope" << endl;
	openScope();
	if (lookahead == '*') {
		openScope();
		//cout<< "open scope" << endl;
	    match('*');
	    prefixExpression();
	    cout << "mul" << endl;
		//cout<< "close scope" << endl;
		closeScope();
	} else if (lookahead == '/') {
		//cout<< "open scope" << endl;
	    openScope();
		match('/');
	    prefixExpression();
	    cout << "div" << endl;
		closeScope();
		//cout<< "close scope" << endl;

	} else if (lookahead == '%') {
		openScope();
		//cout<< "open scope" << endl;
	    match('%');
	    prefixExpression();
	    cout << "rem" << endl;
		closeScope();
		//cout<< "close scope" << endl;

	} else
	    break;

	closeScope();
	//cout<< "close scope" << endl;
    }
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

static void additiveExpression()
{
    multiplicativeExpression();

    while (1) {

	//cout<< "open scope" << endl;
	openScope();

	if (lookahead == '+') {
		openScope();
		//cout<< "open scope" << endl;
	    match('+');
	    multiplicativeExpression();
	    cout << "add" << endl;
		closeScope();
		//cout<< "close scope" << endl;

	} else if (lookahead == '-') {
		//cout<< "open scope" << endl;
	    openScope();
		match('-');
	    multiplicativeExpression();
	    cout << "sub" << endl;
		closeScope();
		//cout<< "close scope" << endl;

	} else
	    break;

	closeScope();
	//cout<< "close scope" << endl;
    }
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

static void relationalExpression()
{
    additiveExpression();

    while (1) {

	openScope();
	//cout<< "open scope" << endl;

	if (lookahead == '<') {
		openScope();//cout<< "open scope" << endl;
	    match('<');
	    additiveExpression();
	    cout << "ltn" << endl;
		closeScope();//cout<< "close scope" << endl;

	} else if (lookahead == '>') {
		openScope();//cout<< "open scope" << endl;
	    match('>');
	    additiveExpression();
	    cout << "gtn" << endl;
		closeScope();//cout<< "close scope" << endl;

	} else if (lookahead == LEQ) {
		openScope();//cout<< "open scope" << endl;
	    match(LEQ);
	    additiveExpression();
	    cout << "leq" << endl;
		closeScope();//cout<< "close scope" << endl;

	} else if (lookahead == GEQ) {
		openScope();//cout<< "open scope" << endl;
	    match(GEQ);
	    additiveExpression();
	    cout << "geq" << endl;
		closeScope();//cout<< "close scope" << endl;

	} else
	    break;

	closeScope();//cout<< "close scope" << endl;
    }
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

static void equalityExpression()
{

    relationalExpression();

    while (1) {
	openScope();//cout<< "open scope" << endl;

	if (lookahead == EQL) {
		openScope();//cout<< "open scope" << endl;
	    match(EQL);
	    relationalExpression();
	    cout << "eql" << endl;
		closeScope();//cout<< "close scope" << endl;

	} else if (lookahead == NEQ) {
		openScope();//cout<< "open scope" << endl;
	    match(NEQ);
	    relationalExpression();
	    cout << "neq" << endl;
		closeScope();//cout<< "close scope" << endl;

	} else
	    break;

	closeScope();//cout<< "close scope" << endl;
    }
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

static void logicalAndExpression()
{
    equalityExpression();

    while (lookahead == AND) {
	openScope();//cout<< "open scope" << endl;
	match(AND);
	equalityExpression();
	cout << "and" << endl;
	closeScope();//cout<< "close scope" << endl;
    }
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

static void expression()
{
    logicalAndExpression();

    while (lookahead == OR) {
	openScope();//cout<< "open scope" << endl;
	match(OR);
	logicalAndExpression();
	cout << "or" << endl;
	closeScope();//cout<< "close scope" << endl;
    }
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

static void statements()
{
    while (lookahead != '}')
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

static void assignment()
{
    expression();
    if (lookahead == '=') {
	openScope();//cout<< "open scope" << endl;
	match('=');
	expression();
	closeScope();//cout<< "close scope" << endl;
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

static void statement()
{
    if (lookahead == '{') {
	openScope();//cout<< "open scope" << endl;
	match('{');
	declarations();
	statements();
	match('}');
	closeScope();//cout<< "close scope" << endl;

    } else if (lookahead == RETURN) {
	openScope();//cout<< "open scope" << endl;
	match(RETURN);
	expression();
	match(';');
	closeScope();//cout<< "close scope" << endl;

    } else if (lookahead == WHILE) {
	openScope();//cout<< "open scope" << endl;
	match(WHILE);
	match('(');
	expression();
	match(')');
	statement();
	closeScope();//cout<< "close scope" << endl;

    } else if (lookahead == FOR) {
	openScope();//cout<< "open scope" << endl;
	match(FOR);
	match('(');
	assignment();
	match(';');
	expression();
	match(';');
	assignment();
	match(')');
	statement();
	closeScope();//cout<< "close scope" << endl;

    } else if (lookahead == IF) {
	openScope();//cout<< "open scope" << endl;
	match(IF);
	match('(');
	expression();
	match(')');
	statement();

	if (lookahead == ELSE) {
		openScope();//cout<< "open scope" << endl;
	    match(ELSE);
	    statement();
		closeScope();//cout<< "close scope" << endl;
	}
	closeScope();//cout<< "close scope" << endl;
    } else {
	closeScope();//cout<< "open scope" << endl;
	assignment();
	match(';');
	closeScope();//cout<< "close scope" << endl;
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
	int typespec = specifier();
    //specifier();
    unsigned indirection = pointers();
    Type type(typespec, indirection);
	string identifier = lexbuf;
	match(ID);


	declareVariable(identifier, type);
	cout << "Parameter: " << identifier << "of type";
	return type;
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

Parameters* parameters()
{

	Parameters *parameter_list = new Parameters;
    if (lookahead == VOID){
	match(VOID);

		if(lookahead == ')')
		{
			return parameter_list;
		}

		else{
			unsigned indirection = pointers();
			Type type(VOID, indirection);

			string identifier = lexbuf;
			match(ID);
			
			declareVariable(identifier, type);
		}
	}
    else {
		Type type;
		openScope();
		type = parameter();


		parameter_list->types.push_back(type);

	while (lookahead == ',') {
		openScope();
	    match(',');


	    if (lookahead == ELLIPSIS) {
		openScope();
		match(ELLIPSIS);
		break;
		closeScope();
	    }

	    //parameter();
		Type type;
		type = parameter();
		
		parameter_list->types.push_back(type);
		
		closeScope();
	}
	closeScope();
    }

	return parameter_list;
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

static void globalDeclarator()
{
	openScope();
    pointers();
    match(ID);

    if (lookahead == '(') {
	openScope();//cout<< "open scope" << endl;
	match('(');
	parameters();
	match(')');
	closeScope();//cout<< "close scope" << endl;

    } else if (lookahead == '[') {
	openScope();//cout<< "open scope" << endl;
	match('[');
	match(INTEGER);
	match(']');
	closeScope();//cout<< "close scope" << endl;
    }
	closeScope();
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
	openScope();
    specifier();
    pointers();
    match(ID);

    if (lookahead == '(') {
	//cout<< "open scope" << endl;
	openScope();
	match('(');
	parameters();
	match(')');

	if (lookahead == '{') {
		openScope();//cout<< "open scope" << endl;
	    match('{');
	    declarations();
	    statements();
	    match('}');
	    return;
		closeScope();//cout<< "close scope" << endl;
	}
	cout<< "close scope" << endl;
    } else if (lookahead == '[') {
	openScope();//cout<< "open scope" << endl;
	match('[');
	match(INTEGER);
	match(']');
	closeScope();//cout<< "close scope" << endl;
    }

    while (lookahead == ',') {
	openScope();//cout<< "open scope" << endl;
	match(',');
	globalDeclarator();
	closeScope();//cout<< "close scope" << endl;
    }

    match(';');
	closeScope();
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
	openScope();//cout << "open scope" << endl;
    lookahead = yylex();
    lexbuf = yytext;

    while (lookahead != DONE)
		functionOrGlobal();


	closeScope();//cout << "close scope" << endl;
    cout << yylineno - 1 << endl;
    exit(EXIT_SUCCESS);
}
