# Bank Management System

A console-based banking application written in C++ that simulates core banking operations including account creation, fund management and inter account transfers. Data is persisted to disk across sessions using file I/O.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Technical Details](#technical-details)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Compilation](#compilation)
  - [Running the Program](#running-the-program)
- [Usage](#usage)
- [Business Rules](#business-rules)
- [File Structure](#file-structure)
- [Limitations](#limitations)
- [License](#license)

---

## Overview

The Pakistan Banking System is a terminal-based C++ application that manages up to 100 bank accounts. It supports two account types (Savings and Current), enforces minimum balance requirements and protects sensitive operations with a 4-digit PIN. Account data is automatically loaded on startup and saved on exit to a local flat file (`accounts.dat`).

---

## Features

- Open new Savings or Current accounts with an initial deposit
- View all existing accounts
- Search for an account by account number
- Deposit funds into any active account
- Withdraw funds with balance and minimum-balance enforcement
- Transfer funds between two active accounts
- Update account holder name and PIN
- Freeze or unfreeze an account
- Close an account (requires zero balance)
- Persistent storage: data survives program restarts
- PIN lockout after three consecutive failed attempts

---

## Technical Details

| Property            | Value                        |
|---------------------|------------------------------|
| Language            | C++ (standard: C++98/11)     |
| Paradigm            | Object-Oriented Programming  |
| Storage             | Flat file (`accounts.dat`)   |
| Max Accounts        | 100                          |
| Account Number Seed | 1001 (auto-incremented)      |
| Currency            | PKR (Pakistani Rupee)        |

**Core class:** `BankAccount`  
Encapsulates account number, owner name, account type, balance, PIN and active status. Provides `deposit()` and `withdraw()` methods with built-in validation.

**Key global functions:**

- `addAccount()` — guided account creation with input validation
- `depositMoney()` / `withdrawMoney()` — PIN-verified transactions
- `transferMoney()` — PIN-verified inter-account transfer
- `freezeAccount()` — toggles account active status
- `deleteAccount()` — removes account only if balance is zero
- `saveToFile()` / `loadFromFile()` — flat-file persistence

---

## Getting Started

### Prerequisites

- A C++ compiler: `g++`, `clang++`, or MSVC
- A terminal or command prompt

### Compilation

**Linux / macOS**
```bash
g++ -o bank_system Bank_Management_System.cpp
```

**Windows (MinGW)**
```bash
g++ -o bank_system.exe Bank_Management_System.cpp
```

**Windows (MSVC — Developer Command Prompt)**
```bash
cl Bank_Management_System.cpp /Fe:bank_system.exe
```

### Running the Program

```bash
./bank_system        # Linux / macOS
bank_system.exe      # Windows
```

On first run, no data file is required. The program creates `accounts.dat` in the working directory when you exit.

---

## Usage

After launching, the main menu is displayed:

```
+=========================================+
|         PAKISTAN BANKING SYSTEM         |
|       Secure . Reliable . Trusted       |
+=========================================+

+----------- MAIN MENU ---------------+
  1. Open New Account
  2. View All Accounts
  3. Search Account
  4. Deposit Money
  5. Withdraw Money
  6. Transfer Money
  7. Update Account Info
  8. Freeze / Unfreeze Account
  9. Close Account
  0. Exit
+-------------------------------------+
```

Enter the number corresponding to the desired operation and follow the on-screen prompts. Pressing `0` saves all data and exits the program.

---

## Business Rules

| Rule                              | Detail                                         |
|-----------------------------------|------------------------------------------------|
| Savings minimum balance           | PKR 500.00 must remain after any withdrawal    |
| Current minimum opening deposit   | PKR 1,000.00                                   |
| Savings minimum opening deposit   | PKR 500.00                                     |
| Maximum single deposit            | PKR 1,000,000.00                               |
| PIN format                        | Exactly 4 numeric digits                       |
| PIN attempts before lockout       | 3 consecutive failures deny access             |
| Account closure requirement       | Balance must be PKR 0.00                       |
| Self-transfer                     | Not permitted                                  |
| Transactions on frozen accounts   | Blocked for both sender and recipient          |

---

## File Structure

```
project-root/
|-- Bank_Management_System.cpp   # Full source code
|-- accounts.dat                 # Auto-generated data file (created on first exit)
|-- README.md
```

**`accounts.dat` format** (plain text, newline-delimited):

```
<total_accounts>
<next_account_number>
<account_number>
<owner_name>
<account_type>
<balance>
<pin>
<is_active>
... (repeated for each account)
```

---

## Limitations

- Account data is stored in plain text; the PIN is not hashed or encrypted.
- The maximum number of accounts is hard-coded at 100 (static array).
- No transaction history or audit log is maintained.
- The application is single-user and not thread-safe.
- Data file uses a fixed name (`accounts.dat`) in the working directory; no configurable path.

---
Author
Syeda Minahil Zahra
C++ Development Intern — NexGen Skills Hub
