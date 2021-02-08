#pragma once

#include <string>
#include <fstream>
#include "scanner.h"
#include "parser.hh"
#include "runtime.h"

struct Operator;

class Driver {
    public:
        Driver();
        int parse(const std::string& s);
        std::string file;
        yy::location location;

        void scan_begin();
        void scan_end();

        friend class Scanner;
        Scanner scanner;
        yy::parser parser;

        Runtime runtime;

        void print(const Value& v);
        void addOp(Operator* op);

        std::vector<Operator*> ops;
    private:
        std::ifstream stream;
};