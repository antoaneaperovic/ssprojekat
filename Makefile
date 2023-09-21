CC = g++

all: assembler linker emulator

clean:
	rm assembler
	rm linker
	rm emulator

assembler:
	$(CC) -std=c++11 -g -pthread -o assembler \
	src/assembler_main.cpp \
    inc/assembler.hpp       src/assembler.cpp \
    inc/exception.hpp \
    inc/cmd_line_parser.hpp src/cmd_line_parser.cpp \
    inc/file_reader.hpp     src/file_reader.cpp \
    inc/file_writer.hpp     src/file_writer.cpp \
    inc/symbol.hpp          src/symbol.cpp \
    inc/section.hpp         src/section.cpp \
    inc/utils.hpp           src/utils.cpp \
	-Iinc

linker:
	$(CC) -std=c++11 -g -pthread -o linker \
	src/linker_main.cpp \
    inc/linker.hpp src/linker.cpp \
    inc/exception.hpp \
    inc/cmd_line_parser.hpp src/cmd_line_parser.cpp \
    inc/file_reader.hpp     src/file_reader.cpp \
    inc/file_writer.hpp     src/file_writer.cpp \
    inc/symbol.hpp          src/symbol.cpp \
    inc/section.hpp         src/section.cpp \
    inc/place.hpp           src/place.cpp \
    inc/binary_record.hpp \
    inc/utils.hpp           src/utils.cpp \
	-Iinc

emulator:
	$(CC) -std=c++11 -g -pthread -o emulator \
	src/emulator_main.cpp \
    inc/emulator.hpp src/emulator.cpp \
    inc/binary_record.hpp \
    inc/cmd_line_parser.hpp src/cmd_line_parser.cpp \
    inc/file_reader.hpp     src/file_reader.cpp \
    inc/exception.hpp \
    inc/symbol.hpp          src/symbol.cpp \
    inc/section.hpp         src/section.cpp \
    inc/instruction.hpp     src/instruction.cpp \
    inc/instruction_executor_factory.hpp src/instruction_executor_factory.cpp \
    inc/instruction_parser.hpp src/instruction_parser.cpp \
    inc/utils.hpp           src/utils.cpp \
	-Iinc
