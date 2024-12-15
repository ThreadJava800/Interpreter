project(ParserExample)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")
add_compile_options(-g -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr)
add_link_options(-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr)

set(
    HEADERS
    ${ParserExample_SOURCE_DIR}/virt_machine.hpp
    ${ParserExample_SOURCE_DIR}/parser.hpp
    )

find_package(BISON)
find_package(FLEX)

BISON_TARGET(
    MyParser
    parser.y
    ${ParserExample_SOURCE_DIR}/parser.cpp
    COMPILE_FLAGS --graph
    DEFINES_FILE ${ParserExample_SOURCE_DIR}/parser.hpp
)

FLEX_TARGET(
    MyScanner
    scanner.l
    ${ParserExample_SOURCE_DIR}/scanner.cpp
)

ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_executable(
    interpret
    main.cpp
    virt_machine.cpp
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
)
target_include_directories(interpret PRIVATE ${ParserExample_SOURCE_DIR})
