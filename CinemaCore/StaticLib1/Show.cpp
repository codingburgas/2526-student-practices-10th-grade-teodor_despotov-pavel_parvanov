#include "Show.h"
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
static const Color BTN_GREEN = { 30, 100,  50, 255 };
static const Color BTN_GREEN_H = { 50, 160,  80, 255 };

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

string HallTypeToString(HallType t) {
    switch (t) {
    case HALL_SMALL:  return "Small";
    case HALL_MEDIUM: return "Medium";
    case HALL_LARGE:  return "Large";
    default:          return "Unknown";
    }
}

string ShowTimeToString(const ShowTime& t) {
    string h = (t.hour < 10 ? "0" : "") + to_string(t.hour);
    string m = (t.minute < 10 ? "0" : "") + to_string(t.minute);
    return h + ":" + m;
}

vector<Show> GetShowsByMovie(const vector<Show>& shows, int movieId) {
    vector<Show> result;
    for (int i = 0; i < (int)shows.size(); i++) {
        if (shows[i].movieId == movieId && shows[i].isActive)
            result.push_back(shows[i]);
    }
    return result;
}

vector<Show> GetShowsByDate(const vector<Show>& shows, const Date& date) {
    vector<Show> result;
    for (int i = 0; i < (int)shows.size(); i++) {
        if (DateEquals(shows[i].date, date) && shows[i].isActive)
            result.push_back(shows[i]);
    }
    return result;
}

