#include "map_grid_repository.h"
#include <fstream>
#include <filesystem>
#include <spdlog/spdlog.h>

nosbazar::MapGrid::MapGrid(const std::string& file_path)
{
	std::ifstream file(file_path, std::ios_base::binary);

	if (!file.is_open()) {
		return;
	}

	file.read(reinterpret_cast<char*>(&width), sizeof(width));
	file.read(reinterpret_cast<char*>(&height), sizeof(height));

	generator.setWorldSize({ width, height});
	//generator.setDiagonalMovement(true);
	generator.setHeuristic(AStar::Heuristic::octagonal);

	cells.reserve(height);

	uint8_t cell_type;
	for (int y = 0; y < height; ++y) {
		std::vector<CellType> row_cells;
		row_cells.reserve(width);

		for (int x = 0; x < width; ++x) {
			file.read(reinterpret_cast<char*>(&cell_type), sizeof(CellType));
			row_cells.push_back(cell_type ? CellType::obstacle : CellType::walkable);

			if (cell_type) {
				generator.addCollision({ x , y });
			}
		}

		cells.push_back(std::move(row_cells));
	}
}

bool nosbazar::MapGrid::is_walkable(const Cell& cell) const
{
	int x = cell.x;
	int y = cell.y;
	if (x < width && y < height) {
		return cells[y][x] == CellType::walkable;
	}

	return false;
}

std::vector<nosbazar::MapGrid::Cell> nosbazar::MapGrid::find_path(const Cell& origin, const Cell& destination)
{
	return generator.findPath(
		{ origin.x, origin.y}, 
		{ destination.x, destination.y}
	);
}

nosbazar::MapGridRepository& nosbazar::MapGridRepository::instance()
{
	static MapGridRepository instance;
	return instance;
}

std::optional<std::reference_wrapper<nosbazar::MapGrid>> nosbazar::MapGridRepository::grid(const int map_id)
{
	if (grids.contains(map_id)) {
		return grids.at(map_id);
	}

	return std::nullopt;
}

nosbazar::MapGridRepository::MapGridRepository()
{
	std::filesystem::path directory("assets/maps");

	try {
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(directory)) {
			if (entry.is_regular_file()) {
				std::string filename = entry.path().filename().string();
				std::string map_id_name = filename.substr(0, filename.find('.'));
				int map_id;
				auto [ptr, ec] = std::from_chars(filename.data(), filename.data() + filename.size(), map_id);

				if (ec != std::errc()) {
					SPDLOG_ERROR("Could not convert string to integer: {}", map_id_name);
					continue;
				}

				grids.emplace(map_id, MapGrid(entry.path().string()));
			}
		}
	}
	catch (std::filesystem::filesystem_error err) {
		SPDLOG_CRITICAL(err.what());
	}
}
