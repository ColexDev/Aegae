#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdio>
#include <string>
#include <ncurses.h>
#include <curses.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <algorithm>
using std::string;
using std::getline;

int main();
void menuInitilization(const std::vector<string> &arrChoice);
void addEntry();
static int choice;
static int highlight = 0;
static string token;

static std::vector<string> choicesMain {"Add an Entry", "Remove an Entry", "View an Entry", "Reports"};
static std::vector<string> choicesType {"Expense", "Income"};
/* Edit the two below to add/delete category options */
static std::vector<string> choicesCategoryExpense {"Food", "Transportation", "Entertainment", "Other"};
static std::vector<string> choicesCategoryIncome {"Salary", "Sale", "Other"};
static std::vector<string> choicesViewEntry {"All", "Specific"};
static std::vector<string> choicesTotalSpending {"Total Spending", "Total Income", "Money Left"};

// Entry class for storing temp values
class Entry
{
private:
    string type;
    string category;
    string amount;
    string date;
    string description;
// Set the value (not currently used)
public:
    void setType(string value) {value = type;}
    void setCategory(string value) {value = category;}
    void setAmount(string value) {value = amount;}
    void setDate(string value) {value = date;}
    void setDescription(string value) {value = description;}

    string getType() {return type;}
    string getCategory() {return category;}
    string getAmount() {return amount;}
    string getDate() {return date;}
    string getDescription() {return description;}
};

Entry entry;

void clearRefresh()
{
    wclear(stdscr);
    wrefresh(stdscr);
}

// Get the date and time
string getCurrentDateTime()
{
    std::time_t _time;
    std::tm* timeinfo;
    char time[80];
    std::time(&_time); // get time -> store in address
    timeinfo = std::localtime(&_time); // address to struct rep
    std::strftime(time, 80 ,"%m-%d-%Y", timeinfo); // format time to `time`
    string __time(time); // convert char[] -> std::string
    return __time;
}

// Get number of entries
/*string listNumEntries()
{
    ifstream database("database.txt");
    string choice;
    cout << "Would you like to view all entries or see the number of entries? [a or n]: " << endl;
    cin >> choice;
    int count = 0;
    string item;
    while(!database.eof())
    {
        database >> item;
        count++;
        if(choice == "a")
        {
            cout << item << endl;
        }
        
    }
    string r = count + " entries found!";
    return r;
    database.close();
}*/


// RETURNS THE MONTH (1, 2, 3, ETC)
int getTimeFrame(string &line)
{
    string input {line};
    std::istringstream ss(input);
    while(getline(ss, token, ',')){}
    std::istringstream ss2(token);
    getline(ss2, token, '-' );
    return stoi(token);
}

// RETURNS THE AMOUNT, FLOAT (15.65)
float getAmountLine(string &line)
{
    string input {line};
    std::istringstream ss(input);
    string amount;
    getline(ss, amount, ',');
    getline(ss, amount, ',');
    getline(ss, amount, ',');
    std::istringstream ssAmount(amount);
    getline(ssAmount, amount, '$');
    getline(ssAmount, amount, '$');
    return stof(amount);
}

// Gets the Type (Expense/Income)
string getType(string &line)
{
    string input {line};
    string type;
    std::istringstream ss(input);
    getline(ss, type, ',');
    return type;
}

float getAmountTotal(const int month, const string _type)
{
    std::ifstream database("database.txt");
    string line;
    int date;
    string type;
    float amount {0};
    while(getline(database, line))
    {
        type = getType(line);
        if (type == _type && month == getTimeFrame(line))
        {
            amount += getAmountLine(line);
        } 
    }
string numAmount {std::to_string(amount)};
return amount;
}

void totalSpending(const int month)
{
    highlight = 0;
    clearRefresh();
    while(true)
    {
        mvwprintw(stdscr, 0, 1, "Which would you like to see: ");
        menuInitilization(choicesTotalSpending);
        if (choice == 108)
        {
            switch (highlight)
            {
                case 0:
                    clearRefresh();
                    mvwprintw(stdscr, 0, 1, "You have spent $");
                    mvwprintw(stdscr, 0, 17, std::to_string(getAmountTotal(month, "Expense")).c_str());
                    break;
                case 1:
                    clearRefresh();
                    mvwprintw(stdscr, 0, 1, "You have made $");
                    mvwprintw(stdscr, 0, 17, std::to_string(getAmountTotal(month, "Income")).c_str());
                    break;
                case 2:
                    clearRefresh();
                    float spent{getAmountTotal(month, "Expense")};
                    float earned{getAmountTotal(month, "Income")};
                    float left{earned-spent};
                    string stringLeft{std::to_string(left)};
                    mvwprintw(stdscr, 0, 1, "You have $");
                    mvwprintw(stdscr, 0, 11, stringLeft.c_str());
                    mvwprintw(stdscr, 0, stringLeft.length() + 11, " left after your expenses");
                    break;
            }
            break;       
        }
    }
    mvwprintw(stdscr, 2, 1, "Press any key to continue...");
    getch();
    clearRefresh();
    highlight = 0;
}

