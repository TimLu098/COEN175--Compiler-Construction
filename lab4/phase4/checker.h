/*
 * File:	checker.h
 *
 * Description:	This file contains the public function declarations for the
 *		semantic checker for Simple C.
 */

# ifndef CHECKER_H
# define CHECKER_H
# include "Scope.h"

Scope *openScope();
Scope *closeScope(bool cleanup = false);

Symbol *defineFunction(const std::string &name, const Type &type);
Symbol *declareFunction(const std::string &name, const Type &type);
Symbol *declareVariable(const std::string &name, const Type &type);
Symbol *checkIdentifier(const std::string &name);

Type checkIf(Type &left);
Type checkWhile(Type &left);
Type checkFor(Type &left);

Type checklogicalOr(Type &left, Type &right);
Type checklogicalAnd(Type &left, Type &right);
Type checkNot(Type &left);
Type checkMul(Type &left, Type &right);
Type checkDiv(Type &left, Type &right);
Type checkRem(Type &left, Type &right);
Type checkNeg(Type &left);
Type checkNeq(Type &left, Type &right);
Type checkEql(Type &left, Type &right);
Type checkGtn(Type &left, Type &right);
Type checkLtn(Type &left, Type &right);
Type checkGeq(Type &left, Type &right);
Type checkLeq(Type &left, Type &right);
Type checkAdd(Type &left, Type &right);
Type checkSub(Type &left, Type &right);
//Type checkCast(Type &left, Type &right);
Type checkCast(Type &left, int& typespec, unsigned& indirection);
Type checkDeref(Type &left);
Type checkAddr(Type &left, bool &lvalue);
Type checkSizeof(Type &left);
Type checkIndex(Type &left, Type &right);
Type checkFunctionCall(Type &left, Parameters *args);
Type checkAssignment(Type &left, Type &right, bool &lvalue);
Type checkReturn(const Type &left, const Type &returned);
# endif /* CHECKER_H */
