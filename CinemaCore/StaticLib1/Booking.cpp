#include "Booking.h"
#include "raylib.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

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

static Color MakeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    Color c; c.r = r; c.g = g; c.b = b; c.a = a; return c;
}

static bool IsHovered(int x, int y, int w, int h) {
    Vector2 m = GetMousePosition();
    return m.x >= x && m.x <= x + w && m.y >= y && m.y <= y + h;
}

static bool DrawButton(int x, int y, int w, int h,
    const char* label, Color base, Color hover, bool disabled = false)
{
    bool hov = !disabled && IsHovered(x, y, w, h);
    Color bg = disabled ? MakeColor(30, 30, 40, 200) : (hov ? hover : base);
    Color txt = disabled ? TEXT_MUTED : TEXT_PRIMARY;
    Rectangle rec = { (float)x, (float)y, (float)w, (float)h };
    DrawRectangleRounded(rec, 0.15f, 8, bg);
    DrawRectangleRoundedLines(rec, 0.15f, 8, disabled ? MakeColor(50, 50, 60, 255) : ACCENT_GOLD);
    int tw = MeasureText(label, 18);
    DrawText(label, x + (w - tw) / 2, y + (h - 18) / 2, 18, txt);
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

string SeatTypeToString(SeatType t) {
    if (t == SEAT_SILVER) return "Silver";
    if (t == SEAT_GOLD)   return "Gold";
    return "Platinum";
}

float SeatTypeToPrice(SeatType t) {
    if (t == SEAT_SILVER) return 8.0f;
    if (t == SEAT_GOLD)   return 12.0f;
    return 18.0f;
}

void GetSeatColor(SeatType t, bool isBooked, bool isSelected,
    unsigned char& r, unsigned char& g, unsigned char& b)
{
    if (isBooked) { r = 60;  g = 30;  b = 30;  return; }
    if (isSelected) { r = 50;  g = 180; b = 80;  return; }
    if (t == SEAT_SILVER) { r = 80;  g = 80;  b = 100; return; }
    if (t == SEAT_GOLD) { r = 140; g = 110; b = 20;  return; }
    r = 80;  g = 20;  b = 120;
}

vector<Seat> GenerateSeats(const Hall& hall) {
    vector<Seat> seats;
    int cols = 8;
    int rows = hall.totalSeats / cols;
    if (rows <= 0) rows = 6;
    int id = 0;
    for (int r = 0; r < rows; r++) {
        SeatType type;
        if (r < rows / 3)          type = SEAT_PLATINUM;
        else if (r < 2 * rows / 3) type = SEAT_GOLD;
        else                        type = SEAT_SILVER;
        for (int c = 0; c < cols; c++) {
            Seat s;
            s.id = id++;
            s.row = r;
            s.col = c;
            s.type = type;
            s.isBooked = false;
            seats.push_back(s);
        }
    }
    return seats;
}

vector<Booking>& GetBookings() {
    static vector<Booking> bookings;
    return bookings;
}

int NextBookingId() {
    return (int)GetBookings().size() + 1;
}

void RunPaymentScreen(
    Show& show,
    const Movie& movie,
    const User& currentUser,
    vector<int>& selectedSeatIds,
    vector<Seat>& seats,
    vector<Booking>& bookings)
{
    float total = 0;
    for (int sid : selectedSeatIds)
        total += SeatTypeToPrice(seats[sid].type);

    bool paid = false;
    bool cancelled = false;

    while (!WindowShouldClose() && !paid && !cancelled) {
        BeginDrawing();
        DrawTopBar("PAYMENT  -  PAY AT COUNTER");

        Rectangle summBox = { 80, 90, 640, 140 };
        DrawRectangleRounded(summBox, 0.1f, 8, BG_PANEL);
        DrawRectangleRoundedLines(summBox, 0.1f, 8, ACCENT_DIM);

        DrawText(movie.title.c_str(), 100, 100, 22, ACCENT_GOLD);
        DrawText(ShowTimeToString(show.time).c_str(), 100, 130, 16, TEXT_MUTED);

        string seatsStr = "Seats: ";
        for (int i = 0; i < (int)selectedSeatIds.size(); i++) {
            int sid = selectedSeatIds[i];
            seatsStr += "R" + to_string(seats[sid].row + 1)
                + "C" + to_string(seats[sid].col + 1);
            if (i + 1 < (int)selectedSeatIds.size()) seatsStr += ", ";
        }
        DrawText(seatsStr.c_str(), 100, 158, 14, TEXT_PRIMARY);

        string totalStr = "TOTAL:  " + to_string((int)total) + " BGN";
        DrawText(totalStr.c_str(), 100, 195, 20, ACCENT_GOLD);

        Rectangle infoBox = { 80, 255, 640, 150 };
        DrawRectangleRounded(infoBox, 0.1f, 8, MakeColor(12, 28, 12, 255));
        DrawRectangleRoundedLines(infoBox, 0.1f, 8, BTN_GREEN);

        const char* l1 = "Please pay at the cinema counter before the show.";
        const char* l2 = "Show this confirmation to the cashier.";
        const char* l3 = "Seats are held until 15 minutes before the show starts.";
        DrawText(l1, (800 - MeasureText(l1, 16)) / 2, 278, 16, TEXT_PRIMARY);
        DrawText(l2, (800 - MeasureText(l2, 16)) / 2, 308, 16, TEXT_PRIMARY);
        DrawText(l3, (800 - MeasureText(l3, 14)) / 2, 338, 14, TEXT_MUTED);

        string countStr = to_string((int)selectedSeatIds.size())
            + " seat(s)  =  " + to_string((int)total) + " BGN total";
        DrawText(countStr.c_str(),
            (800 - MeasureText(countStr.c_str(), 15)) / 2, 372, 15, ACCENT_GOLD);

        bool confirm = DrawButton(80, 460, 300, 54, "Confirm Booking", BTN_GREEN, BTN_GREEN_H);
        bool cancel = DrawButton(420, 460, 300, 54, "Cancel", BTN_RED, BTN_RED_H);

        if (confirm) {
            for (int sid : selectedSeatIds)
                seats[sid].isBooked = true;
            Booking b;
            b.id = NextBookingId();
            b.showId = show.id;
            b.username = currentUser.username;
            b.seatIds = selectedSeatIds;
            b.totalPrice = total;
            b.isPaid = false;
            bookings.push_back(b);
            paid = true;
        }
        if (cancel) cancelled = true;

        EndDrawing();
    }

    if (paid) {
        float timer = 4.0f;
        while (!WindowShouldClose() && timer > 0) {
            timer -= GetFrameTime();
            BeginDrawing();
            DrawTopBar("BOOKING CONFIRMED");

            DrawText("Booking confirmed!",
                (800 - MeasureText("Booking confirmed!", 28)) / 2, 190, 28, BTN_GREEN_H);
            DrawText(movie.title.c_str(),
                (800 - MeasureText(movie.title.c_str(), 22)) / 2, 235, 22, ACCENT_GOLD);

            string info = "Show: " + ShowTimeToString(show.time)
                + "   |   Hall: " + show.hall.name
                + "   |   Seats: " + to_string((int)selectedSeatIds.size());
            DrawText(info.c_str(),
                (800 - MeasureText(info.c_str(), 15)) / 2, 268, 15, TEXT_PRIMARY);

            string priceStr = "Total to pay at counter: " + to_string((int)total) + " BGN";
            DrawText(priceStr.c_str(),
                (800 - MeasureText(priceStr.c_str(), 18)) / 2, 300, 18, ACCENT_GOLD);

            DrawText("Please pay at the cinema counter before the show.",
                (800 - MeasureText("Please pay at the cinema counter before the show.", 14)) / 2,
                345, 14, TEXT_MUTED);

            string closing = "Returning in " + to_string((int)timer + 1) + "...";
            DrawText(closing.c_str(),
                (800 - MeasureText(closing.c_str(), 13)) / 2, 405, 13, TEXT_MUTED);

            EndDrawing();
        }
    }
}

void RunSeatSelectionScreen(
    Show& show,
    const Movie& movie,
    const User& currentUser,
    vector<Booking>& bookings)
{
    vector<Seat> seats = GenerateSeats(show.hall);

    for (auto& b : bookings)
        if (b.showId == show.id)
            for (int sid : b.seatIds)
                if (sid < (int)seats.size())
                    seats[sid].isBooked = true;

    vector<int> selected;
    bool done = false;
    bool cancelled = false;
    int  skipFrames = 5;

    const int cols = 8;
    const int seatW = 54;
    const int seatH = 38;
    const int gapX = 8;
    const int gapY = 8;
    const int gridW = cols * (seatW + gapX) - gapX;
    const int startX = (800 - gridW) / 2;
    const int gridStartY = 108;
    const int legendH = 52;
    const int bottomBarH = 58;
    const int visibleH = 600 - gridStartY - legendH - bottomBarH;

    int rows = (int)seats.size() / cols;
    int gridTotalH = rows * (seatH + gapY);
    int scrollY = 0;
    int maxScroll = gridTotalH - visibleH;
    if (maxScroll < 0) maxScroll = 0;

    while (!WindowShouldClose() && !done && !cancelled) {

        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            scrollY -= (int)(wheel * 30);
            if (scrollY < 0)         scrollY = 0;
            if (scrollY > maxScroll) scrollY = maxScroll;
        }

        BeginDrawing();
        DrawTopBar("SELECT SEATS");

        string info = movie.title + "  |  " + ShowTimeToString(show.time)
            + "  |  " + show.hall.name;
        DrawText(info.c_str(), (800 - MeasureText(info.c_str(), 14)) / 2, 84, 14, TEXT_MUTED);

        int screenBarY = gridStartY - 16;
        Rectangle screenRec = { (float)startX, (float)screenBarY, (float)gridW, 12 };
        DrawRectangleRounded(screenRec, 0.3f, 8, ACCENT_DIM);
        DrawText("SCREEN", startX + (gridW - MeasureText("SCREEN", 11)) / 2,
            screenBarY + 1, 11, TEXT_PRIMARY);

        BeginScissorMode(startX - 2, gridStartY, gridW + 4, visibleH);

        for (auto& s : seats) {
            int px = startX + s.col * (seatW + gapX);
            int py = gridStartY + s.row * (seatH + gapY) - scrollY;

            if (py + seatH < gridStartY || py > gridStartY + visibleH) continue;

            bool isSelected = find(selected.begin(), selected.end(), s.id) != selected.end();

            unsigned char cr, cg, cb;
            GetSeatColor(s.type, s.isBooked, isSelected, cr, cg, cb);
            Color bg = MakeColor(cr, cg, cb, 255);

            Rectangle rec = { (float)px, (float)py, (float)seatW, (float)seatH };
            DrawRectangleRounded(rec, 0.2f, 6, bg);

            string lbl = to_string(s.row + 1) + "/" + to_string(s.col + 1);
            DrawText(lbl.c_str(),
                px + (seatW - MeasureText(lbl.c_str(), 10)) / 2,
                py + (seatH - 10) / 2, 10, TEXT_PRIMARY);

            if (!s.isBooked && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
                && IsHovered(px, py, seatW, seatH) && skipFrames <= 0)
            {
                auto it = find(selected.begin(), selected.end(), s.id);
                if (it != selected.end()) selected.erase(it);
                else                      selected.push_back(s.id);
            }
        }

        EndScissorMode();

        if (maxScroll > 0) {
            float ratio = (float)visibleH / gridTotalH;
            int   barH = max(20, (int)(visibleH * ratio));
            int   barY = gridStartY + (int)((float)scrollY / gridTotalH * visibleH);
            DrawRectangle(startX + gridW + 6, gridStartY, 6, visibleH, MakeColor(30, 35, 50, 255));
            DrawRectangle(startX + gridW + 6, barY, 6, barH, ACCENT_DIM);
        }

        int ly = 600 - legendH - bottomBarH + 2;
        DrawLine(40, ly - 2, 760, ly - 2, ACCENT_DIM);

        struct LegendItem { unsigned char r, g, b; const char* label; };
        LegendItem legend[] = {
            {  80,  80, 100, "Silver 8lv"    },
            { 140, 110,  20, "Gold 12lv"     },
            {  80,  20, 120, "Platinum 18lv" },
            {  60,  30,  30, "Booked"        },
            {  50, 180,  80, "Selected"      },
        };

        int lx = 40;
        for (auto& li : legend) {
            DrawRectangle(lx, ly + 4, 14, 14, MakeColor(li.r, li.g, li.b, 255));
            DrawText(li.label, lx + 18, ly + 4, 13, TEXT_MUTED);
            lx += MeasureText(li.label, 13) + 32;
        }

        float total = 0;
        for (int sid : selected) total += SeatTypeToPrice(seats[sid].type);

        string selStr = "Selected: " + to_string((int)selected.size())
            + "  |  Total: " + to_string((int)total) + " BGN";
        DrawText(selStr.c_str(),
            (800 - MeasureText(selStr.c_str(), 15)) / 2, ly + 24, 15, ACCENT_GOLD);

        int bbY = 600 - bottomBarH + 4;
        DrawLine(40, bbY - 4, 760, bbY - 4, ACCENT_DIM);

        bool proceed = DrawButton(80, bbY, 300, 46, "Proceed to Payment",
            BTN_GREEN, BTN_GREEN_H, selected.empty());
        bool back = DrawButton(420, bbY, 300, 46, "Back",
            BTN_RED, BTN_RED_H);

        if (proceed && !selected.empty()) done = true;
        if (back) cancelled = true;

        if (skipFrames > 0) skipFrames--;
        EndDrawing();
    }

    if (done && !selected.empty())
        RunPaymentScreen(show, movie, currentUser, selected, seats, bookings);
}

