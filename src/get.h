#pragma once
#include <iostream>

int get_month(std::string &par_line);
float get_amount_line_float(std:: string &par_line);
std::string get_amount_line_string(std::string &par_line);
std::string get_type(std::string &par_line);
std::string get_category(std::string &par_line);
float get_amount_total(const int par_MONTH, const std::string par_TYPE, const std::string par_CATEGORY);
std::string get_date(std::string &par_line);
std::string get_description(std::string &par_line);
std::string get_current_date();
int get_current_month();
float get_amount_category(std::string &par_category, int type = 0);
