
%{
/*
 * File:	lexer.l
 *
 * Description:	This file contains the flex description for the lexical
 *		analyzer for Simple C.
 */

# include <cerrno>
# include <cstdlib>
# include <iostream>

using namespace std;

int numerrors = 0;
void report(const std::string &str, const std::string &arg = "");
void comment_func();
%}


%option nounput noyywrap yylineno
%%

"auto"					{ cout << "keyword " << yytext << endl; }
"break"                 { cout << "keyword " << yytext << endl; }
"case"                  { cout << "keyword " << yytext << endl; }
"char"                  { cout << "keyword " << yytext << endl; }
"const"                 { cout << "keyword " << yytext << endl; }
"continue"              { cout << "keyword " << yytext << endl; }
"default"               { cout << "keyword " << yytext << endl; }
"do"                    { cout << "keyword " << yytext << endl; }
"double"                { cout << "keyword " << yytext << endl; }
"else"                  { cout << "keyword " << yytext << endl; }
"enum"                  { cout << "keyword " << yytext << endl; }
"extern"                { cout << "keyword " << yytext << endl; }
"float"                 { cout << "keyword " << yytext << endl; }
"for"                   { cout << "keyword " << yytext << endl; }
"goto"                  { cout << "keyword " << yytext << endl; }
"if"                    { cout << "keyword " << yytext << endl; }
"int"                   { cout << "keyword " << yytext << endl; }
"long"                  { cout << "keyword " << yytext << endl; }
"register"              { cout << "keyword " << yytext << endl; }
"return"                { cout << "keyword " << yytext << endl; }
"short"                 { cout << "keyword " << yytext << endl; }
"signed"                { cout << "keyword " << yytext << endl; }
"sizeof"                { cout << "keyword " << yytext << endl; }
"static"                { cout << "keyword " << yytext << endl; }
"struct"                { cout << "keyword " << yytext << endl; }
"switch"                { cout << "keyword " << yytext << endl; }
"typedef"               { cout << "keyword " << yytext << endl; }
"union"                 { cout << "keyword " << yytext << endl; }
"unsigned"              { cout << "keyword " << yytext << endl; }
"void"                  { cout << "keyword " << yytext << endl; }
"volatile"              { cout << "keyword " << yytext << endl; }
"while"                 { cout << "keyword " << yytext << endl; }

[a-zA-Z_][a-zA-Z0-9_]*  { cout << "identifier " << yytext << endl; }
\"([^\\\"]|(\\.))*\"         { cout << "string " << yytext << endl; }
\'([^\\\']|(\\.))*\'         { cout << "character " << yytext << endl; }

[0-9]+\.[0-9]+([eE][+-]?[0-9]+)?|[0-9]+[eE][+-]?[0-9]+          { cout << "real " << yytext << endl; }

 

"/*"                   { comment_func();}




"="                     { cout << "operator " << yytext << endl; }
"||"                    { cout << "operator " << yytext << endl; }
"&&"                    { cout << "operator " << yytext << endl; }
"|"                     { cout << "operator " << yytext << endl; }
"=="                    { cout << "operator " << yytext << endl; }
"!="                    { cout << "operator " << yytext << endl; }
"<"                     { cout << "operator " << yytext << endl; }
">"                     { cout << "operator " << yytext << endl; }
"<="                    { cout << "operator " << yytext << endl; }
">="                    { cout << "operator " << yytext << endl; }
"+"                     { cout << "operator " << yytext << endl; }
"-"                     { cout << "operator " << yytext << endl; }
"*"                     { cout << "operator " << yytext << endl; }
"/"                     { cout << "operator " << yytext << endl; }
"%"                     { cout << "operator " << yytext << endl; }
"&"                     { cout << "operator " << yytext << endl; }
"!"                     { cout << "operator " << yytext << endl; }
"++"                    { cout << "operator " << yytext << endl; }
"--"                    { cout << "operator " << yytext << endl; }
"->"                    { cout << "operator " << yytext << endl; }
"("                     { cout << "operator " << yytext << endl; }
")"                     { cout << "operator " << yytext << endl; }
"["                     { cout << "operator " << yytext << endl; }
"]"                     { cout << "operator " << yytext << endl; }
"{"                     { cout << "operator " << yytext << endl; }
"}"                     { cout << "operator " << yytext << endl; }
";"                     { cout << "operator " << yytext << endl; }
":"                     { cout << "operator " << yytext << endl; }
"."                     { cout << "operator " << yytext << endl; }
","                     { cout << "operator " << yytext << endl; }
"..."                   { cout << "operator " << yytext << endl; }


[0-9]+					{ cout << "integer " << yytext << endl; }




[ \f\n\r\t\v]+			{/* ignored */}

.						{/* ignored */}

 
%%


/*
 * Function:	report
 *
 * Description:	Report an error to the standard error prefixed with the
 *		line number.  We'll be using this a lot later with an
 *		optional string argument, but C++'s stupid streams don't do
 *		positional arguments, so we actually resort to snprintf.
 *		You just can't beat C for doing things down and dirty.
 */

void report(const string &str, const string &arg)
{
    char buf[1000];


    snprintf(buf, sizeof(buf), str.c_str(), arg.c_str());
    cerr << "line " << yylineno << ": " << buf << endl;
    numerrors ++;
}

/*
 * Function:    comment_func
 *
 * Description: Deal with the input of characters in comments until the end of file
 */

void comment_func() {
    int sign, sign1;
    sign = 0;

    
    while ((sign = yyinput()) != EOF) {
        if (sign == '*') {
            if ((sign1 = yyinput()) == '/') {
        
                break;
            }
        }
        else if (sign1 == '*'){
            
                comment_func();
        }
    }
}


int main() {
	yylex();
	return 0;
}

