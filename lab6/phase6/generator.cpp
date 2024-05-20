//use compiler explorer to see the right one, 
// C , x84    -m32


/*
 * File:	generator.cpp
 *
 * Description:	This file contains the public and member function
 *		definitions for the code generator for Simple C.
 *
 *		Extra functionality:
 *		- putting all the global declarations at the end
 */

# include <map>
# include <vector>
# include <cassert>
# include <iostream>
# include "generator.h"
# include "machine.h"
# include "string.h"
# include "tokens.h"
# include "Tree.h"
# include "Label.h"

using namespace std;

static int offset;
static string funcname, tab = "\t";
static Type character(CHAR), integer(INT), real(DOUBLE);

static ostream &operator <<(ostream &ostr, Expression *expr);

static Register *eax = new Register("%eax", "%al");
static Register *ecx = new Register("%ecx", "%cl");
static Register *edx = new Register("%edx", "%dl");
static vector<Register *> registers = {eax, ecx, edx};

static int sp = -1;
static Expression *fpstack[8];

static void assign(Expression *expr, Register *reg);
static void load(Expression *expr, Register *reg);
void assigntemp(Expression *expr);
static map<string , Label> strings;
static map<string , Label> doubles;


Register* getreg();

void fload(Expression *expr);


/* These will be replaced with functions in the next phase.  They are here
   as placeholders so that Call::generate() is finished. */

//# define assign(expr,reg)
// # define assigntemp(expr)
//# define load(expr,reg)

# define ftop()     fpstack[sp]
# define fpush(expr)    fpstack[++ sp] = expr
# define fpop()     sp --

/*
 * Function:	push_fp_arg (private)
 *
 * Description:	Push a floating-point value onto the run-time stack.  If
 *		the expression is not on the top of the floating-point
 *		stack, then we first push it onto the stack.
 */

int push_fp_arg(Expression *expr)
{
    if (fpstack[sp] != expr) {
	cout << tab << "fldl" << tab << expr << endl;
	sp ++;
    }

    cout << tab << "subl" << tab << "$8, %esp" << endl;
    cout << tab << "fstpl" << tab << "(%esp)" << endl;

    sp --;
    return 8;
}


/*
 * Function:	push_char_arg (private)
 *
 * Description:	Push a character argument on the stack.  We must sign
 *		extend the character because all arguments are required to
 *		be a multiple of 4 bytes.
 */

static int push_char_arg(Expression *arg)
{
    if (arg->reg == nullptr)
	//load(arg, getreg());

    cout << tab << "movsbl" << tab << arg << ", " << arg->reg->name() << endl;
    cout << tab << "pushl" << tab << arg->reg->name() << endl;

    assign(arg, nullptr);
    return 4;
}


/*
 * Function:	push_word_arg (private)
 *
 * Description:	Push a word argument, i.e., an integer or pointer, on the
 *		stack.  We can push a register, an immediate, or a memory
 *		reference.
 */

static int push_word_arg(Expression *arg)
{
    cout << tab << "pushl" << tab << arg << endl;

    assign(arg, nullptr);
    return 4;
}


/*
 * Function:	operator << (private)
 *
 * Description:	Convenience function for writing the operand of an
 *		expression using the output stream operator.
 */

static ostream &operator <<(ostream &ostr, Expression *expr)
{
    if (expr->reg != nullptr)
	return ostr << expr->reg;

    expr->operand(ostr);
    return ostr;
}


/*
 * Function:	Expression::operand
 *
 * Description:	Write an expression as an operand to the specified stream.
 */

void Expression::operand(ostream &ostr) const
{
    // search upcase efrom the hase
    // c++ dynamic class
    assert(offset != 0);
    ostr << offset << "(%ebp)";
}


/*
 * Function:	Identifier::operand
 *
 * Description:	Write an identifier as an operand to the specified stream.
 */

void Identifier::operand(ostream &ostr) const
{
    if (_symbol->offset == 0)
	ostr << global_prefix << _symbol->name();
    else
	ostr << _symbol->offset << "(%ebp)";
}


/*
 * Function:	Integer::operand
 *
 * Description:	Write an integer as an operand to the specified stream.
 */

void Integer::operand(ostream &ostr) const
{
    ostr << "$" << _value;
}


