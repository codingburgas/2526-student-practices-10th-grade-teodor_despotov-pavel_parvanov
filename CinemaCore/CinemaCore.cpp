#include <iostream>
using namespace std;

void showMenu() {
    cout << "=============================\n";
    cout << "   Movie Ticket System Menu  \n";
    cout << "=============================\n";
    cout << "1. Search Movies\n";
    cout << "2. View Showtimes\n";
    cout << "3. Make a Booking\n";
    cout << "4. Select Seats\n";
    cout << "5. Payment\n";
    cout << "6. Admin Panel\n";
    cout << "7. Exit\n";
    cout << "-----------------------------\n";
    cout << "Enter your choice: ";
}

int main() {
    int choice;

    while (true) {
        showMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Searching movies...\n";
            break;
        case 2:
            cout << "Viewing showtimes...\n";
            break;
        case 3:
            cout << "Making a booking...\n";
            break;
        case 4:
            cout << "Selecting seats...\n";
            break;
        case 5:
            cout << "Processing payment...\n";
            break;
        case 6:
            cout << "Opening admin panel...\n";
            break;
        case 7:
            cout << "Exiting program...\n";
            return 0;
        default:
            cout << "Invalid choice. Try again.\n";
        }

        cout << "\n";
    }
}
