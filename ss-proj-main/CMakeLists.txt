cmake_minimum_required(VERSION 3.25)
project(ss-project VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/debug)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/inc")

add_executable(assembler src/assembler_main.cpp
    inc/assembler.hpp       src/assembler.cpp
    inc/exception.hpp
    inc/cmd_line_parser.hpp src/cmd_line_parser.cpp
    inc/file_reader.hpp     src/file_reader.cpp
    inc/symbol1.hpp         
    inc/section1.hpp
    inc/shared.hpp          src/shared.cpp
    inc/relocation_item.hpp    
    inc/pool_item.hpp
)

add_executable(linker src/linker_main.cpp
    inc/linker.hpp       src/linker.cpp
    inc/exception.hpp
    inc/place_item.hpp
    inc/shared.hpp          src/shared.cpp
)

add_executable(emulator src/main_emulator.cpp
    inc/emulator.hpp       src/emulator.cpp
    inc/exception.hpp
    inc/shared.hpp         src/shared.cpp
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})