#include <iostream>
#include <driver.h>
#include "ast.h"

int main(int argc, char** argv) {
    int result = 0;
    Driver driver;
    driver.parse(argv[1]);
    for (auto op : driver.ops)
        op->exec();

    return result;
}