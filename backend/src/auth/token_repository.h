#pragma once

#include <unordered_map>
#include <string>
#include <optional>

namespace nosbazar::auth {
	class TokenRepository {
	public:
		static TokenRepository& instance();

		~TokenRepository();

		[[nodiscard]] std::optional<std::string> get_token(const std::string& email) const;

		void add_token(const std::string& email, const std::string& token);

	private:
		TokenRepository();
		TokenRepository(const TokenRepository& other) = delete;
		TokenRepository(TokenRepository&& other) = delete;

		std::unordered_map<std::string, std::string> tokens;
	};
}