void RunBookingSelectShow(
    vector<Show>& shows,
    vector<Movie>& movies,
    const User& currentUser)
{
    vector<Booking>& bookings = GetBookings();

    int  scrollOffset = 0;
    bool back = false;
    int  skipFrames = 5;

    const int cardH = 88;
    const int cardGap = 6;
    const int listX = 110;
    const int listW = 580;
    const int listStartY = 90;
    const int listEndY = 548;
    const int visibleH = listEndY - listStartY;

    while (!WindowShouldClose() && !back) {

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
        DrawTopBar("MAKE A BOOKING  -  Select a Movie");

        BeginScissorMode(listX, listStartY, listW, visibleH);

        for (int i = 0; i < (int)movies.size(); i++) {
            int y = listStartY + i * (cardH + cardGap) - scrollOffset;
            if (y + cardH < listStartY || y > listEndY) continue;

            bool hov = mp.x >= listX && mp.x <= listX + listW
                && mp.y >= y && mp.y <= y + cardH;

            Rectangle rec = { (float)listX, (float)y, (float)listW, (float)cardH };
            DrawRectangleRounded(rec, 0.1f, 8,
                hov ? MakeColor(30, 35, 60, 255) : BG_PANEL);
            DrawRectangleRoundedLines(rec, 0.1f, 8,
                hov ? ACCENT_GOLD : ACCENT_DIM);

            DrawText(movies[i].title.c_str(), listX + 14, y + 10, 20, TEXT_PRIMARY);

            string info = GenreToString(movies[i].genre) + "  |  "
                + LanguageToString(movies[i].language) + "  |  "
                + to_string(movies[i].durationMinutes) + " min  |  "
                + DateToString(movies[i].releaseDate);
            DrawText(info.c_str(), listX + 14, y + 36, 14, TEXT_MUTED);
            DrawText(movies[i].description.c_str(), listX + 14, y + 58, 13,
                MakeColor(160, 155, 140, 255));

            if (hov) DrawText(">", listX + listW - 22, y + (cardH - 20) / 2, 20, ACCENT_GOLD);

            if (hov && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && skipFrames <= 0) {
                Show* firstShow = nullptr;
                for (auto& s : shows)
                    if (s.movieId == movies[i].id && s.isActive) { firstShow = &s; break; }

                if (firstShow)
                    RunSeatSelectionScreen(*firstShow, movies[i], currentUser, bookings);
            }
        }

        EndScissorMode();

        int totalH = (int)movies.size() * (cardH + cardGap);
        if (totalH > visibleH) {
            float ratio = (float)visibleH / totalH;
            int   barH = max(20, (int)(visibleH * ratio));
            int   barY = listStartY + (int)((float)scrollOffset / totalH * visibleH);
            DrawRectangle(listX + listW + 4, listStartY, 6, visibleH, MakeColor(30, 35, 50, 255));
            DrawRectangle(listX + listW + 4, barY, 6, barH, ACCENT_DIM);
        }

        DrawLine(listX, listEndY, listX + listW, listEndY, ACCENT_DIM);

        if (DrawButton(310, 554, 180, 38, "Back to Menu", BTN_RED, BTN_RED_H))
            back = true;

        if (skipFrames > 0) skipFrames--;
        EndDrawing();
    }
}

