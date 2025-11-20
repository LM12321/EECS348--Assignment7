/*
Name of Program: EECS 348 Assignment7
Author: Logan Whitt
KUID: 3152587

Description: 
Inputs:
Outputs:

Collaborators: 
Other Sources: 

Creation Date: 11/18/25 - ASDFASDF
*/

//TODO Create Table format thing
//  Probably use vector schenanagans
//  Finish from 3 down

// MySQL Connector/C++ (legacy API) headers
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
// Other headers
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

void print_row_of_underscores(vector<int> max_column_widths, bool top_of_table = false){
    for (int entry: max_column_widths){
        //check if we're at the top of the table
        if (top_of_table){
            cout << "."; //period looks better
        }else{
            cout << "|";    //bar for everything else
        }

        for (int i = 0; i < entry; i++){
            cout << "_";
        }
    }

    if (top_of_table){  //check again at the end
        cout << " ";
    }else{
        cout << "|";    
    }

    cout << endl;   //end line
}

void print_tables(vector<vector<string>> query){
    if (query.size() == 1){
        cout << "No results found :( " << endl;
        return;
    }

    int number_of_columns = query[0].size();  //get the number of columns from the header
    int number_of_rows = query.size();  //get the number of rows
    vector<int> max_column_widths;  //initalize max column widths vector
    
    for (int i = 0; i < number_of_columns; i++){
        int max_size = 0;   //initalize max_size

        for (int j = 0; j < number_of_rows; j++){
            int size_at_entry = query[j][i].size();    //get the size of the entry at that query
            if (size_at_entry > max_size){  //if this size is larger than the current max size
                max_size = size_at_entry;   //set the new max size to that
            }
        }

        max_size += 2;  //add 2 to the max size for formatting
        max_column_widths.push_back(max_size);    //add the max_size to the max_column_widths
    }
    
    print_row_of_underscores(max_column_widths, true);    //add top of the table barrier
    for (int i = 0; i < number_of_rows; i++){
        
        for (int j = 0; j < number_of_columns; j++){
            string entry = query[i][j];
            cout << "|";
            int spaces_to_add = (max_column_widths[j] - entry.size())/2;    //get the number of spaces before and after each entry

            for (int spaces = 0; spaces < spaces_to_add; spaces++){ //add spaces
                cout << " ";
            }

            cout << entry;  // output the entry

            if ((max_column_widths[j] - entry.size()) % 2 != 0){    //add an additional space if uneven
                cout << " ";
            }

            for (int spaces = 0; spaces < spaces_to_add; spaces++){ //add spaces
                cout << " ";
            }

        }

        cout << "|" << endl;    //add bar at the end

        print_row_of_underscores(max_column_widths);    //add row of underscores to differenciate tables
    }
    cout << endl;   //for formmating
}

void get_student_info_with_multiple_classes(sql::Connection *connection){
    sql::Statement *statement; //to execute SQL statement
    statement = connection->createStatement();  //establish statement
    sql::ResultSet *result; //a place to store the query results

    result = statement->executeQuery("SELECT S.* FROM Student7 S LEFT JOIN Enrollment7 E ON S.StdNo = E.StdNo Group By E.StdNo HAVING COUNT(E.StdNo) > 1");
    vector<vector<string>> all_rows;
    vector<string> header;

    header.push_back("StdNo");
    header.push_back("StdFirstName");
    header.push_back("StdLastName");
    header.push_back("StdCity");
    header.push_back("StdState");
    header.push_back("StdZip");
    header.push_back("StdMajor");
    header.push_back("StdClass");
    header.push_back("StdGPA");

    all_rows.push_back(header);


    while (result->next()) {

        vector<string> row_result;
        row_result.push_back(result->getString("StdNo"));
        row_result.push_back(result->getString("StdFirstName"));
        row_result.push_back(result->getString("StdLastName"));
        row_result.push_back(result->getString("StdCity"));
        row_result.push_back(result->getString("StdState"));
        row_result.push_back(result->getString("StdZip"));
        row_result.push_back(result->getString("StdMajor"));
        row_result.push_back(result->getString("StdClass"));
        row_result.push_back(result->getString("StdGPA"));

        all_rows.push_back(row_result);
    }
    print_tables(all_rows);
}

