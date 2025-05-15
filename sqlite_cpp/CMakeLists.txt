cmake_minimum_required(VERSION 3.10)
project(SQLiteUSearchTest CXX)

# 启用 C 语言支持
enable_language(C)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Fetch SQLite amalgamation
include(FetchContent)
FetchContent_Declare(
    sqlite3
    URL https://sqlite.org/2024/sqlite-amalgamation-3450200.zip
)
FetchContent_MakeAvailable(sqlite3)

# 构建 SQLite 静态库（或改为 SHARED）
add_library(sqlite3_static STATIC ${sqlite3_SOURCE_DIR}/sqlite3.c)
target_include_directories(sqlite3_static PUBLIC ${sqlite3_SOURCE_DIR})
set_target_properties(sqlite3_static PROPERTIES OUTPUT_NAME "sqlite3")

# 可选：设置供其他模块使用的变量（兼容 FindSQLite3 用法）
set(SQLite3_INCLUDE_DIR ${sqlite3_SOURCE_DIR})
set(SQLite3_LIBRARY $<TARGET_FILE:sqlite3_static>)

# 添加你的主程序
add_executable(sqlite_usarch_test sqlite_usarch_test.cpp)
target_include_directories(sqlite_usarch_test PRIVATE ${SQLite3_INCLUDE_DIR})
target_link_libraries(sqlite_usarch_test PRIVATE sqlite3_static dl)
