#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Struct to hold course details
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;// List of prerequisite course numbers
};

// Struct for each node in the BST
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course c) {
        course = c;
        left = nullptr;
        right = nullptr;
    }
};

// Binary Search Tree to store courses
class CourseBST {
private:
    Node* root;

    // In-order traversal: left → node → right
    void inOrder(Node* node) {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
        inOrder(node->right);
    }

    // Recursively insert a course
    Node* insert(Node* node, Course course) {
        if (node == nullptr) return new Node(course);
        if (course.courseNumber < node->course.courseNumber) {
            node->left = insert(node->left, course);
        }
        else {
            node->right = insert(node->right, course);
        }
        return node;
    }

    // Recursively search for a course
    Node* search(Node* node, string courseNumber) {
        if (node == nullptr || node->course.courseNumber == courseNumber) return node;
        if (courseNumber < node->course.courseNumber) return search(node->left, courseNumber);
        return search(node->right, courseNumber);
    }

public:
    CourseBST() {
        root = nullptr;
    }

    // Public insert method
    void insert(Course course) {
        root = insert(root, course);
    }
    // Print all courses in sorted order
    void printAllCourses() {
        inOrder(root);
    }
    // Print a single course and its prerequisites
    void printCourse(string courseNumber) {
        Node* found = search(root, courseNumber);
        if (found == nullptr) {
            cout << "Course not found.\n";
        }
        else {
            cout << found->course.courseNumber << ", " << found->course.courseTitle << endl;
            if (!found->course.prerequisites.empty()) {
                cout << "Prerequisites: ";
                for (size_t i = 0; i < found->course.prerequisites.size(); ++i) {
                    cout << found->course.prerequisites[i];
                    if (i < found->course.prerequisites.size() - 1) cout << ", ";
                }
                cout << endl;
            }
            else {
                cout << "Prerequisites: None" << endl;
            }
        }
    }
};

// Function to read courses from file and load them into the BST
void loadCoursesFromFile(string filename, CourseBST& bst) {
    cout << "Trying to open file: " << filename << endl;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file.\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string courseNumber, courseTitle, prereq;
        Course course;
        // First two values are course number and title
        getline(ss, courseNumber, ',');
        getline(ss, courseTitle, ',');

        course.courseNumber = courseNumber;
        course.courseTitle = courseTitle;
        // Remaining values are prerequisites
        while (getline(ss, prereq, ',')) {
            course.prerequisites.push_back(prereq);
        }
        // Adding course to BST
        bst.insert(course);
    }

    file.close();
    cout << "Courses loaded successfully!\n";
}

// Displays the main menu
void displayMenu() {
    cout << "\nWelcome to the course planner.\n" << endl;
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit\n" << endl;
    cout << "Select a menu option by entering 1, 2, 3, or 9: ";
}

int main() {
    CourseBST bst;  // The main course storage
    int choice = 0;
    // Menu loop until user exits
    while (choice != 9) {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1: {
            string filename;
            cout << "Enter 'courses.csv' to load course data: ";
            cin >> filename;
            loadCoursesFromFile(filename, bst);
            break;
        }
        case 2:
            cout << "\nHere is a sample schedule:\n" << endl;
            bst.printAllCourses();
            break;
        case 3: {
            string courseNumber;
            cout << "Enter the course number: ";
            cin >> courseNumber;

            // Convert to uppercase for consistency
            transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);

            bst.printCourse(courseNumber);
            break;
        }
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;
        default:
            cout << "\n" << choice << " NOT a valid option.\n" << endl;
        }
    }

    return 0;
}
