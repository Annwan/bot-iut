#include "config.h"

std::vector<std::string> group_role_names() {
    std::vector<std::string> v;

    sqlite3* db;
    int rc = sqlite3_open("config.db", &db);

    if (rc) {
        std::cerr << "[ERROR] Couldnt open database: " << sqlite3_errmsg(db)
                  << std::endl;
        sqlite3_close(db);
        return v;
    } else {
        std::cerr << "[NOTICE] Database opened" << std::endl;
    }

    // TODO actually read the db not hardcode it
    v.push_back("g1s1");
    v.push_back("g2s1");
    v.push_back("g3s1");
    v.push_back("g4s1");
    v.push_back("g5s1");
    v.push_back("g6s2");
    v.push_back("g1s3");
    v.push_back("g2s3");
    v.push_back("g3s3");
    v.push_back("g4s3");
    v.push_back("g5s3");
    v.push_back("g6s4A");
    v.push_back("g6s4B");
    v.push_back("vieux");
    return v;
}

void setup_database() {
    sqlite3* db;
    int rc = sqlite3_open("config.db", &db);

    if (rc) {
        logging::err("Couldnt open database");
        sqlite3_close(db);
    } else {
        logging::note("Database opened");
    }

    const char* sql_init = "";
    const char* next_statement = NULL;
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql_init, -1, &stmt, &next_statement);
}