void RunMyBookingsScreen(
    vector<Booking>& bookings,
    vector<Show>& shows,
    vector<Movie>& movies,
    const User& currentUser)
{
    vector<Booking*> mine;
    for (auto& b : bookings)
        if (b.username == currentUser.username) mine.push_back(&b);

    int  scroll = 0;
    bool back = false;
    const int VISIBLE = 6;
    const int ROW_H = 74;
    const int ROW_GAP = 6;

    while (!WindowShouldClose() && !back) {
        BeginDrawing();
        DrawTopBar("MY BOOKINGS");

        if (mine.empty()) {
            DrawText("You have no bookings yet.",
                (800 - MeasureText("You have no bookings yet.", 20)) / 2,
                300, 20, TEXT_MUTED);
        }

        int startY = 90;

        for (int i = scroll; i < min((int)mine.size(), scroll + VISIBLE); i++) {
            Booking* b = mine[i];
            int y = startY + (i - scroll) * (ROW_H + ROW_GAP);

            string movieTitle = "Unknown";
            string showTime = "";
            string hall = "";

            for (auto& s : shows) {
                if (s.id == b->showId) {
                    showTime = ShowTimeToString(s.time);
                    hall = s.hall.name;
                    for (auto& m : movies)
                        if (m.id == s.movieId) { movieTitle = m.title; break; }
                    break;
                }
            }

            Rectangle rec = { 80, (float)y, 640, (float)ROW_H };
            DrawRectangleRounded(rec, 0.12f, 8, BG_PANEL);
            DrawRectangleRoundedLines(rec, 0.12f, 8, ACCENT_DIM);

            DrawText(movieTitle.c_str(), 100, y + 6, 18, ACCENT_GOLD);

            string line2 = showTime + "  |  " + hall
                + "  |  Seats: " + to_string((int)b->seatIds.size());
            DrawText(line2.c_str(), 100, y + 30, 14, TEXT_PRIMARY);

            string price = "Total: " + to_string((int)b->totalPrice) + " BGN  [Pay at counter]";
            DrawText(price.c_str(), 100, y + 52, 13, ACCENT_GOLD);

            string bid = "#" + to_string(b->id);
            DrawText(bid.c_str(), 686, y + 10, 14, TEXT_MUTED);
        }

        if (scroll > 0)
            if (DrawButton(340, 80, 120, 22, "^ Up", BTN_BLUE, BTN_BLUE_H))
                scroll--;

        if (scroll + VISIBLE < (int)mine.size()) {
            int sy = startY + VISIBLE * (ROW_H + ROW_GAP) + 4;
            if (DrawButton(340, sy, 120, 22, "v Down", BTN_BLUE, BTN_BLUE_H))
                scroll++;
        }

        if (DrawButton(310, 554, 180, 38, "Back to Menu", BTN_RED, BTN_RED_H))
            back = true;

        EndDrawing();
    }
}

