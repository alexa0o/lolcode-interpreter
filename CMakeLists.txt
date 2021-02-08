cmake_minimum_required(VERSION 3.15)
project(lolcode_interpreter)

set(CMAKE_CXX_STANDARD 17)

find_package(BISON)
find_package(FLEX)

set(
        HEADERS
        ${lolcode_interpreter_SOURCE_DIR}/driver.hh
        ${lolcode_interpreter_SOURCE_DIR}/parser.hh
)

BISON_TARGET(
        MyParser
        parser.y
        ${lolcode_interpreter_SOURCE_DIR}/parser.cpp
        COMPILE_FLAGS --graph
        DEFINES_FILE ${lolcode_interpreter_SOURCE_DIR}/parser.hh
)

FLEX_TARGET(
        MyScanner
        scanner.l
        ${lolcode_interpreter_SOURCE_DIR}/scanner.cpp
)

ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_executable(
        lolcode_interpreter
        main.cpp
        driver.cpp
        runtime.cpp
        ${BISON_MyParser_OUTPUTS}
        ${FLEX_MyScanner_OUTPUTS}
)
target_include_directories(lolcode_interpreter PRIVATE ${lolcode_interpreter_SOURCE_DIR})