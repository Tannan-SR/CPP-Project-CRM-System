#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>

using namespace std;

class Person {
protected:
    string name;
    string email;
    string phone;

public:
    Person(const string& name, const string& email, const string& phone)
        : name(name), email(email), phone(phone) {}

    string getName() const {
        return name;
    }

    string getEmail() const {
        return email;
    }

    string getPhone() const {
        return phone;
    }
};

class Customer : public Person {
private:
    int id;

public:
    Customer(int id, const string& name, const string& email, const string& phone)
        : Person(name, email, phone), id(id) {}

    int getID() const {
        return id;
    }

    void printInfo() const {
        cout << "Customer ID: " << id << endl;
        cout << "Name: " << getName() << endl;
        cout << "Email: " << getEmail() << endl;
        cout << "Phone: " << getPhone() << endl;
    }
};

class CustomerManager {
private:
    vector<unique_ptr<Customer>> customers;

public:
    void addCustomer(unique_ptr<Customer> customer) {
        customers.push_back(move(customer));
    }

    void printAllCustomers() const {
        if (customers.empty()) {
            cout << "No customers found." << endl;
        } else {
            for (const auto& customer : customers) {
                customer->printInfo();
                cout << endl;
            }
        }
    }

    Customer* getCustomerByID(int customerID) const {
        auto it = find_if(customers.begin(), customers.end(), [&](const unique_ptr<Customer>& customer) {
            return customer->getID() == customerID;
        });

        if (it != customers.end()) {
            return it->get();
        }

        return nullptr;
    }

    void saveCustomerData() const {
        ofstream file("customerdata.csv", ios::out);
        if (!file) {
            throw runtime_error("Failed to open file for writing.");
        }

        for (const auto& customer : customers) {
            file << customer->getID() << ","
                 << customer->getName() << ","
                 << customer->getEmail() << ","
                 << customer->getPhone() << endl;
        }

        file.close();
    }

    void loadCustomerData() {
        ifstream file("customerdata.csv");
        if (!file) {
            throw runtime_error("Failed to open file for reading.");
        }

        customers.clear();

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string field;
            vector<string> fields;

            while (getline(ss, field, ',')) {
                fields.push_back(field);
            }

            if (fields.size() == 4) {
                int id = stoi(fields[0]);
                string name = fields[1];
                string email = fields[2];
                string phone = fields[3];

                unique_ptr<Customer> customer = make_unique<Customer>(id, name, email, phone);
                addCustomer(move(customer));
            }
        }

        file.close();
    }
};

class UIManager {
public:
    int getUserChoice() const {
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        return choice;
    }

    void displayCustomerDetails(const Customer& customer) const {
        cout << "Customer Details:" << endl;
        cout << "ID: " << customer.getID() << endl;
        cout << "Name: " << customer.getName() << endl;
        cout << "Email: " << customer.getEmail() << endl;
        cout << "Phone: " << customer.getPhone() << endl;
    }

    void displayMessage(const string& message) const {
        cout << message << endl;
    }
};

class CRMSystem {
private:
    CustomerManager customerManager;
    UIManager uiManager;

public:
    void displayMenu() const {
        cout << "1. Add Customer" << endl;
        cout << "2. View Customer Details" << endl;
        cout << "3. Save Customer Data" << endl;
        cout << "4. Load Customer Data" << endl;
        cout << "5. Exit" << endl;
    }

    void run() {
        while (true) {
            displayMenu();
            int choice = uiManager.getUserChoice();

            if (choice == 1) {
                // Add Customer
                int id;
                string name, email, phone;
                cout << "Enter customer ID: ";
                cin >> id;
                cout << "Enter customer name: ";
                cin >> name;
                cout << "Enter customer email: ";
                cin >> email;
                cout << "Enter customer phone: ";
                cin >> phone;
                unique_ptr<Customer> newCustomer = make_unique<Customer>(id, name, email, phone);
                customerManager.addCustomer(move(newCustomer));
                uiManager.displayMessage("Customer added successfully.");
            } else if (choice == 2) {
                // View Customer Details
                int id;
                cout << "Enter customer ID (Enter 0 to view all customers): ";
                cin >> id;
                if (id == 0) {
                    customerManager.printAllCustomers();
                } else {
                    Customer* customer = customerManager.getCustomerByID(id);
                    if (customer) {
                        uiManager.displayCustomerDetails(*customer);
                    } else {
                        uiManager.displayMessage("Customer not found.");
                    }
                }
            } else if (choice == 3) {
                // Save Customer Data
                customerManager.saveCustomerData();
                uiManager.displayMessage("Customer data saved successfully.");
            } else if (choice == 4) {
                // Load Customer Data
                customerManager.loadCustomerData();
                uiManager.displayMessage("Customer data loaded successfully.");
            } else if (choice == 5) {
                // Exit
                break;
            } else {
                uiManager.displayMessage("Invalid choice. Please try again.");
            }
        }
    }
};

int main() {
    CRMSystem crmSystem;
    crmSystem.run();

    return 0;
}
