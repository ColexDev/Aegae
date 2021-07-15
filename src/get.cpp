#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
extern std::string token;

// RETURNS THE MONTH (1, 2, 3, ETC)
int getTimeFrame(std::string &par_line)
{
    std::istringstream ss_line(par_line);
    while(std::getline(ss_line, token, ',')){}
    std::istringstream ss_token(token);
    std::getline(ss_token, token, '-' );
    return stoi(token);
}

// RETURNS THE AMOUNT, FLOAT (15.65)
float getAmountLine(std:: string &par_line)
{
    std::istringstream ss_line(par_line);
    std::string amount;
    std::getline(ss_line, amount, ',');
    std::getline(ss_line, amount, ',');
    std::getline(ss_line, amount, ',');
    std::istringstream ss_amount(amount);
    std::getline(ss_amount, amount, '$');
    std::getline(ss_amount, amount, '$');
    return stof(amount);
}
// REMOVE THIS AND MAKE 1 FUNCTION / TEMPLATE FOR BOTH TYPES
std::string getAmountLineString(std:: string &par_line)
{
    std::istringstream ss_line(par_line);
    std::string amount;
    std::getline(ss_line, amount, ',');
    std::getline(ss_line, amount, ',');
    std::getline(ss_line, amount, ',');
    std::istringstream ss_amount(amount);
    std::getline(ss_amount, amount, '$');
    std::getline(ss_amount, amount, '$');
    return amount;
}

// Gets the Type (Expense/Income)
std::string getType(std::string &par_line)
{
    std::string type;
    std::istringstream ss_line(par_line);
    std::getline(ss_line, type, ',');
    return type;
}

std::string getCategory(std::string &par_line)
{
    std::string category;
    std::istringstream ss_line(par_line);
    std::getline(ss_line, category, ',');
    std::getline(ss_line, category, ',');
    return category;
}

std::string getDate(std::string &par_line)
{
    std::string date;
    std::istringstream ss_line(par_line);
    std::getline(ss_line, date, ',');
    std::getline(ss_line, date, ',');
    std::getline(ss_line, date, ',');
    std::getline(ss_line, date, ',');
    std::getline(ss_line, date, ',');
    return date;
}

std::string getDescription(std::string &par_line)
{
    std::string description;
    std::istringstream ss_line(par_line);
    std::getline(ss_line, description, ',');
    std::getline(ss_line, description, ',');
    std::getline(ss_line, description, ',');
    std::getline(ss_line, description, ',');
    return description;
}

float getAmountTotal(const int par_MONTH, const std::string par_TYPE, const std::string par_CATEGORY)
{
    std::fstream database("database.txt");
    std::string line;
    int date;
    float amount = 0;
    if (par_CATEGORY == "All") {
        while(std::getline(database, line)) {
            if (par_TYPE == getType(line) && par_MONTH == getTimeFrame(line)) {
                amount += getAmountLine(line);
            }
        }
    } else {
        while(std::getline(database, line)) {
            if (par_TYPE == getType(line)&& par_MONTH == getTimeFrame(line) && par_CATEGORY == getCategory(line)) {
            amount += getAmountLine(line);
            }
        }
    }

    std::string numAmount {std::to_string(amount)};
return amount;
}

// Get the date and time
std::string getCurrentDateTime()
{
    std::time_t _time;
    std::tm* timeinfo;
    char time[80];
    std::time(&_time); // get time -> store in address
    timeinfo = std::localtime(&_time); // address to struct rep
    std::strftime(time, 80 ,"%m-%d-%Y", timeinfo); // format time to `time`
    std::string __time(time); // convert char[] -> std::string
    return __time;
}