/*
 * Function:	Call::generate
 *
 * Description:	Generate code for a function call expression.
 *
 * 		On a 32-bit Linux platform, the stack needs to be aligned
 * 		on a 4-byte boundary.  (Online documentation seems to
 * 		suggest that 16 bytes are required, but 4 bytes seems to
 * 		work and is much easier.)  Since all arguments are a
 * 		multiple of 4 bytes, the stack will always be aligned.
 */

void Call::generate()
{
    unsigned bytes;
    Expression *arg;
    Type result;


    /* Generate code for the arguments and push them on the stack. */

    bytes = 0;

    for (int i = _args.size() - 1; i >= 0; i --) {
	arg = _args[i];

	arg->generate();

	if (arg->type() == real)
	    bytes += push_fp_arg(arg);
	else if (arg->type() == character)
	    bytes += push_char_arg(arg);
	else
	    bytes += push_word_arg(arg);
    }


    /* Spill the caller-saved registers. */

    load(nullptr, eax);
    load(nullptr, ecx);
    load(nullptr, edx);

    while (sp >= 0) {
	assigntemp(fpstack[sp]);
	cout << tab << "fstpl" << tab << fpstack[sp --] << endl;
    }


    /* Call the function and then reclaim the stack space. */

    cout << tab << "call" << tab << global_prefix << _id->name() << endl;

    if (bytes > 0)
	cout << tab << "addl" << tab << "$" << bytes << ", %esp" << endl;


    /* Save the return value. */

    result = Type(_type.specifier(), _type.indirection());

    if (result == real)
	fpstack[++ sp] = this;

    else {
	assign(this, eax);

	if (result == character)
	    cout << tab << "movsbl" << tab << "%al, %eax" << endl;
    }
}


/*
 * Function:	Block::generate
 *
 * Description:	Generate code for this block, which simply means we
 *		generate code for each statement within the block.
 */

void Block::generate()
{
    for (auto stmt : _stmts) {
	stmt->generate();

	for (auto reg : registers)
	    assert(reg->node == nullptr);

	assert(sp == -1);
    }
}


/*
 * Function:	Simple::generate
 *
 * Description:	Generate code for a simple (expression) statement, which
 *		means simply generating code for the expression.
 */

void Simple::generate()
{
    _expr->generate();
    if (_expr->type() != real)
    assign(_expr, nullptr);
    else if (sp >= 0) {
        cout << tab << "fstp" << tab << "%st" << endl;
        sp --;
    }
}



/*
 * Function:	Function::generate
 *
 * Description:	Generate code for this function, which entails allocating
 *		space for local variables, then emitting our prologue, the
 *		body of the function, and the epilogue.
 */

void Function::generate()
{
    int param_offset;


    /* Assign offsets to the parameters and local variables. */

    param_offset = 2 * SIZEOF_REG;
    offset = param_offset;
    allocate(offset);


    /* Generate our prologue. */

    funcname = _id->name();
    cout << global_prefix << funcname << ":" << endl;
    cout << tab << "pushl" << tab << "%ebp" << endl;
    cout << tab << "movl" << tab << "%esp, %ebp" << endl;
    cout << tab << "subl" << tab << "$" << funcname << ".size, %esp" << endl;


    /* Generate the body of this function. */

    _body->generate();


    /* Align the stack if necessary. */

    while ((offset - param_offset) % STACK_ALIGNMENT != 0)
	offset --;


    /* Generate our epilogue. */

    cout << endl << global_prefix << funcname << ".exit:" << endl;
    cout << tab << "movl" << tab << "%ebp, %esp" << endl;
    cout << tab << "popl" << tab << "%ebp" << endl;
    cout << tab << "ret" << endl << endl;

    cout << tab << ".set" << tab << funcname << ".size, " << -offset << endl;
    cout << tab << ".globl" << tab << global_prefix << funcname << endl << endl;
}


/*
 * Function:	generateGlobals
 *
 * Description:	Generate code for any global variable declarations.
 */

void generateGlobals(Scope *scope)
{
    const Symbols &symbols = scope->symbols();

    for (auto symbol : symbols)
	if (!symbol->type().isFunction()) {
	    cout << tab << ".comm" << tab << global_prefix << symbol->name();
	    cout << ", " << symbol->type().size() << endl;
	}

    cout << "\t.data" << endl;

    for(auto str : strings)
    {
        cout<< str.second << ":" <<"\t.asciz\t\"" << escapeString(str.first) << "\"" << endl;
    }
    for(auto dbl : doubles)
    {    
        cout<< dbl.second << ":" << "\t.double\t" << escapeString(dbl.first) << endl;
    }
        

}


