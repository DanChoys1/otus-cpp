
#ifndef CUSTOM_TO_STRING_H
#define CUSTOM_TO_STRING_H

#include <sstream>

template <typename T>
std::string to_string(const T& value)
{
  std::ostringstream ss;
  ss << value;
  return ss.str();
}

#endif