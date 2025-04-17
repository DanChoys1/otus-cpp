// #include "Database.h"
// #include <sqlite3.h>
// #include <stdexcept>
// #include <sstream>

// namespace orm {

// Database::Database(const std::string& dbPath) {
//     if (sqlite3_open(dbPath.c_str(), reinterpret_cast<sqlite3**>(&db)) != SQLITE_OK) {
//         throw std::runtime_error("Failed to open database");
//     }
// }

// Database::~Database() {
//     sqlite3_close(reinterpret_cast<sqlite3*>(db));
// }

// void Database::execute(const std::string& query) {
//     char* errMsg = nullptr;
//     if (sqlite3_exec(reinterpret_cast<sqlite3*>(db), query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
//         std::string error = errMsg;
//         sqlite3_free(errMsg);
//         throw std::runtime_error("SQL error: " + error);
//     }
// }

// template <typename T>
// void Database::save(const T& entity) {
//     std::ostringstream query;
//     query << "INSERT INTO " << T::tableName() << " (";

//     const auto& fields = T::fields();
//     for (size_t i = 0; i < fields.size(); ++i) {
//         if (i > 0) query << ", ";
//         query << fields[i];
//     }

//     query << ") VALUES (";

//     for (size_t i = 0; i < fields.size(); ++i) {
//         if (i > 0) query << ", ";
//         query << "'" << entity.getField(fields[i]) << "'";
//     }

//     query << ");";
//     execute(query.str());
// }

// template <typename T>
// std::vector<T> Database::query(const std::string& condition) {
//     std::ostringstream query;
//     query << "SELECT * FROM " << T::tableName();
//     if (!condition.empty()) {
//         query << " WHERE " << condition;
//     }
//     query << ";";

//     sqlite3_stmt* stmt;
//     if (sqlite3_prepare_v2(reinterpret_cast<sqlite3*>(db), query.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
//         throw std::runtime_error("Failed to prepare statement");
//     }

//     std::vector<T> results;
//     while (sqlite3_step(stmt) == SQLITE_ROW) {
//         T entity;
//         entity.loadFromRow(stmt);
//         results.push_back(entity);
//     }

//     sqlite3_finalize(stmt);
//     return results;
// }

// } // namespace orm
