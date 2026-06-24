#include "token_repository.h"
#include <fstream>
#include <nlohmann/json.hpp>

namespace {
	const std::string file_name = "tokens.json";
}

using json = nlohmann::json;

nosbazar::auth::TokenRepository::TokenRepository()
{
	std::ifstream file(file_name);

	if (!file.is_open()) {
		return;
	}

	json json_file;
	file >> json_file;

	this->tokens = json_file.get<std::unordered_map<std::string, std::string>>();
}

nosbazar::auth::TokenRepository& nosbazar::auth::TokenRepository::instance()
{
	static TokenRepository instance;
	return instance;
}

nosbazar::auth::TokenRepository::~TokenRepository()
{
	save();
}

std::optional<std::string> nosbazar::auth::TokenRepository::get_token(const std::string& email) const
{
	if (tokens.contains(email)) {
		return tokens.at(email);
	}

	return std::nullopt;
}

void nosbazar::auth::TokenRepository::add_token(const std::string& email, const std::string& token)
{
	tokens[email] = token;
}

void nosbazar::auth::TokenRepository::save() const
{
	std::ofstream file(file_name);
	json json_data = tokens;
	file << json_data.dump(4);
}
