%{
    #include <iostream>
    #include "scanner.h"
    #include "parser.hh"
    #include "driver.h"
%}

%option noyywrap nounput noinput batch debug

%option c++
%option yyclass="Scanner"

%{
    yy::parser::symbol_type make_INT(const std::string& s,
                                 const yy::parser::location_type& loc);
    yy::parser::symbol_type make_FLOAT(const std::string& s,
                                     const yy::parser::location_type& loc);
                                     yy::parser::symbol_type make_STRING(const std::string& s,
                                                                          const yy::parser::location_type& loc);
    void Scanner::UpdateLocation() {
        driver.location.columns(yyleng);
    }
%}

string \".*\"
id [a-zA-Z][a-zA-Z_0-9]*
float [0-9]+\.[0-9]+
int [0-9]+
blank [ \t\r]

%{
    #define YY_USER_ACTION UpdateLocation();
%}

%%

%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = driver.location;
%}

{blank}+ ;
\n+ {
    loc.lines(yyleng);
    loc.step();
}

BTW.*\n ;

"HAI" return yy::parser::make_START (loc);

"VISIBLE"  return yy::parser::make_PRINT (loc);
"I HAS A" return yy::parser::make_DECLARATION (loc);
"ITZ" return yy::parser::make_ASSIGN (loc);
"R" return yy::parser::make_ASSIGN (loc);


"SUM OF" return yy::parser::make_PLUS (loc);
"DIFF OF" return yy::parser::make_MINUS (loc);
"PRODUKT OF" return yy::parser::make_MUL (loc);
"QUOSHUNT OF" return yy::parser::make_DIV (loc);
"MOD OF" return yy::parser::make_MOD (loc);
"BIGGR OF" return yy::parser::make_MAX (loc);
"SMALLR OF" return yy::parser::make_MIN (loc);
"AN" return yy::parser::make_AN (loc);
"BOTH OF" return yy::parser::make_AND (loc);
"EITHER OF" return yy::parser::make_OR (loc);
"NOT" return yy::parser::make_NOT (loc);
"," ;
"O RLY?" return yy::parser::make_IF (loc);
"YA RLY" return yy::parser::make_THEN (loc);
"NO WAI" return yy::parser::make_ELSE (loc);
"OIC" return yy::parser::make_END_IF (loc);
"IM IN YR" return yy::parser::make_LOOP (loc);
"IM OUTTA YR" return yy::parser::make_LOOP_END (loc);
"YR" return yy::parser::make_YR (loc);

"KTHXBYE" return yy::parser::make_TEND (loc);

{string} return make_STRING (yytext, loc);
{id} return yy::parser::make_IDENTIFIER (yytext, loc);
{float} return make_FLOAT (yytext, loc);
{int} return make_INT (yytext, loc);



<<EOF>> return yy::parser::make_END (loc);
%%

yy::parser::symbol_type make_INT(const std::string& s,
                                 const yy::parser::location_type& loc)
{
    int n = std::stoi(s);
    return yy::parser::make_INT(n, loc);
}

yy::parser::symbol_type make_FLOAT(const std::string& s,
                                 const yy::parser::location_type& loc)
{
    float n = std::stof(s);
    return yy::parser::make_FLOAT(n, loc);
}

yy::parser::symbol_type make_STRING(const std::string& s,
                                 const yy::parser::location_type& loc)
{
    std::string n = s.substr(1, s.size() - 2);
    auto i = n.find(":)");
    while ((i = n.find(":)")) != std::string::npos )
        n.replace(i, 2, "\n");
    while ((i = n.find(":>")) != std::string::npos )
        n.replace(i, 2, "\t");
    while ((i = n.find(":o")) != std::string::npos )
            n.replace(i, 2, "\a");
    while ((i = n.find(":\"")) != std::string::npos )
                n.replace(i, 2, "\"");
    while ((i = n.find("::")) != std::string::npos )
                n.replace(i, 2, ":");
    return yy::parser::make_STRING(n, loc);
}