void RunAllBookingsScreen(
    vector<Booking>& bookings,
    vector<Show>& shows,
    vector<Movie>& movies)
{
    int  scroll = 0;
    bool back = false;
    const int VISIBLE = 6;
    const int ROW_H = 80;
    const int ROW_GAP = 6;

    while (!WindowShouldClose() && !back) {
        BeginDrawing();
        DrawTopBar("ALL BOOKINGS  -  ADMIN VIEW");

        if (bookings.empty()) {
            DrawText("No bookings in the system yet.",
                (800 - MeasureText("No bookings in the system yet.", 20)) / 2,
                300, 20, TEXT_MUTED);
        }

        string countStr = "Total bookings: " + to_string((int)bookings.size());
        DrawText(countStr.c_str(), 110, 84, 14, TEXT_MUTED);

        int startY = 104;

        for (int i = scroll; i < min((int)bookings.size(), scroll + VISIBLE); i++) {
            Booking& b = bookings[i];
            int y = startY + (i - scroll) * (ROW_H + ROW_GAP);

            string movieTitle = "Unknown";
            string showTime = "";
            string hall = "";

            for (auto& s : shows) {
                if (s.id == b.showId) {
                    showTime = ShowTimeToString(s.time);
                    hall = s.hall.name;
                    for (auto& m : movies)
                        if (m.id == s.movieId) { movieTitle = m.title; break; }
                    break;
                }
            }

            Rectangle rec = { 80, (float)y, 640, (float)ROW_H };
            DrawRectangleRounded(rec, 0.12f, 8, BG_PANEL);
            DrawRectangleRoundedLines(rec, 0.12f, 8, ACCENT_DIM);

            string bid = "#" + to_string(b.id);
            DrawText(bid.c_str(), 92, y + 6, 14, ACCENT_GOLD);
            DrawText(b.username.c_str(), 140, y + 6, 16, TEXT_PRIMARY);

            DrawText(movieTitle.c_str(), 92, y + 28, 16, ACCENT_GOLD);

            string line2 = showTime + "  |  " + hall
                + "  |  Seats: " + to_string((int)b.seatIds.size());
            DrawText(line2.c_str(), 92, y + 50, 13, TEXT_MUTED);

            string price = to_string((int)b.totalPrice) + " BGN  [Pay at counter]";
            DrawText(price.c_str(), 580, y + 28, 13, ACCENT_GOLD);
        }

        if (scroll > 0)
            if (DrawButton(340, 88, 120, 20, "^ Up", BTN_BLUE, BTN_BLUE_H))
                scroll--;

        if (scroll + VISIBLE < (int)bookings.size()) {
            int sy = startY + VISIBLE * (ROW_H + ROW_GAP) + 4;
            if (DrawButton(340, sy, 120, 20, "v Down", BTN_BLUE, BTN_BLUE_H))
                scroll++;
        }

        if (DrawButton(310, 554, 180, 38, "Back", BTN_RED, BTN_RED_H))
            back = true;

        EndDrawing();
    }
}

