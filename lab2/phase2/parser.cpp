/*
 * File:	parser.cpp
 *
 * Description:	This file is just a placeholder for the real parser.  It is
 *		here just so that the main function is separate from the
 *		lexer and can be built upon in a separate file.
 */

/* testcase: (a>b+c)*(123 - a[b] || c && d)*/

# include <cerrno>
# include <cstdlib>
# include <iostream>
# include "lexer.h"
# include "string.h"
# include "tokens.h"

using namespace std;



static int lookahead;
void match (int t);
void andExpression();
void equalExpression();
void relationalExpression();
void multiplyExpression();
void prefixExpression();
void postfixExpression(bool lp);
void primaryExpression(bool lp);
void addExpression();
bool isSpecifier(int spec);
void specifier();
void statement();
void statements();
void declarations();
void pointers();
void parameter();
void parameters();
void functionOrGlobal();
void functionOrGlobal2();
void functionOrGlobal3();
void error();


void orExpression()
{
    andExpression();
    while (1)
    {
        if(lookahead == OR){
            match(OR);
            andExpression();
            cout << "or\n";
        }
        else {
            break;
        }
    }

}

void andExpression()
{
    equalExpression();
    while (lookahead == AND)
    {
        match(AND);
        equalExpression();
        cout << "and\n";
    }
}



void equalExpression() {
    relationalExpression();
    while (1) {
        if (lookahead == EQL) {
            match(EQL);
            relationalExpression();
            cout << "eql\n";
        } else if (lookahead == NEQ) {
            match(NEQ);
            relationalExpression();
            cout << "neq\n";
        } else {
            break;
        }
    }
}

void relationalExpression() {
    addExpression();
    while (1) {
        if (lookahead == LTN) {
            match(LTN);
            addExpression();
            cout << "ltn\n";
        } else if (lookahead == GTN) {
            match(GTN);
            addExpression();
            cout << "gtn\n";
        } else if (lookahead == LEQ) {
            match(LEQ);
            addExpression();
            cout << "leq\n";
        } else if (lookahead == GEQ) {
            match(GEQ);
            addExpression();
            cout << "geq\n";
        } else {
            break;
        }
    }
}

void addExpression(){
    multiplyExpression();
    while(1)
    {
        if(lookahead == '+')
        {
            match(PLUS);
            multiplyExpression();
            cout << "add\n";
        
        }

        else if(lookahead == '-')
        {
            match(MINUS);
            multiplyExpression();
            cout << "sub\n";
        }

        else {
            break;
        }

    }

}

void multiplyExpression() {
    prefixExpression();
    while (1) {
        if (lookahead == STAR) {
            match(STAR);
            prefixExpression();
            cout << "mul\n";
        } else if (lookahead == DIV) {
            match(DIV);
            prefixExpression();
            cout << "div\n";
        } else if (lookahead == REM) {
            match(REM);
            prefixExpression();
            cout << "rem\n";
        } else {
            break;
        }
    }
}




void prefixExpression() {
    
    if (lookahead == NOT){ 
        match(NOT);
        prefixExpression();
        cout << "not\n";
    }

    else if (lookahead == MINUS) 
        {
            match(MINUS);
            prefixExpression();
            cout << "neg\n";
        }
    else if (lookahead == ADDR) 
        {
            match(ADDR);
            prefixExpression();
            cout << "addr\n";
        }
    else if (lookahead == STAR) 
        {   
            match(STAR);
            prefixExpression();
            cout << "deref\n";
        }
    else if (lookahead == SIZEOF) 
        {
            match(SIZEOF);

            if(lookahead == LPAREN){
                match(LPAREN);
                specifier();
                pointers();
                match(RPAREN);
                cout << "sizeof\n";
            }
            else
                prefixExpression();
            
        }

   
    //cast is like: (int)(ax+by), the first parentheses is cast
    else if (lookahead == LPAREN) {
        match(LPAREN);
        if(isSpecifier(lookahead))
        {
                match(lookahead);
                match(RPAREN);
                prefixExpression();
                cout << "cast\n";
        }
        else 
            postfixExpression(true);
        
    } 
    else 
        postfixExpression(false);
    
}

void postfixExpression(bool lp) {
    primaryExpression(lp);
    while (lookahead == LBRACK) {
        match(LBRACK);
        orExpression();
        match(RBRACK);
        cout << "index\n";
    }
}

void primaryExpression(bool lp) {
    if(lp){
        orExpression();
        match(')');
    }
    else if (lookahead == ID) {
        match(ID);
        if (lookahead == LPAREN) {
            match(LPAREN);
            if (lookahead != RPAREN) {
                orExpression();
                while (lookahead == COMMA) {
                    match(COMMA);
                }
            
            }
            match(RPAREN);
        } 
    }
    
     else if (lookahead == INTEGER) {
        match(INTEGER);
    } else if (lookahead == REAL) {
        match(REAL);
    } else if (lookahead == CHARACTER) {
        match(CHARACTER);

    } else if (lookahead == STRING) {
        match(STRING);

    } else if (lookahead == LPAREN) {
        match(LPAREN);
        orExpression();
        match(RPAREN);
    }
}

//prefix is right associatives


void error()
{
    report ("Syntax error with the '%s'", yytext);
    exit(0);
}

void match (int t)
{
    if(lookahead == t)
        lookahead = yylex();

    else{
        error();
    }
}


