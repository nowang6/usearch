#!/bin/bash

# This script packages the sqlite_usarch_test program and its dependencies
# into a distributable directory named 'usearch_sqlite_dist'.

# Ensure this script is run from the 'usearch' project root directory.

# Configuration - Adjust these paths if your files are located elsewhere
# Assumes 'sqlite_usarch_test' executable is in the project root
# and shared libraries are in the 'build/' directory as discussed.
TEST_EXECUTABLE_NAME="build_test/sqlite_usarch_test"
USEARCH_SQLITE_SO_PATH="build/libusearch_sqlite.so"
SQLITE3_SO_PATH="build/sqlite/libsqlite3.so"
DIST_DIR="usearch_sqlite_dist"
LIB_DIR="${DIST_DIR}/lib"

echo "Starting packaging process..."

# 0. Check for test executable (minimal check)
if [ ! -f "./${TEST_EXECUTABLE_NAME}" ]; then
    echo "ERROR: Test executable './${TEST_EXECUTABLE_NAME}' not found. Compile it first."
    exit 1
fi

# 1. Create directories
rm -rf "${DIST_DIR}"
mkdir -p "${LIB_DIR}"

# 2. Copy files
cp "./${TEST_EXECUTABLE_NAME}" "${DIST_DIR}/"
cp "${USEARCH_SQLITE_SO_PATH}" "${LIB_DIR}/"
cp "${SQLITE3_SO_PATH}" "${LIB_DIR}/"

echo "Minimal packaging complete. Files in '${DIST_DIR}'."
echo "To run on another machine (from within the '${DIST_DIR}' directory):"
echo "  export LD_LIBRARY_PATH=\`pwd\`/lib:\$LD_LIBRARY_PATH"
echo "  ./${TEST_EXECUTABLE_NAME} lib/libusearch_sqlite.so"

exit 0 
