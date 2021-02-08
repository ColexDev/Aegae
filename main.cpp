#include <iostream>
#include <fstream>
#include <ctime>
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
string getCurrentDateTime()
{
    time_t tt;
    struct tm *st;

    tt = time(NULL);
    st = localtime(&tt);
    return asctime(st);
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
            cout << "Enter the a keyword to search for (date[Jan 01], type, etc): ";
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

// DO NOT USE, BROKEN, DELETES THE WHOLE FILE
void removeEntry() {
    fstream outputFile("outputFileName", std::fstream::in | std::fstream::out | std::fstream::app);
    fstream database("database", std::fstream::in | std::fstream::out | std::fstream::app);
    string line{};
    string find{};
    cout << "Search for the entry you want to remove (type, category, amount, date [Jan 01]): ";
    cin.ignore(256, '\n');
    getline(cin, find);
    while(getline(database, line)){
                if(line.find(find) != string::npos) {
                    cout << line << endl;
            }   
            }
    std::string choice{};
    cout << "Is this the line you want to delete[y/n]: ";
    cin >> choice;
    if(choice == "n") {
        removeEntry();
    } else {
        while(getline(database, line)){
                if(line == find) {
                    outputFile << line << endl;
            }   
            }
        database.close();
        outputFile.close();
        remove("database");
        rename("outputFileName","database"); 
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
    entry.date = getCurrentDateTime();
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