#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string.h>
using namespace std;

class Customer
{
public:
    string id;
    string name;
    double purchaseValue;
    string contactInfo;
    string feedback;
    string category; // GOLD OR PLATINUM OR SILVER
    string product;
    string ptype; // CATEGORY OF PRODUCT
    static vector<string> platinum_members;
    string discountAvail; // Yes or No
    int loyaltyPoints;    // Loyalty points for the customer

    // Other member functions and data remain unchanged.
};

class CRMSystem
{
private:
    std::vector<Customer> customers;
    std::vector<std::string> items;

    void LoadCustomerData()
    {
        std::ifstream file("customerdata.csv");
        if (file)
        {
            std::string line;
            while (std::getline(file, line))
            {
                std::istringstream iss(line);
                std::string id, name, purchaseValue, contactInfo, feedback, category;

                if (std::getline(iss, id, ',') &&
                    std::getline(iss, name, ',') &&
                    std::getline(iss, purchaseValue, ',') &&
                    std::getline(iss, contactInfo, ',') &&
                    std::getline(iss, feedback, ',') &&
                    std::getline(iss, category))
                {
                    customers.push_back({id, name, std::stod(purchaseValue), contactInfo, feedback, category});
                }
            }
            file.close();
        }
    }

    void SaveCustomerData()
    {
        std::ofstream file("customerdata.csv");
        if (file)
        {
            for (const auto &customer : customers)
            {
                file << customer.id << "," << customer.name << "," << customer.purchaseValue << ","
                     << customer.contactInfo << "," << customer.feedback << "," << customer.category << ","
                     << customer.product << "," << customer.ptype << "," << customer.discountAvail << "," << customer.loyaltyPoints << "\n";
            }
            file.close();
        }
    }

    void LoadItemData()
    {
        std::ifstream file("items.csv");
        if (file)
        {
            std::string line;
            while (std::getline(file, line))
            {
                items.push_back(line);
            }
            file.close();
        }
    }

    std::vector<std::string> GetAvailableProducts(const std::string &category)
    {
        std::vector<std::string> availableProducts;
        for (const auto &item : items)
        {
            std::istringstream iss(item);
            std::string itemCategory, itemName, itemPrice;

            if (std::getline(iss, itemCategory, ',') &&
                std::getline(iss, itemName, ',') &&
                std::getline(iss, itemPrice))
            {
                if (itemCategory == category)
                {
                    availableProducts.push_back(itemName);
                }
            }
        }
        return availableProducts;
    }

    void ShowAvailableProducts(const std::vector<std::string> &products)
    {
        int index = 1;
        for (const auto &product : products)
        {
            std::cout << index++ << ". " << product << std::endl;
        }
    }

    std::string GetCustomerCategory(double purchaseValue)
    {
        if (purchaseValue >= 300.0)
        {
            return "PLATINUM";
        }
        else if (purchaseValue >= 100.0)
        {
            return "GOLD";
        }
        else
        {
            return "SILVER";
        }
    }

    std::string GenerateNewCustomerId()
    {
        if (customers.empty())
        {
            return "C1";
        }
        else
        {
            std::string lastId = customers.back().id;
            std::string idNumber = lastId.substr(1);
            int newIdNumber = std::stoi(idNumber) + 1;
            return "C" + std::to_string(newIdNumber);
        }
    }

public:
    CRMSystem()
    {
        LoadCustomerData();
        LoadItemData();
    }

    void Login()
    {
        std::cout << "Enter your name: ";
        std::string name;
        std::cin.ignore();
        std::getline(std::cin, name);

        std::cout << "Enter your contact information: ";
        std::string contactInfo;
        std::getline(std::cin, contactInfo);

        std::string customerId = GenerateNewCustomerId();
        std::cout << "Your customer ID is: " << customerId << std::endl;

        std::string category = "SILVER";

        customers.push_back({customerId, name, 0.0, contactInfo, "", category});
        SaveCustomerData();
    }

