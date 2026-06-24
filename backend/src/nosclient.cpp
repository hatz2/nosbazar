#include "nosclient.h"
#include <crypto/hash.h>
#include <fstream>
#include <net.h>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <unordered_set>

using json = nlohmann::json;

namespace {
	const std::unordered_set<std::string> files_to_look = { "NostaleClient.exe", "NostaleClientX.exe" };

	std::string download_file(std::string_view remote_rel_path) {
		const std::string url = fmt::format("http://patches.gameforge.com/{}", remote_rel_path);
		auto reply = nosbazar::net::get(url);

		if (!reply) {
			SPDLOG_DEBUG("download_file got no reply");
			return {};
		}

		if (reply->status_code != 200) {
			SPDLOG_DEBUG("download_file status {} body {}", reply->status_code, reply->body);
			return {};
		}

		return reply->body;
	}

	std::optional<std::string> read_binary_file_content(const std::string& path) {
		std::ifstream file(path, std::ios_base::binary);

		if (!file.is_open()) {
			return std::nullopt;
		}

		std::ostringstream content;
		content << file.rdbuf();

		return content.str();
	}
}

void nosbazar::nosclient::check_and_download_outdated_files()
{
	std::vector<FileInfo> file_infos = get_remote_client_file_info();
	std::vector<FileInfo> outdated_files = get_outdated_files(file_infos);
	download_files(outdated_files);
}

std::vector<nosbazar::nosclient::FileInfo> nosbazar::nosclient::get_outdated_files(std::span<const FileInfo> files_info)
{
	std::vector<FileInfo> result;

	for (const auto& file_info : files_info) {
		auto file_content = read_binary_file_content(file_info.filename);

		if (!file_content) {
			result.emplace_back(file_info);
			continue;
		}

		std::string local_sha1 = crypto::sha1(file_content.value());

		if (local_sha1 != file_info.sha1) {
			result.emplace_back(file_info);
		}
	}

	return result;
}

std::vector<nosbazar::nosclient::FileInfo> nosbazar::nosclient::get_remote_client_file_info()
{
	std::string_view url = "https://spark.gameforge.com/api/v1/patching/download/latest/nostale/default?locale=en&architecture=x64&branchToken";

	std::vector<std::string> headers = {
		"Content-Type: application/x-www-form-urlencoded",
	};

	auto reply = net::get(url, headers);

	std::vector<FileInfo> result;

	if (reply) {
		SPDLOG_DEBUG("get_remote_nostaleclientx_hash: {}", reply->body);
		
		json response = json::parse(reply->body);

		if (response.contains("entries")) {
			for (const auto& entry : response["entries"]) {
				std::string file = entry["file"];

				if (files_to_look.contains(file)) {
					result.push_back({
						.dwnload_rel_path = entry["path"],
						.sha1 = entry["sha1"],
						.filename = entry["file"],
						.flags = entry["flags"],
						.folder = entry["folder"],
					});
				}
			}
		}
	}

	return result;
}

void nosbazar::nosclient::download_files(std::span<const FileInfo> files_info)
{
	for (const auto& info : files_info) {
		std::string file_content = download_file(info.dwnload_rel_path);

		if (file_content.empty()) {
			continue;
		}

		std::ofstream file(info.filename, std::ios_base::binary);
		if (file.is_open()) {
			file.write(file_content.c_str(), file_content.size());
		}
	}
}

std::string nosbazar::nosclient::get_local_file_md5(const std::string& file_path)
{
	std::optional<std::string> file_content = read_binary_file_content(file_path);

	if (file_content) {
		return nosbazar::crypto::md5(file_content.value(), true);
	}

	return {};

}

std::string nosbazar::nosclient::get_md5_for_login_packet()
{
	bool uppercase = true;
	return crypto::md5(get_local_file_md5("NostaleClientX.exe") + get_local_file_md5("NostaleClient.exe"), uppercase);
}

std::string get_version_from_windows_executable_buffer(const std::vector<uint8_t>& buffer) {
	// "FileVersion" in UTF-16LE (each letter followed by 0x00)
	// F(70) i(105) l(108) e(101) V(86) e(101) r(114) s(115) i(105) o(111) n(110)
	const uint8_t pattern[] = {
		70, 0, 105, 0, 108, 0, 101, 0, 86, 0, 101, 0, 114, 0, 115, 0, 105, 0, 111, 0, 110, 0
	};
	size_t pattern_size = sizeof(pattern);

	std::string result;

	for (size_t i = 0; i <= buffer.size() - pattern_size; ++i) {
		if (memcmp(&buffer[i], pattern, pattern_size) == 0) {
			// Version is usually some bytes ahead.
			// We skip 0 byte characters
			for (size_t j = i + pattern_size; j < i + pattern_size + 60; ++j) {
				if (buffer[j] == '"') {
					break;
				}

				// Only ASCII characters
				if (buffer[j] >= 32 && buffer[j] <= 126) { 
					result.push_back(buffer[j]);
				}
			}
			return result;
		}
	}

	return result;
}

std::string nosbazar::nosclient::get_nostale_client_version()
{
	auto raw_binary = read_binary_file_content("NostaleClientX.exe");

	if (raw_binary) {
		std::vector<uint8_t> buffer(raw_binary.value().begin(), raw_binary.value().end());
		return get_version_from_windows_executable_buffer(buffer);
	}

	return {};
}
