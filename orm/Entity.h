// #include <iostream>
// #include <string>
// #include <vector>
// #include <memory>
// #include <unordered_map>
// #include <typeinfo>
// #include <typeindex>
// #include <sstream>

// // Forward declarations
// class Database;

// // Field descriptor
// template <typename Class, typename FieldType>
// class Field {
//     FieldType Class::* member_ptr;
//     std::string field_name;
//     std::string sql_type;

// public:
//     Field(FieldType Class::* ptr, const std::string& name, const std::string& type)
//         : member_ptr(ptr), field_name(name), sql_type(type) {}

//     const std::string& name() const { return field_name; }
//     FieldType get(const Class& obj) const { return obj.*member_ptr; }
//     void set(Class& obj, FieldType value) const { obj.*member_ptr = value; }
// };

// // Query builder 
// template <typename T>
// class Query {
//     struct Condition {
//         std::string expression;
//     };

//     std::vector<Condition> conditions;
//     std::string table_name;

//     template <typename FieldType>
//     static std::string value_to_sql(FieldType value) {
//         if constexpr (std::is_same_v<FieldType, std::string> || 
//                      std::is_same_v<FieldType, const char*>) {
//             return "'" + std::string(value) + "'";
//         } else {
//             return std::to_string(value);
//         }
//     }

// public:
//     Query(const std::string& table) : table_name(table) {}

//     template <typename FieldType>
//     Query<T>& where(const Field<T, FieldType>& field, const std::string& op, FieldType value) {
//         conditions.push_back({field.name() + op + value_to_sql(value)});
//         return *this;
//     }

//     std::string build() const {
//         std::string sql = "SELECT * FROM " + table_name;
//         if (!conditions.empty()) {
//             sql += " WHERE ";
//             for (size_t i = 0; i < conditions.size(); ++i) {
//                 if (i > 0) sql += " AND ";
//                 sql += conditions[i].expression;
//             }
//         }
//         return sql;
//     }
// };

// // Database class
// class Database {
//     std::unordered_map<std::type_index, std::string> table_names;

// public:
//     Database() = default;

//     template <typename T>
//     void register_table(const std::string& table_name) {
//         table_names[typeid(T)] = table_name;
//     }

//     template <typename T>
//     std::vector<T> get(Query<T> query) {
//         std::string sql = query.build();
//         std::cout << "Executing: " << sql << std::endl;
//         return {}; // In real implementation, fetch and return actual data
//     }
// };

// // User class - completely clean
// class User {
// public:
//     int id = 0;
//     std::string name;
//     int age = 0;
//     std::string email;
// };

// // Field declarations
// namespace UserFields {
//     static Field<User, int> id = {&User::id, "id", "INTEGER PRIMARY KEY"};
//     static Field<User, std::string> name = {&User::name, "name", "TEXT NOT NULL"};
//     static Field<User, int> age = {&User::age, "age", "INTEGER"};
//     static Field<User, std::string> email = {&User::email, "email", "TEXT"};
// }

// // Query syntax helpers
// template <typename T>
// Query<T> query(const std::string& table_name) {
//     return Query<T>(table_name);
// }

// template <typename T, typename FieldType>
// Query<T> operator==(Query<T> q, const std::pair<Field<T, FieldType>, FieldType>& condition) {
//     return q.where(condition.first, " = ", condition  second);
// }

// int main() {
//     Database db;
//     db.register_table<User>("users");

//     // Query 1: Simple equality
//     auto q1 = query<User>("users")  == std::make_pair(UserFields  name, "Anton");
//     auto users = db.get(q1);

//     // Query 2: More complex
//     auto q2 = query<User>("users")
//         .where(UserFields  name, " = ", "Anton")
//         .where(UserFields  age, " > ", 18);
//     auto adult_users = db.get(q2);

//     std::cout << "Queries executed successfully!" << std::endl;
//     return 0;
// }