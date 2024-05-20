
/*
File: checker.cpp
*/

#include <iostream>
#include <set>
#include <vector>
#include "scope.h"
#include "checker.h"
#include "lexer.h"
#include "string.h"

  
using namespace std;

std::vector <Symbol*> v_symbol;

Scope* head = nullptr;
std::set<string> Set;

Scope* current_scope = nullptr;

const std::string E1 = "redefinition of '%s'";
const std::string E2 = "conflicting types for '%s'";
const std::string E3 = "redeclaration of '%s'";
const std::string E4 = "'%s' undeclared";
 
void openScope(){
    
    if( head == nullptr){
        head =  new Scope(head);
    }

    std::cout<< "open scope" << std::endl;

}

void closeScope(){

    head = head -> next;
    
    std::cout<< "close scope" << std::endl;

    
}
void defineFunction(const std::string &name, const Type &type){

    // find if func previously declared in global scope
    Symbol *prev = head->find(name);

    // if not previously declared, return
    if(Set.count(name) > 0) {
        report(E1, name);
    }
    Set.insert(name);

    if(prev == nullptr) {
        head->insert(new Symbol(name, type));
        return;
    } else if(prev->type() != type) { // if different types, we report E2
        report(E2, name);
        return;
    }
    
}

void declareFunction(const std::string &name, const Type &type){
     

    Scope *prev = head->next; 
    Symbol *here = prev -> find(name);

    if(here == nullptr) {
        head->insert(new Symbol(name, type));
        return;
    }
    // if different types, we report E2
    else if(here->type() != type) {
        report(E2, name);
        return;
    }
 
    return;



}

void declareVariable(const std::string &name, const Type &type){
    Symbol *prev = head->find(name);
    Scope* dec = head->next;
     cout << "declare variable " << (dec== nullptr) << endl;
    if(prev == nullptr) {
        Symbol *sign = new Symbol(name, type);
        head -> insert(sign);
    } else if(head->next != nullptr) {
        report(E3, name);
    } else if(prev->type() != type) {
        report(E2, name);
    }

    return;
}
void checkIdentifier(const std::string &name){
     Symbol *prev = current_scope->lookup(name);

    if(prev == nullptr) {
        report(E4, name);
    }

    

}