int main()
{
    lookahead = yylex();
    while (lookahead != DONE) {
        functionOrGlobal();
        //orExpression();
    }
    cout << yylineno - 1 << endl;
    return 0;
}

void pointers(){
    while(lookahead == '*'){
        match('*');
        pointers();
    }
}


void declarator(){
    pointers();
    match(ID);

    if(lookahead == '[')
    {
        match('[');
        match(INTEGER);
        match(']');
    }
}
void declaratorList(){
    declarator();
    if(lookahead == ',')
    {
        match(',');
        declaratorList();
    }
}

void declaration(){
    specifier();
    declaratorList();
    match(SEMI);
}

void declarations(){
    while (isSpecifier(lookahead)){
        declaration();
        declarations();
    }
}

void assignment(){

    orExpression();
    if (lookahead == '='){
        match('=');
        orExpression();
    }
}
void parameter(){
    specifier();
    pointers();
    match(ID);
    
}

void parameterList(){
    parameter();
    if(lookahead == ','){
        match(',');
        parameterList();
        
    }
}

void parameters(){
    if(lookahead == VOID){
        match(VOID);
    }
    else {
        parameterList();
        while(lookahead == ','){
            match(',');
            match(ELLIPSIS);
        }
    }
    
}

void statements(){
    if (lookahead != RBRACE){
        statement();
        statements();
    }

}

void statement(){
    if(lookahead == LBRACE)
    {
        match(LBRACE);
        declarations();
        statements();
        match(RBRACE);
    
    }
    else if(lookahead == RETURN)
    {
        match(RETURN);
        orExpression();
        match(SEMI);

    }
    else if (lookahead == WHILE)
    {
        match(WHILE);
        match(LPAREN);
        orExpression();
        match(RPAREN);
        statement();
    }
    else if(lookahead == FOR)
    {
        match(FOR);
        match(LPAREN);
        assignment();
        match(SEMI);
        orExpression();
        match(SEMI);
        assignment();
        match(RPAREN);
        statement();
        
    }
    else if (lookahead == IF)
    {
        match(IF);
        match(LPAREN);
        orExpression();
        match(RPAREN);
        statement();
        if(lookahead== ELSE)
        {
            match(ELSE);
            statement();

        }

    }

    else{
        assignment();
        match(SEMI);
    }
}

bool isSpecifier(int spec ) {
    if(spec == CHAR){
        return true;
    }
    else if(spec == INT){
        return true;
    }
    else if(spec == DOUBLE){
        return true;
    }
    else{ 
        return false;
    } 
    //return lookahead == CHAR || lookahead == INT || lookahead == DOUBLE;

}
void specifier(){
    if(isSpecifier(lookahead))
        match(lookahead);
    else 
        error();
}

// void specifier(){
//     if(isSpecifier() == true){
//         match(lookahead);
//     }
// }
void functionDeclarator(){
    pointers();
    match(ID);
    match('(');
    parameters();
    match(')');
    
}

void functionDefinition(){
    specifier();
    functionDeclarator();
    match('{');
    declarations();
    statements();
    match('}');

}
void globalDeclarator(){
    pointers();
    match(ID);
    if(lookahead == '[')   // glocal declarator 
    {
        match('[');
        match(INTEGER);
        match(']');
    }
    else if (lookahead == '(')
    {
        match('(');
        parameters();
        match(')');
    }

}

void globalDeclaratorList(){
    globalDeclarator();
    if(lookahead == ',')
    {
        match(',');
        globalDeclaratorList();
    }
}

void globalDeclaration(){
    specifier();
    globalDeclaratorList();
    match(';'); 
}
// function-or-global (combined of function-definition and global declaration)
// → specifier function-declarator { declarations statements }
// | specifier global-declarator , global-declarator-list ;
// | specifier global-declarator ;
void functionOrGlobal(){
    specifier();
    pointers(); //function declarator
    match(ID);

    functionOrGlobal2();
}
    // if (lookahead == '(')
    // {
    //     match('(');
    //     parameters();
    //     match(')');

    //     if(lookahead == '{')  //function definition 
    //     {
    //         match('{');
    //         declarations();
    //         statements();

    //         match('}');
    //         return;    
    //     }
    // }
    // else if(lookahead == '[')   // glocal declarator 
    // {
    //     match('[');
    //     match(INTEGER);
    //     match(']');
    // }

    // while(lookahead == ',')  // global-declarator-list
    // {
    //     match(',');
    //     globalDeclarator();
    // }

    // match(';');


void functionOrGlobal2(){

     if (lookahead == '(')
    {
        match('(');
        parameters();
        match(')');

        if(lookahead == '{')  //function definition 
        {
            match('{');
            declarations();
            statements();
            match('}');
        }
        else {
            functionOrGlobal3();
        }
    
    }
    else if(lookahead == '[')   // glocal declarator 
    {
        match('[');
        match(INTEGER);
        match(']');
        functionOrGlobal3();
    }

    else{
        functionOrGlobal3();
    }
    // while(lookahead == ',')  // global-declarator-list
    // {
    //     match(',');
    //     globalDeclarator();
    // }

    // match(';');

}

void functionOrGlobal3(){
    if(lookahead == ','){
        match(',');
        globalDeclaratorList();
        match(';');
    }
    else 
        match(';');
    
}