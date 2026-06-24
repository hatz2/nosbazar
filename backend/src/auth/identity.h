#pragma once

#include <string_view>
#include <nlohmann/json.hpp>

namespace nosbazar::auth {
	class Identity {
	public:
		explicit Identity(const std::string& file_path);
		~Identity();

		void update();

		void set_request(const nlohmann::json& request);

		std::string string() const;

		nlohmann::json json() const;

	private:
		void update_vector();

		void update_creation();

		void update_server_time();

		void update_timings();

		void save() const;

		std::string get_server_date() const;

		std::string random_string(int size) const;

		std::string generate_uuid() const;

		std::string generate_vector() const;


		std::string filename;
		nlohmann::json fingerprint;
	};
}