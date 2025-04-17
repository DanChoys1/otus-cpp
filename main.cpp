#include <iostream>
#include <string>
#include <utility>
#include <type_traits>

using namespace std;

namespace std
{
    const string& to_string(const string& str)
    {
        return str;
    }
}

struct Condistion
{
    string condistion;

    Condistion& operator&& (const Condistion& cond)
    {
        condistion = "(" + condistion + "and" + cond.condistion + ")";
        return *this;
    }
    Condistion& operator|| (const Condistion& cond)
    {
        condistion = "(" + condistion + "or" + cond.condistion + ")";
        return *this;
    }
    Condistion& operator! ()
    {
        condistion = "not (" + condistion + ")";
        return *this;
    }
};

template <typename T>
struct Field
{
    Field(string name) :
        _name(name)
    {}
    Condistion operator== (const T& val)
    {
        return Condistion{ .condistion = _name + "=" + std::to_string(val) };
    }
    Condistion operator!= (const T& val)
    {
        return !(operator==(val));
    }

private:
    std::string _name;
};
// Создание поля
#define INST_FIELD(type, field) \
    Field<decltype(std::declval<type>().field)> field{#field}

template <typename T>
struct Query {};
// Создание перегрузки запроса для конкретного типа и полей
#define INST_CLASS(type, ...) \
    template <> \
    struct Query<type> { \
        FOR_EACH_FIELDS(INST_FIELD, type, __VA_ARGS__) \
    }

#define PARENS ()
#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

// Создает поля для каждого аргумента
// Максимум 342 аргумента
#define FOR_EACH_FIELDS(macro, type, ...)                                \
  __VA_OPT__(EXPAND(FOR_EACH_FIELDS_HELPER(macro, type, __VA_ARGS__)))
#define FOR_EACH_FIELDS_HELPER(macro, type, field, ...)                  \
  macro(type, field);                                                    \
  __VA_OPT__(FOR_EACH_FIELDS_AGAIN PARENS (macro, type, __VA_ARGS__))
#define FOR_EACH_FIELDS_AGAIN() FOR_EACH_FIELDS_HELPER

class MyClass
{
public:
    int val;
    string val2;
    double val3;
};
INST_CLASS(MyClass, val, val2, val3);

int main() 
{    
    return 0;
}
