#include <iostream>
#include <sstream>

#include "lib/SimpleSQL.h"
namespace sql = simple_sql;

int main() {
    sql::Database db;
    std::string input;
    while (std::getline(std::cin, input)) {
        try {
            std::cout << db.ParseRequest(input);
        } catch (std::exception& e) {
            std::cerr << e.what() << '\n';
        }
        std::cout << '\n';

        if (input == "e") {
            break;
        }
    }

    /*
    std::string create = "CREATE TABLE student (\n"
                         "  student_id INT NOT NULL,\n"
                         "  name VARCHAR(15) NOT NULL,\n"
                         "  major VARCHAR(10),\n"
                         "  PRIMARY KEY(student_id)\n"
                         ");";
    db.ParseRequest(create);


    std::string insert1 = R"(INSERT INTO student VALUES(1, "Jack", "Biology"))";
    std::string insert2 = R"(INSERT INTO student(student_id, name, major) VALUES(2, "Kate", "Sociology"))";
    std::string insert3 = R"(INSERT INTO student(major, name, student_id) VALUES("English", "Claire", 3))";
    std::string insert4 = R"(INSERT INTO student(name, major, student_id) VALUES("Jack", "Biology", 4))";
    std::string insert5 = R"(INSERT INTO student VALUES(5, "Mike", "Science"))";
    std::string insert6 = R"(INSERT INTO student VALUES(6, "Bob", NULL))";
    db.ParseRequest(insert1);
    db.ParseRequest(insert2);
    db.ParseRequest(insert3);
    db.ParseRequest(insert4);
    db.ParseRequest(insert5);
    std::cout << db.ParseRequest(insert6) << '\n';


    // std::string update = R"(UPDATE student SET name="Bob", major = "BIO", student_id =22 WHERE student_id =)";
    // db.ParseRequest(update);

    // std::string drop_table = "DROP TABLE student";
    // db.ParseRequest(drop_table);


    std::string path_save = R"(C:\Programms\LABWORKS\labwork-12-ShaDi777\bin\db.txt)";
    db.SaveToFile(path_save);
    */

    /*std::string path_load = R"(C:\Programms\LABWORKS\labwork-12-ShaDi777\bin\db.txt)";
    db.LoadFrom(path_load);

    // std::string update = R"(UPDATE student SET name="Bob", major = "BIO", student_id =22 WHERE name = "Goga" OR major IS NULL or student_id = 10)";
    //std::string update = R"(UPDATE student SET name="Bob", major = "BIO", student_id =22 WHERE "Goga" = name OR major IS NULL or 10 = student_id)";
    //std::string update = R"(UPDATE student SET name="Bob", major = "BIO", student_id =22)";
    //std::cout << db.ParseRequest(update);


    std::string select =
    "SELECT * FROM student;";

    //std::cout << db.ParseRequest(select);


    //std::string select_2 = "select name from student where major = \"Biology\"";
    //std::cout << db.ParseRequest(select_2);*/

    /*std::string insert =
            "INSERT INTO student "
            "VALUES(7, \"Vova\", \"Chemistry\"),"
            "(8, \"Vladimir\", \"Physics\");";
    std::cout << db.ParseRequest(insert);*/

    return 0;
}