vector<Show> GetAllShows() {
    vector<Show> shows;
    int id = 1;

    Hall small1;  small1.id = 1; small1.name = "Hall 1"; small1.type = HALL_SMALL;  small1.totalSeats = 50;
    Hall medium1; medium1.id = 2; medium1.name = "Hall 2"; medium1.type = HALL_MEDIUM; medium1.totalSeats = 100;
    Hall large1;  large1.id = 3; large1.name = "Hall 3"; large1.type = HALL_LARGE;  large1.totalSeats = 200;

    Show s1;  s1.id = id++;  s1.movieId = 1;  s1.hall = large1;  s1.date = { 26,5,2026 }; s1.time = { 10,0 };  s1.endTime = { 12,32 }; s1.isActive = true; shows.push_back(s1);
    Show s2;  s2.id = id++;  s2.movieId = 13; s2.hall = medium1; s2.date = { 26,5,2026 }; s2.time = { 10,30 }; s2.endTime = { 13,58 }; s2.isActive = true; shows.push_back(s2);
    Show s3;  s3.id = id++;  s3.movieId = 22; s3.hall = small1;  s3.date = { 26,5,2026 }; s3.time = { 13,0 };  s3.endTime = { 15,2 };  s3.isActive = true; shows.push_back(s3);
    Show s4;  s4.id = id++;  s4.movieId = 1;  s4.hall = large1;  s4.date = { 26,5,2026 }; s4.time = { 15,0 };  s4.endTime = { 17,32 }; s4.isActive = true; shows.push_back(s4);
    Show s5;  s5.id = id++;  s5.movieId = 17; s5.hall = medium1; s5.date = { 26,5,2026 }; s5.time = { 15,30 }; s5.endTime = { 18,44 }; s5.isActive = true; shows.push_back(s5);
    Show s6;  s6.id = id++;  s6.movieId = 13; s6.hall = large1;  s6.date = { 26,5,2026 }; s6.time = { 19,0 };  s6.endTime = { 21,28 }; s6.isActive = true; shows.push_back(s6);
    Show s7;  s7.id = id++;  s7.movieId = 22; s7.hall = medium1; s7.date = { 26,5,2026 }; s7.time = { 20,0 };  s7.endTime = { 22,2 };  s7.isActive = true; shows.push_back(s7);

    Show s8;  s8.id = id++;  s8.movieId = 7;  s8.hall = large1;  s8.date = { 27,5,2026 }; s8.time = { 10,0 };  s8.endTime = { 12,22 }; s8.isActive = true; shows.push_back(s8);
    Show s9;  s9.id = id++;  s9.movieId = 14; s9.hall = medium1; s9.date = { 27,5,2026 }; s9.time = { 10,0 };  s9.endTime = { 12,49 }; s9.isActive = true; shows.push_back(s9);
    Show s10; s10.id = id++; s10.movieId = 3; s10.hall = small1;  s10.date = { 27,5,2026 }; s10.time = { 13,30 }; s10.endTime = { 15,11 }; s10.isActive = true; shows.push_back(s10);
    Show s11; s11.id = id++; s11.movieId = 7; s11.hall = large1;  s11.date = { 27,5,2026 }; s11.time = { 15,0 };  s11.endTime = { 17,22 }; s11.isActive = true; shows.push_back(s11);
    Show s12; s12.id = id++; s12.movieId = 14; s12.hall = medium1; s12.date = { 27,5,2026 }; s12.time = { 15,0 };  s12.endTime = { 17,49 }; s12.isActive = true; shows.push_back(s12);
    Show s13; s13.id = id++; s13.movieId = 10; s13.hall = small1;  s13.date = { 27,5,2026 }; s13.time = { 21,0 };  s13.endTime = { 23,26 }; s13.isActive = true; shows.push_back(s13);

    Show s14; s14.id = id++; s14.movieId = 15; s14.hall = large1;  s14.date = { 28,5,2026 }; s14.time = { 10,0 };  s14.endTime = { 12,16 }; s14.isActive = true; shows.push_back(s14);
    Show s15; s15.id = id++; s15.movieId = 8; s15.hall = medium1; s15.date = { 28,5,2026 }; s15.time = { 11,0 };  s15.endTime = { 13,22 }; s15.isActive = true; shows.push_back(s15);
    Show s16; s16.id = id++; s16.movieId = 20; s16.hall = small1;  s16.date = { 28,5,2026 }; s16.time = { 13,0 };  s16.endTime = { 14,28 }; s16.isActive = true; shows.push_back(s16);
    Show s17; s17.id = id++; s17.movieId = 15; s17.hall = large1;  s17.date = { 28,5,2026 }; s17.time = { 15,0 };  s17.endTime = { 17,16 }; s17.isActive = true; shows.push_back(s17);
    Show s18; s18.id = id++; s18.movieId = 4; s18.hall = medium1; s18.date = { 28,5,2026 }; s18.time = { 16,0 };  s18.endTime = { 17,39 }; s18.isActive = true; shows.push_back(s18);
    Show s19; s19.id = id++; s19.movieId = 22; s19.hall = large1;  s19.date = { 28,5,2026 }; s19.time = { 20,0 };  s19.endTime = { 22,2 };  s19.isActive = true; shows.push_back(s19);

    Show s20; s20.id = id++; s20.movieId = 2; s20.hall = large1;  s20.date = { 29,5,2026 }; s20.time = { 10,0 };  s20.endTime = { 12,0 };  s20.isActive = true; shows.push_back(s20);
    Show s21; s21.id = id++; s21.movieId = 18; s21.hall = medium1; s21.date = { 29,5,2026 }; s21.time = { 11,0 };  s21.endTime = { 13,8 };  s21.isActive = true; shows.push_back(s21);
    Show s22; s22.id = id++; s22.movieId = 21; s22.hall = small1;  s22.date = { 29,5,2026 }; s22.time = { 13,0 };  s22.endTime = { 14,36 }; s22.isActive = true; shows.push_back(s22);
    Show s23; s23.id = id++; s23.movieId = 2; s23.hall = large1;  s23.date = { 29,5,2026 }; s23.time = { 15,0 };  s23.endTime = { 17,0 };  s23.isActive = true; shows.push_back(s23);
    Show s24; s24.id = id++; s24.movieId = 23; s24.hall = medium1; s24.date = { 29,5,2026 }; s24.time = { 19,0 };  s24.endTime = { 21,29 }; s24.isActive = true; shows.push_back(s24);
    Show s25; s25.id = id++; s25.movieId = 11; s25.hall = small1;  s25.date = { 29,5,2026 }; s25.time = { 21,30 }; s25.endTime = { 23,14 }; s25.isActive = true; shows.push_back(s25);

    Show s26; s26.id = id++; s26.movieId = 1; s26.hall = large1;  s26.date = { 30,5,2026 }; s26.time = { 10,0 };  s26.endTime = { 12,32 }; s26.isActive = true; shows.push_back(s26);
    Show s27; s27.id = id++; s27.movieId = 13; s27.hall = medium1; s27.date = { 30,5,2026 }; s27.time = { 10,0 };  s27.endTime = { 12,28 }; s27.isActive = true; shows.push_back(s27);
    Show s28; s28.id = id++; s28.movieId = 6; s28.hall = small1;  s28.date = { 30,5,2026 }; s28.time = { 13,0 };  s28.endTime = { 14,43 }; s28.isActive = true; shows.push_back(s28);
    Show s29; s29.id = id++; s29.movieId = 1; s29.hall = large1;  s29.date = { 30,5,2026 }; s29.time = { 15,0 };  s29.endTime = { 17,32 }; s29.isActive = true; shows.push_back(s29);
    Show s30; s30.id = id++; s30.movieId = 13; s30.hall = medium1; s30.date = { 30,5,2026 }; s30.time = { 15,30 }; s30.endTime = { 17,58 }; s30.isActive = true; shows.push_back(s30);
    Show s31; s31.id = id++; s31.movieId = 24; s31.hall = large1;  s31.date = { 30,5,2026 }; s31.time = { 19,0 };  s31.endTime = { 21,7 };  s31.isActive = true; shows.push_back(s31);
    Show s32; s32.id = id++; s32.movieId = 10; s32.hall = small1;  s32.date = { 30,5,2026 }; s32.time = { 22,0 };  s32.endTime = { 24,26 }; s32.isActive = true; shows.push_back(s32);

    Show s33; s33.id = id++; s33.movieId = 19; s33.hall = small1;  s33.date = { 31,5,2026 }; s33.time = { 10,0 };  s33.endTime = { 12,5 };  s33.isActive = true; shows.push_back(s33);
    Show s34; s34.id = id++; s34.movieId = 17; s34.hall = large1;  s34.date = { 31,5,2026 }; s34.time = { 10,0 };  s34.endTime = { 13,14 }; s34.isActive = true; shows.push_back(s34);
    Show s35; s35.id = id++; s35.movieId = 5; s35.hall = medium1; s35.date = { 31,5,2026 }; s35.time = { 11,0 };  s35.endTime = { 12,53 }; s35.isActive = true; shows.push_back(s35);
    Show s36; s36.id = id++; s36.movieId = 19; s36.hall = small1;  s36.date = { 31,5,2026 }; s36.time = { 14,0 };  s36.endTime = { 16,5 };  s36.isActive = true; shows.push_back(s36);
    Show s37; s37.id = id++; s37.movieId = 17; s37.hall = large1;  s37.date = { 31,5,2026 }; s37.time = { 15,0 };  s37.endTime = { 18,14 }; s37.isActive = true; shows.push_back(s37);
    Show s38; s38.id = id++; s38.movieId = 9; s38.hall = medium1; s38.date = { 31,5,2026 }; s38.time = { 15,0 };  s38.endTime = { 18,15 }; s38.isActive = true; shows.push_back(s38);
    Show s39; s39.id = id++; s39.movieId = 22; s39.hall = large1;  s39.date = { 31,5,2026 }; s39.time = { 20,0 };  s39.endTime = { 22,2 };  s39.isActive = true; shows.push_back(s39);
    Show s40; s40.id = id++; s40.movieId = 13; s40.hall = medium1; s40.date = { 31,5,2026 }; s40.time = { 21,0 };  s40.endTime = { 23,28 }; s40.isActive = true; shows.push_back(s40);

    Show s41; s41.id = id++; s41.movieId = 20; s41.hall = small1;  s41.date = { 1,6,2026 };  s41.time = { 10,0 };  s41.endTime = { 11,28 }; s41.isActive = true; shows.push_back(s41);
    Show s42; s42.id = id++; s42.movieId = 16; s42.hall = medium1; s42.date = { 1,6,2026 };  s42.time = { 11,0 };  s42.endTime = { 13,3 };  s42.isActive = true; shows.push_back(s42);
    Show s43; s43.id = id++; s43.movieId = 7; s43.hall = large1;  s43.date = { 1,6,2026 };  s43.time = { 11,0 };  s43.endTime = { 13,22 }; s43.isActive = true; shows.push_back(s43);
    Show s44; s44.id = id++; s44.movieId = 21; s44.hall = small1;  s44.date = { 1,6,2026 };  s44.time = { 13,0 };  s44.endTime = { 14,36 }; s44.isActive = true; shows.push_back(s44);
    Show s45; s45.id = id++; s45.movieId = 18; s45.hall = medium1; s45.date = { 1,6,2026 };  s45.time = { 15,0 };  s45.endTime = { 17,8 };  s45.isActive = true; shows.push_back(s45);
    Show s46; s46.id = id++; s46.movieId = 7; s46.hall = large1;  s46.date = { 1,6,2026 };  s46.time = { 15,0 };  s46.endTime = { 17,22 }; s46.isActive = true; shows.push_back(s46);
    Show s47; s47.id = id++; s47.movieId = 15; s47.hall = large1;  s47.date = { 1,6,2026 };  s47.time = { 19,0 };  s47.endTime = { 21,16 }; s47.isActive = true; shows.push_back(s47);
    Show s48; s48.id = id++; s48.movieId = 23; s48.hall = medium1; s48.date = { 1,6,2026 };  s48.time = { 20,0 };  s48.endTime = { 22,29 }; s48.isActive = true; shows.push_back(s48);

    return shows;
}

