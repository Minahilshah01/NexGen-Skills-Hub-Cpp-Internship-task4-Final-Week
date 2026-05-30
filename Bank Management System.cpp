#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <climits>

using namespace std;

class BankAccount {
private:
    int accountNumber;
    string ownerName;
    string accountType;
    double balance;
    string pin;
    bool isActive;

public:
    BankAccount() : accountNumber(0), balance(0.0), isActive(true) {}

    void setAccountNumber(int accNo)  { accountNumber = accNo; }
    void setOwnerName(string name)    { ownerName = name; }
    void setAccountType(string type)  { accountType = type; }
    void setBalance(double bal)       { balance = bal; }
    void setPin(string p)             { pin = p; }
    void setActive(bool status)       { isActive = status; }

    int    getAccountNumber() const { return accountNumber; }
    string getOwnerName()     const { return ownerName; }
    string getAccountType()   const { return accountType; }
    double getBalance()       const { return balance; }
    string getPin()           const { return pin; }
    bool   getActive()        const { return isActive; }

    bool deposit(double amount) {
        if (amount <= 0) return false;
        if (amount > 1000000) return false;
        balance += amount;
        return true;
    }

    bool withdraw(double amount) {
        if (amount <= 0) return false;
        if (amount > balance) return false;
        if (accountType == "Savings" && (balance - amount) < 500) return false;
        balance -= amount;
        return true;
    }

    void display() const {
        cout << "\n+--------------------------------------+" << endl;
        cout << "  Account No   : " << accountNumber << endl;
        cout << "  Owner Name   : " << ownerName << endl;
        cout << "  Account Type : " << accountType << endl;
        cout << "  Status       : " << (isActive ? "Active" : "Frozen") << endl;
        cout << "  Balance      : PKR " << fixed << setprecision(2) << balance << endl;
        cout << "+--------------------------------------+" << endl;
    }
};

const string FILE_NAME = "accounts.dat";
const int MAX_ACCOUNTS = 100;
const double SAVINGS_MIN_BALANCE = 500.0;
const double MAX_DEPOSIT_LIMIT = 1000000.0;
const int MAX_PIN_ATTEMPTS = 3;

BankAccount accounts[MAX_ACCOUNTS];
int totalAccounts = 0;
int nextAccountNumber = 1001;

void clearInput() {
    cin.clear();
    cin.ignore(INT_MAX, '\n');
}

bool isValidName(const string& name) {
    if (name.empty() || name.length() < 3) return false;
    for (int i = 0; i < (int)name.length(); i++) {
        char c = name[i];
        if (!isalpha(c) && c != ' ') return false;
    }
    return true;
}

bool isValidPin(const string& pin) {
    if (pin.length() != 4) return false;
    for (int i = 0; i < (int)pin.length(); i++) {
        char c = pin[i];
        if (!isdigit(c)) return false;
    }
    return true;
}

bool isValidAmount(double amount) {
    return amount > 0 && amount <= MAX_DEPOSIT_LIMIT;
}

int findAccount(int accNo) {
    for (int i = 0; i < totalAccounts; i++)
        if (accounts[i].getAccountNumber() == accNo) return i;
    return -1;
}

bool verifyPin(int index) {
    string enteredPin;
    int attempts = 0;
    while (attempts < MAX_PIN_ATTEMPTS) {
        cout << "  Enter PIN (" << (MAX_PIN_ATTEMPTS - attempts) << " attempts left): ";
        cin >> enteredPin;
        if (enteredPin == accounts[index].getPin()) return true;
        attempts++;
        if (attempts < MAX_PIN_ATTEMPTS)
            cout << "  [!] Wrong PIN. Try again." << endl;
    }
    cout << "  [!] Too many failed attempts. Access denied." << endl;
    return false;
}

void showHeader() {
    cout << "\n+========================================+" << endl;
    cout << "|        PAKISTAN BANKING SYSTEM         |" << endl;
    cout << "|      Secure . Reliable . Trusted       |" << endl;
    cout << "+========================================+" << endl;
}

void showMenu() {
    cout << "\n+----------- MAIN MENU ---------------+" << endl;
    cout << "  1. Open New Account" << endl;
    cout << "  2. View All Accounts" << endl;
    cout << "  3. Search Account" << endl;
    cout << "  4. Deposit Money" << endl;
    cout << "  5. Withdraw Money" << endl;
    cout << "  6. Transfer Money" << endl;
    cout << "  7. Update Account Info" << endl;
    cout << "  8. Freeze / Unfreeze Account" << endl;
    cout << "  9. Close Account" << endl;
    cout << "  0. Exit" << endl;
    cout << "+-------------------------------------+" << endl;
    cout << "  Enter choice: ";
}

