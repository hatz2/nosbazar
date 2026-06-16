#pragma once

#include <fstream>

namespace nosbazar::io {
	template<typename T>
	bool read_value(std::ifstream& file, T& value) {
		return file.read(reinterpret_cast<char*>(&value), sizeof(T)) ? true : false;
	}

	template<typename T>
	bool read_bytes(std::ifstream& file, T* buffer, std::size_t buffer_size) {
		return file.read(reinterpret_cast<char*>(buffer), buffer_size * sizeof(T)) ? true : false;
	}
}