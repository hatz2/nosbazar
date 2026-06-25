#pragma once

#include <string>
#include <vector>
#include <span>

namespace nosbazar::nosclient {
	struct FileInfo {
		std::string dwnload_rel_path;
		std::string sha1;
		std::string filename;
		std::string output_path;
		int flags;
		int size;
		bool folder;
	};

	void check_and_download_outdated_files();

	std::vector<FileInfo> get_outdated_files(std::span<const FileInfo> remote_files_info);

	std::vector<FileInfo> get_remote_client_file_info();

	void download_files(std::span<const FileInfo> files_info);

	std::string get_local_file_md5(const std::string& file_path);

	std::string get_md5_for_login_packet();

    std::string get_nostale_client_version();
}