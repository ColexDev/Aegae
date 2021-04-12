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
int choice;
int highlight{0}; 
string token;

const std::vector<string> choicesMain {"Add an Entry", "Remove an Entry", "View an Entry", "Reports"};
const std::vector<string> choicesType {"Expense", "Income"};
/* Edit the two below to add/delete category options */
const std::vector<string> choicesCategoryExpense {"Food", "Transportation", "Entertainment", "Other"};
const std::vector<string> choicesCategoryIncome {"Salary", "Sale", "Gift", "Other"};
const std::vector<string> choicesViewEntry {"All", "Specific"};
const std::vector<string> choicesTotalSpending {"Total Spending", "Total Income", "Money Left"};

// Entry class for storing temp values
class Entry
{
private:
    string m_type;
    string m_category;
    string m_amount;
    string m_date;
    string m_description;
// Set the value (not currently used)
public:
    void setType(string t_value) {m_type = t_value;}
    void setCategory(string t_value) {m_category = t_value;}
    void setAmount(string t_value) {m_amount = t_value;}
    void setDate(string t_value) {m_date = t_value;}
    void setDescription(string t_value) {m_description = t_value;}

    string getType() {return m_type;}
    string getCategory() {return m_category;}
    string getAmount() {return m_amount;}
    string getDate() {return m_date;}
    string getDescription() {return m_description;}
};

const std::vector<string> months {"January", "Feburary", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

Entry entry;

void clearRefresh(){ wclear(stdscr); wrefresh(stdscr); }

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
int getTimeFrame(string &t_line)
{
    std::istringstream ss(t_line);
    while(getline(ss, token, ',')){}
    std::istringstream ss2(token);
    getline(ss2, token, '-' );
    return stoi(token);
}

// RETURNS THE AMOUNT, FLOAT (15.65)
float getAmountLine(string &t_line)
{
    std::istringstream ss(t_line);
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
string getType(string &t_line)
{
    string type;
    std::istringstream ss(t_line);
    getline(ss, type, ',');
    return type;
}

string getCategory(string &t_line)
{
    string category;
    std::istringstream ss(t_line);
    getline(ss, category, ',');
    getline(ss, category, ',');
    return category;
}

float getAmountTotal(const int t_MONTH, const string t_TYPE, const string t_CATEGORY)
{
    std::ifstream database("database.txt");
    string line;
    int date;
    string type;
    float amount {0};
    if (t_CATEGORY == "All")
    {
        while(getline(database, line))
        {
            type = getType(line);
            if (type == t_TYPE && t_MONTH == getTimeFrame(line))
            {
                amount += getAmountLine(line);
            } 
        } 
    } else {
        while(getline(database, line))
        {
            type = getType(line);
            if (type == t_TYPE && t_MONTH == getTimeFrame(line) && t_CATEGORY == getCategory(line))
            {
            amount += getAmountLine(line);
            } 
        }
    }

string numAmount {std::to_string(amount)};
return amount;
}

void totalSpending(const int t_MONTH)
{
    // TO-DO: Find % of spending and income for each
    clearRefresh();
    mvwprintw(stdscr, 0, 1, "Expense Report for "); mvwprintw(stdscr, 0, 20, months[t_MONTH - 1].c_str());
    mvwprintw(stdscr, 2, 1, "You have spent $");
    mvwprintw(stdscr, 2, 17, std::to_string(getAmountTotal(t_MONTH, "Expense", "All")).c_str());
    
    mvwprintw(stdscr, 4, 1, "You have made $");
    mvwprintw(stdscr, 4, 16, std::to_string(getAmountTotal(t_MONTH, "Income", "All")).c_str());

    float spent{getAmountTotal(t_MONTH, "Expense", "All")};
    float earned{getAmountTotal(t_MONTH, "Income", "All")};
    float left{earned-spent};
    string stringLeft{std::to_string(left)};
    mvwprintw(stdscr, 6, 1, "You have $");
    mvwprintw(stdscr, 6, 11, stringLeft.c_str());
    mvwprintw(stdscr, 6, stringLeft.length() + 11, " left after your expenses for the month");  
    
    mvwprintw(stdscr, 8, 1, "Press any key to continue...");
    getch();
    clearRefresh();
}

void menuInitilization(const std::vector<string> &t_ARRCHOICE)
{
    int size = t_ARRCHOICE.size();
    for(int i = 0; i < size; i++)
    {
        if(i == highlight) {wattron(stdscr, A_REVERSE);}
        mvwprintw(stdscr, i+2, 1, t_ARRCHOICE[i].c_str());
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
    constexpr char MESG[]="Enter the a keyword to search for (date[02-21-2021], type, etc): ";
    char find[80];
    echo();
    mvprintw(0, 0, "%s", MESG);
    getstr(find);
    noecho();
    clearRefresh();
    int n{0};
    while(getline(database, line))
    {
        if(line.find(find) != string::npos)
        {
            n++;
            mvwprintw(stdscr, n-1, 1, line.c_str());
        }   
    }
    if (n == 0)
    {
        mvwprintw(stdscr, n, 1, "No results were found.");
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
    string line;
    std::ifstream database("database.txt");
    int n{0};
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
void eraseFileLine(const string t_PATH, const string t_ERASELINE)
{
    string line;
    std::ifstream fin;
    
    fin.open(t_PATH);
    // contents of path must be copied to a temp file then
    // renamed back to the path file
    std::ofstream temp;
    temp.open("temp.txt");

    while (getline(fin, line))
    {
        // write all lines to temp other than the line marked for erasing
        if (line != t_ERASELINE) {temp << line << std::endl;}
    }

    temp.close();
    fin.close();

    // required conversion for remove and rename functions
    const char * p = t_PATH.c_str();
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
    constexpr char MESG[]="Search for the entry you want to remove (type, category, amount, date [02-21-2021]): ";
    char find[80];
    echo();
    mvprintw(0, 0, "%s", MESG);
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
    constexpr char MESG[]="Enter the Amount: ";
    char str[80];
    echo();
    mvprintw(0, 0, "%s", MESG);
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
    outputFile << entry.getType() << ", " << entry.getCategory() << ", " << "$" 
    << entry.getAmount() << ", " << entry.getDescription() << ", " << entry.getDate();
    outputFile << inputFile.rdbuf();
    inputFile.close();
    outputFile.close();
    remove("database.txt");
    rename("outputFileName","database.txt");
    clearRefresh();
    highlight = 0;
    main();
}

void setCategoryIncome()
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
                getSetAmount();
            case 104:
                clearRefresh();
                addEntry();
        }
    }
}

void setCategoryExpense()
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
                entry.setCategory(choicesCategoryExpense[highlight]);
                clearRefresh();
                getSetAmount();
                break;
            case 104:
                clearRefresh();
                addEntry();
        }
    }
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
                if(choicesType[highlight] == "Expense")
                {
                    setCategoryExpense();
                } else if (choicesType[highlight] == "Income") {
                    setCategoryIncome();
                }
            case 104:
                main();
                clearRefresh();
                break;
        }
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
    if(firstRun == false)
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
            firstRun = true;
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
                            constexpr char MESG[]="What month do you want to see a report for?(1, 2, 3, etc): ";
                            char find[2];
                            echo();
                            mvprintw(0, 0, "%s", MESG);
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