/*
 * Function:	Assignment::generate
 *
 * Description:	Generate code for an assignment statement.
 *
 *		NOT FINISHED: Only works if the right-hand side is an
 *		integer literal and the left-hand side is an integer
 *		scalar.
 */

void Assignment::generate()
{
    Expression *pointer;
    _right -> generate();

    if (_left->isDereference(pointer)) 
    {
        cout << "#Dereference in Assignment" << endl;
        pointer->generate();
        
        if(pointer->reg == nullptr)
        {
            load(pointer, getreg());
        }
        
        if (_right->type() == real)
        {   
            cout<< "#Dereference float in Assignment" << endl;
            fload(_right);
            cout << "\tfstpl\t" << "(" << pointer << ")"<< endl; 
            fpop();
        }
        else 
        {   
            cout<< "#Dereference integer in Assignment" << endl;
            if(_right->reg == nullptr)
            {
                load(_right, getreg());
            }
            cout << "\tmov\t" << _right << ", (" << pointer << ")" << endl;
            assign(nullptr, _right->reg);
            assign(nullptr, _left->reg);
            assign(nullptr, pointer->reg);
        }
        // else  // character case
        // {
        //     cout << "\tmovb\t" << ", (" << pointer << ")" << endl;
        // }
        assign(nullptr, pointer->reg);      
    }
    //non-pointer case
    else {
        if(_left->type() == real)
        {
            fload(_right);
            cout << "\tfstpl\t" << _left << endl;
            fpop();
        }
        else {
            if(_right->reg == nullptr)
            {
                load(_right, getreg());
            }
            if (_left ->type().size() == 4)
            {   
                cout << "\tmovl\t" << _right << ", " << _left << endl;
            }
            else
             {   
                cout << "\tmovb\t" << _right << ", " << _left << endl;
             }
            assign(_left, nullptr);
        }
        // else if(_left->type() == character)
        // {
        //     cout << "\tmovb\t" << _right << ", " << _left << endl;
        // }
        // assign(_left, nullptr);
    }

    assign(_right, nullptr);
    

}



static void assign(Expression *expr, Register *reg)
{
    if(expr != nullptr)
    {
        if(expr->reg != nullptr)
            expr->reg->node = nullptr;
            
        expr->reg = reg;

    }

    if(reg != nullptr)
    {
        if(reg->node != nullptr)
            reg->node->reg = nullptr;
        reg->node = expr;
    }
}

void assigntemp(Expression *expr)
{
    offset = offset- expr ->type().size();
    expr ->offset = offset;

}

static void load(Expression *expr, Register *reg)
{
    if(reg->node != expr)
    {
        if(reg->node != nullptr)
        {
            unsigned n = reg->node->type().size();
            assigntemp(reg->node);
            cout<< (n ==1 ? "\tmovb\t" : "\tmovl\t");
            cout << reg << ", " <<offset << "(%ebp)" << endl;
        }

        if (expr != nullptr) {
            unsigned n = expr->type().size();
            cout<< (n ==1 ? "\tmovb\t" : "\tmovl\t");
            cout << expr << ", " << reg->name(n) << endl;
        }

        assign(expr, reg);
    }
}

Register *getreg()
{
    for(auto reg : registers)
        if(reg->node == nullptr)
            return reg;

    load(nullptr, registers[0]);
    return registers[0];
}

void fload(Expression *expr)
{
    for(int i = sp; i>=0 ; i--)
        if(fpstack[i] == expr)
            return;
    cout << "\tfldl\t" << expr << endl;
    fpush(expr);
}


void Add::generate()
{
    cout<< "\t#Generating add \t" << endl; 
    _left->generate();
    _right->generate();

    if (_left->type() == real) {

        fload(_left);
        fload(_right);

        cout << "\tfaddp" << endl; //  faddl is the use of the memory when we want to tell them whether it is 32 or 64 bits
        

        fpop();
        ftop() = this;

    }
    else {
        if(_left->reg == nullptr)
            load(_left, getreg());

        //run the operation
        cout << "\taddl\t" << _right << ", "<< _left << endl;

        assign(_right, nullptr);

        assign(this, _left->reg);
    }
}

