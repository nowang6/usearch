#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>

// Callback function for sqlite3_exec (can be useful for simple queries)
// For this example, we'll use sqlite3_prepare_v2 and sqlite3_step for more control.
/*
static int callback(void *data, int argc, char **argv, char **azColName) {
    fprintf(stderr, "%s: ", (const char*)data);
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
*/

int main(int argc, char **argv) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    // 1. Open database
    if (sqlite3_open(":memory:", &db)) {
        std::cerr << "ERROR: Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // 2. Enable loadable extensions
    if (sqlite3_enable_load_extension(db, 1) != SQLITE_OK) {
        std::cerr << "ERROR: Failed to enable loadable extensions: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    // 3. Load the USearch extension
    // ### IMPORTANT: Adjust this path to your compiled USearch SQLite extension ###
    const char* extension_path = "./libusearch_sqlite.so"; 
    if (argc > 1) { // Still allow override via command line for convenience
        extension_path = argv[1];
        std::cout << "Using extension path from command line: " << extension_path << std::endl;
    } else {
        std::cout << "Using default extension path: " << extension_path << std::endl;
        std::cout << "(You can provide the path as the first command line argument to override this)" << std::endl;
    }

    if (sqlite3_load_extension(db, extension_path, 0, &zErrMsg) != SQLITE_OK) {
        std::cerr << "ERROR: Failed to load extension '" << extension_path << "': " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return 1;
    }
    std::cout << "USearch SQLite extension loaded." << std::endl;

    // 4. Create table and insert data
    const char* sql_setup = R"SQL(
        CREATE TABLE vectors_table (id INTEGER PRIMARY KEY, vector TEXT NOT NULL);
        INSERT INTO vectors_table (id, vector) VALUES (42, '[1.0, 2.0, 3.0]');
    )SQL";
    if (sqlite3_exec(db, sql_setup, 0, 0, &zErrMsg) != SQLITE_OK) {
        std::cerr << "SQL error (setup): " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return 1;
    }
    std::cout << "Table created and data inserted." << std::endl;

    // 5. Perform SELECT query using a USearch distance function
    const char* sql_select = R"SQL(
        SELECT id, distance_cosine_f32(vector, '[0.5, 1.5, 2.5]') AS dist FROM vectors_table WHERE id = 42;
    )SQL";
    
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql_select, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "SQL error (SELECT prepare): " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    std::cout << "\nQuery Result:" << std::endl;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        double distance = sqlite3_column_double(stmt, 1);
        std::cout << "ID: " << id << ", Cosine Distance: " << distance << std::endl;
    } else {
        std::cout << "No rows/Error in execution." << std::endl;
    }

    sqlite3_finalize(stmt);

    // 6. Close database
    sqlite3_close(db);
    std::cout << "\nDatabase closed." << std::endl;

    return 0;
}

/*
Simplified Compilation (example g++ on Linux):
g++ sqlite_usarch_test.cpp -o sqlite_usarch_test -lsqlite3 -ldl

Run:
./sqlite_usarch_test [optional_path_to/libusearch_sqlite.so]
*/ 