void RunSendNotificationScreen(vector<Movie>& movies) {
    int  selectedType = -1;
    int  selectedMovie = -1;
    bool sent = false;
    bool back = false;
    int  skipFrames = 5;
    int  scrollOffset = 0;

    const char* notifTypes[] = {
        "New Movie Released",
        "Booking Confirmed",
        "Booking Cancelled"
    };

    const int cardH = 56;
    const int cardGap = 6;
    const int listStartY = 280;
    const int listEndY = 500;
    const int visibleH = listEndY - listStartY;

    while (!WindowShouldClose() && !back && !sent) {
        float wheel = GetMouseWheelMove();
        if (wheel != 0 && selectedType == 0) {
            int maxScroll = (int)movies.size() * (cardH + cardGap) - visibleH;
            if (maxScroll < 0) maxScroll = 0;
            scrollOffset -= (int)(wheel * 30);
            if (scrollOffset < 0)          scrollOffset = 0;
            if (scrollOffset > maxScroll)  scrollOffset = maxScroll;
        }

        Vector2 mp = GetMousePosition();

        BeginDrawing();
        DrawTopBar("SEND NOTIFICATION");

        DrawText("Select notification type:", 110, 86, 16, TEXT_MUTED);

        for (int i = 0; i < 3; i++) {
            int y = 110 + i * 52;
            bool hov = IsHovered(110, y, 580, 44);
            bool sel = (selectedType == i);

            Rectangle rec = { 110, (float)y, 580, 44 };
            DrawRectangleRounded(rec, 0.12f, 8,
                sel ? MakeColor(30, 60, 120, 255) :
                hov ? MakeColor(25, 30, 50, 255) : BG_PANEL);
            DrawRectangleRoundedLines(rec, 0.12f, 8,
                sel ? ACCENT_GOLD : ACCENT_DIM);

            DrawText(notifTypes[i], 130, y + 14, 16, sel ? ACCENT_GOLD : TEXT_PRIMARY);

            if (hov && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && skipFrames <= 0) {
                selectedType = i;
                selectedMovie = -1;
                scrollOffset = 0;
            }
        }

        if (selectedType == 0) {
            DrawLine(110, 272, 690, 272, ACCENT_DIM);
            DrawText("Select movie:", 110, 276, 14, TEXT_MUTED);

            BeginScissorMode(110, listStartY, 580, visibleH);

            for (int i = 0; i < (int)movies.size(); i++) {
                int y = listStartY + i * (cardH + cardGap) - scrollOffset;
                if (y + cardH < listStartY || y > listEndY) continue;

                bool hov = mp.x >= 110 && mp.x <= 690 && mp.y >= y && mp.y <= y + cardH;
                bool sel = (selectedMovie == i);

                Rectangle rec = { 110, (float)y, 580, (float)cardH };
                DrawRectangleRounded(rec, 0.1f, 8,
                    sel ? MakeColor(30, 60, 30, 255) :
                    hov ? MakeColor(25, 35, 25, 255) : BG_PANEL);
                DrawRectangleRoundedLines(rec, 0.1f, 8,
                    sel ? BTN_GREEN_H : ACCENT_DIM);

                DrawText(movies[i].title.c_str(), 126, y + 10, 16, TEXT_PRIMARY);
                string info = GenreToString(movies[i].genre) + "  |  " + DateToString(movies[i].releaseDate);
                DrawText(info.c_str(), 126, y + 32, 13, TEXT_MUTED);

                if (hov && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && skipFrames <= 0)
                    selectedMovie = i;
            }

            EndScissorMode();
        }

        bool canSend = (selectedType == 1 || selectedType == 2)
            || (selectedType == 0 && selectedMovie >= 0);

        DrawLine(110, 506, 690, 506, ACCENT_DIM);

        if (canSend) {
            string preview = "";
            if (selectedType == 0 && selectedMovie >= 0)
                preview = "NOTIFICATION: New movie \"" + movies[selectedMovie].title + "\" is now available!";
            else if (selectedType == 1)
                preview = "NOTIFICATION: Your booking has been confirmed!";
            else if (selectedType == 2)
                preview = "NOTIFICATION: Your booking has been cancelled.";

            DrawText(preview.c_str(),
                (800 - MeasureText(preview.c_str(), 13)) / 2, 512, 13,
                MakeColor(100, 200, 120, 255));
        }

        bool sendBtn = DrawButton(110, 538, 280, 44, "Send Notification",
            BTN_GREEN, BTN_GREEN_H, !canSend);
        bool backBtn = DrawButton(410, 538, 280, 44, "Cancel",
            BTN_RED, BTN_RED_H);

        if (sendBtn && canSend && skipFrames <= 0) sent = true;
        if (backBtn && skipFrames <= 0) back = true;

        if (skipFrames > 0) skipFrames--;
        EndDrawing();
    }

    if (sent) {
        float timer = 3.0f;
        while (!WindowShouldClose() && timer > 0) {
            timer -= GetFrameTime();
            BeginDrawing();
            DrawTopBar("NOTIFICATION SENT");
            DrawText("Notification sent successfully!",
                (800 - MeasureText("Notification sent successfully!", 24)) / 2,
                260, 24, BTN_GREEN_H);
            DrawText("All users will see this notification.",
                (800 - MeasureText("All users will see this notification.", 15)) / 2,
                300, 15, TEXT_MUTED);
            string closing = "Returning in " + to_string((int)timer + 1) + "...";
            DrawText(closing.c_str(),
                (800 - MeasureText(closing.c_str(), 13)) / 2, 360, 13, TEXT_MUTED);
            EndDrawing();
        }
    }
}