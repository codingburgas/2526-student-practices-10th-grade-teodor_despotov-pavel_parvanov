#include "Movie.h"
#include "raylib.h"
#include <string>
#include <vector>

using namespace std;

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
static const Color BTN_GREEN = { 20, 100,  40, 255 };
static const Color BTN_GREEN_H = { 40, 170,  70, 255 };
static const Color FIELD_BG = { 25,  30,  48, 255 };
static const Color FIELD_ACTIVE = { 35,  42,  68, 255 };

static bool IsHovered(int x, int y, int w, int h) {
    Vector2 m = GetMousePosition();
    return m.x >= x && m.x <= x + w && m.y >= y && m.y <= y + h;
}

static bool DrawBtn(int x, int y, int w, int h, const char* label, Color base, Color hover, bool disabled = false) {
    bool hov = !disabled && IsHovered(x, y, w, h);
    Color bg = disabled ? MakeColor(30, 30, 40, 200) : (hov ? hover : base);
    Color txt = disabled ? TEXT_MUTED : TEXT_PRIMARY;
    Rectangle rec = { (float)x, (float)y, (float)w, (float)h };
    DrawRectangleRounded(rec, 0.15f, 8, bg);
    DrawRectangleRoundedLines(rec, 0.15f, 8, disabled ? MakeColor(50, 50, 60, 255) : ACCENT_GOLD);
    int tw = MeasureText(label, 16);
    DrawText(label, x + (w - tw) / 2, y + (h - 16) / 2, 16, txt);
    return hov && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

static void DrawTopBar(const char* title) {
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
    Rectangle header = { 110, 18, 580, 52 };
    DrawRectangleRounded(header, 0.3f, 8, BG_PANEL);
    DrawRectangleRoundedLines(header, 0.3f, 8, ACCENT_DIM);
    int tw = MeasureText(title, 22);
    DrawText(title, (800 - tw) / 2, 30, 22, ACCENT_GOLD);
    DrawLine(110, 78, 690, 78, ACCENT_DIM);
}

static void DrawField(int x, int y, int w, int h, const char* label, const string& value, bool active) {
    DrawText(label, x, y - 18, 13, TEXT_MUTED);
    Rectangle rec = { (float)x, (float)y, (float)w, (float)h };
    DrawRectangleRounded(rec, 0.12f, 8, active ? FIELD_ACTIVE : FIELD_BG);
    DrawRectangleRoundedLines(rec, 0.12f, 8, active ? ACCENT_GOLD : ACCENT_DIM);
    string display = value;
    if (active && ((int)(GetTime() * 2) % 2 == 0)) display += "|";
    DrawText(display.c_str(), x + 10, y + (h - 16) / 2, 16, TEXT_PRIMARY);
}

static void HandleInput(string& text, int maxLen) {
    int key = GetCharPressed();
    while (key > 0) {
        if (key >= 32 && key <= 125 && (int)text.size() < maxLen)
            text += (char)key;
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !text.empty())
        text.pop_back();
}

static void DrawOptionRow(int x, int y, int w, int h, const char* label, bool selected) {
    Rectangle rec = { (float)x, (float)y, (float)w, (float)h };
    DrawRectangleRounded(rec, 0.15f, 8, selected ? MakeColor(30, 60, 120, 255) : FIELD_BG);
    DrawRectangleRoundedLines(rec, 0.15f, 8, selected ? ACCENT_GOLD : ACCENT_DIM);
    int tw = MeasureText(label, 14);
    DrawText(label, x + (w - tw) / 2, y + (h - 14) / 2, 14, selected ? ACCENT_GOLD : TEXT_MUTED);
}

void RunAddMovieScreen(vector<Movie>& movies) {
    string titleText = "";
    string descText = "";
    string durText = "";
    string dayText = "";
    string monText = "";
    string yearText = "";

    int activeField = 0;
    int genreIdx = 0;
    int langIdx = 0;

    bool done = false;
    bool cancelled = false;
    bool success = false;
    int  skipFrames = 5;

    const char* genres[] = { "Action", "Comedy", "Drama", "Horror", "Sci-Fi", "Romance", "Animation", "Thriller" };
    const char* languages[] = { "Bulgarian", "English", "Dubbed" };

    while (!WindowShouldClose() && !done && !cancelled) {
        BeginDrawing();
        DrawTopBar("ADD MOVIE");

        Vector2 mp = GetMousePosition();

        int fx = 110;
        int fw = 560;

        DrawField(fx, 92, fw, 34, "Title", titleText, activeField == 0);
        DrawField(fx, 158, fw, 34, "Description", descText, activeField == 1);

        DrawField(fx, 224, 160, 34, "Duration (min)", durText, activeField == 2);
        DrawField(fx + 180, 224, 80, 34, "Day", dayText, activeField == 3);
        DrawField(fx + 280, 224, 80, 34, "Month", monText, activeField == 4);
        DrawField(fx + 380, 224, 100, 34, "Year", yearText, activeField == 5);

        DrawText("Genre:", fx, 276, 13, TEXT_MUTED);
        int gx = fx;
        for (int i = 0; i < 8; i++) {
            int gw = MeasureText(genres[i], 14) + 16;
            DrawOptionRow(gx, 292, gw, 28, genres[i], genreIdx == i);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && skipFrames <= 0
                && IsHovered(gx, 292, gw, 28))
                genreIdx = i;
            gx += gw + 6;
        }

        DrawText("Language:", fx, 334, 13, TEXT_MUTED);
        int lx = fx;
        for (int i = 0; i < 3; i++) {
            int lw = MeasureText(languages[i], 14) + 20;
            DrawOptionRow(lx, 350, lw, 28, languages[i], langIdx == i);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && skipFrames <= 0
                && IsHovered(lx, 350, lw, 28))
                langIdx = i;
            lx += lw + 6;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && skipFrames <= 0) {
            if (IsHovered(fx, 92, fw, 34))        activeField = 0;
            else if (IsHovered(fx, 158, fw, 34))  activeField = 1;
            else if (IsHovered(fx, 224, 160, 34)) activeField = 2;
            else if (IsHovered(fx + 180, 224, 80, 34)) activeField = 3;
            else if (IsHovered(fx + 280, 224, 80, 34)) activeField = 4;
            else if (IsHovered(fx + 380, 224, 100, 34)) activeField = 5;
        }

        if (activeField == 0) HandleInput(titleText, 50);
        if (activeField == 1) HandleInput(descText, 80);
        if (activeField == 2) HandleInput(durText, 4);
        if (activeField == 3) HandleInput(dayText, 2);
        if (activeField == 4) HandleInput(monText, 2);
        if (activeField == 5) HandleInput(yearText, 4);

        bool canSave = !titleText.empty() && !descText.empty() && !durText.empty()
            && !dayText.empty() && !monText.empty() && !yearText.empty();

        string errorMsg = "";
        if (!canSave) errorMsg = "Fill in all fields to save.";

        if (!errorMsg.empty())
            DrawText(errorMsg.c_str(),
                (800 - MeasureText(errorMsg.c_str(), 13)) / 2, 392, 13,
                MakeColor(200, 80, 80, 255));

        bool save = DrawBtn(110, 410, 260, 46, "Save Movie", BTN_GREEN, BTN_GREEN_H, !canSave);
        bool cancel = DrawBtn(420, 410, 260, 46, "Cancel", BTN_RED, BTN_RED_H);

        if (save && canSave && skipFrames <= 0) {
            Movie m;
            m.id = (int)movies.size() + 1;
            m.title = titleText;
            m.description = descText;
            m.durationMinutes = atoi(durText.c_str());
            m.releaseDate = { atoi(dayText.c_str()), atoi(monText.c_str()), atoi(yearText.c_str()) };
            m.genre = (Genre)genreIdx;
            m.language = (Language)langIdx;
            movies.push_back(m);
            success = true;
            done = true;
        }
        if (cancel && skipFrames <= 0) cancelled = true;

        if (skipFrames > 0) skipFrames--;
        EndDrawing();
    }

    if (success) {
        float timer = 2.5f;
        while (!WindowShouldClose() && timer > 0) {
            timer -= GetFrameTime();
            BeginDrawing();
            DrawTopBar("MOVIE ADDED");
            DrawText("Movie added successfully!",
                (800 - MeasureText("Movie added successfully!", 24)) / 2,
                260, 24, BTN_GREEN_H);
            DrawText(("\"" + movies.back().title + "\" is now in the system.").c_str(),
                (800 - MeasureText(("\"" + movies.back().title + "\" is now in the system.").c_str(), 16)) / 2,
                300, 16, TEXT_MUTED);
            EndDrawing();
        }
    }
}

