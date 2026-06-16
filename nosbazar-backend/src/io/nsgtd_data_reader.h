#pragma once

#include <string>

namespace nosbazar::io {
	class NSgtdDataReader {
	public:
		NSgtdDataReader(const std::string& file_path);
		NSgtdDataReader();

		inline std::string get_item_dat_file() const { return item_dat_file; };
		
	private:
		void read(const std::string& file_path);

		// TODO: Add file classes to read their fields
		std::string item_dat_file;
	};
}
