#include "Login.h"
#include "raylib.h"
#include <vector>

using namespace std;

static vector<pair<string, string>> registeredUsers;

static Color MakeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    Color c; c.r = r; c.g = g; c.b = b; c.a = a; return c;
}

static const Color BG_DARK = { 10,  12,  20,  255 };
static const Color BG_PANEL = { 18,  22,  36,  255 };
static const Color ACCENT_GOLD = { 212, 175,  55,  255 };
static const Color ACCENT_DIM = { 120,  95,  28,  255 };
static const Color TEXT_PRIMARY = { 235, 230, 210,  255 };
static const Color TEXT_MUTED = { 130, 125, 110,  255 };
static const Color BTN_GREEN = { 46, 139,  87,  255 };
static const Color BTN_RED = { 180,  40,  40,  255 };
static const Color BTN_GRAY = { 55,  60,  75,  255 };
static const Color FIELD_BG = { 25,  30,  48,  255 };
static const Color FIELD_ACTIVE = { 35,  42,  68,  255 };

static bool IsHovered(int x, int y, int w, int h) {
    Vector2 m = GetMousePosition();
    return m.x >= x && m.x <= x + w && m.y >= y && m.y <= y + h;
}

static void DrawButtonPro(int x, int y, int w, int h,
    const char* label, Color base, Color hover, int fontSize = 18)
{
    Color c = IsHovered(x, y, w, h) ? hover : base;
    Rectangle rec = { (float)x, (float)y, (float)w, (float)h };
    DrawRectangleRounded(rec, 0.25f, 8, c);
    DrawRectangleRoundedLines(rec, 0.25f, 8, ACCENT_GOLD);

    int tw = MeasureText(label, fontSize);
    DrawText(label, x + (w - tw) / 2, y + (h - fontSize) / 2, fontSize, TEXT_PRIMARY);
}

static void DrawFieldPro(int x, int y, int w, int h,
    const char* label, const string& value,
    bool active, bool secret)
{
    Color bg = active ? FIELD_ACTIVE : FIELD_BG;
    Color border = active ? ACCENT_GOLD : ACCENT_DIM;
    Rectangle rec = { (float)x, (float)y, (float)w, (float)h };
    DrawRectangleRounded(rec, 0.18f, 8, bg);
    DrawRectangleRoundedLines(rec, 0.18f, 8, border);

    if (value.empty() && !active) {

        DrawText(label, x + 12, y + (h - 16) / 2, 16, TEXT_MUTED);
    }
    else {
        string display = secret ? string(value.length(), '*') : value;
        if (active && ((int)(GetTime() * 2) % 2 == 0)) display += "|";
        DrawText(display.c_str(), x + 12, y + (h - 18) / 2, 18, TEXT_PRIMARY);
    }

    if (value.empty() && active && ((int)(GetTime() * 2) % 2 == 0)) {
        DrawText("|", x + 12, y + (h - 18) / 2, 18, TEXT_PRIMARY);
    }
}

static void DrawTitle(const char* line1, const char* line2, Color col) {
    int w1 = MeasureText(line1, 14);
    DrawText(line1, (800 - w1) / 2, 55, 14, TEXT_MUTED);
    int w2 = MeasureText(line2, 34);
    DrawText(line2, (800 - w2) / 2, 72, 34, col);
    DrawLine(200, 115, 600, 115, ACCENT_DIM);
}

void handleTextInput(string& text, int maxLength) {
    int key = GetCharPressed();
    while (key > 0) {
        if ((key >= 32) && (key <= 125) && ((int)text.length() < maxLength))
            text += (char)key;
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !text.empty())
        text.pop_back();
}

bool checkAdminCredentials(const string& user, const string& pass) {

    string lowerUser = user;
    for (int i = 0; i < (int)lowerUser.size(); i++)
        lowerUser[i] = (char)tolower((unsigned char)lowerUser[i]);
    return (lowerUser == "admin" && pass == "admin123");
}

