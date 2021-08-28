#include "config.h"

std::vector<std::string> group_role_names() {
    std::vector<std::string> v;

    sqlite3* db;
    int rc;
    rc = sqlite3_open("config.db", &db);

    if (rc) {
        std::cerr << "[ERROR] Couldnt open database: " << sqlite3_errmsg(db)
                  << std::endl;
        sqlite3_close(db);
        return v;
    } else {
        std::cerr << "[NOTICE] Database opened" << std::endl;
    }

    const char* req_sql = "SELECT name FROM groups;";
    
    sqlite3_stmt* req;
    sqlite3_prepare_v2(db, req_sql, -1, &req, &req_sql);

    while (true) {
        rc = sqlite3_step(req);
        if (rc == SQLITE_DONE) break;
        const unsigned char* name_cstr = sqlite3_column_text(req, 0);
        std::string name ((char*) name_cstr);
        v.push_back(name);
        
    }

    sqlite3_close(db);
    
    return v;
}

void setup_database() {
    sqlite3* db;
    int rc;
    rc = sqlite3_open("config.db", &db);

    if (rc) {
        FATAL("Couldnt open database");
        sqlite3_close(db);
    } else {
        NOTE("Database opened");
    }

    const char* sql_init =
        "CREATE TABLE IF NOT EXISTS"
        "groups (id INTEGER AUTO_INCREMENT UNIQUE NOT NULL, name TEXT, role "
        "INT, channel INT);"

        "CREATE TABLE IF NOT EXISTS "
        "reminds (id INTEGER AUTO_INCREMENT UNIQUE NOT NULL, user INT, "
        "channel INT, time TEXT, message TEXT);";
    const char* instr = sql_init;
    sqlite3_stmt* stmt;

    while (instr) {
        sqlite3_prepare_v2(db, instr, -1, &stmt, &instr);
        do {
            rc = sqlite3_step(stmt);
        } while (rc != SQLITE_DONE);
    }

    sqlite3_close(db);
}
