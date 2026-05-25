#include "raylib.h"
#include "../Tickets/Login.h"
#include "../StaticLib1/Movie.h"
#include "../StaticLib1/Show.h"

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
static const Color BTN_GRAY = { 55,  60,  75, 255 };
static const Color BTN_GRAY_H = { 80,  85, 105, 255 };
static const Color BTN_ADMIN = { 80,  20,  80, 255 };
static const Color BTN_ADMIN_H = { 140,  40, 140, 255 };
static const Color BTN_PURPLE = { 60,  20, 100, 255 };
static const Color BTN_PURPLE_H = { 110,  40, 170, 255 };

static bool IsHovered(int x, int y, int w, int h) {
    Vector2 m = GetMousePosition();
    return m.x >= x && m.x <= x + w && m.y >= y && m.y <= y + h;
}

static bool DrawMenuButton(int x, int y, int w, int h,
    const char* icon, const char* label,
    Color base, Color hover, bool locked = false)
{
    bool  hov = !locked && IsHovered(x, y, w, h);
    Color bg = locked ? MakeColor(30, 30, 40, 200) : (hov ? hover : base);
    Color txt = locked ? TEXT_MUTED : TEXT_PRIMARY;

    Rectangle rec = { (float)x, (float)y, (float)w, (float)h };
    DrawRectangleRounded(rec, 0.15f, 8, bg);
    Color borderCol = locked ? MakeColor(50, 50, 60, 255) : ACCENT_GOLD;
    DrawRectangleRoundedLines(rec, 0.15f, 8, borderCol);

    Color circleCol = locked ? MakeColor(40, 40, 50, 255) : MakeColor(0, 0, 0, 80);
    DrawCircle(x + 36, y + h / 2, 16, circleCol);
    DrawText(icon,
        x + 36 - MeasureText(icon, 16) / 2,
        y + h / 2 - 8, 16,
        locked ? TEXT_MUTED : ACCENT_GOLD);

    DrawText(label, x + 65, y + (h - 20) / 2, 20, txt);

    if (locked) {
        const char* lk = "[LOCKED]";
        DrawText(lk, x + w - MeasureText(lk, 14) - 12, y + (h - 14) / 2, 14,
            MakeColor(80, 80, 90, 255));
    }
    else if (hov) {
        DrawText(">", x + w - 24, y + (h - 20) / 2, 20, ACCENT_GOLD);
    }

    return hov && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

static void DrawHeader(const User& currentUser) {
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

    Rectangle header = { 100, 22, 600, 60 };
    DrawRectangleRounded(header, 0.3f, 8, BG_PANEL);
    DrawRectangleRoundedLines(header, 0.3f, 8, ACCENT_DIM);

    const char* title = "CINEMA  BOOKING  SYSTEM";
    int tw = MeasureText(title, 26);
    DrawText(title, (800 - tw) / 2, 37, 26, ACCENT_GOLD);

    string badge = currentUser.isAdmin ? "[ADMIN] " + currentUser.username
        : currentUser.isGuest ? "[GUEST]"
        : currentUser.username;
    Color badgeCol = currentUser.isAdmin ? MakeColor(200, 100, 200, 255)
        : currentUser.isGuest ? TEXT_MUTED
        : MakeColor(100, 200, 150, 255);
    DrawText(badge.c_str(), 620, 44, 14, badgeCol);
}

static void DrawSeparator(const char* label) {
    DrawLine(100, 92, 700, 92, ACCENT_DIM);
    int lw = MeasureText(label, 13);
    DrawText(label, (800 - lw) / 2, 97, 13, TEXT_MUTED);
    DrawLine(100, 112, 700, 112, ACCENT_DIM);
}

static void DrawFooter(const char* hint) {
    DrawLine(100, 574, 700, 574, ACCENT_DIM);
    DrawText(hint, (800 - MeasureText(hint, 12)) / 2, 582, 12, TEXT_MUTED);
}

int main() {
    InitWindow(800, 600, "Cinema Booking System");
    SetTargetFPS(60);

    User          currentUser = runCinemaAuthSystem();
    vector<Movie> movies = GetAllMovies();
    vector<Show>  shows = GetAllShows();

    if (!currentUser.isLogged) {
        CloseWindow();
        return 0;
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BG_DARK);

        DrawHeader(currentUser);

        if (currentUser.isAdmin) {

            DrawSeparator("ADMIN  PANEL");

            struct AdminItem {
                const char* icon;
                const char* label;
                Color       base, hover;
            };

            AdminItem adminItems[7];
            adminItems[0].icon = "+"; adminItems[0].label = "Add Movie";
            adminItems[0].base = BTN_ADMIN; adminItems[0].hover = BTN_ADMIN_H;

            adminItems[1].icon = "-"; adminItems[1].label = "Delete Movie";
            adminItems[1].base = BTN_RED; adminItems[1].hover = BTN_RED_H;

            adminItems[2].icon = "+"; adminItems[2].label = "Add Show";
            adminItems[2].base = BTN_PURPLE; adminItems[2].hover = BTN_PURPLE_H;

            adminItems[3].icon = "-"; adminItems[3].label = "Delete Show";
            adminItems[3].base = BTN_RED; adminItems[3].hover = BTN_RED_H;

            adminItems[4].icon = "*"; adminItems[4].label = "Update Show";
            adminItems[4].base = BTN_PURPLE; adminItems[4].hover = BTN_PURPLE_H;

            adminItems[5].icon = "#"; adminItems[5].label = "View All Bookings";
            adminItems[5].base = BTN_BLUE; adminItems[5].hover = BTN_BLUE_H;

            adminItems[6].icon = "!"; adminItems[6].label = "Send Notification";
            adminItems[6].base = BTN_BLUE; adminItems[6].hover = BTN_BLUE_H;

            int startY = 122;
            int btnH = 48;
            int gap = 6;

            for (int i = 0; i < 7; i++) {
                int  y = startY + i * (btnH + gap);
                bool clicked = DrawMenuButton(100, y, 580, btnH,
                    adminItems[i].icon, adminItems[i].label,
                    adminItems[i].base, adminItems[i].hover, false);
                if (clicked) {
                    if (i == 0) RunMovieSearchScreen(movies);
                    if (i == 1) RunShowtimesScreen(shows, movies);
                }
            }

            bool exitClicked = DrawMenuButton(100, startY + 7 * (btnH + gap) + 4,
                580, 44, "X", "Logout", BTN_RED, BTN_RED_H, false);
            if (exitClicked) break;

            DrawFooter("Admin mode  |  Full access to all system functions");

        }
        else {

            DrawSeparator("MAIN  MENU");

            struct MenuItem {
                const char* icon;
                const char* label;
                Color       base, hover;
                bool        locked;
            };

            MenuItem items[6];
            items[0].icon = "?";  items[0].label = "Search Movies";
            items[0].base = BTN_BLUE; items[0].hover = BTN_BLUE_H;
            items[0].locked = false;

            items[1].icon = "#";  items[1].label = "View Showtimes";
            items[1].base = BTN_BLUE; items[1].hover = BTN_BLUE_H;
            items[1].locked = false;

            items[2].icon = "+";  items[2].label = "Make a Booking";
            items[2].base = BTN_BLUE; items[2].hover = BTN_BLUE_H;
            items[2].locked = currentUser.isGuest;

            items[3].icon = "[]"; items[3].label = "Select Seats";
            items[3].base = BTN_BLUE; items[3].hover = BTN_BLUE_H;
            items[3].locked = currentUser.isGuest;

            items[4].icon = "$";  items[4].label = "Payment";
            items[4].base = BTN_BLUE; items[4].hover = BTN_BLUE_H;
            items[4].locked = currentUser.isGuest;

            items[5].icon = "@";  items[5].label = "My Bookings";
            items[5].base = BTN_BLUE; items[5].hover = BTN_BLUE_H;
            items[5].locked = currentUser.isGuest;

            int startY = 128;
            int btnH = 56;
            int gap = 8;

            for (int i = 0; i < 6; i++) {
                int  y = startY + i * (btnH + gap);
                bool clicked = DrawMenuButton(100, y, 580, btnH,
                    items[i].icon, items[i].label,
                    items[i].base, items[i].hover,
                    items[i].locked);
                if (clicked) {
                    if (i == 0) RunMovieSearchScreen(movies);
                    if (i == 1) RunShowtimesScreen(shows, movies);
                }
            }

            bool exitClicked = DrawMenuButton(100, startY + 6 * (btnH + gap) + 4,
                580, 44, "X", "Exit", BTN_RED, BTN_RED_H, false);
            if (exitClicked) break;

            DrawFooter("Click any option to continue  |  Locked options require an account");
        }

        EndDrawing();
    }

    CloseWindow();
}