#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

#include <string>
#include <iostream>
#include <deque>
#include <utility>

namespace bccc
{
    struct Constant
    {
        int value;

        Constant() = delete;

        explicit Constant(const std::string &sValue_) : value(std::stoi(sValue_, nullptr, 0)) {}

        Constant(const Constant &constant_) = default;
    };

    std::ostream &operator<<(std::ostream &os, Constant &constant_);

    struct Expression
    {
        Constant body;

        Expression() = delete;

        explicit Expression(Constant &body_) : body(body_) {}

        Expression(const Expression &expression_) = default;
    };

    std::ostream &operator<<(std::ostream &os, Expression &expression_);

    struct Return
    {
        Expression expression;

        Return() = delete;

        explicit Return(Expression &expression_) : expression(expression_) {}

        Return(const Return &return_) = default;
    };

    std::ostream &operator<<(std::ostream &os, Return &return_);

    struct Statement
    {
        Return body;

        Statement() = delete;

        explicit Statement(Return &body_) : body(body_) {}

        Statement(const Statement &statement_) = default;
    };

    std::ostream &operator<<(std::ostream &os, Statement &statement_);

    struct Function
    {
        std::string sName;
        Statement body;

        Function() = delete;

        Function(std::string &sName_, Statement &body_) : sName(sName_), body(body_) {}

        Function(const Function &function_) = default;
    };

    std::ostream &operator<<(std::ostream &os, Function &function);

    struct Program
    {
        Function body;

        Program() = delete;

        explicit Program(Function &body_) : body(body_) {}

        Program(const Program &program_) = default;
    };

    std::ostream &operator<<(std::ostream &os, Program &program_);

    std::pair<Expression, std::deque<Token>> parseExpression(std::deque<Token> &tokens);

    std::pair<Statement, std::deque<Token>> parseStatement(std::deque<Token> &tokens);

    std::pair<Function, std::deque<Token>> parseFunction(std::deque<Token> &tokens);

    Program parseProgram(std::deque<Token> &tokens);
}

#endif