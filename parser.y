%skeleton "lalr1.cc"
%require "3.5"

%defines
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
    #include <string>
    class Scanner;
    class Driver;
    struct Expression;
    struct Operator;
}

%define parse.trace
%define parse.error verbose

%code {
    #include "driver.h"
    #include "location.hh"
    #include "ast.h"

    static yy::parser::symbol_type yylex(Scanner& scanner, Driver& driver) {
        return scanner.ScanToken();
    }
}

%lex-param { Scanner &scanner }
%lex-param { Driver &driver }
%parse-param { Scanner &scanner }
%parse-param { Driver &driver }

%locations

%define api.token.prefix {TOK_}

%token
    END 0 "end of file"
    TEND
    START
    ASSIGN
    DECLARATION
    PRINT
    PLUS
    MINUS
    MUL
    DIV
    MOD
    MAX
    MIN
    AN
    AND
    OR
    NOT
    IF
    THEN
    ELSE
    END_IF
    LOOP
    LOOP_END
    YR
;

%token <std::string> STRING
%token <std::string> IDENTIFIER "identifier"
%token <int> INT
%token <float> FLOAT

%nterm <Expression*> exp
%nterm <Operator*> command

%printer { yyo << $$; } <*>;

%left PLUS MINUS
%left MUL DIV MOD
%left MIN MAX
%left CAST

%%

%start unit;
unit: START FLOAT commands TEND { };

commands:
    %empty {}
    | commands command { driver.addOp($2); };

command:
    PRINT exp { $$ = new PrintOperator($2, driver); }
    | exp IF THEN command END_IF { $$ = new IfThenOperator($1, $4); }
    | exp IF THEN command ELSE command END_IF { $$ = new IfThenElseOperator($1, $4, $6); }
    | LOOP "identifier" "identifier" YR "identifier"
    "identifier" exp command LOOP_END "identifier" { $$ = new LoopOperator($3, $5, $6, $7, $8, driver.runtime); }
    | DECLARATION "identifier" ASSIGN exp { $$ = new AssignOperator($2, $4, driver.runtime); }
    | "identifier" ASSIGN exp { $$ = new AssignOperator($1, $3, driver.runtime); }
    | DECLARATION "identifier" { $$ = new AssignOperator($2, new ValueExpression(0), driver.runtime); }
    | exp { $$ = new ExpressionOperator($1); }

exp:
     "identifier" { $$ = new GetValueExpression($1, driver.runtime); }
    | FLOAT { $$ = new ValueExpression($1); }
    | INT { $$ = new ValueExpression($1); }
    | STRING { $$ = new ValueExpression($1); }
    | PLUS exp AN exp { $$ = new AddExpression($2, $4); }
    | MINUS exp AN exp { $$ = new MinusExpression($2, $4); }
    | DIV exp AN exp { $$ = new DivExpression($2, $4); }
    | MOD exp AN exp { $$ = new ModExpression($2, $4); }
    | MUL exp AN exp { $$ = new MulExpression($2, $4); }
    | MIN exp AN exp { $$ = new MinExpression($2, $4); }
    | MAX exp AN exp { $$ = new MaxExpression($2, $4); }
    | AND exp AN exp { $$ = new AndExpression($2, $4); }
    | OR exp AN exp { $$ = new OrExpression($2, $4); }
    | NOT exp { $$ = new NotExpression($2); }

%%

void yy::parser::error(const location_type& l, const std::string& m) {
  std::cerr << l << ": " << m << '\n';
}