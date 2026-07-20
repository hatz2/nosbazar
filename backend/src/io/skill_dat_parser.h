#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

namespace nosbazar::io {

struct Skill {
	uint32_t vnum{};
	std::string name{};

	nlohmann::json json() const;
};

class SkillDatParser {
public:
	static SkillDatParser& instance();

	void parse(const std::string& file_content);

	const Skill& skill_data(uint32_t vnum) const;
	bool has_skill(uint32_t vnum) const;

private:
	SkillDatParser() = default;
	SkillDatParser(SkillDatParser&) = delete;
	void operator=(SkillDatParser&) = delete;

	std::unordered_map<uint32_t, Skill> skills;
};

}
