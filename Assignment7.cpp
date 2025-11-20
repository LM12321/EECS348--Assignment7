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

//headers for accessing sql server
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/exception.h>




//other headers for helpful things
#include <string>
#include <fstream>

using namespace std;    //for namespace



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

    delete connection;
    
    return 0;
}