void Subtract::generate()
{
    cout<< "\t#Generating subtract \t" << endl; 
    _left->generate();
    _right->generate();

    if(_left ->type() == real)
    {
        fload(_left);
        fload(_right);

        if(ftop() == _left)
            cout << "\tfsubp\t" <<  endl;

        else if(ftop() == _right)
        {
            cout << "\tfsubrp\t" << endl;
        } 

        fpop();
        ftop()= this;
    }
    else{
        load(_left, eax);

        //run the operation
        cout << "\tsubl\t" << _right << ", " << _left << endl;

        assign(_right, nullptr);

        //assign(this, eax);
        assign(this, _left->reg);
    }


}


void Multiply::generate()
{
    cout<< "\t#Generating multiply \t" << endl; 

    _left->generate();
    _right->generate();


    if(_left ->type() == real)
    {
        fload(_left);
        fload(_right);

        cout << "\tfmulp\t" << endl;
    
        
        fpop();
        ftop()= this;
    }

    else{
        load(_left, eax);

        //run the operation
        cout << "\timul\t" << _right << ", " << _left << endl;

        assign(_right, nullptr);

        //assign(this, eax);
        assign(this, _left->reg);
    }

}


void Divide::generate()
{
    cout<< "\t#Generating divide \t" << endl;
    _left->generate();
    _right->generate();

    if(_left ->type() == real)
    {
        fload(_left);
        fload(_right);

        if(ftop() == _left)
            cout << "\tfdivp\t" << endl;

        else if(ftop() == _right)
        {
            cout << "\tfdivrp\t" << endl;
        } 

        fpop();
        ftop()= this;
    }

    else{
        load(_left, eax);
        load(nullptr, edx);
        load(_right, ecx);

        //run the operation
        if(_left->type().size() == 4)
            cout << "\tcltd\t" << endl;


        cout << "\tidivl\t" << _right <<endl;
        assign(_right, nullptr);
        assign(_left, nullptr);
        
        assign(this, eax);
    }

}


void Remainder::generate()
{
    cout<< "\t#Generating remainder \t" << endl;
    _left->generate();
    _right->generate();

    load(_left, eax);
    load(nullptr, edx);
    load(_right, ecx);

    //run the operation
    if(_left->type().size() == 4)
        cout << "\tcltd\t" << endl;


    cout << "\tidivl\t" << _right <<endl;
    assign(_right, nullptr);
    assign(_left, nullptr);
    
    assign(this, edx);
}


void LessThan::generate()
{
    cout<< "\t#Generating less than \t" << endl;
     _left->generate();
    _right->generate();

    if(_left ->type() == real || _right->type() == real)
    {
        fload(_left);
        fload(_right);

        load(nullptr, eax);
        
        cout<< "\tfcompp\t" << _right << ", " << _left << endl;
        
        cout << "\tfstsw\t" << "%ax" << endl;

        cout << "\tsahf\t" << endl;
        if(this->reg == nullptr)
            assign(this, getreg());

        if(ftop() == _left)
            cout << "\tsetb\t" <<  endl;

        else if(ftop() == _right)
        {
            cout << "\tseta\t" << endl;
        } 

        cout << "\tmovzbl\t" << this->reg->byte() << ", " << this->reg << endl;
        fpop();
        fpop();
    }

    else{
        if(_left->reg == nullptr)
            load(_left, getreg());

        cout<< "\tcmp\t" << _right << ", " << _left << endl;

        // assign(_left, nullptr);
        // assign(_right, nullptr);
        assign(nullptr, _left->reg);
        assign(nullptr, _right->reg);

        if(this->reg == nullptr)
            assign(this, getreg());
        
        
        cout << "\tsetl\t" << this->reg->byte() << endl;

        cout << "\tmovzbl\t" << this->reg->byte() << ", " << this->reg << endl;
    }

    
}

