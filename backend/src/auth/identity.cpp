#include "identity.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <time/timestamp.h>
#include <time/datetime.h>
#include <crypto/base64.h>
#include <random/random.h>
#include <net.h>
#include <spdlog/spdlog.h>

using json = nlohmann::json;

namespace nosbazar::auth {
	Identity::Identity(const std::string& file_path) : file_path(file_path)
	{
		load_from_disk();
	}

	void Identity::update()
	{
		load_from_disk();
		update_vector();
		update_server_time();
		update_creation();
		update_timings();
		save();
	}

	void Identity::update_vector()
	{
		int64_t current_time_in_ms = time::current_timestamp_ms();
		std::vector<uint8_t> decoded = crypto::base64_decode(fingerprint["vector"]);
		std::string content(decoded.begin(), decoded.end());
		size_t last_blank_index = content.find_last_of(' ');
		if (last_blank_index == std::string::npos) {
			return;
		}
		const std::string old_time_str = content.substr(last_blank_index + 1);
		const long long old_time = std::stoll(old_time_str);

		content = content.substr(0, last_blank_index);

		// old_time + 1000 < current_time_in_ms
		if (old_time + 0x3e8 < current_time_in_ms)
		{
			if (!content.empty())
			{
				content.erase(0, 1);
			}

			content += random::random_ascii_character();
		}

		const std::string new_vector = content + " " + std::to_string(current_time_in_ms);

		std::vector<uint8_t> new_vector_bytes(new_vector.begin(), new_vector.end());

		fingerprint["vector"] = crypto::base64_encode(new_vector_bytes);
	}

	void Identity::update_creation()
	{
		fingerprint["creation"] = time::current_datetime_iso8601_utc_ms();
	}

	void Identity::update_server_time()
	{
		fingerprint["serverTimeInMS"] = get_server_date();
	}

	void Identity::update_timings()
	{
		fingerprint["d"] = random::random_int(150, 300);
	}


	void Identity::set_request(const nlohmann::json& request)
	{
		fingerprint["request"] = request;
	}

	void Identity::load_from_disk()
	{
		std::ifstream file(file_path);
		std::ostringstream content;

		if (!file.is_open()) {
			SPDLOG_ERROR("Identity file not found at {}", file_path);
			return;
		}

		content << file.rdbuf();

		fingerprint = json::parse(content.str());

		file.close();
	}

	std::string Identity::string() const
	{
		return fingerprint.dump();
	}

	nlohmann::json Identity::json() const
	{
		return fingerprint;
	}

	void Identity::save() const
	{
		std::ofstream file(file_path);

		if (!file.is_open()) {
			return;
		}

		auto content = fingerprint.dump(4);
		file.write(content.c_str(), content.size());
		file.close();
	}
	std::string Identity::get_server_date() const
	{
		auto result = net::get("https://gameforge.com/tra/game1.js");
		if (!result) {
			return time::current_datetime_iso8601_utc_ms();
		}

		auto& response = *result;
		auto date_str = response.headers["Date"];
		if (date_str.empty()) {
			return time::current_datetime_iso8601_utc_ms();
		}

		return time::parse_http_date(date_str);
	}
	std::string Identity::random_string(int size) const
	{
		std::string str;

		for (int i = 0; i < size; ++i) {
			char r = random::random_ascii_character();
			str.push_back(r);
		}

		return str;
	}
	

	std::string Identity::generate_uuid() const
	{
		constexpr int uuid_length = 27;
		std::string str = random_string(uuid_length);
		std::vector<uint8_t> raw(str.begin(), str.end());
		std::string result = crypto::base64_encode(raw);

		std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
			return std::tolower(c);
		});

		return result;
	}

	std::string Identity::generate_vector() const
	{
		constexpr int vector_length = 100;
		std::string str = random_string(vector_length);
		uint64_t time = time::current_timestamp_ms();
		std::string vec = str + " " + std::to_string(time);
		std::vector<uint8_t> raw(vec.begin(), vec.end());
		return crypto::base64_encode(raw);
	}
}