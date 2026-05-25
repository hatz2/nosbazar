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


include_directories("src")



# Add source to this project's executable.
add_executable (nosbazar-backend "src/main.cpp"  "src/noscrypto.h" "src/noscrypto.cpp" "src/net.h" "src/net.cpp" "src/auth/nosauth.h" "src/auth/nosauth.cpp" "src/auth/identity.h" "src/auth/identity.cpp" "src/crypto/base64.h" "src/crypto/base64.cpp" "src/time/timestamp.h" "src/time/datetime.h" "src/random/random.h" "src/auth/blackbox.h" "src/auth/blackbox.cpp" "src/crypto/hash.h" "src/crypto/hash.cpp" "src/crypto/uuid.h" "src/crypto/uuid.cpp" "src/dotenv.h")

target_compile_definitions(nosbazar-backend PRIVATE
    $<$<CONFIG:Debug>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE>
    $<$<CONFIG:Release>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO>
)

target_link_libraries(nosbazar-backend PRIVATE 
	CURL::libcurl 
	spdlog::spdlog
	OpenSSL::SSL
	OpenSSL::Crypto
	Crow::Crow asio::asio
	nlohmann_json::nlohmann_json
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



