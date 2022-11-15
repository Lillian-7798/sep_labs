#include "exp.h"
#include <string>
#include <math.h>

void error(std::string reminder){
    throw reminder;
}
Expression::Expression()
{

}

Expression::~Expression()
{

}

ConstantExp::ConstantExp(int val)
{
    value = val;
}
IdentifierExp::IdentifierExp(std::string name)
{
    this->name = name;
}
CompoundExp::CompoundExp(string op, Expression *lhs, Expression *rhs){
    this->op = op;
    this->lhs = lhs;
    this->rhs = rhs;
}
std::string IdentifierExp::getIdentifierName(){
    return  name;
}
int ConstantExp::getConstantValue(){
    return value;
}
std::string CompoundExp::getIdentifierName(){return  "";};
std::string ConstantExp::getIdentifierName(){return  "";};
std::string ConstantExp::getOperator(){return  "";};
Expression* ConstantExp::getLHS(){};
Expression* ConstantExp::getRHS(){};
ExpressionType ConstantExp::type(){
    return CONSTANT;
}
ExpressionType IdentifierExp::type(){
    return IDENTIFIER;
}
ExpressionType CompoundExp::type(){
    return COMPOUND;
}
int ConstantExp::eval(EvaluationContext & context) {
   return value;
}
int IdentifierExp::eval(EvaluationContext & context) {
   if (!context.isDefined(name)) error(name + " is undefined");
   else return context.getValue(name);
}
int CompoundExp::eval(EvaluationContext & context) {
   int right = rhs->eval(context);
   if (op == "=") {
      context.setValue(lhs->getIdentifierName(), right);
      return right;
   }
   int left = lhs->eval(context);
   if (op == "+") return left + right;
   if (op == "-") return left - right;
   if (op == "*") return left * right;
   if (op == "/") {
      if (right == 0) error("Division by 0");
      else return left / right;
   }
   if(op == "**") return std::pow(left,right);
   error("Illegal operator in expression");
   return 0;
}

Expression* CompoundExp::getLHS(){
    return lhs;
}
Expression* CompoundExp::getRHS(){
    return rhs;
}
std::string CompoundExp::getOperator()
{
    return op;
}
void EvaluationContext::setValue(std::string var, int value){
    symbolTable[var]=value;
}
int EvaluationContext::getValue(std::string var){
    return  symbolTable[var];
}
bool EvaluationContext::isDefined(std::string var){
    return symbolTable.count(var);
}
