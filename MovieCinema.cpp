#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>

using namespace std;

class Movie {
private:
    string title;
    string genre;
    int durationMinutes;
    double price;
    string rating;

public:
    Movie(string t, string g, int d, double p, string r)
        : title(t), genre(g), durationMinutes(d), price(p), rating(r) {}

    string getTitle() const { return title; }
    double getPrice() const { return price; }

    void display(int index) const {
        cout << left << setw(4) << index
             << setw(25) << title
             << setw(12) << genre
             << setw(10) << (to_string(durationMinutes) + " min")
             << setw(10) << rating
             << "$" << fixed << setprecision(2) << price << "\n";
    }
};

class Seat {
private:
    bool booked;

public:
    Seat() : booked(false) {}

    bool isBooked() const { return booked; }
    void book() { booked = true; }
};

class Hall {
private:
    string name;
    int rows;
    int cols;
    vector<vector<Seat>> seats;

public:
    Hall(string n, int r, int c)
        : name(n), rows(r), cols(c), seats(r, vector<Seat>(c)) {}

    string getName() const { return name; }
    int getRows() const { return rows; }
    int getCols() const { return cols; }

    bool isValidSeat(int r, int c) const {
        return r >= 0 && r < rows && c >= 0 && c < cols;
    }

    bool isSeatBooked(int r, int c) const {
        return seats[r][c].isBooked();
    }

    void bookSeat(int r, int c) {
        seats[r][c].book();
    }

    void displaySeats() const {
        cout << "\nSeating layout for hall: " << name << "\n";
        cout << "(O = available, X = booked)\n\n   ";
        for (int c = 0; c < cols; ++c) {
            cout << setw(3) << (c + 1);
        }
        cout << "\n";

        for (int r = 0; r < rows; ++r) {
            cout << "R" << setw(2) << (r + 1) << " ";
            for (int c = 0; c < cols; ++c) {
                cout << setw(3) << (seats[r][c].isBooked() ? "X" : "O");
            }
            cout << "\n";
        }
    }
};

struct Booking {
    string customerName;
    string movieTitle;
    string hallName;
    int row;
    int col;
    double pricePaid;

    void display(int index) const {
        cout << left << setw(4) << index
             << setw(18) << customerName
             << setw(25) << movieTitle
             << setw(15) << hallName
             << "Seat R" << (row + 1) << "C" << (col + 1) << "   "
             << "$" << fixed << setprecision(2) << pricePaid << "\n";
    }
};

class CinemaSystem {
private:
    vector<Movie> movies;
    vector<Hall> halls;
    vector<Booking> bookings;
    double totalRevenue;

public:
    CinemaSystem() : totalRevenue(0.0) {
        movies.push_back(Movie("The Last Horizon", "Sci-Fi", 128, 12.50, "PG-13"));
        movies.push_back(Movie("Laugh Out Loud", "Comedy", 95, 9.00, "PG"));
        movies.push_back(Movie("Silent Shadows", "Thriller", 110, 11.00, "R"));
        movies.push_back(Movie("Adventures of Nova", "Animation", 90, 8.50, "G"));

        halls.push_back(Hall("Hall A", 5, 8));
        halls.push_back(Hall("Hall B", 4, 6));
    }

    void viewMovies() const {
        cout << "\n===== Available Movies =====\n";
        cout << left << setw(4) << "#" << setw(25) << "Title" << setw(12) << "Genre"
             << setw(10) << "Duration" << setw(10) << "Rating" << "Price\n";
        cout << string(70, '-') << "\n";
        for (size_t i = 0; i < movies.size(); ++i) {
            movies[i].display(static_cast<int>(i + 1));
        }
    }

    void viewHallsMenu() const {
        cout << "\n===== Available Halls =====\n";
        for (size_t i = 0; i < halls.size(); ++i) {
            cout << (i + 1) << ". " << halls[i].getName()
                 << " (" << halls[i].getRows() << " rows x "
                 << halls[i].getCols() << " cols)\n";
        }
    }

