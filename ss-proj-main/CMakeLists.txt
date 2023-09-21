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
    inc/file_writer.hpp     src/file_writer.cpp
    inc/symbol.hpp          src/symbol.cpp
    inc/section.hpp         src/section.cpp

    inc/utils.hpp           src/utils.cpp
)

add_executable(linker src/linker_main.cpp
    inc/linker.hpp src/linker.cpp
    inc/exception.hpp
    inc/cmd_line_parser.hpp src/cmd_line_parser.cpp
    inc/file_reader.hpp     src/file_reader.cpp
    inc/file_writer.hpp     src/file_writer.cpp
    inc/symbol.hpp          src/symbol.cpp
    inc/section.hpp         src/section.cpp
    inc/place.hpp           src/place.cpp
    inc/binary_record.hpp

    inc/utils.hpp           src/utils.cpp
)

add_executable(emulator src/emulator_main.cpp
    inc/emulator.hpp src/emulator.cpp
    inc/binary_record.hpp
    inc/cmd_line_parser.hpp src/cmd_line_parser.cpp
    inc/file_reader.hpp     src/file_reader.cpp
    inc/exception.hpp
    inc/symbol.hpp          src/symbol.cpp
    inc/section.hpp         src/section.cpp
    
    inc/instruction.hpp     src/instruction.cpp
    inc/instruction_executor_factory.hpp src/instruction_executor_factory.cpp 
    inc/instruction_parser.hpp src/instruction_parser.cpp 
    inc/utils.hpp           src/utils.cpp
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
