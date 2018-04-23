#pragma once

#include <string>

bool file_exists(const char* filename);

void write_to_file(const char* filename, const char* content);

void append_to_file(const char* filename, const char* content);

std::string read_from_file(const char* filename);

std::string extract_filename(const char* full_path);

