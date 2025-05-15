cmake_minimum_required(VERSION 3.10)
project(SQLiteUSearchAndroidTest CXX)

# The android-toolchain.cmake might set this, but it's good practice.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable from the current source directory (sqlite/)
add_executable(sqlite_usarch_test_android sqlite_usarch_test.cpp)

# Link against the NDK's SQLite3 library.
# The Android NDK provides libsqlite3.so, and the toolchain should help find it.
# 'sqlite3' is the typical name to link against.
target_link_libraries(sqlite_usarch_test_android sqlite3)

# For sqlite3_load_extension, which uses dlopen:
# On Android, libdl is usually linked automatically or is part of libc.
# If you encounter linker errors for dlopen/dlsym, you might need to add 'dl'.
# target_link_libraries(sqlite_usarch_test_android dl) 