void menuInitilization(const std::vector<string> &arrChoice)
{
    auto size = arrChoice.size();
    for(int i = 0; i < size; i++)
    {
        if(i == highlight) {wattron(stdscr, A_REVERSE);}
        mvwprintw(stdscr, i+2, 1, arrChoice[i].c_str());
        wattroff(stdscr, A_REVERSE);
    }
    choice = wgetch(stdscr);
    switch(choice)
    {
        case 107:
            highlight--;
            if (highlight == -1)
                highlight = 0;
                break;
        case 106:
            highlight++;
            if (highlight == size)
                highlight = size - 1;
                break;
    }
}

void findEntry()
{
    clearRefresh();
    std::ifstream database("database.txt");
    string line;
    char mesg[]="Enter the a keyword to search for (date[02-21-2021], type, etc): ";
    char find[80];
    echo();
    mvprintw(0, 0, "%s", mesg);
    getstr(find);
    noecho();
    clearRefresh();
    int n {0};
    while(getline(database, line))
    {
        if(line.find(find) != string::npos)
        {
            n++;
            mvwprintw(stdscr, n-1, 1, line.c_str());
        }   
    }
    mvwprintw(stdscr, n+2, 1, "Press any key to continue...");
    getch();
    clearRefresh();
}

// Get an entry from the date that the user enters
void viewEntry()
{
    highlight = 0;
    clearRefresh();
    string line{};
    std::ifstream database("database.txt");
    int n {0};
    while(true) {
        mvwprintw(stdscr, 0, 1, "Which entries would you like to see: ");
        menuInitilization(choicesViewEntry);
        if(choice == 108)
        {
            switch (highlight)
            {
                case 0:
                    clearRefresh();
                    while(getline(database, line))
                    {
                        mvwprintw(stdscr, n, 1, line.c_str());
                        n++;
                    }
                    mvwprintw(stdscr, n+2, 1, "Press any key to continue...");
                    getch();
                    clearRefresh();
                    break;
                case 1:
                    clearRefresh();
                    findEntry();
            }
            break;
        }
    }
    clearRefresh();
    highlight = 0;
    main();
}

// Copied from stack overflow, its function is self-explanatory. 
void eraseFileLine(const string path, const string eraseLine)
{
    string line;
    std::ifstream fin;
    
    fin.open(path);
    // contents of path must be copied to a temp file then
    // renamed back to the path file
    std::ofstream temp;
    temp.open("temp.txt");

    while (getline(fin, line))
    {
        // write all lines to temp other than the line marked for erasing
        if (line != eraseLine) {temp << line << std::endl;}
    }

    temp.close();
    fin.close();

    // required conversion for remove and rename functions
    const char * p = path.c_str();
    remove(p);
    rename("temp.txt", p);
}

// Deletes a specific entry
void removeEntry()
{
    std::ofstream outputFile("outputFileName");
    std::ifstream database("database.txt");
    string line;
    string var;
    char mesg[]="Search for the entry you want to remove (type, category, amount, date [02-21-2021]): ";
    char find[80];
    echo();
    mvprintw(0, 0, "%s", mesg);
    getstr(find);
    noecho();
    while(getline(database, line))
    {
        if(line.find(find) != string::npos)
        {
            var = line;
            clearRefresh();
            mvwprintw(stdscr, 2, 1, line.c_str());
            int choice;
            echo();
            mvwprintw(stdscr, 0, 1, "Is this the line you want to delete[y/n]: ");
            choice = getch();
            noecho();
            if (choice == 121)
            {
                eraseFileLine("database.txt", var);
                highlight = 0;
                main();
                clearRefresh();
                break;
            } else {
                continue;
            }
        } 
    }
    clearRefresh();
}

// Create a new database
// NOT CURRENTLY IN USE (automatically created when first entry is added.)
void createDatabase()
{
    mvwprintw(stdscr, 0, 1, "Your database has been created.");
    std::ofstream database("database.txt");
    database.close();
    clearRefresh();
    highlight = 0;
    main();
}

