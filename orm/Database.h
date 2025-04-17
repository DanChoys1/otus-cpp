// #pragma once
// #include <string>
// #include <vector>
// #include <type_traits>
// #include "Entity.h"

// namespace orm {

// class Database {
// public:
//     explicit Database(const std::string& dbPath);
//     ~Database();

//     void execute(const std::string& query);

//     template <typename T>
//     void save(const T& entity);

//     template <typename T>
//     std::vector<T> query(const std::string& condition);

// private:
//     void* db; // SQLite3 database handle
// };

// } // namespace orm