void GreaterThan::generate()
{
    cout<< "\t#Generating greater than \t" << endl;
     _left->generate();
    _right->generate();

    if(_left ->type() == real)
    {
        fload(_left);
        fload(_right);

        load(nullptr, eax);
        
        cout<< "\tfcompp\t" << _right << ", " << _left << endl;
        
        cout << "\tfstsw\t" << "%ax" << endl;

        cout << "\tsahf\t" << endl;
        assign(this, getreg());

        if(ftop() == _left)
            cout << "\tseta\t" <<  endl;

        else if(ftop() == _right)
        {
            cout << "\tsetb\t" << endl;
        } 

        cout << "\tmovzbl\t" << this->reg->byte() << ", " << this->reg << endl;
        fpop();
        fpop();
    }

    else{
        if(_left->reg == nullptr)
            load(_left, getreg());

        cout<< "\tcmpl\t" << _right << ", " << _left << endl;

        assign(_left, nullptr);
        assign(_right, nullptr);

        assign(this, getreg());
        
        
        cout << "\tsetg\t" << this->reg->byte() << endl;

        cout << "\tmovzbl\t" << this->reg->byte() << ", " << this->reg << endl;
    }
    
}

void LessOrEqual::generate()
{
    cout<< "\t#Generating less or equal\t" << endl;
     _left->generate();
    _right->generate();

    if(_left ->type() == real)
    {
        fload(_left);
        fload(_right);

        load(nullptr, eax);
        
        cout<< "\tfcompp\t" << _right << ", " << _left << endl;
        
        cout << "\tfstsw\t" << "%ax" << endl;

        cout << "\tsahf\t" << endl;
        assign(this, getreg());

        if(ftop() == _left)
            cout << "\tsetbe\t" <<  endl;

        else if(ftop() == _right)
        {
            cout << "\tsetae\t" << endl;
        } 

        cout << "\tmovzbl\t" << this->reg->byte() << ", " << this->reg << endl;
        fpop();
        fpop();
    }

    else{
        if(_left->reg == nullptr)
            load(_left, getreg());

        cout<< "\tcmpl\t" << _right << ", " << _left << endl;

        assign(_left, nullptr);
        assign(_right, nullptr);

        assign(this, getreg());
        
        
        cout << "\tsetle\t" << this->reg->byte() << endl;

        cout << "\tmovzbl\t" << this->reg->byte() << ", " << this->reg << endl;
    }
    
}


void GreaterOrEqual::generate()
{
    cout<< "\t#Generating greater or equal \t" << endl;
     _left->generate();
    _right->generate();

    if(_left ->type() == real)
    {
        fload(_left);
        fload(_right);

        load(nullptr, eax);
        
        cout<< "\tfcompp\t" << _right << ", " << _left << endl;
        
        cout << "\tfstsw\t" << "%ax" << endl;

        cout << "\tsahf\t" << endl;
        assign(this, getreg());

        if(ftop() == _left)
            cout << "\tsetae\t" <<  endl;

        else if(ftop() == _right)
        {
            cout << "\tsetbe\t" << endl;
        } 

        cout << "\tmovzbl\t" << this->reg->byte() << ", " << this->reg << endl;
        fpop();
        fpop();
    }

    else{
        if(_left->reg == nullptr)
            load(_left, getreg());

        cout<< "\tcmpl\t" << _right << ", " << _left << endl;

        assign(_left, nullptr);
        assign(_right, nullptr);

        assign(this, getreg());
        
        
        cout << "\tsetge\t" << this->reg->byte() << endl;

        cout << "\tmovzbl\t" << this->reg->byte() << ", " << this->reg << endl;
    }
}

void NotEqual::generate()
{
    cout<< "\t#Generating not equal \t" << endl;
     _left->generate();
    _right->generate();

    if(_left ->type() == real)
    {
        fload(_left);
        fload(_right);

        load(nullptr, eax);
        
        cout<< "\tfcompp\t"  << endl;
        
        cout << "\tfstsw\t" << "%ax" << endl;

        cout << "\tsahf\t" << endl;
        assign(this, eax);

        cout << "\tsetne\t%al" << endl;
        cout << "\tmovzbl\t%al, %eax" << endl;
        fpop();
        fpop();
    } 

    else{
        if(_left->reg == nullptr)
            load(_left, getreg());

        cout<< "\tcmpl\t" << _right << ", " << _left << endl;

        assign(_left, nullptr);
        assign(_right, nullptr);

        assign(this, getreg());
        
        
        cout << "\tsetne\t" << this->reg->byte() << endl;

        cout << "\tmovzbl\t" << this->reg->byte() << ", " << this->reg << endl;
    }
    
}

