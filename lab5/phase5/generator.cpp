/*
File: generator.cpp 
*/


# include <iostream>
# include "generator.h"
# include  "Type.h"
# include  "Scope.h"
# include "Tree.h"
using namespace std;


static ostream &operator<<(ostream &ostr, Expression *expr);


void Block::generate()
{
    for(auto stmt:_stmts)
    {
        stmt->generate();
    }

}

void Simple::generate()
{
    _expr->generate();
}


static ostream &operator<<(ostream &ostr, Expression *expr)
{
    expr->operand(ostr);
    return ostr;

}

void Expression::operand(ostream &ostr) const
{
    ostr << _offset << "(%ebp)";
}



void Integer::operand(ostream &ostr) const
{
    ostr << "$" << _value;
}


void Identifier::operand(ostream &ostr) const
{
    if(_symbol->_offset == 0)
        ostr << _symbol->name();
    else 
        ostr << _symbol->_offset << "(%ebp)";
}

void generateGlobals(Scope *scope)
{
    const Symbols &symbols = scope->symbols();

    for (auto symbol : symbols)
    {
        Type symTy = symbol->type();
        if(!symTy.isFunction())
        {
            cout << "\t.comm\t" << symbol->name() << ", " << symTy.size() <<endl;
        }

    }
}


void Function::generate() 
{
    int offsetCounter = 0;
    const Symbols &symbols = _body->declarations()->symbols();
    unsigned numofParams = _id->type().parameters()->types.size();
    
    for(unsigned i=0; i< symbols.size(); i++)
    {
        if(i >= numofParams) 
        {
            offsetCounter -= symbols[i]->type().size();
            symbols[i]->_offset = offsetCounter;
        }

        else
        {
            symbols[i]->_offset = 8+4*i;
        }
    }


    while(offsetCounter % 4 != 0)
    {
        offsetCounter --;
    }
    cout << _id->name() << ":" << endl;

    // prologue
    cout << "\tpushl %ebp" << endl;
    cout << "\tmovl %esp, %ebp" << endl;
    cout << "\tsubl $" << -offsetCounter << ", %esp" << endl;

    // Generate the body
    _body->generate();

    //epilogue
    cout << "\tmovl %ebp, %esp" << endl;
    cout << "\tpopl %ebp"<< endl;
    cout << "\tret" << endl;


    cout << "\t.globl " << _id->name() << endl;
}




void Assignment::generate()
{
    cout << "\tmovl " << _right << ", " << _left << endl;
}


void Call::generate()
{
    for(int i = _args.size()-1; i>=0; i--)
    {
        cout << "\tpushl " << _args[i] << endl; 
    }

    cout<< "\tcall " << _id->name() << endl;
}