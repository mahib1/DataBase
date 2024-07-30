# Database Project

## Overview
This project implements a simple database system in C, allowing for the creation, modification, and querying of a custom database file format. The database stores information about employees, including their names, addresses, and hours worked.

## Features
- **Create a new database**: Initialize a new database file with a custom header and an empty list of employees.
- **Add records**: Add new employee records to the database.
- **List records**: Display all employee records in the database.
- **Delete records**: Remove an employee record by name.
- **Update records**: Modify the hours worked for a specific employee.
- **Count records**: Display the total number of employees in the database.

## Usage

### Command-Line Options
The program can be run from the command line with the following options:

- `-n`: Create a new database file.
- `-f <filepath>`: Specify the path to the database file (required).
- `-a <employee record>`: Add a new employee record. The record should be in the format `"Name,Address,Hours"`.
- `-l`: List all employee records in the database.
- `-t`: Display the total number of employees in the database.
- `-d <name>`: Delete an employee record by name.
- `-u <employee update>`: Update the hours worked for an employee. The update should be in the format `"Name,NewHours"`.

### Example Usage
```sh
# Create a new database file named mynewdb.db
./bin/dbview -f ./mynewdb.db -n

# Add new employee records
./bin/dbview -f ./mynewdb.db -a "Timmy H.,123 Seshire Ln.,122"
./bin/dbview -f ./mynewdb.db -a "Jane Doe,456 Elm St.,200"

# List all employees
./bin/dbview -f ./mynewdb.db -l

# Display total number of employees
./bin/dbview -f ./mynewdb.db -t

# Delete an employee by name
./bin/dbview -f ./mynewdb.db -d "Timmy H." -l

# Update hours worked for an employee
./bin/dbview -f ./mynewdb.db -u "Jane Doe,300"
```

## Compilation and Building

### Prerequisites
- GCC compiler
- Make

### Build Instructions
1. Clone the repository and navigate to the project directory.
2. Build the project using the provided Makefile:

```sh
make
```
3. This will compile the source files and generate the executable `dbview` in the `bin` directory.
4. This will also create an example database for showcasing the usage.

### Cleaning Up

To clean up the generated files, use the `clean` target:

```sh
make clean
```
This will remove the object files, the binary executable, and any generated database files.

# Thankyou!