void Equal::generate()
{
    cout<< "\t#Generating equal \t" << endl;
    _left->generate();
    _right->generate();

    if(_left ->type() == real)
    {
        fload(_left);
        fload(_right);

        load(nullptr, eax);
        
        cout<< "\tfcompp\t" << endl;
        
        cout << "\tfstsw\t" << "%ax" << endl;

        cout << "\tsahf\t" << endl;
        
        assign(this, getreg());


        cout << "\tsete\t" << this->reg->byte() << endl;


        cout << "\tmovzbl\t" << this->reg->byte() << ", " << this->reg << endl;
        fpop();
        fpop();
    }

    else{
        if(_left->reg == nullptr)
            load(_left, getreg());

        cout<< "\tcmpl\t" << _right << ", " << _left << endl;

        assign(_left, nullptr);
        assign(_right, nullptr);

        assign(this, getreg());
        
        
        cout << "\tsete\t" << this->reg->byte() << endl;

        cout << "\tmovzbl\t" << this->reg->byte() << ", " << this->reg << endl;
    }
    
}

void Not::generate()
{
    cout << "#Generating Not" << endl;
    _expr->generate();

    if(_expr ->type() == real)
    {
        load(nullptr, eax);
        fload(_expr); // fload the expr being negated
        cout << "\tfldz\t" << endl;

        cout << "\tfcompp\t" << endl;
        cout << "\tfstsw\t" << "%ax" << endl;
        cout << "\tsahf\t" << endl;
        assign(this, getreg());

        cout << "\tsete\t" << this->reg->byte() << endl;
        cout << "\tmovzbl\t" << this->reg->byte() << this << endl;
        fpop();
        
    }
    else {
        if (_expr->reg == nullptr) {
            load(_expr, getreg());
        }
        
        cout << "\tcmpl\t" << "$0, " << _expr << endl; 
        cout << "\tsete\t" << _expr->reg->byte() << endl; 
        cout << "\tmovzbl\t" << _expr->reg->byte() << ", " << _expr << endl;
        assign(this, _expr->reg);
    }
}

void Negate::generate()
{
    cout << "#Generating Negate" << endl;
    _expr->generate();

    if(_expr ->type() == real)
    {  
        fload(_expr);
        cout << "\tfchs\t" << endl;
        assign(this, getreg());
        ftop() = this;

    }
    else
    {
        cout << "\tnegl\t" << _expr << endl;

    }
    
}

void Expression::test(const Label &label, bool ifTrue)
{
    generate();

    if( _type == real)
    {  
        load(nullptr, eax);
        fload(this); // fload the expr being negated
        cout << "\tfldz\t" << endl;

        cout << "\tfcompp\t" << endl;
        cout << "\tfstsw\t" << "%ax" << endl;
        cout << "\tsahf\t" << endl;

        cout << (ifTrue ? "\tjne\t" : "\tje\t") << label << endl;

        fpop();
    }
    else{
        if (reg == nullptr) {
            load(this, getreg());
        }

        cout << "\tcmpl\t" << "$0, " << this << endl;
        cout << (ifTrue ? "\tjne\t" : "\tje\t") << label << endl;

        assign(this, nullptr);
    }
}


void LogicalAnd::generate() {
    cout << "#Generating Logical And" << endl;
    Label first_L, second_L;

    _left->test(first_L, false);
    _right->test(first_L, false);

    assign(this, getreg());

    cout << "\tmovl\t" <<  "$1, " << this << endl;
    cout << "\tjmp\t" << second_L << endl;

    cout << first_L << ":" << endl;
    cout << "\tmovl\t" <<  "$0, " << this << endl;

    cout << second_L << ":" << endl;
}

void LogicalOr::generate()
{
    cout << "#Generating Logical Or" << endl;
    Label first_L, second_L;

    _left->test(first_L, true);
    _right->test(first_L, true);

    assign(this, getreg());
    cout << "\tmovl\t" << "$0, " << this << endl;
    cout << "\tjmp\t" << second_L << endl;

    cout << first_L << ":" << endl;
    cout << "\tmovl\t" << "$1, " << this << endl;

    cout << second_L << ":" << endl;
}

void While::generate()
{
    cout << "#Generating While" << endl;
    Label loop, exit;
    cout << loop << ":" << endl;

    _expr->test(exit, false);
    _stmt->generate();

    cout << "\tjmp\t" << loop << endl;
    cout << exit << ":" << endl;
}

