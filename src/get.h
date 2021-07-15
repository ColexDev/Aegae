#pragma once
#include <iostream>

int getTimeFrame(std::string &par_line);
float getAmountLine(std:: string &par_line);
std::string getAmountLineString(std::string &par_line);
std::string getType(std::string &par_line);
std::string getCategory(std::string &par_line);
float getAmountTotal(const int par_MONTH, const std::string par_TYPE, const std::string par_CATEGORY);
std::string getDate(std::string &par_line);
std::string getDescription(std::string &par_line);
std::string getCurrentDateTime();

