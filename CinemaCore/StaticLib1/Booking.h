#pragma once
#include <string>
#include <vector>
#include "../StaticLib1/Show.h"
#include "../StaticLib1/Movie.h"
#include "../Tickets/Login.h"
using namespace std;
enum SeatType {
    SEAT_SILVER,
    SEAT_GOLD,
    SEAT_PLATINUM
};
struct Seat {
    int      id;
    int      row;
    int      col;
    SeatType type;
    bool     isBooked;
};
struct Booking {
    int         id;
    int         showId;
    string      username;
    vector<int> seatIds;
    float       totalPrice;
    bool        isPaid;
};
string SeatTypeToString(SeatType t);
float  SeatTypeToPrice(SeatType t);
void   GetSeatColor(SeatType t, bool isBooked, bool isSelected, unsigned char& r, unsigned char& g, unsigned char& b);
vector<Seat>     GenerateSeats(const Hall& hall);
vector<Booking>& GetBookings();
int              NextBookingId();
void RunBookingSelectShow(
    vector<Show>& shows,
    vector<Movie>& movies,
    const User& currentUser
);
void RunSeatSelectionScreen(
    Show& show,
    const Movie& movie,
    const User& currentUser,
    vector<Booking>& bookings
);
void RunPaymentScreen(
    Show& show,
    const Movie& movie,
    const User& currentUser,
    vector<int>& selectedSeatIds,
    vector<Seat>& seats,
    vector<Booking>& bookings
);
void RunMyBookingsScreen(
    vector<Booking>& bookings,
    vector<Show>& shows,
    vector<Movie>& movies,
    const User& currentUser
);
void RunAllBookingsScreen(
    vector<Booking>& bookings,
    vector<Show>& shows,
    vector<Movie>& movies
);
void RunSendNotificationScreen(vector<Movie>& movies);