void addAccount() {
    if (totalAccounts >= MAX_ACCOUNTS) {
        cout << "\n  [!] Bank is full. Cannot open more accounts." << endl;
        return;
    }

    BankAccount acc;
    string name, type, pin, confirmPin;
    double initialDeposit;

    cout << "\n======= OPEN NEW ACCOUNT =======" << endl;

    clearInput();
    while (true) {
        cout << "  Full Name (letters only, min 3): ";
        getline(cin, name);
        if (isValidName(name)) break;
        cout << "  [!] Invalid name. Letters only, minimum 3 characters." << endl;
    }

    while (true) {
        cout << "  Account Type (1=Savings / 2=Current): ";
        int choice;
        cin >> choice;
        if (!cin) { clearInput(); cout << "  [!] Enter 1 or 2." << endl; continue; }
        if (choice == 1) { type = "Savings"; break; }
        else if (choice == 2) { type = "Current"; break; }
        else cout << "  [!] Invalid. Enter 1 or 2." << endl;
    }

    double minDeposit = (type == "Savings") ? SAVINGS_MIN_BALANCE : 1000.0;
    while (true) {
        cout << "  Initial Deposit (Min PKR " << minDeposit << "): ";
        cin >> initialDeposit;
        if (!cin) { clearInput(); cout << "  [!] Invalid. Enter a number." << endl; continue; }
        if (initialDeposit < minDeposit) { cout << "  [!] Minimum deposit is PKR " << minDeposit << endl; continue; }
        if (initialDeposit > MAX_DEPOSIT_LIMIT) { cout << "  [!] Cannot exceed PKR " << MAX_DEPOSIT_LIMIT << endl; continue; }
        break;
    }

    while (true) {
        cout << "  Set 4-digit PIN : ";
        cin >> pin;
        if (!isValidPin(pin)) { cout << "  [!] PIN must be exactly 4 digits." << endl; continue; }
        cout << "  Confirm PIN     : ";
        cin >> confirmPin;
        if (pin != confirmPin) { cout << "  [!] PINs do not match. Try again." << endl; continue; }
        break;
    }

    acc.setAccountNumber(nextAccountNumber++);
    acc.setOwnerName(name);
    acc.setAccountType(type);
    acc.setBalance(initialDeposit);
    acc.setPin(pin);
    acc.setActive(true);
    accounts[totalAccounts++] = acc;

    cout << "\n  [+] Account opened successfully!" << endl;
    cout << "  Account Number : " << acc.getAccountNumber() << endl;
    cout << "  Keep your PIN safe and do not share it." << endl;
}

void displayAllAccounts() {
    cout << "\n======= ALL ACCOUNTS =======" << endl;
    if (totalAccounts == 0) { cout << "  [!] No accounts found." << endl; return; }
    for (int i = 0; i < totalAccounts; i++)
        accounts[i].display();
    cout << "\n  Total Accounts: " << totalAccounts << endl;
}

void searchAccount() {
    int accNo;
    cout << "\n======= SEARCH ACCOUNT =======" << endl;
    cout << "  Enter Account Number: ";
    cin >> accNo;
    if (!cin) { clearInput(); cout << "  [!] Invalid input." << endl; return; }

    int index = findAccount(accNo);
    if (index == -1) cout << "  [!] Account not found." << endl;
    else accounts[index].display();
}

void depositMoney() {
    int accNo;
    double amount;

    cout << "\n======= DEPOSIT MONEY =======" << endl;
    cout << "  Enter Account Number: ";
    cin >> accNo;
    if (!cin) { clearInput(); cout << "  [!] Invalid input." << endl; return; }

    int index = findAccount(accNo);
    if (index == -1) { cout << "  [!] Account not found." << endl; return; }
    if (!accounts[index].getActive()) { cout << "  [!] Account is frozen." << endl; return; }

    cout << "  Enter Amount (PKR): ";
    cin >> amount;
    if (!cin) { clearInput(); cout << "  [!] Invalid amount." << endl; return; }
    if (!isValidAmount(amount)) { cout << "  [!] Amount must be between PKR 1 and PKR " << MAX_DEPOSIT_LIMIT << endl; return; }

    if (accounts[index].deposit(amount)) {
        cout << "\n  [+] PKR " << fixed << setprecision(2) << amount << " deposited." << endl;
        cout << "  Updated Balance: PKR " << accounts[index].getBalance() << endl;
    } else {
        cout << "  [!] Deposit failed." << endl;
    }
}

