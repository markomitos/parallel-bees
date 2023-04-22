#include <iostream>
#include "Api.h"

using namespace std;

int main()
{
    //// TEST ZA JSON
    //// 
    //// 
    //// JSON string to parse
    //std::string json_string = "{\"name\": \"John\", \"age\": 30, \"isStudent\": true, \"grades\": [8.5, 9, 7.5]}";

    //// Parse the JSON string
    //json data = json::parse(json_string);

    //// Access the JSON data as a C++ object
    //std::string name = data["name"];
    //int age = data["age"];
    //bool is_student = data["isStudent"];
    //std::vector<double> grades = data["grades"];

    //// Print the parsed data
    //std::cout << "Name: " << name << std::endl;
    //std::cout << "Age: " << age << std::endl;
    //std::cout << "Is student: " << is_student << std::endl;
    //std::cout << "Grades: ";
    //for (auto grade : grades) {
    //    std::cout << grade << " ";
    //}
    //std::cout << std::endl;

    Api api;
    api.InitializeGame();

    return 0;
}