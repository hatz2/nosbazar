#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <unordered_map>
#include <optional>
#include <astar/AStar.hpp>

namespace nosbazar {
	class MapGrid {
	public:
		using Cell = AStar::Vec2i;

		enum class CellType : uint8_t {
			walkable,
			obstacle
		};

		explicit MapGrid(const std::string& file_path);

		[[nodiscard]] bool is_walkable(const Cell& cell) const;

		[[nodiscard]] inline uint16_t get_width() const { return width; }
		[[nodiscard]] inline uint16_t get_height() const { return height; }

		std::vector<Cell> find_path(const Cell& origin, const Cell& destination);

	private:
		uint16_t width{};
		uint16_t height{};
		std::vector<std::vector<CellType>> cells;

		AStar::Generator generator;
	};
	class MapGridRepository {
	public:
		static MapGridRepository& instance();

		[[nodiscard]] std::optional<std::reference_wrapper<MapGrid>> grid(const int map_id);

	private:
		explicit MapGridRepository();
		MapGridRepository(const MapGridRepository&) = delete;
		void operator=(const MapGridRepository&) = delete;

		std::unordered_map<int, MapGrid> grids;

	};
}