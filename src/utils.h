#pragma once
#include <iostream>
#include <sstream>

void clear_refresh();
void add_trailing_zeros(std::string &str);

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 2)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}
