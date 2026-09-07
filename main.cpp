#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

struct Student {
    int id{};
    std::string name;
    int age{};
    std::string course;
    std::string email;
};

const std::string DATA_FILE = "students.txt";

std::vector<Student> loadStudents() {
    std::vector<Student> students;
    std::ifstream file(DATA_FILE);
    Student student;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::size_t first = line.find('|');
        std::size_t second = line.find('|', first + 1);
        std::size_t third = line.find('|', second + 1);
        std::size_t fourth = line.find('|', third + 1);

        if (first == std::string::npos || second == std::string::npos ||
            third == std::string::npos || fourth == std::string::npos) {
            continue;
        }

        try {
            student.id = std::stoi(line.substr(0, first));
            student.name = line.substr(first + 1, second - first - 1);
            student.age = std::stoi(line.substr(second + 1, third - second - 1));
            student.course = line.substr(third + 1, fourth - third - 1);
            student.email = line.substr(fourth + 1);
            students.push_back(student);
        } catch (const std::exception&) {
            std::cerr << "Warning: skipped an invalid record in " << DATA_FILE << ".\n";
        }
    }

    return students;
}

bool saveStudents(const std::vector<Student>& students) {
    const std::string temporaryFile = DATA_FILE + ".tmp";
    std::ofstream file(temporaryFile, std::ios::trunc);

    if (!file) {
        return false;
    }

    for (const Student& student : students) {
        file << student.id << '|'
             << student.name << '|'
             << student.age << '|'
             << student.course << '|'
             << student.email << '\n';
    }

    file.close();
    if (!file) {
        return false;
    }

    std::remove(DATA_FILE.c_str());
    return std::rename(temporaryFile.c_str(), DATA_FILE.c_str()) == 0;
}

std::string readText(const std::string& prompt) {
    std::string value;
    do {
        std::cout << prompt;
        std::getline(std::cin, value);
        if (value.empty()) {
            std::cout << "This field cannot be empty.\n";
        }
        if (value.find('|') != std::string::npos) {
            std::cout << "The | character is not allowed.\n";
            value.clear();
        }
    } while (value.empty() && std::cin);
    return value;
}

int readInteger(const std::string& prompt, int minimum) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= minimum) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cout << "Please enter a number of at least " << minimum << ".\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

Student* findStudent(std::vector<Student>& students, int id) {
    auto result = std::find_if(students.begin(), students.end(),
                               [id](const Student& student) { return student.id == id; });
    return result == students.end() ? nullptr : &(*result);
}

void addStudent(std::vector<Student>& students) {
    int id = readInteger("Enter student ID: ", 1);
    if (findStudent(students, id) != nullptr) {
        std::cout << "A student with that ID already exists.\n";
        return;
    }

    Student student;
    student.id = id;
    student.name = readText("Enter name: ");
    student.age = readInteger("Enter age: ", 1);
    student.course = readText("Enter course: ");
    student.email = readText("Enter email: ");
    students.push_back(student);

    std::cout << (saveStudents(students) ? "Student added successfully.\n"
                                         : "Could not save the student data.\n");
}

void displayStudents(const std::vector<Student>& students) {
    if (students.empty()) {
        std::cout << "No student records found.\n";
        return;
    }

    std::cout << "\n"
              << "ID\tName\t\tAge\tCourse\t\tEmail\n"
              << "------------------------------------------------------------\n";
    for (const Student& student : students) {
        std::cout << student.id << '\t'
                  << student.name << "\t\t"
                  << student.age << '\t'
                  << student.course << "\t\t"
                  << student.email << '\n';
    }
}

void updateStudent(std::vector<Student>& students) {
    int id = readInteger("Enter the ID to update: ", 1);
    Student* student = findStudent(students, id);
    if (student == nullptr) {
        std::cout << "Student not found.\n";
        return;
    }

    std::cout << "Enter the new details.\n";
    student->name = readText("Enter name: ");
    student->age = readInteger("Enter age: ", 1);
    student->course = readText("Enter course: ");
    student->email = readText("Enter email: ");

    std::cout << (saveStudents(students) ? "Student updated successfully.\n"
                                         : "Could not save the student data.\n");
}

void deleteStudent(std::vector<Student>& students) {
    int id = readInteger("Enter the ID to delete: ", 1);
    auto result = std::remove_if(students.begin(), students.end(),
                                 [id](const Student& student) { return student.id == id; });
    if (result == students.end()) {
        std::cout << "Student not found.\n";
        return;
    }

    students.erase(result, students.end());
    std::cout << (saveStudents(students) ? "Student deleted successfully.\n"
                                         : "Could not save the student data.\n");
}

void printMenu() {
    std::cout << "\n===== Student Management System =====\n"
              << "1. Add student\n"
              << "2. Display students\n"
              << "3. Update student\n"
              << "4. Delete student\n"
              << "5. Exit\n";
}

int main() {
    std::vector<Student> students = loadStudents();

    while (true) {
        printMenu();
        int choice = readInteger("Choose an option: ", 1);

        switch (choice) {
            case 1:
                addStudent(students);
                break;
            case 2:
                displayStudents(students);
                break;
            case 3:
                updateStudent(students);
                break;
            case 4:
                deleteStudent(students);
                break;
            case 5:
                std::cout << "Goodbye.\n";
                return 0;
            default:
                std::cout << "Please choose an option from 1 to 5.\n";
        }
    }
}
