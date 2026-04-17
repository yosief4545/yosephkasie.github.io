// ========
// Yoseph Kasie 
// 04/20/2025
// CS 300 Project Two 
//


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Course object
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Tree node structure
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course aCourse) : course(aCourse), left(nullptr), right(nullptr) {}
};

// Binary Search Tree class
class BinarySearchTree {
private:
    Node* root;

    void insert(Node*& node, Course course) {
        if (node == nullptr) {
            node = new Node(course);
        }
        else if (course.courseNumber < node->course.courseNumber) {
            insert(node->left, course);
        }
        else {
            insert(node->right, course);
        }
    }

    void inOrder(Node* node) {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
            inOrder(node->right);
        }
    }

    Course* search(Node* node, string courseNumber) {
        if (node == nullptr) return nullptr;
        if (node->course.courseNumber == courseNumber) return &node->course;
        if (courseNumber < node->course.courseNumber) return search(node->left, courseNumber);
        return search(node->right, courseNumber);
    }

public:
    BinarySearchTree() : root(nullptr) {}

    void Insert(Course course) {
        insert(root, course);
    }

    void PrintCourseList() {
        inOrder(root);
    }

    Course* FindCourse(string courseNumber) {
        return search(root, courseNumber);
    }
};

// Helper function to split string by delimiter
vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Convert string to uppercase
string toUpper(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// Load courses from file
void LoadCourses(string fileName, BinarySearchTree& bst) {
    ifstream file(fileName);
    string line;

    while (getline(file, line)) {
        vector<string> tokens = split(line, ',');
        if (tokens.size() >= 2) {
            Course course;
            course.courseNumber = tokens[0];
            course.courseTitle = tokens[1];
            for (size_t i = 2; i < tokens.size(); ++i) {
                course.prerequisites.push_back(tokens[i]);
            }
            bst.Insert(course);
        }
    }

    file.close();
}

int main() {
    BinarySearchTree bst;
    int choice = 0;
    string fileName;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        cout << "\n1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            cout << "Enter file name: ";
            getline(cin, fileName);
            LoadCourses(fileName, bst);
            cout << "Courses loaded successfully." << endl;
        }
        else if (choice == 2) {
            cout << "\nHere is a sample schedule:" << endl;
            bst.PrintCourseList();
        }
        else if (choice == 3) {
            string courseNum;
            cout << "What course do you want to know about? ";
            getline(cin, courseNum);
            courseNum = toUpper(courseNum);
            Course* course = bst.FindCourse(courseNum);
            if (course) {
                cout << course->courseNumber << ", " << course->courseTitle << endl;
                if (!course->prerequisites.empty()) {
                    cout << "Prerequisites: ";
                    for (size_t i = 0; i < course->prerequisites.size(); ++i) {
                        cout << course->prerequisites[i];
                        if (i < course->prerequisites.size() - 1) cout << ", ";
                    }
                    cout << endl;
                }
                else {
                    cout << "Prerequisites: None" << endl;
                }
            }
            else {
                cout << "Course not found." << endl;
            }
        }
        else if (choice == 9) {
            cout << "Thank you for using the course planner!" << endl;
        }
        else {
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}