void withdrawMoney() {
    int accNo;
    double amount;

    cout << "\n======= WITHDRAW MONEY =======" << endl;
    cout << "  Enter Account Number: ";
    cin >> accNo;
    if (!cin) { clearInput(); cout << "  [!] Invalid input." << endl; return; }

    int index = findAccount(accNo);
    if (index == -1) { cout << "  [!] Account not found." << endl; return; }
    if (!accounts[index].getActive()) { cout << "  [!] Account is frozen." << endl; return; }
    if (!verifyPin(index)) return;

    cout << "  Current Balance: PKR " << fixed << setprecision(2) << accounts[index].getBalance() << endl;
    cout << "  Enter Amount (PKR): ";
    cin >> amount;
    if (!cin) { clearInput(); cout << "  [!] Invalid amount." << endl; return; }
    if (!isValidAmount(amount)) { cout << "  [!] Invalid amount." << endl; return; }

    if (accounts[index].withdraw(amount)) {
        cout << "\n  [+] PKR " << fixed << setprecision(2) << amount << " withdrawn." << endl;
        cout << "  Remaining Balance: PKR " << accounts[index].getBalance() << endl;
    } else {
        if (accounts[index].getAccountType() == "Savings")
            cout << "  [!] Minimum balance of PKR " << SAVINGS_MIN_BALANCE << " must be maintained." << endl;
        else
            cout << "  [!] Insufficient balance." << endl;
    }
}

void transferMoney() {
    int fromAcc, toAcc;
    double amount;

    cout << "\n======= TRANSFER MONEY =======" << endl;
    cout << "  Your Account Number       : ";
    cin >> fromAcc;
    if (!cin) { clearInput(); cout << "  [!] Invalid input." << endl; return; }

    int fromIndex = findAccount(fromAcc);
    if (fromIndex == -1) { cout << "  [!] Account not found." << endl; return; }
    if (!accounts[fromIndex].getActive()) { cout << "  [!] Your account is frozen." << endl; return; }
    if (!verifyPin(fromIndex)) return;

    cout << "  Recipient Account Number  : ";
    cin >> toAcc;
    if (!cin) { clearInput(); cout << "  [!] Invalid input." << endl; return; }
    if (fromAcc == toAcc) { cout << "  [!] Cannot transfer to same account." << endl; return; }

    int toIndex = findAccount(toAcc);
    if (toIndex == -1) { cout << "  [!] Recipient account not found." << endl; return; }
    if (!accounts[toIndex].getActive()) { cout << "  [!] Recipient account is frozen." << endl; return; }

    cout << "  Recipient Name: " << accounts[toIndex].getOwnerName() << endl;
    cout << "  Enter Transfer Amount (PKR): ";
    cin >> amount;
    if (!cin) { clearInput(); cout << "  [!] Invalid amount." << endl; return; }
    if (!isValidAmount(amount)) { cout << "  [!] Invalid amount." << endl; return; }

    if (accounts[fromIndex].withdraw(amount)) {
        accounts[toIndex].deposit(amount);
        cout << "\n  [+] PKR " << fixed << setprecision(2) << amount;
        cout << " transferred to " << accounts[toIndex].getOwnerName() << endl;
        cout << "  Your Remaining Balance: PKR " << accounts[fromIndex].getBalance() << endl;
    } else {
        if (accounts[fromIndex].getAccountType() == "Savings")
            cout << "  [!] Minimum balance of PKR " << SAVINGS_MIN_BALANCE << " must be maintained." << endl;
        else
            cout << "  [!] Insufficient balance." << endl;
    }
}

void updateAccount() {
    int accNo;
    cout << "\n======= UPDATE ACCOUNT =======" << endl;
    cout << "  Enter Account Number: ";
    cin >> accNo;
    if (!cin) { clearInput(); cout << "  [!] Invalid input." << endl; return; }

    int index = findAccount(accNo);
    if (index == -1) { cout << "  [!] Account not found." << endl; return; }
    if (!verifyPin(index)) return;

    string newName;
    clearInput();
    cout << "  New Name (Enter to skip): ";
    getline(cin, newName);
    if (!newName.empty()) {
        if (!isValidName(newName)) cout << "  [!] Invalid name. Not updated." << endl;
        else accounts[index].setOwnerName(newName);
    }

    char choice;
    cout << "  Change PIN? (y/n): ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        string newPin, confirmPin;
        while (true) {
            cout << "  New 4-digit PIN : ";
            cin >> newPin;
            if (!isValidPin(newPin)) { cout << "  [!] PIN must be 4 digits." << endl; continue; }
            cout << "  Confirm PIN     : ";
            cin >> confirmPin;
            if (newPin != confirmPin) { cout << "  [!] PINs do not match." << endl; continue; }
            accounts[index].setPin(newPin);
            cout << "  [+] PIN updated." << endl;
            break;
        }
    }

    cout << "\n  [+] Account updated successfully!" << endl;
    accounts[index].display();
}

