#include "File.h"
#include <Debug.h>
#include <cstdio>
#include <fstream>

/**

These functions are implemented with an old C style since
they are at least slightly faster (and some times quite a bit)
and they are used for some stuff we want to go as fast as it can
like loading/saving entity and component state.

*/


bool file_exists(const char* filename) {
	if (auto file = fopen(filename, "r")) {
		fclose(file);
		return true;
	}
	return false;
}

void write_to_file(const char * filename, const char * content) {

	auto file = fopen(filename, "w");
	if (file == nullptr) {
		LOG("We couldn't open " << filename << " to write to");
	}
	else {
		fputs(content, file);
		fclose(file);
	}

}

void append_to_file(const char * filename, const char * content) {

	auto file = fopen(filename, "a");
	if (file == nullptr) {
		LOG("We couldn't open " << filename << " to write to");
	}
	else {
		fputs(content, file);
		fclose(file);
	}

}

std::string read_from_file(const char* filename) {
	auto file = fopen(filename, "rb");
	if (file == nullptr) {
		LOG("We couldn't open " << filename << " to read from");
	}
	else {
		auto content = std::string{};
		fseek(file, 0, SEEK_END);
		content.resize(ftell(file));
		rewind(file);
		fread(&content[0], 1, content.size(), file);
		fclose(file);
		return content;
	}
	return "{}";
}

std::string extract_filename(const char * full_path) {

	auto s = std::string(full_path);

	char sep = '/';

#ifdef _WIN32
	sep = '\\';
#endif

	size_t i = s.rfind(sep, s.length());
	if (i != std::string::npos) {
		return(s.substr(i + 1, s.length() - i));
	}

	return("");
}


std::string extract_path(const char * full_path) {

	auto s = std::string(full_path);

	char sep = '/';

#ifdef _WIN32
	sep = '\\';
#endif

	size_t i = s.rfind(sep, s.length());
	if (i != std::string::npos) {
		return(s.substr(0, i + 1));
	}

	return("");
}

