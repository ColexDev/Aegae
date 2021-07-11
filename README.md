# Corcyra
A simple CLI application to manage money

Work in progress

## Dependencies
- g++ (if you use the makefile, otherwise you can use any C compiler if you include ncurses)
- ncurses (powers the menu functionality)

## Installation
```
$ git clone https://github.com/ColexDev/Corcyra
$ cd Corcyra
$ make
$ ./Corcyra
```

## TODO
TOP PRIORITY:
- Have all entries be shown when the program is launched, have it be a menu system so that user can select entires with
keyboard. Press 'n' to add a new entry, and press 'x' to delete the current selected entry.

- Finish refactoring code base
- Allow the user to exit at ANY time by pressing 'q'
- Allow the user to go back at any time by pressing 'h'
- Fix error with no description adding an extra comma (may need to move description to the end of an entry)
- Finish the Reports section