void freezeAccount() {
    int accNo;
    cout << "\n======= FREEZE / UNFREEZE ACCOUNT =======" << endl;
    cout << "  Enter Account Number: ";
    cin >> accNo;
    if (!cin) { clearInput(); cout << "  [!] Invalid input." << endl; return; }

    int index = findAccount(accNo);
    if (index == -1) { cout << "  [!] Account not found." << endl; return; }
    if (!verifyPin(index)) return;

    if (accounts[index].getActive()) {
        accounts[index].setActive(false);
        cout << "  [+] Account has been FROZEN." << endl;
    } else {
        accounts[index].setActive(true);
        cout << "  [+] Account has been UNFROZEN." << endl;
    }
}

void deleteAccount() {
    int accNo;
    cout << "\n======= CLOSE ACCOUNT =======" << endl;
    cout << "  Enter Account Number: ";
    cin >> accNo;
    if (!cin) { clearInput(); cout << "  [!] Invalid input." << endl; return; }

    int index = findAccount(accNo);
    if (index == -1) { cout << "  [!] Account not found." << endl; return; }
    if (!verifyPin(index)) return;

    if (accounts[index].getBalance() > 0) {
        cout << "  [!] Balance PKR " << fixed << setprecision(2) << accounts[index].getBalance() << " remaining." << endl;
        cout << "  Withdraw all funds before closing." << endl;
        return;
    }

    char confirm;
    cout << "  Confirm close account? (y/n): ";
    cin >> confirm;
    if (confirm != 'y' && confirm != 'Y') { cout << "  Cancelled." << endl; return; }

    for (int i = index; i < totalAccounts - 1; i++)
        accounts[i] = accounts[i + 1];
    totalAccounts--;

    cout << "  [+] Account closed successfully." << endl;
}

void saveToFile() {
    ofstream outFile(FILE_NAME.c_str());
    if (!outFile) { cout << "  [!] Error saving data." << endl; return; }

    outFile << totalAccounts << "\n" << nextAccountNumber << "\n";
    for (int i = 0; i < totalAccounts; i++) {
        outFile << accounts[i].getAccountNumber() << "\n"
                << accounts[i].getOwnerName()     << "\n"
                << accounts[i].getAccountType()   << "\n"
                << accounts[i].getBalance()        << "\n"
                << accounts[i].getPin()            << "\n"
                << accounts[i].getActive()         << "\n";
    }
    outFile.close();
}

void loadFromFile() {
    ifstream inFile(FILE_NAME.c_str());
    if (!inFile) return;

    inFile >> totalAccounts >> nextAccountNumber;
    inFile.ignore(INT_MAX, '\n');

    for (int i = 0; i < totalAccounts; i++) {
        int accNo;
        string name, type, pin;
        double balance;
        bool active;

        inFile >> accNo;
        inFile.ignore(INT_MAX, '\n');
        getline(inFile, name);
        getline(inFile, type);
        inFile >> balance >> pin >> active;
        inFile.ignore(INT_MAX, '\n');

        accounts[i].setAccountNumber(accNo);
        accounts[i].setOwnerName(name);
        accounts[i].setAccountType(type);
        accounts[i].setBalance(balance);
        accounts[i].setPin(pin);
        accounts[i].setActive(active);
    }
    inFile.close();
}

int main() {
    loadFromFile();
    showHeader();

    int choice;
    do {
        showMenu();
        cin >> choice;
        if (!cin) {
            clearInput();
            cout << "  [!] Invalid input. Enter a number 0-9." << endl;
            continue;
        }

        switch (choice) {
            case 1: addAccount();         break;
            case 2: displayAllAccounts(); break;
            case 3: searchAccount();      break;
            case 4: depositMoney();       break;
            case 5: withdrawMoney();      break;
            case 6: transferMoney();      break;
            case 7: updateAccount();      break;
            case 8: freezeAccount();      break;
            case 9: deleteAccount();      break;
            case 0:
                saveToFile();
                cout << "\n  Data saved. Goodbye!" << endl;
                break;
            default:
                cout << "  [!] Invalid choice. Enter 0-9." << endl;
        }

        if (choice != 0) {
            cout << "\n  Press Enter to continue...";
            cin.ignore(INT_MAX, '\n');
            cin.get();
        }

    } while (choice != 0);

    return 0;
}
