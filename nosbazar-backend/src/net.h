#pragma once

#include <curl/curl.h>
#include <string>
#include <string_view>
#include <expected>
#include <vector>

namespace nosbazar::net {
	struct Response {
		int status_code;
		std::string body;
	};

	enum class Error {
		curl_init,
		curl_easy_perform,
	};

	std::expected<Response, Error> post(std::string_view url, std::string_view body, const std::vector<std::string>& headers = {});

	std::expected<Response, Error> get(std::string_view url, const std::vector<std::string>& headers = {});
}