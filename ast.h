#pragma once

#include <utility>

#include "runtime.h"
#include "driver.h"

struct Expression {
    virtual Value eval() = 0;
    virtual ~Expression() = default;
};

struct ValueExpression: Expression {
    template <class T>
    ValueExpression(T value): value(value)
    { }

    Value eval() override {
        return value;
    }
private:
    Value value;
};

struct GetValueExpression: Expression {
    GetValueExpression(std::string  name, Runtime& runtime): name(std::move(name)), runtime(runtime)
    { }

    Value eval() override {
        return runtime.vars[name];
    }

private:
    const std::string name;
    Runtime& runtime;
};

struct AddExpression: Expression {
    AddExpression(Expression* expr1, Expression* expr2):
                    expr1(expr1), expr2(expr2)
    { }

    Value eval() override {
        return expr1->eval() + expr2->eval();
    }
    
private:
    Expression* expr1;
    Expression* expr2;
};

struct MulExpression: Expression {
    MulExpression(Expression* expr1, Expression* expr2):
            expr1(expr1), expr2(expr2)
    { }

    Value eval() override {
        return expr1->eval() * expr2->eval();
    }

private:
    Expression* expr1;
    Expression* expr2;
};

struct MinusExpression: Expression {
    MinusExpression(Expression* expr1, Expression* expr2):
            expr1(expr1), expr2(expr2)
    { }

    Value eval() override {
        return expr1->eval() - expr2->eval();
    }

private:
    Expression* expr1;
    Expression* expr2;
};

struct DivExpression: Expression {
    DivExpression(Expression* expr1, Expression* expr2):
            expr1(expr1), expr2(expr2)
    { }

    Value eval() override {
        return expr1->eval() / expr2->eval();
    }

private:
    Expression* expr1;
    Expression* expr2;
};

struct ModExpression: Expression {
    ModExpression(Expression* expr1, Expression* expr2):
            expr1(expr1), expr2(expr2)
    { }

    Value eval() override {
        return expr1->eval() % expr2->eval();
    }

private:
    Expression* expr1;
    Expression* expr2;
};

struct MaxExpression: Expression {
    MaxExpression(Expression* expr1, Expression* expr2):
            expr1(expr1), expr2(expr2)
    { }

    Value eval() override {
        return expr1->eval().max(expr2->eval());
    }

private:
    Expression* expr1;
    Expression* expr2;
};

struct MinExpression: Expression {
    MinExpression(Expression* expr1, Expression* expr2):
            expr1(expr1), expr2(expr2)
    { }

    Value eval() override {
        return expr1->eval().min(expr2->eval());
    }

private:
    Expression* expr1;
    Expression* expr2;
};

struct AndExpression: Expression {
    AndExpression(Expression* expr1, Expression* expr2):
            expr1(expr1), expr2(expr2)
    { }

    Value eval() override {
        return expr1->eval() && expr2->eval();
    }

private:
    Expression* expr1;
    Expression* expr2;
};

struct OrExpression: Expression {
    OrExpression(Expression* expr1, Expression* expr2):
            expr1(expr1), expr2(expr2)
    { }

    Value eval() override {
        return expr1->eval() || expr2->eval();
    }

private:
    Expression* expr1;
    Expression* expr2;
};

struct NotExpression: Expression {
    NotExpression(Expression* expr1):
            expr1(expr1)
    { }

    Value eval() override {
        return !expr1->eval();
    }

private:
    Expression* expr1;
};

///////////////////////////////////////////////////////////////////////////////

struct Operator {
    virtual void exec() = 0;
    virtual ~Operator() = default;
};

struct ExpressionOperator: Operator {
    ExpressionOperator(Expression* expr): expr(expr)
    { }

    void exec() override {
        expr->eval();
    }

private:
    Expression* expr;
};

struct AssignOperator: Operator {
    AssignOperator(std::string  name, Expression* expr, Runtime& runtime):
                    name(std::move(name)), expr(expr), runtime(runtime)
    { }

    void exec() override {
        runtime.vars[name] = expr->eval();
    }

private:
    const std::string name;
    Expression* expr;
    Runtime& runtime;
};

struct PrintOperator: Operator {
    PrintOperator(Expression* expr, Driver& driver): expr(expr), driver(driver)
    { }

    void exec() override {
        driver.print(expr->eval());
    }

private:
    Expression* expr;
    Driver& driver;
};

struct IfThenOperator: Operator {
    IfThenOperator(Expression* cond, Operator* then): cond(cond), then(then)
    { }

    void exec() override {
        if (cond->eval())
            then->exec();
    }

private:
    Expression* cond;
    Operator* then;
};

struct IfThenElseOperator: Operator {
    IfThenElseOperator(Expression* cond, Operator* then, Operator* els): cond(cond), then(then), els(els)
    { }

    void exec() override {
        if (cond->eval())
            then->exec();
        else
            els->exec();
    }

private:
    Expression* cond;
    Operator* then;
    Operator* els;
};

struct LoopOperator: Operator {
    LoopOperator(std::string iter, std::string var, std::string type, Expression* cond, Operator* loop, Runtime& runtime):
        iter(std::move(iter)), var(std::move(var)), type(std::move(type)), cond(cond), runtime(runtime), loop(loop)
    { }

    void exec() override {
        auto& i = runtime.vars[var];
        i = 0;
        if (type == "TIL") {
            for (; !cond->eval(); i = i + ((iter == "UPPIN") ? 1 : -1))
                loop->exec();
        } else if (type == "WILE") {
            for (; cond->eval(); i = i + ((iter == "UPPIN") ? 1 : -1))
                loop->exec();
        }

    }

private:
    std::string iter;
    std::string var;
    std::string type;
    Expression* cond;
    Operator* loop;
    Runtime& runtime;
};