// Yoseph Kasie
// 04/20/2025
// CS 300 Project Two - Enhanced Version

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

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

    Node(const Course& aCourse) : course(aCourse), left(nullptr), right(nullptr) {}
};

// Binary Search Tree class
class BinarySearchTree {
private:
    Node* root;

    void insert(Node*& node, const Course& course) {
        if (node == nullptr) {
            node = new Node(course);
        }
        else if (course.courseNumber < node->course.courseNumber) {
            insert(node->left, course);
        }
        else if (course.courseNumber > node->course.courseNumber) {
            insert(node->right, course);
        }
    }

    void inOrder(Node* node) const {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
            inOrder(node->right);
        }
    }

    Course* search(Node* node, const string& courseNumber) const {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->course.courseNumber == courseNumber) {
            return &node->course;
        }

        if (courseNumber < node->course.courseNumber) {
            return search(node->left, courseNumber);
        }

        return search(node->right, courseNumber);
    }

    void destroyTree(Node* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    BinarySearchTree() : root(nullptr) {}

    ~BinarySearchTree() {
        destroyTree(root);
    }

    void Insert(const Course& course) {
        insert(root, course);
    }

    void PrintCourseList() const {
        inOrder(root);
    }

    Course* FindCourse(const string& courseNumber) const {
        return search(root, courseNumber);
    }

    bool IsEmpty() const {
        return root == nullptr;
    }
};

// Helper function to split string
vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);

    while (getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }

    return tokens;
}

// Convert string to uppercase
string toUpper(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// Load courses from file
bool LoadCourses(const string& fileName, BinarySearchTree& bst) {
    ifstream file(fileName);
    string line;

    if (!file.is_open()) {
        cout << "Error: Could not open file \"" << fileName << "\"." << endl;
        return false;
    }

    while (getline(file, line)) {
        vector<string> tokens = split(line, ',');

        if (tokens.size() >= 2) {
            Course course;
            course.courseNumber = toUpper(tokens[0]);
            course.courseTitle = tokens[1];

            for (size_t i = 2; i < tokens.size(); ++i) {
                course.prerequisites.push_back(toUpper(tokens[i]));
            }

            bst.Insert(course);
        }
    }

    file.close();
    return true;
}

int main() {
    BinarySearchTree bst;
    int choice = 0;
    string fileName;
    bool dataLoaded = false;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        cout << "\nMenu:" << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "What would you like to do? ";

        cin >> choice;

        while (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number from the menu: ";
            cin >> choice;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            if (dataLoaded) {
                cout << "Data has already been loaded." << endl;
                continue;
            }

            cout << "Enter file name: ";
            getline(cin, fileName);

            if (LoadCourses(fileName, bst)) {
                dataLoaded = true;
                cout << "Courses loaded successfully." << endl;
            }
        }
        else if (choice == 2) {
            if (!dataLoaded || bst.IsEmpty()) {
                cout << "Please load the course data first." << endl;
            }
            else {
                cout << "\nHere is a sample schedule:" << endl;
                bst.PrintCourseList();
            }
        }
        else if (choice == 3) {
            if (!dataLoaded || bst.IsEmpty()) {
                cout << "Please load the course data first." << endl;
            }
            else {
                string courseNum;
                cout << "What course do you want to know about? ";
                getline(cin, courseNum);
                courseNum = toUpper(courseNum);

                Course* course = bst.FindCourse(courseNum);

                if (course != nullptr) {
                    cout << course->courseNumber << ", " << course->courseTitle << endl;

                    if (!course->prerequisites.empty()) {
                        cout << "Prerequisites: ";
                        for (size_t i = 0; i < course->prerequisites.size(); ++i) {
                            cout << course->prerequisites[i];
                            if (i < course->prerequisites.size() - 1) {
                                cout << ", ";
                            }
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