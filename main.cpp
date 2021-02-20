#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdio>
#include <string>
#include <ncurses.h>
#include <curses.h>
#include <string.h>
using namespace std;
int main();
static int choice;
static int highlight = 0;
// Add ability for users to add to Category choices (will need to use vectors)
static string choicesMain[4] = {"Add an Entry", "Remove an Entry", "View an Entry", "Create a Database"};
static string choicesType[2] = {"Expense", "Income"};
static string choicesCategoryExpense[4] = {"Food", "Transportation", "Entertainment", "Other"};
static string choicesCategoryIncome[3] = {"Salary", "Sale", "Other"};
//Entry class for storing temp values
class Entry
{
public:
    string type;
    string category;
    string amount;
    string date;
    string description;
// Set the value (not currently used)
    void setType(string value) { value = type; }
    void setCategory(string value) { value = category; }
    void setAmount(string value) { value = amount; }
    void setDate(string value) { value = date; }
    void setDescription(string value) { value = description; }
	       	     
};
Entry entry;
// Get the date and time
std::string getCurrentDateTime() {
    std::time_t _time;
    std::tm* timeinfo;
    char time[80];

    std::time(&_time); // get time -> store in address
    timeinfo = std::localtime(&_time); // address to struct rep

    std::strftime(time, 80 ,"%m-%d-%Y", timeinfo); // format time to `time`
    
    std::string __time(time); // convert char[] -> std::string
    return __time;
}
  



// Get number of entries
/*string listNumEntries()
{
    ifstream database("database");
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

// ADD A FEATURE TO SHOW THE TOTAL SPENDING IN A CERTAIN TIME FRAME.

void clearRefresh() {

    wclear(stdscr);
    wrefresh(stdscr);

}


void menuInitilization(int numChoices, string arrChoice[], int height = 2){
    for(int i = 0; i < numChoices; i++)
    {
        if(i == highlight)
            wattron(stdscr, A_REVERSE);
        mvwprintw(stdscr, i+height, 1, arrChoice[i].c_str());
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
            if (highlight == numChoices)
                highlight = numChoices - 1;
                break;
    }
}


void findEntry() {
    ifstream database("database");
    string line;
    string find;
            cout << "Enter the a keyword to search for (date[02-21-2021], type, etc): ";
            getline(cin, find);
            int ns;
            cout << "How many occurances do you want [#]: " << endl;
            cin >> ns;
            int n {0};
            while(getline(database, line) && n < ns){
                if(line.find(find) != string::npos) {
                    n++;
                    cout << line << endl;
            }   
         }
}

// Get an entry from the date that the user enters
void viewEntry() {
    string choice;
    cout << "Would you like to see all entries or a specific entry? [all/specific]" << endl;
    cin.ignore(256, '\n');
    getline(cin, choice);
    string line{};
    ifstream database("database");
        if(choice == "all"){
            while(getline(database, line)){
                cout << line << endl;
            }
          } else if(choice == "specific") {
            findEntry();
        }
}

// Copied from stack overflow, its function is self-explanatory. 
void eraseFileLine(std::string path, std::string eraseLine) {
    std::string line;
    std::ifstream fin;
    
    fin.open(path);
    // contents of path must be copied to a temp file then
    // renamed back to the path file
    std::ofstream temp;
    temp.open("temp.txt");

    while (getline(fin, line)) {
        // write all lines to temp other than the line marked for erasing
        if (line != eraseLine)
            temp << line << std::endl;
    }

    temp.close();
    fin.close();

    // required conversion for remove and rename functions
    const char * p = path.c_str();
    remove(p);
    rename("temp.txt", p);
}




// Deletes a specific entry
void removeEntry() {
    ofstream outputFile("outputFileName");
    ifstream database("database");
    string line;
    string find;
    string var;
    cout << "Search for the entry you want to remove (type, category, amount, date [02-21-2021]): ";
    cin.ignore(256, '\n');
    getline(cin, find);

    while(getline(database, line)){
        if(line.find(find) != string::npos) {
            var = line;
            cout << line << endl;
            string choice{};
            cout << "Is this the line you want to delete[y/n]: ";
            cin >> choice; 
            if (choice =="y")
            {
                eraseFileLine("database", var);
            } else {
                continue;
            }
        } 
    }
}

// Create a new database
void createDatabase() {
    //string fileName{};
    //cout << "What would you like to name your database?\n";
    //cin >> fileName;
    cout << "Your database has been created." << endl;
    ofstream database("database");
    database.close();
    main();

}
// Add an entry to an already existing database
/*void addEntry() {
    Entry entry;
    ofstream outputFile("outputFileName");
    ifstream inputFile("database");
    
// Check for error
    if (outputFile.fail())
    {
        cout << "Error Opening File" << endl;
        exit(1);
    }
// Determines type 
    mvwprintw(stdscr, 1, 1, "Expense");
    mvwprintw(stdscr, 1, 1, "Income");
    if(entry.type == "Expense") {
        cout << "What was it spent on? (Food, Transportation, etc.): " << endl;
        cin >> entry.category;
    } else {
        cout << "What was it from? (Salary, Sale, etc): " << endl;
        cin >> entry.category;
    }
// Determines Amount 
    cout << "Amount: \n" << "$ ";
    cin >> entry.amount;
// Sets the date and time
    entry.date = getCurrentDateTime() + '\n';
// Add function to check if the database is empty, if so take out the first "\n", if it is not empty leave it in
    outputFile << entry.type << ", " << entry.category << ", " << "$" << entry.amount << ", " << entry.date;
    outputFile << inputFile.rdbuf();
    inputFile.close();
    outputFile.close();
    remove("database");
    rename("outputFileName","database");
    
}
*/

