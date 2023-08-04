#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <ctime> 

using namespace std;

class Wallet
{
public:
    string name;
    double balance;
    Wallet(string name, double balance) : name(name), balance(balance) {}
};

class Card
{
public:
    string name;
    double balance;
    bool isCredit;
    Card(string name, double balance, bool isCredit) : name(name), balance(balance), isCredit(isCredit) {}
};

class Expense
{
public:
    string date;
    string category;
    double amount;
    Expense(string date, string category, double amount) : date(date), category(category), amount(amount) {}
};

bool comparePairs(const pair<string, double>& a, const pair<string, double>& b)
{
    return a.second > b.second;
}

class FinanceManager
{
private:
    vector<Wallet> wallets;
    vector<Card> cards;
    vector<Expense> expenses;
public:
    void addWallet(string name, double balance)
    {
        wallets.push_back(Wallet(name, balance));
    }

    void addCard(string name, double balance, bool isCredit)
    {
        cards.push_back(Card(name, balance, isCredit));
    }

    void addWalletBalance(string walletName, double amount)
    {
        for (auto& wallet : wallets)
        {
            if (wallet.name == walletName)
            {
                wallet.balance += amount;
                cout << "Added " << walletName << " with " << amount << ". New balance: " << wallet.balance << endl;
                return;
            }
        }
        cout << "Wallet not found: " << walletName << endl;
    }

    void addCardBalance(string cardName, double amount)
    {
        for (auto& card : cards)
        {
            if (card.name == cardName)
            {
                card.balance += amount;
                cout << "Added " << cardName << " with " << amount << ". New balance: " << card.balance << endl;
                return;
            }
        }
        cout << "Card not found: " << cardName << endl;
    }

    void addExpense(string date, double amount, string category)
    {
        expenses.push_back(Expense(date, category, amount));
    }

    void generateReportByPeriod(const string& period)
    {
        map<string, double> report;
        for (auto& expense : expenses)
        {
            report[expense.category] += expense.amount;
        }

        cout << "Report by " << period << ":" << endl;
        for (const auto& item : report) {
            cout << item.first << ": " << item.second << endl;
        }
    }

    void generateRating()
    {
        map<string, double> categoryTotals;
        for (const auto& expense : expenses)
        {
            categoryTotals[expense.category] += expense.amount;
        }

        vector<pair<string, double>> sortedCategoryTotals(categoryTotals.begin(), categoryTotals.end());
        sort(sortedCategoryTotals.begin(), sortedCategoryTotals.end(), comparePairs);

        cout << "\nTop 3 Categories by Total Expense:\n";
        int count = min(3, static_cast<int>(sortedCategoryTotals.size()));
        for (int i = 0; i < count; ++i) {
            cout << sortedCategoryTotals[i].first << ": " << sortedCategoryTotals[i].second << endl;
        }
    }

    void saveReports(const string& filename)
    {
        ofstream file(filename);

        file << "Wallets:" << endl;
        for (auto& wallet : wallets)
        {
            file << wallet.name << ": " << wallet.balance << endl;
        }

        file << "Cards:" << endl;
        for (auto& card : cards)
        {
            file << card.name << ": " << card.balance << endl;
        }

        file << "Expenses:" << endl;
        for (auto& expense : expenses)
        {
            file << expense.date << " | " << expense.category << " | " << expense.amount << endl;
        }

        file.close();
    }
};

void printMainMenu() {
    cout << "\nSelect an option:\n";
    cout << "1. Add Wallet\n";
    cout << "2. Add Card\n";
    cout << "3. Add Wallet or Card Balance\n";
    cout << "4. Add Expense\n";
    cout << "5. Generate Reports\n";
    cout << "6. Exit\n";
    cout << "Enter your choice: ";
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main() {
    cout << "Welcome to the Bank!" << endl;

    FinanceManager manager;

    int choice;
    while (true) 
    {
        printMainMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) 
        {
        case 1: {
            string name;
            double balance;
            cout << "Enter wallet name: ";
            cin >> name;
            cout << "Enter initial balance: ";
            cin >> balance;
            manager.addWallet(name, balance);
            break;
        }
        case 2: 
        {
            string name;
            double balance;
            cout << "Enter card name: ";
            cin >> name;
            cout << "Enter initial balance: ";
            cin >> balance;
            manager.addCard(name, balance, false); 
            break;
        }
        case 3:
        {
            int cardOrWallet;
            cout << "Enter 1 to replenish card balance or 2 to replenish wallet balance: ";
            cin >> cardOrWallet;
            cin.ignore();

            if (cardOrWallet == 1) 
            {
                string cardName;
                double amount;
                cout << "Enter the card name to replenish: ";
                getline(cin, cardName);
                cout << "Enter the amount to replenish: ";
                cin >> amount;
                cin.ignore();
                manager.addCardBalance(cardName, amount);
            }
            else if (cardOrWallet == 2) 
            {
                string walletName;
                double amount;
                cout << "Enter the wallet name to replenish: ";
                getline(cin, walletName);
                cout << "Enter the amount to replenish: ";
                cin >> amount;
                cin.ignore();
                manager.addWalletBalance(walletName, amount);
            }
            else
            {
                cout << "Invalid choice. Please try again." << endl;
            }
            break;
        }
        case 4: 
        {
            string date, category;
            double amount;
            cout << "Enter expense date (YYYY MM DD): ";
            getline(cin, date);
            cout << "Enter expense category: ";
            getline(cin, category);
            cout << "Enter expense amount: ";
            cin >> amount;
            manager.addExpense(date, amount, category);
            break;

        }
        case 5: 
        {
            string period;
            cout << "Enter the period for which to generate the report (Day/Week/Month): ";
            getline(cin, period);
            manager.generateReportByPeriod(period);
            manager.generateRating();
            time_t now = time(nullptr);
            string filename = "reports_" + to_string(now) + ".txt";
            manager.saveReports(filename);
            cout << "\nReports generated and saved successfully in " << filename << "!" << endl;
            break;

        }
        case 6:
        {
            cout << "Exiting the program. Goodbye!" << endl;
            return 0;
        }
        default: 
        {
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();

        clearScreen();
    }

    return 0;
}