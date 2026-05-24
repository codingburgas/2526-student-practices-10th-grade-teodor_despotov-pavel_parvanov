#include "Movie.h"
#include "raylib.h"
#include <cctype>

static Color MakeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    Color c; c.r = r; c.g = g; c.b = b; c.a = a; return c;
}

static const Color BG_DARK = { 10,  12,  20, 255 };
static const Color BG_PANEL = { 18,  22,  36, 255 };
static const Color ACCENT_GOLD = { 212, 175,  55, 255 };
static const Color ACCENT_DIM = { 120,  95,  28, 255 };
static const Color TEXT_PRIMARY = { 235, 230, 210, 255 };
static const Color TEXT_MUTED = { 130, 125, 110, 255 };
static const Color BTN_BLUE = { 30,  70, 130, 255 };
static const Color BTN_BLUE_H = { 60, 130, 200, 255 };
static const Color BTN_RED = { 100,  20,  20, 255 };
static const Color BTN_RED_H = { 180,  40,  40, 255 };
static const Color FIELD_BG = { 25,  30,  48, 255 };
static const Color FIELD_ACTIVE = { 35,  42,  68, 255 };

static bool IsHovered(int x, int y, int w, int h) {
    Vector2 m = GetMousePosition();
    return m.x >= x && m.x <= x + w && m.y >= y && m.y <= y + h;
}

