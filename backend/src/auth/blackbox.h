#pragma once

#include <string>
#include "identity.h"
#include <nlohmann/json.hpp>
#include <memory>

namespace nosbazar::auth {
	class Blackbox {
	public:
		explicit Blackbox(std::shared_ptr<Identity> identity);

		std::string encoded() const;

	protected:
		static inline const std::vector<std::string> fields = { "v", "tz", "osType", "app", "vendor", "mem", 
			"con", "lang", "plugins", "gpu", "fonts", "audioC", "width", "height", "video", "audio", "media", 
			"permissions", "audioFP", "webglFP", "canvasFP", "creation", "uuid", "d", "osVersion", "vector", 
			"userAgent", "serverTimeInMS", "request", "browserEnvMask" };

		std::shared_ptr<Identity> identity;
	};

	class EncryptedBlackbox : public Blackbox {
	public:
		EncryptedBlackbox(std::shared_ptr<Identity> identity, std::string account_id, std::string gsid, std::string installation_id);

		std::string encrypted() const;

	private:
		std::string account_id;
		std::string gsid;
	};
}