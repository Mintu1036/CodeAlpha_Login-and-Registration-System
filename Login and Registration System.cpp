#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h> // Requires OpenSSL for hashing

using namespace std;

// Function to hash passwords (SHA-256)
string sha256(const string &str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)str.c_str(), str.size(), hash);
    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

// Function to load users from file into a map
unordered_map<string, string> loadUsers(const string &filename) {
    unordered_map<string, string> users;
    ifstream file(filename);
    string username, passwordHash;
    while (file >> username >> passwordHash) {
        users[username] = passwordHash;
    }
    return users;
}

// Function to save a new user to file
void saveUser(const string &filename, const string &username, const string &passwordHash) {
    ofstream file(filename, ios::app);
    file << username << " " << passwordHash << endl;
}

// Registration function
void registerUser(const string &filename) {
    auto users = loadUsers(filename);
    string username, password;

    cout << "Enter username: ";
    cin >> username;

    if (users.find(username) != users.end()) {
        cout << "❌ Username already exists! Please choose another.\n";
        return;
    }

    cout << "Enter password: ";
    cin >> password;

    string hashed = sha256(password);
    saveUser(filename, username, hashed);
    cout << "✅ Registration successful!\n";
}

// Login function
void loginUser(const string &filename) {
    auto users = loadUsers(filename);
    string username, password;

    cout << "Enter username: ";
    cin >> username;

    cout << "Enter password: ";
    cin >> password;

    string hashed = sha256(password);

    if (users.find(username) != users.end() && users[username] == hashed) {
        cout << "✅ Login successful! Welcome, " << username << "!\n";
    } else {
        cout << "❌ Invalid username or password!\n";
    }
}

int main() {
    const string filename = "users.txt";
    int choice;

    do {
        cout << "\n--- Login & Registration System ---\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                registerUser(filename);
                break;
            case 2:
                loginUser(filename);
                break;
            case 3:
                cout << "Exiting...\n";
                break;
            default:
                cout << "❌ Invalid choice!\n";
        }
    } while (choice != 3);

    return 0;
}
