#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <regex>
#include <exception>
#include <stdexcept>
#include  <iomanip>
#include "exception.hpp"

using namespace std;

string clear_line(const string &s);

int convert_hex_to_decimal(const string &s, bool is_number_12_bit = false);

unsigned int convert_hex_to_unsigned_decimal(const string &s);

bool is_string_correct(const string &s);

string replace_special_characters(const string &s);

unsigned int create_byte(unsigned int four_bits_7654, unsigned int four_bits_3210);

vector<unsigned int> split_number_in_nibbles(unsigned int number, int numOfNibbles);

vector<unsigned int> split_number_in_bytes(unsigned int number, int numOfBytes, bool littleEndian);

bool is_literal_12_bits_signed(int number);

string convert_int_to_hex(int number);

string convert_int_to_hex_with_setw(int number, int places);

vector<uint8_t> em_split_number_in_bytes(uint32_t number, bool littleEndian = false);

uint32_t em_generate_data_from_bytes(vector<uint8_t> bytes, bool littleEndian = false);

#endif /* UTILS_H */
