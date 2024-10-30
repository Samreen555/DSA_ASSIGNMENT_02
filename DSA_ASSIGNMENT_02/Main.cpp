#include <iostream>
#include <cmath>
#include <string>
#include <stdexcept>
using namespace std;
struct Node {
    int coefficient, exponent;
    Node* next;
    Node(int coeff, int exp) : coefficient(coeff), exponent(exp), next(nullptr) {}
};
class Polynomial {
private:
    Node* head;
public:
    Polynomial();
    ~Polynomial();
    void insert(int coefficient, int exponent);
    void display() const;
    Polynomial operator+(const Polynomial& other) const;
    Polynomial operator-(const Polynomial& other) const;
    Polynomial operator*(const Polynomial& other) const;
    int evaluate(float x) const;
    bool isEmpty() const; // Check if the polynomial is empty
};
Polynomial::Polynomial() : head(nullptr) {}
Polynomial::~Polynomial() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}
bool Polynomial::isEmpty() const {
    return head == nullptr;
}
void Polynomial::insert(int coefficient, int exponent) {
    if (coefficient == 0) return;

    Node* newNode = new Node(coefficient, exponent);
    if (!head || head->exponent < exponent) {
        newNode->next = head;
        head = newNode;
        return;
    }

    Node* current = head;
    Node* prev = nullptr;
    while (current && current->exponent > exponent) {
        prev = current;
        current = current->next;
    }

    if (current && current->exponent == exponent) {
        current->coefficient += coefficient;
        if (current->coefficient == 0) { // Remove term if coefficient becomes zero
            if (prev) prev->next = current->next;
            else head = current->next;
            delete current;
        }
        delete newNode;
    }
    else {
        newNode->next = current;
        if (prev) prev->next = newNode;
        else head = newNode;
    }
}
void Polynomial::display() const {
    if (!head) {
        cout << "0" << endl;
        return;
    }

    Node* temp = head;
    bool isFirstTerm = true;

    while (temp) {
        if (!isFirstTerm && temp->coefficient > 0) cout << " + ";
        isFirstTerm = false;

        if (temp->exponent == 0) cout << temp->coefficient;
        else if (temp->exponent == 1) cout << temp->coefficient << "x";
        else cout << temp->coefficient << "x^" << temp->exponent;

        temp = temp->next;
    }
    cout << endl;
}
Polynomial Polynomial::operator+(const Polynomial& other) const {
    if (this->isEmpty() || other.isEmpty()) {
        throw runtime_error("Addition requires two non-empty polynomials.");
    }

    Polynomial result;
    Node* current1 = head;
    Node* current2 = other.head;

    while (current1 || current2) {
        if (current1 && (!current2 || current1->exponent > current2->exponent)) {
            result.insert(current1->coefficient, current1->exponent);
            current1 = current1->next;
        }
        else if (current2 && (!current1 || current2->exponent > current1->exponent)) {
            result.insert(current2->coefficient, current2->exponent);
            current2 = current2->next;
        }
        else {
            result.insert(current1->coefficient + current2->coefficient, current1->exponent);
            current1 = current1->next;
            current2 = current2->next;
        }
    }
    return result;
}
Polynomial Polynomial::operator-(const Polynomial& other) const {
    if (this->isEmpty() || other.isEmpty()) {
        throw runtime_error("Subtraction requires two non-empty polynomials.");
    }

    Polynomial result;
    Node* current1 = head;
    Node* current2 = other.head;

    while (current1 || current2) {
        if (current1 && (!current2 || current1->exponent > current2->exponent)) {
            result.insert(current1->coefficient, current1->exponent);
            current1 = current1->next;
        }
        else if (current2 && (!current1 || current2->exponent > current1->exponent)) {
            result.insert(-current2->coefficient, current2->exponent);
            current2 = current2->next;
        }
        else {
            result.insert(current1->coefficient - current2->coefficient, current1->exponent);
            current1 = current1->next;
            current2 = current2->next;
        }
    }
    return result;
}
Polynomial Polynomial::operator*(const Polynomial& other) const {
    if (this->isEmpty() || other.isEmpty()) {
        throw runtime_error("Multiplication requires two non-empty polynomials.");
    }

    Polynomial result;
    for (Node* temp1 = head; temp1; temp1 = temp1->next) {
        for (Node* temp2 = other.head; temp2; temp2 = temp2->next) {
            result.insert(temp1->coefficient * temp2->coefficient, temp1->exponent + temp2->exponent);
        }
    }
    return result;
}
int Polynomial::evaluate(float x) const {
    float result = 0;
    Node* temp = head;
    while (temp) {
        result += temp->coefficient * pow(x, temp->exponent);
        temp = temp->next;
    }
    return result;
}
void display_menu() {
    cout << "********Polynomial Operations*************\n";
    cout << "1. Insert a term into the polynomial\n";
    cout << "2. Display the polynomial\n";
    cout << "3. Add two polynomials\n";
    cout << "4. Subtract two polynomials\n";
    cout << "5. Multiply two polynomials\n";
    cout << "6. Evaluate a polynomial\n";
    cout << "7. Exit\n";
    cout << "*******************************************\n";
}
int main() {
    Polynomial p1, p2;
    string choice;
    int poly_choice;

    do {
        display_menu();
        cout << "Enter the choice:\n";
        getline(cin, choice);

        if (choice == "1") {
            int coefficient, exponent;
            cout << "Enter coefficient and exponent: ";
            while (!(cin >> coefficient >> exponent)) {
                cout << "Invalid input. Please enter an integer: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Insert into Polynomial 1 or 2? ";
            while (!(cin >> poly_choice)) {
                cout << "Invalid input. Please enter an integer: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (poly_choice == 1) {
                p1.insert(coefficient, exponent);
            }
            else if (poly_choice == 2) {
                p2.insert(coefficient, exponent);
            }
            cin.ignore();
        }
        else if (choice == "2") {
            cout << "Polynomial 1: ";
            p1.display();
            cout << "Polynomial 2: ";
            p2.display();
        }
        else if (choice == "3") {
            try {
                Polynomial sum = p1 + p2;
                cout << "Sum: ";
                sum.display();
            }
            catch (const runtime_error& e) {
                cout << e.what() << endl;
            }
        }
        else if (choice == "4") {
            try {
                Polynomial diff = p1 - p2;
                cout << "Difference: ";
                diff.display();
            }
            catch (const runtime_error& e) {
                cout << e.what() << endl;
            }
        }
        else if (choice == "5") {
            try {
                Polynomial product = p1 * p2;
                cout << "Product: ";
                product.display();
            }
            catch (const runtime_error& e) {
                cout << e.what() << endl;
            }
        }
        else if (choice == "6") {
            int x;
            cout << "Evaluate which polynomial (1 or 2)? ";
            int poly_choice;
            while (!(cin >> poly_choice)) {
                cout << "Invalid input. Please enter an integer: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Enter value of x: ";
            while (!(cin >> x)) {
                cout << "Invalid input. Please enter an integer: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cin.ignore();

            if (poly_choice == 1) {
                cout << "Result: " << p1.evaluate(x) << endl;
            }
            else if (poly_choice == 2) {
                cout << "Result: " << p2.evaluate(x) << endl;
            }
            else {
                cout << "Invalid polynomial choice." << endl;
            }
        }
        else if (choice == "7") {
            cout << "Exiting the program." << endl;
        }
        else {
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != "7");

    system("pause");
    return 0;
}