// This does too much based off its name
void getSetAmount()
{
    std::ofstream outputFile("outputFileName");
    std::ifstream inputFile("database.txt");
    char mesg[]="Enter the Amount: ";
    char str[80];
    echo();
    mvprintw(0, 0, "%s", mesg);
    getstr(str); 
    entry.setAmount(str);
    noecho();
    char msg[]="Enter a Description (Press enter for none): ";
    char desc[80];
    echo();
    mvprintw(0, 0, "%s", msg);
    getstr(desc);
    noecho();
    entry.setDescription(desc);
    // Sets the date and time
    entry.setDate(getCurrentDateTime() + '\n');
    // Writes to file
    outputFile << entry.getType() << ", " << entry.getCategory() << ", " << "$" << entry.getAmount() << ", " << entry.getDescription() << ", " << entry.getDate();
    outputFile << inputFile.rdbuf();
    inputFile.close();
    outputFile.close();
    remove("database.txt");
    rename("outputFileName","database.txt");
    clearRefresh();
    highlight = 0;
    main();
}

void setTypeIncome()
{
    highlight = 0;
    entry.setType("Income");
    while(true)
    {
        clearRefresh();
        mvwprintw(stdscr, 0, 1, "What was it from ");
        menuInitilization(choicesCategoryIncome);
        switch(choice)
        {
            case 113:
                clearRefresh();
                break;
            case 108: 
                entry.setCategory(choicesCategoryIncome[highlight]);
                clearRefresh();
                break;
            case 104:
                clearRefresh();
                addEntry();
        }
    }
    getSetAmount();
    clearRefresh();
    mvwprintw(stdscr, 0, 1, "Your entry has been submitted to the database!");
}

void setTypeExpense()
{
    highlight = 0;
    entry.setType("Expense");
    while(true)
    {  
        clearRefresh();
        mvwprintw(stdscr, 0, 1, "What was it spent on: ");
        menuInitilization(choicesCategoryExpense); 
        switch(choice)
        {
            case 113:
                clearRefresh();
                break;
            case 108: 
                entry.setCategory(choicesCategoryIncome[highlight]);
                clearRefresh();
                break;
            case 104:
                clearRefresh();
                addEntry();
        }
    }
    getSetAmount();
    clearRefresh();
    mvwprintw(stdscr, 0, 1, "Your entry has been submitted to the database!");
}

void addEntry()
{
    highlight = 0;
    while(true)
    { 
        mvwprintw(stdscr, 0, 1, "Please Select the Category: ");
        menuInitilization(choicesType);
        switch(choice)
        {
            case 108:
                clearRefresh();
                break;
            case 104:
                main();
                clearRefresh();
                break;
        }
    }
    if(choicesType[highlight] == "Expense")
    {
        setTypeExpense();
    } else if (choicesType[highlight] == "Income") {
        setTypeIncome();
    }
}

// MAKE AN EXPENSE SHEET, HAVE IT SHOW ALL MONTHS AND YEARLY TOO
int main()
{
    static bool firstRun{false};
    clearRefresh();
    highlight = 0;
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    if(firstRun == true)
    {
        while(true)
        {
            wattron(stdscr, A_BOLD);
            mvwprintw(stdscr, 0, 1, "Money Manager");
            mvwprintw(stdscr, 1, 1, "By: ColexDev");
            wattroff(stdscr, A_BOLD);
            mvwprintw(stdscr, 3, 1, "Press any key to continue...");
            getch();
            clearRefresh();
            break;
        }
            firstRun = false;
    }
        while(true)
        {
            curs_set(0);
            mvwprintw(stdscr, 0, 1, "Please Select an Option: ");
            menuInitilization(choicesMain);
            switch (choice)
            {
                case 108:
                    clearRefresh();
                    switch (highlight)
                    {
                        case 0:
                            clearRefresh();
                            addEntry();
                        case 1:
                            clearRefresh();
                            removeEntry();
                        case 2:
                            clearRefresh();
                            viewEntry();
                        case 3:
                            clearRefresh();
                            char mesg[]="What month do you want to see a report for?(1, 2, 3, etc): ";
                            char find[2];
                            echo();
                            mvprintw(0, 0, "%s", mesg);
                            getstr(find);
                            noecho();
                            int x {std::stoi(find)};
                            totalSpending(x);
                    }
               case 113 || 104:
                break;
            }
        }
    endwin();
}