#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdio>
#include <string>
using namespace std;
int main();

//Entry class for storing temp values
class Entry
{
public:
    string type{};
    string category{};
    double amount{};
    string date{};
    string description{};
// Set the value (not currently used)
    void setType(string value) { value = type; }
    void setCategory(string value) { value = category; }
    void setAmount(double value) { value = amount; }
    void setDate(string value) { value = date; }
    void setDescription(string value) { value = description; }
     
};

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

// ADD A FEATURE TO SHOW THE TOTAL SPEND IN A CERTAIN TIME FRAME.

void findEntry() {
    ifstream database("database");
    string line{};
    string find{};
            cout << "Enter the a keyword to search for (date[02-21-2021], type, etc): ";
            getline(cin, find);
            int ns{};
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
    string choice{};
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
// Add and entry to an already existing database
void addEntry() {
    Entry entry;
    fstream outputFile("outputFileName", std::fstream::in | std::fstream::out | std::fstream::app);
    fstream inputFile("database", std::fstream::in | std::fstream::out | std::fstream::app);
    
// Check for error
    if (outputFile.fail())
    {
        cout << "Error Opening File" << endl;
        exit(1);
    }
// Determines type 
    cout << "Expense or Income?: " << endl;
    cin >> entry.type;
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

// Show option menu and run the above functions
int main()
{
    cout << "Please Select and Option" << endl;
    cout << "1: Add an Entry" << endl;
    cout << "2: Remove an Entry" << endl;
    cout << "3: View an Entry" << endl;
    cout << "4: Edit an Entry" << endl;
    cout << "5: Create a Database" << endl;
    int choice{};
    cin >> choice;
    switch (choice)
    {
        case 1:
            addEntry();
            break;
        case 2:
            removeEntry();
            break;
        case 3:
            viewEntry();
            break;
        case 4:
            //editEntry();
            break;
        case 5:
            createDatabase();
            break;
        default:
            main();
            break;
    }    
}