void RunDeleteMovieScreen(vector<Movie>& movies) {
    int  scrollOffset = 0;
    int  selectedIdx = -1;
    bool done = false;
    int  skipFrames = 5;

    const int cardH = 72;
    const int cardGap = 6;
    const int listStartY = 90;
    const int listEndY = 500;
    const int visibleH = listEndY - listStartY;

    while (!WindowShouldClose() && !done) {
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            int maxScroll = (int)movies.size() * (cardH + cardGap) - visibleH;
            if (maxScroll < 0) maxScroll = 0;
            scrollOffset -= (int)(wheel * 30);
            if (scrollOffset < 0)          scrollOffset = 0;
            if (scrollOffset > maxScroll)  scrollOffset = maxScroll;
        }

        Vector2 mp = GetMousePosition();

        BeginDrawing();
        DrawTopBar("DELETE MOVIE");
        DrawText("Select a movie to delete:", 110, 82, 14, TEXT_MUTED);

        BeginScissorMode(110, listStartY, 580, visibleH);

        for (int i = 0; i < (int)movies.size(); i++) {
            int y = listStartY + i * (cardH + cardGap) - scrollOffset;
            if (y + cardH < listStartY || y > listEndY) continue;

            bool sel = (selectedIdx == i);
            bool hov = mp.x >= 110 && mp.x <= 690 && mp.y >= y && mp.y <= y + cardH;

            Rectangle rec = { 110, (float)y, 580, (float)cardH };
            DrawRectangleRounded(rec, 0.1f, 8,
                sel ? MakeColor(80, 20, 20, 255) :
                hov ? MakeColor(40, 25, 25, 255) : BG_PANEL);
            DrawRectangleRoundedLines(rec, 0.1f, 8,
                sel ? MakeColor(200, 60, 60, 255) : ACCENT_DIM);

            DrawText(movies[i].title.c_str(), 126, y + 10, 18, TEXT_PRIMARY);
            string info = GenreToString(movies[i].genre) + "  |  "
                + LanguageToString(movies[i].language) + "  |  "
                + to_string(movies[i].durationMinutes) + " min";
            DrawText(info.c_str(), 126, y + 36, 13, TEXT_MUTED);

            if (hov && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && skipFrames <= 0)
                selectedIdx = (selectedIdx == i) ? -1 : i;
        }

        EndScissorMode();

        int totalH = (int)movies.size() * (cardH + cardGap);
        if (totalH > visibleH) {
            float ratio = (float)visibleH / totalH;
            int   barH = max(20, (int)(visibleH * ratio));
            int   barY = listStartY + (int)((float)scrollOffset / totalH * visibleH);
            DrawRectangle(694, listStartY, 6, visibleH, MakeColor(30, 35, 50, 255));
            DrawRectangle(694, barY, 6, barH, ACCENT_DIM);
        }

        DrawLine(110, 504, 690, 504, ACCENT_DIM);

        if (selectedIdx >= 0) {
            string warn = "Delete \"" + movies[selectedIdx].title + "\"? This cannot be undone.";
            DrawText(warn.c_str(),
                (800 - MeasureText(warn.c_str(), 13)) / 2, 510, 13,
                MakeColor(220, 80, 80, 255));
        }

        bool del = DrawBtn(110, 530, 260, 44, "Delete Selected",
            BTN_RED, BTN_RED_H, selectedIdx < 0);
        bool cancel = DrawBtn(430, 530, 260, 44, "Back",
            BTN_BLUE, BTN_BLUE_H);

        if (del && selectedIdx >= 0 && skipFrames <= 0) {
            movies.erase(movies.begin() + selectedIdx);
            selectedIdx = -1;
        }
        if (cancel && skipFrames <= 0) done = true;

        if (skipFrames > 0) skipFrames--;
        EndDrawing();
    }
}