void For::generate()
{
    cout << "#Generating For" << endl;
    Label loop, exit;

    _init->generate();
    cout << loop << ":" << endl;

    _expr->test(exit, false);
    _stmt->generate();
    _incr->generate();

    cout << "\tjmp\t" << loop << endl;
    cout << exit << ":" << endl;
}

void If::generate()
{
    cout << "#Generating If" << endl;
    Label first, exit;

    _expr->test(first, false);
    _thenStmt->generate();

    if( _elseStmt == nullptr)
    {
        cout << first << ":" << endl;
    }
    else
    {
        cout << "\tjmp\t" << exit << endl;

        cout << first << ":" << endl;

        _elseStmt->generate();
        cout << exit << ":" <<endl;

    }
}

void Return::generate()
{
    cout << "#Generating Return" << endl;
    _expr->generate();

    if(_expr->type() == real)
    {
        fload(_expr);
        cout << "\tjmp\t" << funcname << ".exit" << endl;
        fpop();
    }

    else{
        load(_expr, eax);
        cout << "\tjmp\t" << funcname << ".exit" << endl;
        assign(nullptr, _expr->reg); 
        
    }

}

void String::operand(std::ostream &ostr) const
{

    std::map<string, Label>::const_iterator _iterator;
    _iterator = strings.find(_value);
    Label temp;

    if (_iterator == strings.end()) {
        //temp = Label();
        strings.insert({_value, temp});
        ostr << ".L" << temp.number();
    }
    else {
        temp = _iterator->second;
        ostr << ".L" << temp.number();

    }

    //ostr << temp;
}

void  Real::operand(std::ostream &ostr) const
{
    map<string, Label>::iterator _iterator;
    _iterator = doubles.find(_value);
    Label temp;

    if (_iterator == doubles.end()) {
        temp = Label();
        doubles.insert({_value, temp});

    }
    else {
        temp = _iterator->second;
    }

    ostr << temp;

}
void Address::generate()
{
    cout << "#Generating Address" << endl;
    Expression *pointer;

    if(_expr->isDereference(pointer))
    {   
        pointer-> generate();

        if(pointer-> reg == nullptr)
            load(pointer, getreg());

        assign(this, pointer->reg);

    }
    else{
        assign(this, getreg());
        cout << "\tleal\t" << _expr << ", " << this << endl;
    }

}
void Dereference::generate()
{
    cout << "#Generating Dereference" << endl;
    _expr->generate();

    if(_expr->reg == nullptr)
        load(_expr, getreg());

    if(_type == real){
        cout << "\tfldl" << "\t(" << _expr << ")" << endl;

        //update fpstack
        fpush(this);
        // unbind register for our compiler
        assign(this, nullptr);

    }
    else{
        unsigned n = _type.size();
        
        //assign(this, _expr->reg);

        if (n == 4)
                cout << "\tmovl\t" << "(" << _expr << "), ";
        else
                cout << "\tmovb\t" << "(" << _expr << "), ";

        // <this> is bound to a register
        assign(this, _expr->reg);

        cout << this << endl;

    }


}


void Cast::generate()
{
    cout << "#Generating Cast " << endl;
    _expr->generate();

    if(_type == real)
    {
        //spill _expr if in register
        if (_expr->reg != nullptr)
            load(nullptr, _expr->reg); 
        
        //fildl _expr
        cout << "\tfildl\t" << _expr << endl;
        
        fpush(this);

    }
    else if (_expr->type() == real)
    {
        assigntemp(this);
        fload(_expr);

        //refer to fp lecture slides on casting double to int
        // mem32 in slides should be <this>
        cout << "\tsubl\t" << "$4, " << "%esp" << endl;
        cout << "\tfstcw\t" << "(%esp)" << endl;
        cout << "\tfstcw\t" << "2(%esp)" << endl;
        cout << "\torw\t" << "$0x0c00, 2(%esp)" <<endl;
        cout << "\tfldcw\t" << "2(%esp)" << endl; 
        cout << "\tfistpl\t" << this << endl; 
        cout << "\tfldcw\t" << "(%esp)" << endl;
    
        fpop();
    }
    else{
        if (_expr->reg == nullptr)
            load(_expr, getreg());

        if(_type.size() > _expr->type().size())
        {
            cout << "\tmovsbl\t" << _expr << ", " << _expr->reg->name() <<  endl;
        }
        assign(this, _expr->reg);
    }

    assign(_expr, nullptr);

}