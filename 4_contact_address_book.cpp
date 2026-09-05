#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class Contact {
public:
    string name;
    string phone;
    string email;

    Contact() {}
    Contact(string n, string p, string e) : name(n), phone(p), email(e) {}

    string toFileString() const {
        return name + "," + phone + "," + email;
    }

    static Contact fromFileString(const string& line) {
        stringstream ss(line);
        string n, p, e;
        getline(ss, n, ',');
        getline(ss, p, ',');
        getline(ss, e, ',');
        return Contact(n, p, e);
    }

    void display() const {
        cout << "Name: " << name << " | Phone: " << phone << " | Email: " << email << endl;
    }
};

class AddressBook {
private:
    vector<Contact> contacts;
    const string filename = "contacts.csv";

public:
    AddressBook() { load(); }

    void load() {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            if (!line.empty()) contacts.push_back(Contact::fromFileString(line));
        }
    }

    void save() {
        ofstream file(filename);
        for (const auto& c : contacts) file << c.toFileString() << endl;
    }

    void addContact(const string& name, const string& phone, const string& email) {
        contacts.emplace_back(name, phone, email);
        save();
        cout << "Contact added.\n";
    }

    void listContacts() {
        if (contacts.empty()) {
            cout << "No contacts found.\n";
            return;
        }
        for (const auto& c : contacts) c.display();
    }

    void sortByName() {
        sort(contacts.begin(), contacts.end(), [](const Contact& a, const Contact& b) {
            return a.name < b.name;
        });
        save();
        cout << "Contacts sorted by name.\n";
    }

    // Linear search
    void searchByName(const string& name) {
        bool found = false;
        for (const auto& c : contacts) {
            if (c.name.find(name) != string::npos) {
                c.display();
                found = true;
            }
        }
        if (!found) cout << "No matching contact found.\n";
    }

    bool updateContact(const string& name, const string& newPhone, const string& newEmail) {
        for (auto& c : contacts) {
            if (c.name == name) {
                c.phone = newPhone;
                c.email = newEmail;
                save();
                return true;
            }
        }
        return false;
    }

    bool deleteContact(const string& name) {
        auto it = remove_if(contacts.begin(), contacts.end(), [&](const Contact& c) {
            return c.name == name;
        });
        if (it != contacts.end()) {
            contacts.erase(it, contacts.end());
            save();
            return true;
        }
        return false;
    }
};

void showMenu() {
    cout << "\n===== Contact / Address Book =====\n";
    cout << "1. Add Contact\n";
    cout << "2. List All Contacts\n";
    cout << "3. Search Contact by Name\n";
    cout << "4. Update Contact\n";
    cout << "5. Delete Contact\n";
    cout << "6. Sort Contacts by Name\n";
    cout << "7. Exit\n";
    cout << "Choose an option: ";
}

int main() {
    AddressBook book;
    int choice;

    do {
        showMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                string name, phone, email;
                cout << "Enter name: ";
                getline(cin, name);
                cout << "Enter phone: ";
                getline(cin, phone);
                cout << "Enter email: ";
                getline(cin, email);
                book.addContact(name, phone, email);
                break;
            }
            case 2:
                book.listContacts();
                break;
            case 3: {
                string name;
                cout << "Enter name to search: ";
                getline(cin, name);
                book.searchByName(name);
                break;
            }
            case 4: {
                string name, phone, email;
                cout << "Enter name of contact to update: ";
                getline(cin, name);
                cout << "Enter new phone: ";
                getline(cin, phone);
                cout << "Enter new email: ";
                getline(cin, email);
                if (book.updateContact(name, phone, email))
                    cout << "Contact updated.\n";
                else
                    cout << "Contact not found.\n";
                break;
            }
            case 5: {
                string name;
                cout << "Enter name to delete: ";
                getline(cin, name);
                if (book.deleteContact(name))
                    cout << "Contact deleted.\n";
                else
                    cout << "Contact not found.\n";
                break;
            }
            case 6:
                book.sortByName();
                break;
            case 7:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid option.\n";
        }
    } while (choice != 7);

    return 0;
}