    void viewSeats() {
        if (halls.empty()) {
            cout << "No halls available.\n";
            return;
        }
        viewHallsMenu();
        int choice = readInt("Select a hall number to view seats: ", 1, static_cast<int>(halls.size()));
        halls[choice - 1].displaySeats();
    }

    void bookTicket() {
        if (movies.empty() || halls.empty()) {
            cout << "No movies or halls available.\n";
            return;
        }

        viewMovies();
        int movieChoice = readInt("Select a movie number: ", 1, static_cast<int>(movies.size()));
        Movie& selectedMovie = movies[movieChoice - 1];

        viewHallsMenu();
        int hallChoice = readInt("Select a hall number: ", 1, static_cast<int>(halls.size()));
        Hall& selectedHall = halls[hallChoice - 1];

        selectedHall.displaySeats();

        int row = readInt("Enter row number: ", 1, selectedHall.getRows()) - 1;
        int col = readInt("Enter seat number (column): ", 1, selectedHall.getCols()) - 1;

        if (!selectedHall.isValidSeat(row, col)) {
            cout << "Invalid seat selection.\n";
            return;
        }

        if (selectedHall.isSeatBooked(row, col)) {
            cout << "Sorry, that seat is already booked. Please choose another seat.\n";
            return;
        }

        cout << "Enter your name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string customerName;
        getline(cin, customerName);
        if (customerName.empty()) {
            customerName = "Guest";
        }

        selectedHall.bookSeat(row, col);

        Booking newBooking{
            customerName,
            selectedMovie.getTitle(),
            selectedHall.getName(),
            row,
            col,
            selectedMovie.getPrice()
        };
        bookings.push_back(newBooking);
        totalRevenue += selectedMovie.getPrice();

        cout << "\nBooking confirmed for " << customerName
             << "! Movie: " << selectedMovie.getTitle()
             << ", Hall: " << selectedHall.getName()
             << ", Seat: R" << (row + 1) << "C" << (col + 1)
             << ", Price: $" << fixed << setprecision(2) << selectedMovie.getPrice() << "\n";
    }

    void viewBookings() const {
        if (bookings.empty()) {
            cout << "\nNo bookings yet.\n";
            return;
        }
        cout << "\n===== All Bookings =====\n";
        cout << left << setw(4) << "#" << setw(18) << "Customer" << setw(25) << "Movie"
             << setw(15) << "Hall" << "Seat / Price\n";
        cout << string(80, '-') << "\n";
        for (size_t i = 0; i < bookings.size(); ++i) {
            bookings[i].display(static_cast<int>(i + 1));
        }
    }

    void viewTotalRevenue() const {
        cout << "\nTotal revenue generated: $" << fixed << setprecision(2) << totalRevenue << "\n";
    }

    static int readInt(const string& prompt, int minVal, int maxVal) {
        int value;
        while (true) {
            cout << prompt;
            cin >> value;
            if (cin.fail() || value < minVal || value > maxVal) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number between "
                     << minVal << " and " << maxVal << ".\n";
            } else {
                return value;
            }
        }
    }
};

void printMenu() {
    cout << "\n===== Movie Cinema Management System =====\n";
    cout << "1. View Movies\n";
    cout << "2. Book Ticket\n";
    cout << "3. View Seats\n";
    cout << "4. View Bookings\n";
    cout << "5. View Total Revenue\n";
    cout << "6. Exit\n";
}

int main() {
    CinemaSystem cinema;
    bool running = true;

    cout << "Welcome to the Movie Cinema Management System!\n";

    while (running) {
        printMenu();
        int choice = CinemaSystem::readInt("Enter your choice: ", 1, 6);

        switch (choice) {
            case 1:
                cinema.viewMovies();
                break;
            case 2:
                cinema.bookTicket();
                break;
            case 3:
                cinema.viewSeats();
                break;
            case 4:
                cinema.viewBookings();
                break;
            case 5:
                cinema.viewTotalRevenue();
                break;
            case 6:
                cout << "Thank you for using the Movie Cinema Management System. Goodbye!\n";
                running = false;
                break;
        }
    }

    return 0;
}