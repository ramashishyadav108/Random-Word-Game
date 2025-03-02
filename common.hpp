#ifndef COMMON_HPP
#define COMMON_HPP

#include <GL/glut.h>
#include <string>
#include <vector>
#include "database_manager.hpp"
using namespace std;

// Forward declaration of Ball struct
struct Ball;

extern int windowWidth;
extern int windowHeight;
extern bool showLoginForm;
extern bool showSignupForm;
extern string inputUsername;
extern string inputPassword;
extern bool isLoggedIn;
extern string loggedInUsername;
extern DatabaseManager dbManager;
extern vector<Ball> balls;
void init();
void display();
void mouse(int button, int state, int x, int y);

void drawLoginForm();
void drawSignupForm();

#endif // COMMON_HPP
