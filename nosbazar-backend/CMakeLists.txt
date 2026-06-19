# CMakeList.txt : CMake project for nosbazar-backend, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("nosbazar-backend")



find_package(CURL CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Crow CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(ZLIB REQUIRED)


include_directories("src")
include_directories("third_party")



# Add source to this project's executable.
add_executable (nosbazar-backend "src/main.cpp"  "src/noscrypto.h" "src/noscrypto.cpp" "src/net.h" "src/net.cpp" "src/auth/nosauth.h" "src/auth/nosauth.cpp" "src/auth/identity.h" "src/auth/identity.cpp" "src/crypto/base64.h" "src/crypto/base64.cpp" "src/time/timestamp.h" "src/time/datetime.h" "src/random/random.h" "src/auth/blackbox.h" "src/auth/blackbox.cpp" "src/crypto/hash.h" "src/crypto/hash.cpp" "src/crypto/uuid.h" "src/crypto/uuid.cpp" "src/dotenv.h" "src/packets/login_packet.h" "src/packets/login_packet.cpp" "src/strings/hex.h" "src/strings/hex.cpp" "src/nosclient.h" "src/nosclient.cpp" "src/env.h"   "src/strings/parse.h" "src/auth/token_repository.h" "src/auth/token_repository.cpp" "src/time/interval_timer.h" "src/time/interval_timer.cpp" "src/packets/packet_publisher.h" "src/packets/packet_publisher.cpp" "src/clientless.h" "src/clientless.cpp" "src/game/sensors.h" "src/game/sensors.cpp" "src/packets/in.h" "src/game/enums.h" "src/packets/c_map.h" "src/packets/gp.h" "src/game/constants.h" "src/packets/at.h" "src/packets/cond.h" "src/agent/state_machine.h" "src/agent/state_machine.cpp" "third_party/astar/AStar.hpp" "third_party/astar/AStar.cpp" "src/map_grid_repository.h" "src/map_grid_repository.cpp" "src/agent/walk_state.h" "src/agent/walk_state.cpp" "src/packets/walk.h" "src/agent/agent.h" "src/agent/agent.cpp" "src/agent/walk_think_state.h" "src/agent/walk_think_state.cpp" "src/agent/open_bazar_state.h" "src/agent/open_bazar_state.cpp" "src/time/elapsed_timer.h" "src/time/elapsed_timer.cpp" "src/distances.h" "src/distances.cpp" "src/packets/c_blist.h" "src/agent/search_bazar_state.h" "src/agent/search_bazar_state.cpp" "src/bazar_search_queue.h" "src/bazar_search_queue.cpp" "src/io/file_reader.h" "src/io/nos_text_dat_file_decryptor.cpp" "src/io/nos_text_dat_file_decryptor.h"  "src/io/item_dat_parser.h" "src/io/item_dat_parser.cpp" "src/io/nos_file_text_reader.h" "src/io/nos_file_text_reader.cpp" "src/io/lang_file_parser.h" "src/io/lang_file_parser.cpp" "src/io/lang_manager.h" "src/io/lang_manager.cpp")

target_compile_definitions(nosbazar-backend PRIVATE
    $<$<CONFIG:Debug>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE>
    $<$<CONFIG:Release>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO>
)

target_link_libraries(nosbazar-backend PRIVATE 
	CURL::libcurl 
	spdlog::spdlog
	OpenSSL::SSL
	OpenSSL::Crypto
	Crow::Crow 
    asio::asio
	nlohmann_json::nlohmann_json
    ZLIB::ZLIB
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET nosbazar-backend PROPERTY CXX_STANDARD 23)
endif()

add_custom_command(TARGET nosbazar-backend POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:nosbazar-backend>/assets
)

# Copy .env to build folder
add_custom_command(TARGET nosbazar-backend POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/.env
        $<TARGET_FILE_DIR:nosbazar-backend>/.env
)