    void BuyItem(const std::string &customerId)
    {
        double purchaseValue = 0.0;
        ifstream fp("items.csv");
        vector<vector<string>> content;
        vector<string> row;
        string line, word;

        if (fp.is_open())
        {
            while (getline(fp, line))
            {
                row.clear();
                stringstream str(line);
                while (getline(str, word))
                    row.push_back(word);
                content.push_back(row);
            }
        }
        else
        {
            cout << "Could not open the file\n";
        }

        for (int i = 0; i < content.size(); i++)
        {
            int j = 0;
            cout << content[i][0] << " ";

            cout << "\n";
        }

        std::cout << "Enter the category of the product you want to buy: ";
        std::string category;
        std::cin >> category;

        std::vector<std::string> availableProducts = GetAvailableProducts(category);
        if (availableProducts.empty())
        {
            std::cout << "No available products in the chosen category." << std::endl;
            return;
        }

        std::cout << "Available Products:" << std::endl;
        ShowAvailableProducts(availableProducts);

        int choice;
        std::cout << "Enter the index of the product you want to buy: ";
        std::cin >> choice;

        if (choice < 1 || choice > availableProducts.size())
        {
            std::cout << "Invalid choice. Please try again." << std::endl;
            return;
        }

        std::string chosenProduct = availableProducts[choice - 1];

        std::string price;
        string itemm;
        for (const auto &item : items)
        {
            std::istringstream iss(item);
            std::string itemCategory, itemName, itemPrice;

            if (std::getline(iss, itemCategory, ',') &&
                std::getline(iss, itemName, ',') &&
                std::getline(iss, itemPrice))
            {
                if (itemCategory == category && itemName == chosenProduct)
                {
                    price = itemPrice;
                    itemm = itemName;
                    break;
                }
            }
        }

        if (price.empty())
        {
            std::cout << "Invalid product. Please try again." << std::endl;
            return;
        }

        double itemPrice = std::stod(price);
        purchaseValue += itemPrice;

        for (auto &customer : customers)
        {
            if (customer.id == customerId)
            {
                customer.purchaseValue += purchaseValue;
                customer.category = GetCustomerCategory(customer.purchaseValue);
                customer.product = itemm;
                customer.ptype = category;
                customer.loyaltyPoints += static_cast<int>(customer.purchaseValue) / 100 * 10; // 10 loyalty points for every $100 purchase

                break;
            }
        }

        std::cout << "Purchase successful! The price of the item is: " << itemPrice << std::endl;

        SaveCustomerData();
    }

    void ProvideRecommendations(const std::string &customerId)
    {
        std::string customerCategory;
        for (const auto &customer : customers)
        {
            if (customer.id == customerId)
            {
                customerCategory = customer.ptype;

                break;
            }
        }

        std::vector<std::string> recommendations;
        for (const auto &item : items)
        {
            std::istringstream iss(item);
            std::string itemCategory, itemName, itemPrice;

            if (std::getline(iss, itemCategory, ',') &&
                std::getline(iss, itemName, ',') &&
                std::getline(iss, itemPrice))
            {
                if (itemCategory == customerCategory)
                {
                    recommendations.push_back(itemName);
                }
            }
        }

        if (recommendations.empty())
        {
            std::cout << "No recommendations available for your category." << std::endl;
        }
        else
        {
            std::cout << "Recommendations for you:\n"
                      << std::endl;
            ShowAvailableProducts(recommendations);
            cout << '\n';
        }
    }

    void GetFeedback(const std::string &customerId)
    {
        std::cout << "Please provide your feedback: ";
        std::string feedback;
        std::cin.ignore();
        std::getline(std::cin, feedback);

        for (auto &customer : customers)
        {
            if (customer.id == customerId)
            {
                customer.feedback = feedback;
                break;
            }
        }
        SaveCustomerData();
        std::cout << "Thank you for your feedback!" << std::endl;
    }

    void offerdiscount(const vector<string> &platinumMembers)
    {
        double discountPercentage = 0.1; // 10% discount

        for (const auto &memberId : platinumMembers)
        {
            for (auto &customer : customers)
            {
                if (customer.id == memberId)
                {
                    customer.purchaseValue -= (customer.purchaseValue * discountPercentage);
                    break;
                }
            }
        }

        ofstream outputFile("customerdata.csv");
        if (!outputFile)
        {
            cout << "Failed to open the file." << endl;
            return;
        }

        for (const auto &customer : customers)
        {
            outputFile << customer.id << "," << customer.name << "," << customer.purchaseValue << "," << customer.contactInfo << "," << customer.feedback << "," << customer.category << "," << customer.product << "," << customer.ptype << "," << customer.discountAvail << "," << customer.loyaltyPoints << endl;
        }

        cout << "Discount of " << discountPercentage * 100 << "% applied to the purchase values of platinum members." << endl;
    }

