#pragma once
#include <string>
#include <vector>
using namespace std;

enum Genre {
    GENRE_ACTION,
    GENRE_COMEDY,
    GENRE_DRAMA,
    GENRE_HORROR,
    GENRE_SCIFI,
    GENRE_ROMANCE,
    GENRE_ANIMATION,
    GENRE_THRILLER
};

enum Language {
    LANG_BULGARIAN,
    LANG_ENGLISH,
    LANG_DUBBED
};

struct Date {
    int day;
    int month;
    int year;
};

struct Movie {
    int      id;
    string   title;
    Language language;
    Genre    genre;
    Date     releaseDate;
    string   description;
    int      durationMinutes;
};

string        GenreToString(Genre g);
string        LanguageToString(Language l);
string        DateToString(const Date& d);
bool          DateEquals(const Date& a, const Date& b);

vector<Movie> SearchByTitle(const vector<Movie>& movies, const string& title);
vector<Movie> SearchByGenre(const vector<Movie>& movies, Genre genre);
vector<Movie> SearchByLanguage(const vector<Movie>& movies, Language lang);
vector<Movie> SearchByDate(const vector<Movie>& movies, const Date& date);

vector<Movie> GetAllMovies();

void RunMovieSearchScreen(vector<Movie>& movies);