User runCinemaAuthSystem() {
    Screen currentScreen = SCREEN_START;
    User loggedInUser;

    string inputUser, inputPass;
    int    activeField = 0;
    string errorMsg;

    while (!WindowShouldClose()) {

        if (currentScreen == SCREEN_LOGIN ||
            currentScreen == SCREEN_SIGNUP ||
            currentScreen == SCREEN_ADMIN_LOGIN)
        {
            Vector2 mp = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (mp.y >= 190 && mp.y <= 234 && mp.x >= 220 && mp.x <= 560)
                    activeField = 1;
                else if (mp.y >= 265 && mp.y <= 309 && mp.x >= 220 && mp.x <= 560)
                    activeField = 2;
                else if (mp.y < 420)
                    activeField = 0;
            }
            if (activeField == 1) handleTextInput(inputUser, 20);
            if (activeField == 2) handleTextInput(inputPass, 20);
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

        if (currentScreen == SCREEN_START) {

            DrawTitle("WELCOME TO", "CINEMA SYSTEM", ACCENT_GOLD);

            Rectangle panel = { 220, 140, 360, 310 };
            DrawRectangleRounded(panel, 0.08f, 8, BG_PANEL);
            DrawRectangleRoundedLines(panel, 0.08f, 8, ACCENT_DIM);

            const char* labels[] = { "LOG IN", "SIGN UP", "CONTINUE AS GUEST", "ADMINISTRATOR" };
            int         ys[] = { 165, 225, 285, 345 };

            Color baseColors[4];
            baseColors[0] = MakeColor(30, 70, 130, 255);
            baseColors[1] = MakeColor(25, 90, 55, 255);
            baseColors[2] = MakeColor(55, 55, 70, 255);
            baseColors[3] = MakeColor(100, 20, 20, 255);

            Color hoverColors[4];
            hoverColors[0] = MakeColor(60, 130, 200, 255);
            hoverColors[1] = MakeColor(46, 139, 87, 255);
            hoverColors[2] = MakeColor(100, 100, 120, 255);
            hoverColors[3] = MakeColor(160, 30, 30, 255);

            Vector2 mp = GetMousePosition();
            for (int i = 0; i < 4; i++)
                DrawButtonPro(240, ys[i], 320, 48, labels[i], baseColors[i], hoverColors[i], 17);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (mp.x >= 240 && mp.x <= 560) {
                    for (int i = 0; i < 4; i++) {
                        if (mp.y >= ys[i] && mp.y <= ys[i] + 48) {
                            inputUser = ""; inputPass = ""; errorMsg = ""; activeField = 0;
                            if (i == 0) currentScreen = SCREEN_LOGIN;
                            else if (i == 1) currentScreen = SCREEN_SIGNUP;
                            else if (i == 2) {
                                loggedInUser.username = "Guest";
                                loggedInUser.password = "";
                                loggedInUser.isAdmin = false;
                                loggedInUser.isGuest = true;
                                loggedInUser.isLogged = true;
                                EndDrawing();
                                return loggedInUser;
                            }
                            else currentScreen = SCREEN_ADMIN_LOGIN;
                        }
                    }
                }
            }
        }

        else {
            bool isAdmin = (currentScreen == SCREEN_ADMIN_LOGIN);
            bool isSignup = (currentScreen == SCREEN_SIGNUP);

            const char* title1 = isAdmin ? "RESTRICTED ACCESS"
                : isSignup ? "CREATE ACCOUNT"
                : "USER ACCESS";
            const char* title2 = isAdmin ? "ADMINISTRATOR"
                : isSignup ? "SIGN  UP"
                : "LOG  IN";
            Color titleCol = isAdmin ? BTN_RED : ACCENT_GOLD;

            DrawTitle(title1, title2, titleCol);

            Rectangle formPanel = { 180, 130, 440, 280 };
            DrawRectangleRounded(formPanel, 0.08f, 8, BG_PANEL);
            DrawRectangleRoundedLines(formPanel, 0.08f, 8, isAdmin ? BTN_RED : ACCENT_DIM);

            DrawFieldPro(220, 190, 340, 44, "Username", inputUser, activeField == 1, false);
            DrawFieldPro(220, 265, 340, 44, "Password", inputPass, activeField == 2, true);

            if (!errorMsg.empty()) {
                int ew = MeasureText(errorMsg.c_str(), 15);
                DrawText(errorMsg.c_str(), (800 - ew) / 2, 330, 15, BTN_RED);
            }

            const char* confirmLabel = isSignup ? "REGISTER" : "LOG  IN";
            Color confirmBase = isAdmin ? MakeColor(100, 20, 20, 255) : BTN_GREEN;
            Color confirmHover = isAdmin ? BTN_RED : MakeColor(60, 180, 100, 255);
            Color backHover = MakeColor(80, 85, 105, 255);

            DrawButtonPro(215, 450, 170, 46, confirmLabel, confirmBase, confirmHover, 17);
            DrawButtonPro(415, 450, 170, 46, "BACK", BTN_GRAY, backHover, 17);

            Vector2 mp = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

                if (mp.x >= 415 && mp.x <= 585 && mp.y >= 450 && mp.y <= 496) {
                    currentScreen = SCREEN_START;
                    errorMsg = "";
                    activeField = 0;
                }

                else if (mp.x >= 215 && mp.x <= 385 && mp.y >= 450 && mp.y <= 496) {
                    if (inputUser.empty() || inputPass.empty()) {
                        errorMsg = "Please fill in all fields!";
                    }
                    else if (isSignup) {

                        bool exists = false;
                        for (int j = 0; j < (int)registeredUsers.size(); j++) {
                            if (registeredUsers[j].first == inputUser) {
                                exists = true;
                                break;
                            }
                        }
                        if (exists) {
                            errorMsg = "Username already taken!";
                        }
                        else {
                            registeredUsers.push_back(make_pair(inputUser, inputPass));
                            errorMsg = "Account created! Please log in.";
                            currentScreen = SCREEN_LOGIN;
                            inputUser = ""; inputPass = "";
                        }
                    }
                    else if (isAdmin) {
                        if (checkAdminCredentials(inputUser, inputPass)) {
                            loggedInUser.username = inputUser;
                            loggedInUser.password = inputPass;
                            loggedInUser.isAdmin = true;
                            loggedInUser.isGuest = false;
                            loggedInUser.isLogged = true;
                            EndDrawing();
                            return loggedInUser;
                        }
                        else {
                            errorMsg = "Invalid administrator credentials!";
                            inputPass = "";
                        }
                    }
                    else {

                        bool found = false;
                        for (int j = 0; j < (int)registeredUsers.size(); j++) {
                            if (registeredUsers[j].first == inputUser &&
                                registeredUsers[j].second == inputPass) {
                                found = true;
                                break;
                            }
                        }
                        if (found) {
                            loggedInUser.username = inputUser;
                            loggedInUser.password = inputPass;
                            loggedInUser.isAdmin = false;
                            loggedInUser.isGuest = false;
                            loggedInUser.isLogged = true;
                            EndDrawing();
                            return loggedInUser;
                        }
                        else {
                            errorMsg = "Invalid username or password!";
                            inputPass = "";
                        }
                    }
                }
            }
        }

        EndDrawing();
    }

    return loggedInUser;
}