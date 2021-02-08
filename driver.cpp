#include <iostream>

#include "driver.h"
#include "parser.hh"

Driver::Driver(): scanner(*this), parser(scanner, *this)
{ }

int Driver::parse(const std::string& f) {
    file = f;
    // initialize location positions
    location.initialize(&file);
    scan_begin();
    int res = parser();
    scan_end();
    return res;
}

void Driver::scan_begin() {
    if (file.empty () || file == "-") {
    } else {
        stream.open(file);
        std::cerr << "File name is " << file << std::endl;

        // Restart scanner resetting buffer!
        scanner.yyrestart(&stream);
    }
}

void Driver::scan_end()
{
    stream.close();
}

void Driver::print(const Value& v) {
    std::cout << v << '\n';
}

void Driver::addOp(Operator* op) {
    ops.push_back(op);
}