    void getplatinum(const vector<Customer> &customers)
    {

        fstream f("customerdata.csv", ios::in);
        if (!f.is_open())
        {
            cout << "Failed to open the file." << endl;
            return;
        }

        vector<string> row;
        string line, word;

        int count = 0;

        while (getline(f, line))
        {
            row.clear();
            stringstream s(line);

            while (getline(s, word, ','))
            {
                row.push_back(word);
            }

            string category = row[5];

            if (category == "PLATINUM")
            {
                count++;
                Customer::platinum_members.push_back(row[0]);
            }
        }

        cout << "Total Platinum Members: " << count << endl;
        cout << "Platinum Members' IDs: ";
        for (int i = 0; i < Customer::platinum_members.size(); i++)
        {
            cout << Customer::platinum_members[i] << " ";
        }
        cout << endl;

        if (count == 0)
        {
            cout << "No platinum members found." << endl;
        }
        else
        {
            cout << "Offering discount to platinum members" << endl;
            offerdiscount(Customer::platinum_members);
        }

        f.close();
    }
    void read_file()
    {

        ifstream fp("customerdata.csv");
        vector<vector<string>> content;
        vector<string> row;
        string line, word;

        if (fp.is_open())
        {
            while (getline(fp, line))
            {
                row.clear();
                stringstream str(line);
                while (getline(str, word, ','))
                    row.push_back(word);
                content.push_back(row);
            }
        }
        else
        {
            cout << "Could not open the file\n";
        }

        for (int i = 0; i < content.size(); i++)
        {
            for (int j = 0; j < content[i].size(); j++)
            {
                cout << content[i][j] << " ";
            }
            cout << "\n";
        }
    }

    void AdminFeature()
    {
        int choice;
        cout << "Enter 0 to view feedbacks,1 to view all platinum members and offer discounts and 2 to show all customers";
        cin >> choice;

        if (choice == 0)
        {
            // Show feedback
            cout << "Feedback from customers:" << endl;
            for (const auto &customer : customers)
            {
                cout << "Customer ID: " << customer.id << endl;
                cout << "Feedback: " << customer.feedback << endl;
                cout << "---------------------------" << endl;
            }
        }
        else if (choice == 1)
        {
            // Show platinum members and offer discounts
            getplatinum(customers);
        }

        else if (choice == 2)
        {
            read_file();
        }
        else
        {
            cout << "Invalid choice." << endl;
        }
    }
};

vector<string> Customer::platinum_members;

int main()
{
    CRMSystem crm;
    std::string customerId;
label:
    std::cout << "* Customer Relationship Management System *" << std::endl;
    std::cout << "1. Login" << std::endl;
    std::cout << "2. Admin Feature" << std::endl;
    std::cout << "Enter your choice: ";
    int choice;
    std::cin >> choice;

    try
    {
        if (choice == 1)
        {
            crm.Login();
            std::cout << "Login successful!" << std::endl;
            std::cout << "Enter your customer ID to proceed: ";
            std::cin >> customerId;

            while (true)
            {
                std::cout << "1. Buy Item" << std::endl;
                std::cout << "2. Provide Recommendations" << std::endl;
                std::cout << "3. Give Feedback" << std::endl;
                std::cout << "4. Logout" << std::endl;
                std::cout << "Enter your choice: ";
                std::cin >> choice;

                if (choice == 1)
                {
                    crm.BuyItem(customerId);
                }
                else if (choice == 2)
                {
                    crm.ProvideRecommendations(customerId);
                }
                else if (choice == 3)
                {
                    crm.GetFeedback(customerId);
                }
                else if (choice == 4)
                {
                    goto label;
                    break;
                }
                else
                {
                    std::cout << "Invalid choice. Please try again." << std::endl;
                }
            }
        }
        else if (choice == 2)
        {
            crm.AdminFeature();
        }
        else
        {
            std::cout << "Invalid choice. Exiting program." << std::endl;
        }

        goto label;
    }
    catch (...)
    {
        cout << "Choice is invalid\n";
    }

    return 0;
}