void RunShowtimesScreen(vector<Show>& shows, vector<Movie>& movies) {
    const char* dayNames[] = { "MON 26", "TUE 27", "WED 28", "THU 29", "FRI 30", "SAT 31", "SUN 1" };
    Date        weekDates[] = {
        {26,5,2026},{27,5,2026},{28,5,2026},{29,5,2026},{30,5,2026},{31,5,2026},{1,6,2026}
    };
    int selectedDay = 0;
    int scrollOffset = 0;

    int cardH = 78;
    int cardGap = 6;
    int listStartY = 175;
    int listEndY = 540;
    int visibleArea = listEndY - listStartY;

    while (!WindowShouldClose()) {

        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            vector<Show> cur = GetShowsByDate(shows, weekDates[selectedDay]);
            int totalH = (int)cur.size() * (cardH + cardGap);
            int maxScroll = totalH - visibleArea;
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

        Rectangle header = { 110, 18, 580, 52 };
        DrawRectangleRounded(header, 0.3f, 8, BG_PANEL);
        DrawRectangleRoundedLines(header, 0.3f, 8, ACCENT_DIM);
        const char* title = "WEEKLY  SCHEDULE";
        DrawText(title, (800 - MeasureText(title, 24)) / 2, 30, 24, ACCENT_GOLD);

        int tabW = 80;
        int tabX = (800 - 7 * tabW - 6 * 4) / 2;
        int tabY = 78;

        for (int i = 0; i < 7; i++) {
            bool sel = (selectedDay == i);
            bool hov = IsHovered(tabX, tabY, tabW, 32);
            Color bg = sel ? BTN_BLUE : (hov ? MakeColor(40, 45, 70, 255) : MakeColor(22, 26, 42, 255));
            Color brd = sel ? ACCENT_GOLD : ACCENT_DIM;
            Rectangle tr = { (float)tabX, (float)tabY, (float)tabW, 32 };
            DrawRectangleRounded(tr, 0.2f, 8, bg);
            DrawRectangleRoundedLines(tr, 0.2f, 8, brd);
            int tw = MeasureText(dayNames[i], 13);
            DrawText(dayNames[i], tabX + (tabW - tw) / 2, tabY + 9, 13,
                sel ? TEXT_PRIMARY : TEXT_MUTED);
            if (hov && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                selectedDay = i;
                scrollOffset = 0;
            }
            tabX += tabW + 4;
        }

        DrawLine(110, listStartY - 8, 690, listStartY - 8, ACCENT_DIM);

        vector<Show> dayShows = GetShowsByDate(shows, weekDates[selectedDay]);

        BeginScissorMode(110, listStartY, 580, visibleArea);

        if (dayShows.empty()) {
            const char* msg = "No shows scheduled for this day.";
            DrawText(msg, (800 - MeasureText(msg, 18)) / 2, 340, 18, TEXT_MUTED);
        }
        else {
            Vector2 mp = GetMousePosition();
            for (int i = 0; i < (int)dayShows.size(); i++) {
                int y = listStartY + i * (cardH + cardGap) - scrollOffset;
                if (y + cardH < listStartY || y > listEndY) continue;

                Show& s = dayShows[i];

                string mTitle = "Unknown";
                for (int j = 0; j < (int)movies.size(); j++)
                    if (movies[j].id == s.movieId) { mTitle = movies[j].title; break; }

                bool hov = IsHovered(110, y, 580, cardH);
                Color bg = hov ? MakeColor(30, 35, 60, 255) : BG_PANEL;
                Color border = hov ? ACCENT_GOLD : ACCENT_DIM;
                Rectangle rec = { 110, (float)y, 580, (float)cardH };
                DrawRectangleRounded(rec, 0.1f, 8, bg);
                DrawRectangleRoundedLines(rec, 0.1f, 8, border);

                DrawText(mTitle.c_str(), 124, y + 8, 20, TEXT_PRIMARY);

                string timeRange = ShowTimeToString(s.time) + "  -  " + ShowTimeToString(s.endTime);
                DrawText(timeRange.c_str(), 124, y + 34, 16, ACCENT_GOLD);

                string hallInfo = s.hall.name + "  |  " +
                    HallTypeToString(s.hall.type) + "  |  " +
                    to_string(s.hall.totalSeats) + " seats";
                DrawText(hallInfo.c_str(), 124, y + 56, 13, TEXT_MUTED);
            }
        }

        EndScissorMode();

        int totalH = (int)dayShows.size() * (cardH + cardGap);
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