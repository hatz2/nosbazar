#pragma once

#include <string_view>
#include <charconv>

namespace nosbazar::strings {
	template<typename T>
	T token(std::string_view& s, char delim = ' ');

	template<>
	inline std::string_view token(std::string_view& s, char delim)
	{
		size_t pos = s.find(delim);
		std::string_view token = s.substr(0, pos);
		s.remove_prefix(pos == std::string_view::npos ? s.size() : pos + 1);
		return token;
	}

	template<>
	inline unsigned int token(std::string_view& s, char delim) {
		unsigned int result = 0;
		std::string_view token_str = token<std::string_view>(s, delim);
		std::from_chars(token_str.data(), token_str.data() + token_str.size(), result);
		return result;
	}

	template<>
	inline unsigned short token(std::string_view& s, char delim) {
		return static_cast<unsigned short>(token<unsigned int>(s, delim));
	}

	template<>
	inline unsigned long long token(std::string_view& s, char delim) {
		unsigned long long result = 0;
		std::string_view token_str = token<std::string_view>(s, delim);
		std::from_chars(token_str.data(), token_str.data() + token_str.size(), result);
		return result;
	}

	template<>
	inline unsigned long token(std::string_view& s, char delim) {
		unsigned long result = 0;
		std::string_view token_str = token<std::string_view>(s, delim);
		std::from_chars(token_str.data(), token_str.data() + token_str.size(), result);
		return result;
	}

	template<>
	inline int token(std::string_view& s, char delim) {
		int result = 0;
		std::string_view token_str = token<std::string_view>(s, delim);
		std::from_chars(token_str.data(), token_str.data() + token_str.size(), result);
		return result;
	}

	template<>
	inline std::string token(std::string_view& s, char delim) {
		return std::string(token<std::string_view>(s, delim));
	}

	template<>
	inline bool token(std::string_view& s, char delim) {
		return static_cast<bool>(token<int>(s, delim));
	}
}
