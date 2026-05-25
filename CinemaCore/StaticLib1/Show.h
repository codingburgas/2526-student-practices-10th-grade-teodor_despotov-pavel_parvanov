#pragma once
#include <string>
#include <vector>
#include "Movie.h"
using namespace std;

enum HallType {
    HALL_SMALL,
    HALL_MEDIUM,
    HALL_LARGE
};

struct Hall {
    int      id;
    string   name;
    HallType type;
    int      totalSeats;
};

struct ShowTime {
    int hour;
    int minute;
};

struct Show {
    int      id;
    int      movieId;
    Hall     hall;
    ShowTime time;
    ShowTime endTime;
    Date     date;
    bool     isActive;
};

string       HallTypeToString(HallType t);
string       ShowTimeToString(const ShowTime& t);
vector<Show> GetAllShows();
vector<Show> GetShowsByMovie(const vector<Show>& shows, int movieId);
vector<Show> GetShowsByDate(const vector<Show>& shows, const Date& date);
void         RunShowtimesScreen(vector<Show>& shows, vector<Movie>& movies);