static bool DrawButton(int x, int y, int w, int h, const char* label, Color base, Color hover) {
    bool hov = IsHovered(x, y, w, h);
    Rectangle rec = { (float)x, (float)y, (float)w, (float)h };
    DrawRectangleRounded(rec, 0.2f, 8, hov ? hover : base);
    DrawRectangleRoundedLines(rec, 0.2f, 8, ACCENT_GOLD);
    int tw = MeasureText(label, 16);
    DrawText(label, x + (w - tw) / 2, y + (h - 16) / 2, 16, TEXT_PRIMARY);
    return hov && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

static void DrawSearchField(int x, int y, int w, int h, const string& value, bool active) {
    Color bg = active ? FIELD_ACTIVE : FIELD_BG;
    Color border = active ? ACCENT_GOLD : ACCENT_DIM;
    Rectangle rec = { (float)x, (float)y, (float)w, (float)h };
    DrawRectangleRounded(rec, 0.15f, 8, bg);
    DrawRectangleRoundedLines(rec, 0.15f, 8, border);
    if (value.empty() && !active)
        DrawText("Search by title...", x + 10, y + (h - 16) / 2, 16, TEXT_MUTED);
    else {
        string display = value;
        if (active && ((int)(GetTime() * 2) % 2 == 0)) display += "|";
        DrawText(display.c_str(), x + 10, y + (h - 16) / 2, 16, TEXT_PRIMARY);
    }
    if (value.empty() && active && ((int)(GetTime() * 2) % 2 == 0))
        DrawText("|", x + 10, y + (h - 16) / 2, 16, TEXT_PRIMARY);
}

static void HandleTextInput(string& text, int maxLength) {
    int key = GetCharPressed();
    while (key > 0) {
        if (key >= 32 && key <= 125 && (int)text.length() < maxLength)
            text += (char)key;
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !text.empty())
        text.pop_back();
}

static void DrawMovieCard(int x, int y, int w, const Movie& m, bool selected) {
    Color border = selected ? ACCENT_GOLD : ACCENT_DIM;
    Color bg = selected ? MakeColor(30, 35, 60, 255) : BG_PANEL;
    Rectangle rec = { (float)x, (float)y, (float)w, 80.0f };
    DrawRectangleRounded(rec, 0.1f, 8, bg);
    DrawRectangleRoundedLines(rec, 0.1f, 8, border);

    DrawText(m.title.c_str(), x + 14, y + 10, 20, TEXT_PRIMARY);

    string info = GenreToString(m.genre) + "  |  " +
        LanguageToString(m.language) + "  |  " +
        to_string(m.durationMinutes) + " min  |  " +
        DateToString(m.releaseDate);
    DrawText(info.c_str(), x + 14, y + 38, 14, TEXT_MUTED);
    DrawText(m.description.c_str(), x + 14, y + 58, 13, MakeColor(160, 155, 140, 255));
}

string GenreToString(Genre g) {
    switch (g) {
    case GENRE_ACTION:    return "Action";
    case GENRE_COMEDY:    return "Comedy";
    case GENRE_DRAMA:     return "Drama";
    case GENRE_HORROR:    return "Horror";
    case GENRE_SCIFI:     return "Sci-Fi";
    case GENRE_ROMANCE:   return "Romance";
    case GENRE_ANIMATION: return "Animation";
    case GENRE_THRILLER:  return "Thriller";
    default:              return "Unknown";
    }
}

string LanguageToString(Language l) {
    switch (l) {
    case LANG_BULGARIAN: return "Bulgarian";
    case LANG_ENGLISH:   return "English";
    case LANG_DUBBED:    return "Dubbed";
    default:             return "Unknown";
    }
}

string DateToString(const Date& d) {
    string day = (d.day < 10 ? "0" : "") + to_string(d.day);
    string month = (d.month < 10 ? "0" : "") + to_string(d.month);
    return day + "/" + month + "/" + to_string(d.year);
}

bool DateEquals(const Date& a, const Date& b) {
    return a.day == b.day && a.month == b.month && a.year == b.year;
}

static string ToLower(const string& s) {
    string result = s;
    for (int i = 0; i < (int)result.size(); i++)
        result[i] = (char)tolower((unsigned char)result[i]);
    return result;
}

vector<Movie> SearchByTitle(const vector<Movie>& movies, const string& title) {
    vector<Movie> result;
    string searchLower = ToLower(title);
    for (int i = 0; i < (int)movies.size(); i++) {
        if (ToLower(movies[i].title).find(searchLower) != string::npos)
            result.push_back(movies[i]);
    }
    return result;
}

vector<Movie> SearchByGenre(const vector<Movie>& movies, Genre genre) {
    vector<Movie> result;
    for (int i = 0; i < (int)movies.size(); i++) {
        if (movies[i].genre == genre)
            result.push_back(movies[i]);
    }
    return result;
}

vector<Movie> SearchByLanguage(const vector<Movie>& movies, Language lang) {
    vector<Movie> result;
    for (int i = 0; i < (int)movies.size(); i++) {
        if (movies[i].language == lang)
            result.push_back(movies[i]);
    }
    return result;
}

vector<Movie> SearchByDate(const vector<Movie>& movies, const Date& date) {
    vector<Movie> result;
    for (int i = 0; i < (int)movies.size(); i++) {
        if (DateEquals(movies[i].releaseDate, date))
            result.push_back(movies[i]);
    }
    return result;
}

vector<Movie> GetAllMovies() {
    vector<Movie> movies;
    int id = 1;

    Movie a1; a1.id = id++; a1.title = "The Dark Knight";
    a1.language = LANG_ENGLISH; a1.genre = GENRE_ACTION;
    a1.releaseDate = { 18, 7, 2008 }; a1.durationMinutes = 152;
    a1.description = "Batman faces the Joker in Gotham City.";
    movies.push_back(a1);

    Movie a2; a2.id = id++; a2.title = "Mad Max: Fury Road";
    a2.language = LANG_ENGLISH; a2.genre = GENRE_ACTION;
    a2.releaseDate = { 15, 5, 2015 }; a2.durationMinutes = 120;
    a2.description = "A woman rebels against a tyrant in a post-apocalyptic wasteland.";
    movies.push_back(a2);

    Movie a3; a3.id = id++; a3.title = "John Wick";
    a3.language = LANG_DUBBED; a3.genre = GENRE_ACTION;
    a3.releaseDate = { 24, 10, 2014 }; a3.durationMinutes = 101;
    a3.description = "A retired hitman seeks vengeance for the death of his dog.";
    movies.push_back(a3);

    Movie c1; c1.id = id++; c1.title = "The Grand Budapest Hotel";
    c1.language = LANG_ENGLISH; c1.genre = GENRE_COMEDY;
    c1.releaseDate = { 28, 2, 2014 }; c1.durationMinutes = 99;
    c1.description = "Adventures of a legendary hotel concierge and his protege.";
    movies.push_back(c1);

    Movie c2; c2.id = id++; c2.title = "Superbad";
    c2.language = LANG_DUBBED; c2.genre = GENRE_COMEDY;
    c2.releaseDate = { 17, 8, 2007 }; c2.durationMinutes = 113;
    c2.description = "Two inseparable best friends try to buy alcohol for a party.";
    movies.push_back(c2);

    Movie c3; c3.id = id++; c3.title = "Home Alone";
    c3.language = LANG_BULGARIAN; c3.genre = GENRE_COMEDY;
    c3.releaseDate = { 16, 11, 1990 }; c3.durationMinutes = 103;
    c3.description = "A boy is accidentally left behind when his family flies to Paris.";
    movies.push_back(c3);

    Movie d1; d1.id = id++; d1.title = "The Shawshank Redemption";
    d1.language = LANG_ENGLISH; d1.genre = GENRE_DRAMA;
    d1.releaseDate = { 23, 9, 1994 }; d1.durationMinutes = 142;
    d1.description = "Two imprisoned men bond over years, finding solace and redemption.";
    movies.push_back(d1);

    Movie d2; d2.id = id++; d2.title = "Forrest Gump";
    d2.language = LANG_BULGARIAN; d2.genre = GENRE_DRAMA;
    d2.releaseDate = { 6, 7, 1994 }; d2.durationMinutes = 142;
    d2.description = "The presidencies of Kennedy through Bush seen through the eyes of an Alabama man.";
    movies.push_back(d2);

    Movie d3; d3.id = id++; d3.title = "Schindlers List";
    d3.language = LANG_ENGLISH; d3.genre = GENRE_DRAMA;
    d3.releaseDate = { 15, 12, 1993 }; d3.durationMinutes = 195;
    d3.description = "A businessman saves over a thousand Jewish refugees during the Holocaust.";
    movies.push_back(d3);

    Movie h1; h1.id = id++; h1.title = "The Shining";
    h1.language = LANG_ENGLISH; h1.genre = GENRE_HORROR;
    h1.releaseDate = { 23, 5, 1980 }; h1.durationMinutes = 146;
    h1.description = "A family heads to an isolated hotel where an evil presence influences the father.";
    movies.push_back(h1);

    Movie h2; h2.id = id++; h2.title = "Get Out";
    h2.language = LANG_DUBBED; h2.genre = GENRE_HORROR;
    h2.releaseDate = { 24, 2, 2017 }; h2.durationMinutes = 104;
    h2.description = "A young Black man visits his white girlfriend's family estate.";
    movies.push_back(h2);

    Movie h3; h3.id = id++; h3.title = "A Quiet Place";
    h3.language = LANG_DUBBED; h3.genre = GENRE_HORROR;
    h3.releaseDate = { 6, 4, 2018 }; h3.durationMinutes = 90;
    h3.description = "A family struggles to survive in a post-apocalyptic world of blind monsters.";
    movies.push_back(h3);

    Movie s1; s1.id = id++; s1.title = "Inception";
    s1.language = LANG_ENGLISH; s1.genre = GENRE_SCIFI;
    s1.releaseDate = { 16, 7, 2010 }; s1.durationMinutes = 148;
    s1.description = "A thief who steals corporate secrets through dreams.";
    movies.push_back(s1);

    Movie s2; s2.id = id++; s2.title = "Interstellar";
    s2.language = LANG_ENGLISH; s2.genre = GENRE_SCIFI;
    s2.releaseDate = { 7, 11, 2014 }; s2.durationMinutes = 169;
    s2.description = "A team of explorers travel through a wormhole in space.";
    movies.push_back(s2);

    Movie s3; s3.id = id++; s3.title = "The Matrix";
    s3.language = LANG_BULGARIAN; s3.genre = GENRE_SCIFI;
    s3.releaseDate = { 31, 3, 1999 }; s3.durationMinutes = 136;
    s3.description = "A hacker discovers reality is a simulation controlled by machines.";
    movies.push_back(s3);

    Movie r1; r1.id = id++; r1.title = "The Notebook";
    r1.language = LANG_DUBBED; r1.genre = GENRE_ROMANCE;
    r1.releaseDate = { 25, 6, 2004 }; r1.durationMinutes = 123;
    r1.description = "A love story spanning decades.";
    movies.push_back(r1);

    Movie r2; r2.id = id++; r2.title = "Titanic";
    r2.language = LANG_BULGARIAN; r2.genre = GENRE_ROMANCE;
    r2.releaseDate = { 19, 12, 1997 }; r2.durationMinutes = 194;
    r2.description = "A love story aboard the ill-fated RMS Titanic.";
    movies.push_back(r2);

    Movie r3; r3.id = id++; r3.title = "La La Land";
    r3.language = LANG_ENGLISH; r3.genre = GENRE_ROMANCE;
    r3.releaseDate = { 9, 12, 2016 }; r3.durationMinutes = 128;
    r3.description = "A jazz musician and an aspiring actress fall in love in Los Angeles.";
    movies.push_back(r3);

    Movie an1; an1.id = id++; an1.title = "Spirited Away";
    an1.language = LANG_DUBBED; an1.genre = GENRE_ANIMATION;
    an1.releaseDate = { 20, 7, 2001 }; an1.durationMinutes = 125;
    an1.description = "A girl trapped in a spirit world must find her way home.";
    movies.push_back(an1);

    Movie an2; an2.id = id++; an2.title = "The Lion King";
    an2.language = LANG_BULGARIAN; an2.genre = GENRE_ANIMATION;
    an2.releaseDate = { 24, 6, 1994 }; an2.durationMinutes = 88;
    an2.description = "A young lion prince flees his kingdom after his father is murdered.";
    movies.push_back(an2);

    Movie an3; an3.id = id++; an3.title = "Up";
    an3.language = LANG_DUBBED; an3.genre = GENRE_ANIMATION;
    an3.releaseDate = { 29, 5, 2009 }; an3.durationMinutes = 96;
    an3.description = "An elderly man fulfills his dream of a great adventure with a young stowaway.";
    movies.push_back(an3);

    Movie t1; t1.id = id++; t1.title = "Joker";
    t1.language = LANG_BULGARIAN; t1.genre = GENRE_THRILLER;
    t1.releaseDate = { 4, 10, 2019 }; t1.durationMinutes = 122;
    t1.description = "The origin story of the iconic villain.";
    movies.push_back(t1);

    Movie t2; t2.id = id++; t2.title = "Gone Girl";
    t2.language = LANG_ENGLISH; t2.genre = GENRE_THRILLER;
    t2.releaseDate = { 3, 10, 2014 }; t2.durationMinutes = 149;
    t2.description = "A man becomes the prime suspect when his wife goes missing.";
    movies.push_back(t2);

    Movie t3; t3.id = id++; t3.title = "Se7en";
    t3.language = LANG_DUBBED; t3.genre = GENRE_THRILLER;
    t3.releaseDate = { 22, 9, 1995 }; t3.durationMinutes = 127;
    t3.description = "Two detectives hunt a serial killer using the seven deadly sins as motives.";
    movies.push_back(t3);

    return movies;
}

void RunMovieSearchScreen(vector<Movie>& movies) {
    string        searchText = "";
    bool          fieldActive = false;
    int           filterMode = 0;
    int           selectedIdx = -1;
    int           scrollOffset = 0;
    vector<Movie> results = movies;

    const char* filterLabels[] = { "ALL", "ACTION", "COMEDY", "DRAMA",
                                   "HORROR", "SCI-FI", "ROMANCE", "ANIMATION", "THRILLER" };
    int filterCount = 9;

    int cardH = 88;
    int cardGap = 6;
    int listStartY = 205;
    int listEndY = 540;
    int visibleArea = listEndY - listStartY;

    while (!WindowShouldClose()) {

        Vector2 mp = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (mp.x >= 110 && mp.x <= 690 && mp.y >= 90 && mp.y <= 128)
                fieldActive = true;
            else
                fieldActive = false;
        }

        if (fieldActive)
            HandleTextInput(searchText, 30);

        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
            results = searchText.empty() ? movies : SearchByTitle(movies, searchText);
            scrollOffset = 0;
            selectedIdx = -1;
        }

        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            int maxScroll = (int)results.size() * (cardH + cardGap) - visibleArea;
            if (maxScroll < 0) maxScroll = 0;
            scrollOffset -= (int)(wheel * 30);
            if (scrollOffset < 0)         scrollOffset = 0;
            if (scrollOffset > maxScroll) scrollOffset = maxScroll;
        }

        BeginDrawing();
        ClearBackground(BG_DARK);

        for (int i = 0; i < 600; i += 4)
            DrawLine(0, i, 800, i, MakeColor(0, 0, 0, 18));

        DrawLine(30, 30, 80, 30, ACCENT_DIM);
        DrawLine(30, 30, 30, 80, ACCENT_DIM);
        DrawLine(720, 30, 770, 30, ACCENT_DIM);
        DrawLine(770, 30, 770, 80, ACCENT_DIM);
        DrawLine(30, 570, 80, 570, ACCENT_DIM);
        DrawLine(30, 520, 30, 570, ACCENT_DIM);
        DrawLine(720, 570, 770, 570, ACCENT_DIM);
        DrawLine(770, 520, 770, 570, ACCENT_DIM);

        Rectangle header = { 110, 18, 580, 55 };
        DrawRectangleRounded(header, 0.3f, 8, BG_PANEL);
        DrawRectangleRoundedLines(header, 0.3f, 8, ACCENT_DIM);
        const char* title = "SEARCH  MOVIES";
        DrawText(title, (800 - MeasureText(title, 24)) / 2, 32, 24, ACCENT_GOLD);

        DrawSearchField(110, 82, 580, 38, searchText, fieldActive);
        DrawText("Press ENTER to search", (800 - MeasureText("Press ENTER to search", 13)) / 2, 126, 13, TEXT_MUTED);

        int totalFiltersW = 0;
        int filterWidths[9];
        for (int i = 0; i < filterCount; i++) {
            filterWidths[i] = MeasureText(filterLabels[i], 13) + 18;
            totalFiltersW += filterWidths[i] + 5;
        }
        totalFiltersW -= 5;
        int fx = (800 - totalFiltersW) / 2;

        for (int i = 0; i < filterCount; i++) {
            int fw = filterWidths[i];
            bool hovered = IsHovered(fx, 144, fw, 26);
            Color base = (filterMode == i) ? BTN_BLUE : MakeColor(30, 35, 50, 255);
            Color hov = (filterMode == i) ? BTN_BLUE_H : MakeColor(50, 55, 80, 255);
            Color bcol = (filterMode == i) ? ACCENT_GOLD : ACCENT_DIM;
            Rectangle fr = { (float)fx, 144, (float)fw, 26 };
            DrawRectangleRounded(fr, 0.3f, 8, hovered ? hov : base);
            DrawRectangleRoundedLines(fr, 0.3f, 8, bcol);
            DrawText(filterLabels[i], fx + 9, 150, 13, TEXT_PRIMARY);
            if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                filterMode = i;
                searchText = "";
                selectedIdx = -1;
                scrollOffset = 0;
                results = (i == 0) ? movies : SearchByGenre(movies, (Genre)(i - 1));
            }
            fx += fw + 5;
        }

        DrawLine(110, 178, 690, 178, ACCENT_DIM);

        BeginScissorMode(110, listStartY, 580, visibleArea);

        if (results.empty()) {
            DrawText("No movies found.", (800 - MeasureText("No movies found.", 18)) / 2, 320, 18, TEXT_MUTED);
        }
        else {
            for (int i = 0; i < (int)results.size(); i++) {
                int y = listStartY + i * (cardH + cardGap) - scrollOffset;
                if (y + cardH < listStartY || y > listEndY) continue;
                bool sel = (selectedIdx == i);
                DrawMovieCard(110, y, 580, results[i], sel);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if (mp.x >= 110 && mp.x <= 690 && mp.y >= y && mp.y <= y + cardH)
                        selectedIdx = (selectedIdx == i) ? -1 : i;
                }
            }
        }

        EndScissorMode();

        int totalH = (int)results.size() * (cardH + cardGap);
        if (totalH > visibleArea) {
            float ratio = (float)visibleArea / totalH;
            int   barH = (int)(visibleArea * ratio);
            int   barY = listStartY + (int)((float)scrollOffset / totalH * visibleArea);
            DrawRectangle(694, listStartY, 6, visibleArea, MakeColor(40, 45, 65, 255));
            DrawRectangle(694, barY, 6, barH, ACCENT_DIM);
        }

        DrawLine(110, 544, 690, 544, ACCENT_DIM);

        if (DrawButton((800 - 180) / 2, 550, 180, 36, "BACK TO MENU", BTN_RED, BTN_RED_H))
            break;

        EndDrawing();
    }
}