void getSetAmount() {
    ofstream outputFile("outputFileName");
    ifstream inputFile("database");
    char mesg[]="Enter the Amount: ";
    char str[80];
    echo();
    mvprintw(0, 0, "%s", mesg);
    getstr(str);
    entry.amount = str;
    noecho();
    // Sets the date and time
    entry.date = getCurrentDateTime() + '\n';
    // Writes to file
    outputFile << entry.type << ", " << entry.category << ", " << "$" << entry.amount << ", " << entry.date;
    outputFile << inputFile.rdbuf();
    inputFile.close();
    outputFile.close();
    remove("database");
    rename("outputFileName","database");
    
}
// Make 1 settype function with parameters to choose if its income or expense
void setTypeIncome() {
    highlight = 0;
    entry.type = "Income";
    while(true){
        clearRefresh();
        mvwprintw(stdscr, 0, 1, "What was it from ");
        menuInitilization(3, choicesCategoryIncome); 
        if (choice == 113 || choice == 104) {
            clearRefresh();
            break;
        } else if (choice == 108) {
            if (choicesCategoryIncome[highlight] == "Salary") {
                entry.category = "Salary";
            } else if (choicesCategoryIncome[highlight] == "Sale") {
                entry.category = "Sale";
            } else if (choicesCategoryIncome[highlight] == "Other") {
                entry.category = "Other";
            }
            clearRefresh();
            getSetAmount();
            break;
        }
    }
    clearRefresh();
    mvwprintw(stdscr, 0, 1, "Your entry has been submitted to the database!");
}

void setTypeExpense() {
    highlight = 0;
    entry.type = "Expense";
    while(true) {  
        clearRefresh();
        mvwprintw(stdscr, 0, 1, "What was it spent on: ");
        menuInitilization(4, choicesCategoryExpense); 
        if (choice == 113 || choice == 104) {
            clearRefresh();
            break;
        } else if (choice == 108) {
            if (choicesCategoryExpense[highlight] == "Food") {
                entry.category = "Food";
            } else if (choicesCategoryExpense[highlight] == "Transportation") {
                entry.category = "Transportation";
            } else if (choicesCategoryExpense[highlight] == "Entertainment") {
                entry.category = "Entertainment";
            } else if (choicesCategoryExpense[highlight] == "Other") {
                entry.category = "Other";
            }
            clearRefresh();
            getSetAmount();
            break;
        }
    }
    clearRefresh();
    mvwprintw(stdscr, 0, 1, "Your entry has been submitted to the database!");
}
// Add ability for user to add a description
void addEntry() {
while(true)
{
    mvwprintw(stdscr, 0, 1, "Please Select the Category: ");
    menuInitilization(2, choicesType);
        if (choice == 108){
           if(choicesType[highlight] == "Expense"){
               setTypeExpense();
            } else if (choicesType[highlight] == "Income"){
                setTypeIncome();
            }
        } else if (choice == 113 || choice == 104) {
            clearRefresh();
            break;
    }
}
// Determines Amount 
 /*   cout << "Amount: \n" << "$ ";
    cin >> entry.amount;
// Sets the date and time
    entry.date = getCurrentDateTime() + '\n';
// Add function to check if the database is empty, if so take out the first "\n", if it is not empty leave it in
    outputFile << entry.type << ", " << entry.category << ", " << "$" << entry.amount << ", " << entry.date;
    outputFile << inputFile.rdbuf();
    inputFile.close();
    outputFile.close();
    remove("database");
    rename("outputFileName","database");
   */ 
}



int main()
{
initscr();
cbreak();
noecho();
refresh();
wrefresh(stdscr);
keypad(stdscr, true);

    while(true) 
    {
        curs_set(0);
        mvwprintw(stdscr, 0, 1, "Please Select an Option: ");
        menuInitilization(4, choicesMain);
        if (choice == 108){
            wclear(stdscr);
            if(choicesMain[highlight] == "Add an Entry"){
                clearRefresh();
                addEntry();
            }
        } else if (choice == 113 || choice == 104) {
            break;
        }
    }
endwin();
}
