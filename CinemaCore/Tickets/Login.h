#pragma once
#include <string>
using namespace std;

enum Screen {
    SCREEN_START,
    SCREEN_LOGIN,
    SCREEN_SIGNUP,
    SCREEN_ADMIN_LOGIN,
    SCREEN_MAIN_MENU
};

struct User {
    string username = "";
    string password = "";
    bool isAdmin = false;
    bool isGuest = false;
    bool isLogged = false;
};

void handleTextInput(string& text, int maxLength);
bool checkAdminCredentials(const string& user, const string& pass);
User runCinemaAuthSystem();