void get_student_info_with_major_IS(sql::Connection *connection){
    sql::Statement *statement; //to execute SQL statement
    statement = connection->createStatement();  //establish statement
    sql::ResultSet *result; //a place to store the query results

    result = statement->executeQuery("SELECT * FROM Student7 WHERE StdMajor = 'IS';");
    vector<vector<string>> all_rows;
    vector<string> header;

    header.push_back("StdNo");
    header.push_back("StdFirstName");
    header.push_back("StdLastName");
    header.push_back("StdCity");
    header.push_back("StdState");
    header.push_back("StdZip");
    header.push_back("StdMajor");
    header.push_back("StdClass");
    header.push_back("StdGPA");

    all_rows.push_back(header);


    while (result->next()) {

        vector<string> row_result;
        row_result.push_back(result->getString("StdNo"));
        row_result.push_back(result->getString("StdFirstName"));
        row_result.push_back(result->getString("StdLastName"));
        row_result.push_back(result->getString("StdCity"));
        row_result.push_back(result->getString("StdState"));
        row_result.push_back(result->getString("StdZip"));
        row_result.push_back(result->getString("StdMajor"));
        row_result.push_back(result->getString("StdClass"));
        row_result.push_back(result->getString("StdGPA"));

        all_rows.push_back(row_result);
    }
    print_tables(all_rows);
}

void create_table_from_file(sql::Connection *connection, string filepath){
    
    std::string line;   //initalize line string
    std::ifstream myfile (filepath);    //initalize file from filepath

    if (!myfile){   //couldn't find file
        cout << "SQL file does not exist." << endl; //let the user know
        cout << "File path: " << filepath << endl;   //show the file path 
        return;
    }
    //start database things
    sql::Statement *statement; //to execute SQL statement
    statement = connection->createStatement();  //establish statement

    if (myfile.is_open()){  //if the file is open
        string sql_statement = "";  //initalize empty string to build statment onto

        while ( getline (myfile, line)){  //while there are still lines to read
            if (line.empty()){  //empty line 
                continue;   //ignore and continue iteration
            }else if (line == "\r"){    //ignore Windows (as God intended)
                continue;   //ignore and continue iteration
            }

            if (line.find_first_not_of(" \t\r\n") == string::npos){ //trim whitespace
                continue;
            }

            if (line.rfind("--", 0) == 0){
                continue;   //it's a comment
            }

            sql_statement += line + " ";    //add command

            if (line.find(';') != string::npos){    //if we found a semi colon
                try {
                    statement->execute(sql_statement);  //try to execute the statement
                }catch (sql::SQLException &e) {
                std::cout << "SQL Error in file: " << filepath << std::endl;
                std::cout << "Statement:\n" << sql_statement << std::endl;
                std::cout << "Message: " << e.what() << std::endl;
                }

                sql_statement.clear();  // reset for next statement
            }
        }

        cout << "Successfully added table!" << endl;
        myfile.close();  //close the file
        delete statement;   //delete the statement
    }
    

}

int main(){
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *connection; //to establish connection
    

    driver = sql::mysql::get_mysql_driver_instance();

    connection = driver->connect("mysql.eecs.ku.edu", "348f25_l099w160", "aLeek7Xo");
    connection->setSchema("348f25_l099w160"); //establish the database we want to use

    create_table_from_file(connection, "course.SQL");
    create_table_from_file(connection, "enrollment.SQL");
    create_table_from_file(connection, "student.SQL");
    create_table_from_file(connection, "faculty.SQL");
    create_table_from_file(connection, "offering.SQL");
    cout << endl;

    get_student_info_with_major_IS(connection);
    get_student_info_with_multiple_classes(connection);

    delete connection;
    
    return 0;
}
