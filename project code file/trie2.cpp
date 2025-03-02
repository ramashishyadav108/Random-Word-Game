#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <fstream>
#include <locale>
#include <codecvt>
#include <unordered_set>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "common.hpp"
#include "database_manager.hpp"
using namespace std;
DatabaseManager dbManager;

void initGame();

int restartButtonX, restartButtonY;
int quitButtonX, quitButtonY;

float backspaceIconX, backspaceIconY;
const float backspaceIconSize = 60.0f;

// Window dimensions
int windowWidth = 800;
int windowHeight = 600;
const float ballRadius = 50.0f;
float ballSpeed = 2.0f;
const int buttonWidth = 300;
const int buttonHeight = 75;
int menu = 1;
int startLogIn = 0;
int gamestate = 0;

bool showLoginForm = false;
bool showSignupForm = false;
string inputUsername = "";
string inputPassword = "";
bool isLoggedIn = false;
string loggedInUsername = "";

// New button positions
float loginButtonX, loginButtonY;
float signupButtonX, signupButtonY;
const float authButtonWidth = 200;
const float authButtonHeight = 50;
int high_score = 0;
map<unsigned char, wchar_t> keyboardMap;
void initKeyboardMapHindi()
{
    // Map ASCII characters to Hindi Unicode characters
    keyboardMap['a'] = L'अ';
    keyboardMap['A'] = L'आ';
    keyboardMap['i'] = L'इ';
    keyboardMap['I'] = L'ई';
    keyboardMap['u'] = L'उ';
    keyboardMap['U'] = L'ऊ';
    keyboardMap['e'] = L'ए';
    keyboardMap['E'] = L'ऐ';
    keyboardMap['o'] = L'ओ';
    keyboardMap['O'] = L'औ';

    keyboardMap['k'] = L'क';
    keyboardMap['K'] = L'ख';
    keyboardMap['g'] = L'ग';
    keyboardMap['G'] = L'घ';
    keyboardMap['c'] = L'च';
    keyboardMap['C'] = L'छ';
    keyboardMap['j'] = L'ज';
    keyboardMap['J'] = L'झ';
    keyboardMap['T'] = L'ट';
    keyboardMap[';'] = L'ठ';
    keyboardMap['D'] = L'ड';
    keyboardMap[':'] = L'ढ';
    keyboardMap['N'] = L'ण';
    keyboardMap['t'] = L'त';
    keyboardMap['\\'] = L'थ';
    keyboardMap['d'] = L'द';
    keyboardMap['|'] = L'ध';
    keyboardMap['n'] = L'न';
    keyboardMap['p'] = L'प';
    keyboardMap['P'] = L'फ';
    keyboardMap['b'] = L'ब';
    keyboardMap['B'] = L'भ';
    keyboardMap['m'] = L'म';
    keyboardMap['y'] = L'य';
    keyboardMap['r'] = L'र';
    keyboardMap['l'] = L'ल';
    keyboardMap['v'] = L'व';
    keyboardMap['S'] = L'श';
    keyboardMap['s'] = L'स';
    keyboardMap['h'] = L'ह';

    // Map vowel matras
    keyboardMap['1'] = L'ि'; // ि (Short i)
    keyboardMap['2'] = L'ी'; // ी (Long i)
    keyboardMap['3'] = L'ु';  // ु (Short u)
    keyboardMap['4'] = L'ू';  // ू (Long u)
    keyboardMap['5'] = L'ृ';  // ृ (R with a matra)
    keyboardMap['6'] = L'े';  // े (Short e)
    keyboardMap['7'] = L'ै';  // ै (Long e)
    keyboardMap['-'] = L'ो'; // ो (Short o)
    keyboardMap['9'] = L'ौ'; // ौ (Long o)
    keyboardMap['0'] = L'ा';

    // Add more mappings as needed
}
void initKeyboardMapEnglish()
{
    // Map ASCII characters to English letters
    keyboardMap['a'] = L'a';
    keyboardMap['A'] = L'A';
    keyboardMap['b'] = L'b';
    keyboardMap['B'] = L'B';
    keyboardMap['c'] = L'c';
    keyboardMap['C'] = L'C';
    keyboardMap['d'] = L'd';
    keyboardMap['D'] = L'D';
    keyboardMap['e'] = L'e';
    keyboardMap['E'] = L'E';
    keyboardMap['f'] = L'f';
    keyboardMap['F'] = L'F';
    keyboardMap['g'] = L'g';
    keyboardMap['G'] = L'G';
    keyboardMap['h'] = L'h';
    keyboardMap['H'] = L'H';
    keyboardMap['i'] = L'i';
    keyboardMap['I'] = L'I';
    keyboardMap['j'] = L'j';
    keyboardMap['J'] = L'J';
    keyboardMap['k'] = L'k';
    keyboardMap['K'] = L'K';
    keyboardMap['l'] = L'l';
    keyboardMap['L'] = L'L';
    keyboardMap['m'] = L'm';
    keyboardMap['M'] = L'M';
    keyboardMap['n'] = L'n';
    keyboardMap['N'] = L'N';
    keyboardMap['o'] = L'o';
    keyboardMap['O'] = L'O';
    keyboardMap['p'] = L'p';
    keyboardMap['P'] = L'P';
    keyboardMap['q'] = L'q';
    keyboardMap['Q'] = L'Q';
    keyboardMap['r'] = L'r';
    keyboardMap['R'] = L'R';
    keyboardMap['s'] = L's';
    keyboardMap['S'] = L'S';
    keyboardMap['t'] = L't';
    keyboardMap['T'] = L'T';
    keyboardMap['u'] = L'u';
    keyboardMap['U'] = L'U';
    keyboardMap['v'] = L'v';
    keyboardMap['V'] = L'V';
    keyboardMap['w'] = L'w';
    keyboardMap['W'] = L'W';
    keyboardMap['x'] = L'x';
    keyboardMap['X'] = L'X';
    keyboardMap['y'] = L'y';
    keyboardMap['Y'] = L'Y';
    keyboardMap['z'] = L'z';
    keyboardMap['Z'] = L'Z';
}

float currentBallSpeed = 2.0f; // Initial ball speed
const float speedIncreaseFactor = 1.2f;

int levelTime[] = {50, 45, 40, 35, 30}; // Time for each level

class TrieNode
{
public:
    unordered_map<wchar_t, unique_ptr<TrieNode>> children;
    bool isEndOfWord;

    TrieNode() : isEndOfWord(false) {}
};

class LanguageTrie
{
private:
    unique_ptr<TrieNode> root;

public:
    LanguageTrie() : root(make_unique<TrieNode>()) {}

    void insert(const wstring &word)
    {
        TrieNode *current = root.get();
        for (wchar_t ch : word)
        {
            if (current->children.find(ch) == current->children.end())
            {
                current->children[ch] = make_unique<TrieNode>();
            }
            current = current->children[ch].get();
        }
        current->isEndOfWord = true;
    }

    bool search(const wstring &word) const
    {
        const TrieNode *node = findNode(word);
        return (node != nullptr && node->isEndOfWord);
    }

    bool startsWith(const wstring &prefix) const
    {
        return (findNode(prefix) != nullptr);
    }

private:
    const TrieNode *findNode(const wstring &prefix) const
    {
        const TrieNode *current = root.get();
        for (wchar_t ch : prefix)
        {
            auto it = current->children.find(ch);
            if (it == current->children.end())
            {
                return nullptr;
            }
            current = it->second.get();
        }
        return current;
    }
};

// Global instance of LanguageTrie
LanguageTrie hindiWordTrie;
LanguageTrie englishWordTrie;
LanguageTrie bhojpuriWordTrie;

enum class InputField
{
    None,
    Username,
    Password,
    ConfirmPassword
};

InputField inputFocus = InputField::None;

const wstring hindiLetters[] = {
    L"अ", L"आ", L"इ", L"ई", L"उ", L"ऊ", L"ऋ", L"ए", L"ऐ", L"ओ", L"औ",
    L"क", L"ख", L"ग", L"घ", L"ङ",
    L"च", L"छ", L"ज", L"झ", L"ञ",
    L"ट", L"ठ", L"ड", L"ढ", L"ण",
    L"त", L"थ", L"द", L"ध", L"न",
    L"प", L"फ", L"ब", L"भ", L"म",
    L"य", L"र", L"ल", L"व",
    L"श", L"ष", L"स", L"ह",
    L"ी", L"ु", L"ू",
    L"ृ", L"े", L"ै", L"ो",
    L"ौ", L"ा"};

const wstring englishLetters[] = {
    L"a", L"b", L"c", L"d", L"e", L"f", L"g", L"h", L"i", L"j", L"k", L"l", L"m",
    L"n", L"o", L"p", L"q", L"r", L"s", L"t", L"u", L"v", L"w", L"x", L"y", L"z"};

const int numHindiLetters = sizeof(hindiLetters) / sizeof(hindiLetters[0]);

const int numEnglishLetters = sizeof(englishLetters) / sizeof(englishLetters[0]);

int hindiGame = 0;
int bhojpuriGame = 0;
int englishGame = 0;

struct Ball
{
    float x, y;
    float dx, dy;
    wstring letter;
    bool clicked;
    float color[3];
};

int remainingTime = 50; // Start with 50 seconds for the first level
int startTime = 0;
bool fail = false;
bool gameOver = false;
bool playon = true;
float homeIconX;
float homeIconY;
const float homeIconSize = 60.0f;

int HindiButtonX, HindiButtonY;
int BhojpuriButtonX, BhojpuriButtonY;
int englishButtonX, englishButtonY;

vector<Ball> balls;
wstring userInput;
int score = 0;

bool notfail = true;
int currentLevel = 1;                           // Start at level 1
const int ballsPerLevel[] = {18, 15, 12, 9, 6}; // Number of balls for each level
int maxLevels = sizeof(ballsPerLevel) / sizeof(ballsPerLevel[0]);
string errorMessage = "";
string inputConfirmPassword = "";

// FreeType variables
FT_Library ft;
FT_Face face;

inline string trim(const string &s)
{
    string result = s;
    result.erase(result.begin(), find_if(result.begin(), result.end(), [](unsigned char ch)
                                         { return !isspace(ch); }));
    return result;
}

void updateTimer(int value)
{
    if (playon && !fail && !gameOver)
    {
        int currentTime = glutGet(GLUT_ELAPSED_TIME);
        int elapsedSeconds = (currentTime - startTime) / 1000;

        if (elapsedSeconds >= 1) // Check if at least 1 second has passed
        {
            remainingTime--;
            startTime = currentTime; // Reset the start time

            if (remainingTime <= 0)
            {
                remainingTime = 0;
                fail = true;
                notfail = false;
                playon = false;
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(100, updateTimer, 0); // Call every 100ms for smoother updates
}
void drawHomeIcon()
{
    // Dark gray color for the icon background
    glColor3f(0.2f, 0.2f, 0.2f);

    // Draw the main square
    glBegin(GL_QUADS);
    glVertex2f(homeIconX, homeIconY);
    glVertex2f(homeIconX + homeIconSize, homeIconY);
    glVertex2f(homeIconX + homeIconSize, homeIconY + homeIconSize);
    glVertex2f(homeIconX, homeIconY + homeIconSize);
    glEnd();

    // Light gray color for the house symbol
    glColor3f(0.8f, 0.8f, 0.8f);
    glLineWidth(3.0f);

    // Draw the roof of the house
    glBegin(GL_TRIANGLES);
    glVertex2f(homeIconX + homeIconSize * 0.2f, homeIconY + homeIconSize * 0.6f);
    glVertex2f(homeIconX + homeIconSize * 0.5f, homeIconY + homeIconSize * 0.9f);
    glVertex2f(homeIconX + homeIconSize * 0.8f, homeIconY + homeIconSize * 0.6f);
    glEnd();

    // Draw the body of the house
    glBegin(GL_QUADS);
    glVertex2f(homeIconX + homeIconSize * 0.3f, homeIconY + homeIconSize * 0.6f);
    glVertex2f(homeIconX + homeIconSize * 0.7f, homeIconY + homeIconSize * 0.6f);
    glVertex2f(homeIconX + homeIconSize * 0.7f, homeIconY + homeIconSize * 0.2f);
    glVertex2f(homeIconX + homeIconSize * 0.3f, homeIconY + homeIconSize * 0.2f);
    glEnd();

    glLineWidth(1.0f); // Reset line width
}
void drawRectangle(float x1, float y1, float x2, float y2, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}
float renderText(float x, float y, const wstring &text, float scale, bool measureOnly)
{
    float totalAdvance = 0;
    FT_GlyphSlot slot = face->glyph;

    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1.0f);

    for (const wchar_t &wc : text)
    {
        if (FT_Load_Char(face, wc, FT_LOAD_RENDER))
        {
            wcerr << "Could not load character " << wc << endl;
            continue;
        }

        if (!measureOnly)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_ALPHA,
                slot->bitmap.width,
                slot->bitmap.rows,
                0,
                GL_ALPHA,
                GL_UNSIGNED_BYTE,
                slot->bitmap.buffer);

            float x2 = totalAdvance + slot->bitmap_left;
            float y2 = -slot->bitmap_top;
            float w = slot->bitmap.width;
            float h = slot->bitmap.rows;

            glEnable(GL_TEXTURE_2D);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex2f(x2, -y2);
            glTexCoord2f(1, 0);
            glVertex2f(x2 + w, -y2);
            glTexCoord2f(1, 1);
            glVertex2f(x2 + w, -y2 - h);
            glTexCoord2f(0, 1);
            glVertex2f(x2, -y2 - h);
            glEnd();

            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
        }

        totalAdvance += (slot->advance.x >> 6);
    }

    glPopMatrix();

    return totalAdvance * scale;
}

void drawLoginForm()
{
    int formWidth = windowWidth / 3;
    int formHeight = windowHeight / 3;
    int baseX = (windowWidth - formWidth) / 2;
    int baseY = (windowHeight - formHeight) / 2;

    // Draw login form background
    drawRectangle(baseX, baseY, baseX + formWidth, baseY + formHeight, 0.2f, 0.2f, 0.2f);

    // Draw input fields and labels
    glColor3f(1.0f, 1.0f, 1.0f);
    renderText(baseX + 40, baseY + formHeight - 60, L"Username:", 1.0f, false);
    renderText(baseX + 40, baseY + formHeight - 140, L"Password:", 1.0f, false);

    drawRectangle(baseX + 40, baseY + formHeight - 100, baseX + formWidth - 40, baseY + formHeight - 65, 1.0f, 1.0f, 1.0f);
    drawRectangle(baseX + 40, baseY + formHeight - 180, baseX + formWidth - 40, baseY + formHeight - 145, 1.0f, 1.0f, 1.0f);

    glColor3f(0.0f, 0.0f, 0.0f);
    renderText(baseX + 45, baseY + formHeight - 95, wstring(inputUsername.begin(), inputUsername.end()), 1.0f, false);
    renderText(baseX + 45, baseY + formHeight - 175, wstring(inputPassword.length(), '*'), 1.0f, false);

    // Draw login button
    drawRectangle(baseX + formWidth / 2 - 75, baseY + 40, baseX + formWidth / 2 + 75, baseY + 90, 0.0f, 0.7f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    renderText(baseX + formWidth / 2 - 65, baseY + 55, L"Log In", 1.0f, false);

    // Display error message if any
    if (!errorMessage.empty())
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        renderText(baseX, baseY - 100, wstring(errorMessage.begin(), errorMessage.end()), 1.0f, false);
    }
}
void drawSignupForm()
{
    int formWidth = windowWidth / 3;
    int formHeight = windowHeight / 2.5;
    int baseX = (windowWidth - formWidth) / 2;
    int baseY = (windowHeight - formHeight) / 2;

    // Draw signup form background
    drawRectangle(baseX, baseY, baseX + formWidth, baseY + formHeight, 0.2f, 0.2f, 0.2f);

    // Draw input fields and labels
    glColor3f(1.0f, 1.0f, 1.0f);
    renderText(baseX + 40, baseY + formHeight - 60, L"Username:", 1.0f, false);
    renderText(baseX + 40, baseY + formHeight - 140, L"Password:", 1.0f, false);
    renderText(baseX + 40, baseY + formHeight - 220, L"Confirm Password:", 1.0f, false);

    drawRectangle(baseX + 40, baseY + formHeight - 100, baseX + formWidth - 40, baseY + formHeight - 65, 1.0f, 1.0f, 1.0f);
    drawRectangle(baseX + 40, baseY + formHeight - 180, baseX + formWidth - 40, baseY + formHeight - 145, 1.0f, 1.0f, 1.0f);
    drawRectangle(baseX + 40, baseY + formHeight - 260, baseX + formWidth - 40, baseY + formHeight - 225, 1.0f, 1.0f, 1.0f);

    glColor3f(0.0f, 0.0f, 0.0f);
    renderText(baseX + 45, baseY + formHeight - 95, wstring(inputUsername.begin(), inputUsername.end()), 1.0f, false);
    renderText(baseX + 45, baseY + formHeight - 175, wstring(inputPassword.length(), '*'), 1.0f, false);
    renderText(baseX + 45, baseY + formHeight - 255, wstring(inputConfirmPassword.length(), '*'), 1.0f, false);

    // Draw signup button
    drawRectangle(baseX + formWidth / 2 - 85, baseY + 40, baseX + formWidth / 2 + 85, baseY + 90, 0.0f, 0.0f, 0.7f);
    glColor3f(1.0f, 1.0f, 1.0f);
    renderText(baseX + formWidth / 2 - 83, baseY + 55, L"Sign Up", 1.0f, false);

    // Display error message if any
    if (!errorMessage.empty())
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        renderText(baseX + 75, baseY - 100, wstring(errorMessage.begin(), errorMessage.end()), 1.0f, false);
    }
}

void drawBackspaceIcon()
{
    // Dark gray color for the icon
    glColor3f(0.2f, 0.2f, 0.2f);

    // Draw the main rectangle
    glBegin(GL_QUADS);
    glVertex2f(backspaceIconX, backspaceIconY);
    glVertex2f(backspaceIconX + backspaceIconSize, backspaceIconY);
    glVertex2f(backspaceIconX + backspaceIconSize, backspaceIconY + backspaceIconSize);
    glVertex2f(backspaceIconX, backspaceIconY + backspaceIconSize);
    glEnd();

    // Draw the "<-" symbol
    glColor3f(0.8f, 0.8f, 0.8f); // Light gray for contrast
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    // Horizontal line
    glVertex2f(backspaceIconX + backspaceIconSize * 0.2f, backspaceIconY + backspaceIconSize * 0.5f);
    glVertex2f(backspaceIconX + backspaceIconSize * 0.8f, backspaceIconY + backspaceIconSize * 0.5f);
    // Left arrow part
    glVertex2f(backspaceIconX + backspaceIconSize * 0.4f, backspaceIconY + backspaceIconSize * 0.3f);
    glVertex2f(backspaceIconX + backspaceIconSize * 0.2f, backspaceIconY + backspaceIconSize * 0.5f);
    glVertex2f(backspaceIconX + backspaceIconSize * 0.4f, backspaceIconY + backspaceIconSize * 0.7f);
    glVertex2f(backspaceIconX + backspaceIconSize * 0.2f, backspaceIconY + backspaceIconSize * 0.5f);
    glEnd();
    glLineWidth(1.0f); // Reset line width
}

void drawCircle(const Ball &ball)
{

    glColor3fv(ball.color);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(ball.x, ball.y);
    for (int i = 0; i <= 360; i += 10)
    {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(ball.x + cos(angle) * ballRadius, ball.y + sin(angle) * ballRadius);
    }
    glEnd();

    // Draw the letter on the circle
    glColor3f(0.0, 0.0, 0.0); // Set text color to black
    float textScale = 1.0f;   // Adjust this value to change text size
    float textWidth = renderText(0, 0, ball.letter, textScale, true);

    float textX = ball.x - textWidth / 2;

    // Get the current glyph height and adjust Y position for better centering
    float glyphHeight = (face->size->metrics.height >> 6) * textScale;
    float textY = ball.y - glyphHeight / 4; // Adjust the factor as needed for centering

    renderText(textX, textY, ball.letter, textScale, false);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (menu)
    {
        glClearColor(1.0, 0.75, 0.8, 0.0); // pink color
        drawRectangle(loginButtonX, loginButtonY, loginButtonX + authButtonWidth, loginButtonY + authButtonHeight, 0.5f, 0.5f, 0.5f);
        glColor3f(0.0f, 0.0f, 0.0f);
        renderText(loginButtonX + 50, loginButtonY + 10, L"LogIn", 1.0f, false);

        drawRectangle(signupButtonX, signupButtonY, signupButtonX + authButtonWidth, signupButtonY + authButtonHeight, 0.5f, 0.5f, 0.5f);
        glColor3f(0.0f, 0.0f, 0.0f);
        renderText(signupButtonX + 25, signupButtonY + 10, L"SignUp", 1.0f, false);
    }

    if (menu && (!startLogIn))
    {
        glColor3f(1.0, 0.0, 0.0); // Red color
        renderText(windowWidth / 2.0 - 125, windowHeight / 2.0 + 310, L"Word Game", 1.0f, false);

        drawRectangle(HindiButtonX, HindiButtonY, HindiButtonX + buttonWidth, HindiButtonY + buttonHeight, 0.0f, 1.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 0.0f);
        renderText(HindiButtonX + buttonWidth * 0.5 - 65, HindiButtonY + buttonHeight * 0.5 - 12, L"Hindi", 1.0f, false);

        drawRectangle(BhojpuriButtonX, BhojpuriButtonY, BhojpuriButtonX + buttonWidth, BhojpuriButtonY + buttonHeight, 0.0f, 1.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 0.0f);
        renderText(BhojpuriButtonX + buttonWidth * 0.5 - 90, BhojpuriButtonY + buttonHeight * 0.5 - 12, L"Bhojpuri", 1.0f, false);

        drawRectangle(englishButtonX, englishButtonY, englishButtonX + buttonWidth, englishButtonY + buttonHeight, 0.0f, 1.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 0.0f);
        renderText(englishButtonX + buttonWidth * 0.5 - 95, englishButtonY + buttonHeight * 0.5 - 12, L"English", 1.0f, false);
    }
    if (menu && (!isLoggedIn) && (!startLogIn))
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        renderText(windowWidth / 2 - 250, windowHeight / 2 + 170, L" Welcome to Home Page ", 1.0f, false);

        glColor3f(0.0f, 0.0f, 0.0f);
        renderText(windowWidth / 2.0 - 350, windowHeight / 2.0 + 75, L"First LogIn or SignUp to play Game", 1.0f, false);
    }
    if (menu && isLoggedIn)
    {
        // Display logged in username
        glColor3f(0.0f, 0.0f, 0.0f);
        renderText(windowWidth / 2 - 300, windowHeight / 2 + 170, L"Hi " + wstring(loggedInUsername.begin(), loggedInUsername.end()) + L"  , Welcome to Home Page ", 1.0f, false);

        glColor3f(0.0f, 0.0f, 0.0f);
        renderText(windowWidth / 2.0 - 450, windowHeight / 2.0 + 75, L"Select your favourite language to play game", 1.0f, false);
    }

    if (menu && startLogIn)
    {
        // Display login form
        if (showLoginForm)
        {
            drawLoginForm();
        }

        // Display signup form
        if (showSignupForm)
        {
            drawSignupForm();
        }
        drawHomeIcon();
    }
    // Display score
    if (hindiGame)
    {
        if ((!menu) && notfail)
        {
            if (gamestate)
            {
                glClearColor(0.53, 0.81, 0.92, 1.0);
                glColor3f(1.0, 0.0, 0.0);
                renderText(windowWidth / 2.0 - 150, windowHeight / 2.0, L" स्तर  " + to_wstring(currentLevel) + L" खेलें", 1.0f, false);
            }
            else
            {
                glClearColor(0.53, 0.81, 0.92, 1.0);
                for (const Ball &ball : balls)
                {
                    drawCircle(ball);
                }

                glColor3f(1.0, 1.0, 1.0);
                renderText(10, windowHeight - 75, L"बनाया जा रहा है : " + userInput, 1.0f, false);

                renderText(10, windowHeight - 35, L"स्कोर: " + to_wstring(score), 1.0f, false);

                renderText(10, windowHeight - 115, L"स्तर: " + to_wstring(currentLevel), 1.0f, false);
                renderText(10, windowHeight - 155, L"समय शेष: " + to_wstring(remainingTime), 1.0f, false);

                drawBackspaceIcon();
            }
        }
        else if ((!menu) && (!notfail))
        {
            glClearColor(0.53, 0.81, 0.92, 1.0);
            glColor3f(1.0, 0.0, 0.0);
            renderText(windowWidth / 2.0 - 150, windowHeight / 2.0 + 350, L"गेम ओवर! स्कोर: " + to_wstring(score), 1.0f, false);

            drawRectangle(restartButtonX, restartButtonY, restartButtonX + buttonWidth, restartButtonY + buttonHeight, 0.0f, 1.0f, 0.0f);
            glColor3f(0.0f, 0.0f, 0.0f);
            renderText(restartButtonX + buttonWidth * 0.5 - 115, restartButtonY + buttonHeight * 0.5 - 12, L"पुनः आरंभ करें", 1.0f, false);
            drawRectangle(quitButtonX, quitButtonY, quitButtonX + buttonWidth, quitButtonY + buttonHeight, 1.0f, 0.0f, 0.0f);
            glColor3f(0.0f, 0.0f, 0.0f);
            renderText(quitButtonX + buttonWidth * 0.5 - 55, quitButtonY + buttonHeight * 0.5 - 12, L"बंद करें", 1.0f, false);

            glColor3f(1.0, 1.0, 0.0);
            high_score = dbManager.getHighScore(loggedInUsername, 0);
            if (score > high_score)
            {
                high_score = score;
                dbManager.updateHighScore(loggedInUsername, high_score, 0);
                renderText(windowWidth / 2.0 - 150, windowHeight / 2.0 + 210, L"नया उच्च स्कोर: " + to_wstring(high_score), 1.0f, false);
            }
            else
            {
                renderText(windowWidth / 2.0 - 100, windowHeight / 2.0 + 210, L"उच्च स्कोर: " + to_wstring(high_score), 1.0f, false);
            }

            drawHomeIcon();
        }
    }
    else if (bhojpuriGame)
    {
        if ((!menu) && notfail)
        {
            if (gamestate)
            {
                glClearColor(0.53, 0.81, 0.92, 1.0);
                glColor3f(1.0, 0.0, 0.0);
                renderText(windowWidth / 2.0 - 150, windowHeight / 2.0, L" लेवल  " + to_wstring(currentLevel) + L" खेलीं", 1.0f, false);
            }
            else
            {
                glClearColor(0.53, 0.81, 0.92, 1.0);
                for (const Ball &ball : balls)
                {
                    drawCircle(ball);
                }

                glColor3f(1.0, 1.0, 1.0);
                renderText(10, windowHeight - 75, L"टाइप करत बानी : " + userInput, 1.0f, false);

                renderText(10, windowHeight - 35, L"स्कोर: " + to_wstring(score), 1.0f, false);

                renderText(10, windowHeight - 115, L"लेवल: " + to_wstring(currentLevel), 1.0f, false);
                renderText(10, windowHeight - 155, L"बाकी समय: " + to_wstring(remainingTime), 1.0f, false);

                drawBackspaceIcon();
            }
        }
        else if ((!menu) && (!notfail))
        {
            glClearColor(0.53, 0.81, 0.92, 1.0);
            glColor3f(1.0, 0.0, 0.0);
            renderText(windowWidth / 2.0 - 150, windowHeight / 2.0 + 350, L"खेल खतम! स्कोर: " + to_wstring(score), 1.0f, false);

            drawRectangle(restartButtonX, restartButtonY, restartButtonX + buttonWidth, restartButtonY + buttonHeight, 0.0f, 1.0f, 0.0f);
            glColor3f(0.0f, 0.0f, 0.0f);
            renderText(restartButtonX + buttonWidth * 0.5 - 115, restartButtonY + buttonHeight * 0.5 - 12, L"फेर से खेली", 1.0f, false);
            drawRectangle(quitButtonX, quitButtonY, quitButtonX + buttonWidth, quitButtonY + buttonHeight, 1.0f, 0.0f, 0.0f);
            glColor3f(0.0f, 0.0f, 0.0f);
            renderText(quitButtonX + buttonWidth * 0.5 - 55, quitButtonY + buttonHeight * 0.5 - 12, L"बंद करीं", 1.0f, false);

            glColor3f(1.0, 1.0, 0.0);
            high_score = dbManager.getHighScore(loggedInUsername, 2);
            if (score > high_score)
            {
                high_score = score;
                dbManager.updateHighScore(loggedInUsername, high_score, 2);
                renderText(windowWidth / 2.0 - 150, windowHeight / 2.0 + 210, L"नया टॉप स्कोर: " + to_wstring(high_score), 1.0f, false);
            }
            else
            {
                renderText(windowWidth / 2.0 - 100, windowHeight / 2.0 + 210, L"टॉप स्कोर: " + to_wstring(high_score), 1.0f, false);
            }

            drawHomeIcon();
        }
    }
    else if (englishGame)
    {
        if ((!menu) && notfail)
        {
            if (gamestate)
            {
                glClearColor(0.53, 0.81, 0.92, 1.0); // Sky blue background

                glColor3f(1.0, 0.0, 0.0); // Red color
                renderText(windowWidth / 2.0 - 200, windowHeight / 2.0, L"Click to play " + to_wstring(currentLevel) + L" level", 1.0f, false);
            }
            else
            {
                glClearColor(0.53, 0.81, 0.92, 1.0); // Sky blue background

                for (const Ball &ball : balls)
                {
                    drawCircle(ball);
                }

                // Display forming word
                glColor3f(1.0, 1.0, 1.0);
                renderText(10, windowHeight - 75, L"Forming: " + userInput, 1.0f, false);

                renderText(10, windowHeight - 35, L"Score: " + to_wstring(score), 1.0f, false);

                renderText(10, windowHeight - 115, L"Level: " + to_wstring(currentLevel), 1.0f, false); // Display current level
                renderText(10, windowHeight - 155, L"Time remaining: " + to_wstring(remainingTime), 1.0f, false);

                drawBackspaceIcon();
            }
        }
        else if ((!menu) && (!notfail))
        {
            glClearColor(0.53, 0.81, 0.92, 1.0); // Sky blue background

            glColor3f(1.0, 0.0, 0.0); // Red color
            renderText(windowWidth / 2.0 - 150, windowHeight / 2.0 + 350, L"Game Over! Score: " + to_wstring(score), 1.0f, false);

            drawRectangle(restartButtonX, restartButtonY, restartButtonX + buttonWidth, restartButtonY + buttonHeight, 0.0f, 1.0f, 0.0f); // Green button
            glColor3f(0.0f, 0.0f, 0.0f);
            renderText(restartButtonX + buttonWidth * 0.5 - 115, restartButtonY + buttonHeight * 0.5 - 12, L"Restart", 1.0f, false);

            drawRectangle(quitButtonX, quitButtonY, quitButtonX + buttonWidth, quitButtonY + buttonHeight, 1.0f, 0.0f, 0.0f); // Red button
            glColor3f(0.0f, 0.0f, 0.0f);
            renderText(quitButtonX + buttonWidth * 0.5 - 55, quitButtonY + buttonHeight * 0.5 - 12, L"Quit", 1.0f, false);

            glColor3f(1.0, 1.0, 0.0); // Yellow color for high score
            high_score = dbManager.getHighScore(loggedInUsername, 1);

            if (score > high_score)
            {
                high_score = score;
                dbManager.updateHighScore(loggedInUsername, high_score, 1);
                renderText(windowWidth / 2.0 - 150, windowHeight / 2.0 + 210, L"New High Score: " + to_wstring(high_score), 1.0f, false);
            }
            else
            {
                renderText(windowWidth / 2.0 - 100, windowHeight / 2.0 + 210, L"High Score: " + to_wstring(high_score), 1.0f, false);
            }

            drawHomeIcon();
        }
    }

    glutSwapBuffers();
}

void timer(int value)
{
    // Update ball positions
    for (size_t i = 0; i < balls.size(); ++i)
    {
        balls[i].x += balls[i].dx;
        balls[i].y += balls[i].dy;

        // Bounce off the edges
        if (balls[i].x <= ballRadius || balls[i].x >= windowWidth - ballRadius)
        {
            balls[i].dx = -balls[i].dx;
            balls[i].x = max(ballRadius, min(balls[i].x, windowWidth - ballRadius));
        }
        if (balls[i].y <= ballRadius || balls[i].y >= windowHeight - ballRadius)
        {
            balls[i].dy = -balls[i].dy;
            balls[i].y = max(ballRadius, min(balls[i].y, windowHeight - ballRadius));
        }

        // Check for collisions with other balls
        for (size_t j = i + 1; j < balls.size(); ++j)
        {
            float dx = balls[j].x - balls[i].x;
            float dy = balls[j].y - balls[i].y;
            float distanceSquared = dx * dx + dy * dy;

            if (distanceSquared <= 4 * ballRadius * ballRadius)
            {
                // Collision detected, resolve it
                float distance = sqrt(distanceSquared);

                // Normal vector
                float nx = dx / distance;
                float ny = dy / distance;

                // Relative velocity
                float dvx = balls[j].dx - balls[i].dx;
                float dvy = balls[j].dy - balls[i].dy;

                // Relative velocity along the normal
                float velAlongNormal = dvx * nx + dvy * ny;

                // Do not resolve if velocities are separating
                if (velAlongNormal > 0)
                    continue;

                // Collision impulse
                float impulse = -2 * velAlongNormal;

                // Apply impulse to the ball velocities
                balls[i].dx -= impulse * nx;
                balls[i].dy -= impulse * ny;
                balls[j].dx += impulse * nx;
                balls[j].dy += impulse * ny;

                // Normalize velocities to maintain constant speed
                for (Ball *ball : {&balls[i], &balls[j]})
                {
                    float speed = sqrt(ball->dx * ball->dx + ball->dy * ball->dy);
                    if (speed != 0)
                    {
                        ball->dx = (ball->dx / speed) * ballSpeed;
                        ball->dy = (ball->dy / speed) * ballSpeed;
                    }
                }

                // Move balls apart to prevent sticking
                float overlap = 2 * ballRadius - distance;
                balls[i].x -= overlap * 0.5f * nx;
                balls[i].y -= overlap * 0.5f * ny;
                balls[j].x += overlap * 0.5f * nx;
                balls[j].y += overlap * 0.5f * ny;
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}
void restartGame()
{

    score = 0;
    currentLevel = 1;
    notfail = true;
    ballSpeed = 2.0f;
    remainingTime = 50;
    userInput.clear();
    initGame(); // This function will create new balls with random letters
}
void handleInput(wchar_t input, int ballIndex)
{
    if (input == L'\b')
    { // ASCII code for backspace
        if (!userInput.empty())
        {
            wchar_t lastChar = userInput.back();
            userInput.pop_back();
            // Find the last clicked ball with this letter and unclick it
            for (int i = balls.size() - 1; i >= 0; --i)
            {
                if (balls[i].letter[0] == lastChar && balls[i].clicked)
                {
                    balls[i].clicked = false;
                    balls[i].color[0] = static_cast<float>(rand()) / RAND_MAX;
                    balls[i].color[1] = static_cast<float>(rand()) / RAND_MAX;
                    balls[i].color[2] = static_cast<float>(rand()) / RAND_MAX;
                    break;
                }
            }
        }
    }
    else if (ballIndex != -1 && ballIndex < int(balls.size()))
    {
        Ball &ball = balls[ballIndex];
        if (!ball.clicked)
        {
            userInput += ball.letter;
            ball.clicked = true;
            ball.color[0] = 1.0f; // Change color to red
            ball.color[1] = 0.0f;
            ball.color[2] = 0.0f;
        }
    }
    glutPostRedisplay();
}
void loadHindiValidWords()
{
    ifstream file("hindi_words.txt");
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    string word;
    while (file >> word)
    {
        hindiWordTrie.insert(converter.from_bytes(word));
    }
}
void loadBhojpuriValidWords()
{
    ifstream file("bhojpuri_words.txt");
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    string word;
    while (file >> word)
    {
        bhojpuriWordTrie.insert(converter.from_bytes(word));
    }
}
void loadEnglishValidWords()
{
    ifstream file("/usr/share/dict/words"); // Open the system word list file
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    string word;
    while (file >> word)
    {
        englishWordTrie.insert(converter.from_bytes(word));
    }
}
void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        float mouseX = static_cast<float>(x);
        float mouseY = windowHeight - static_cast<float>(y);
        if (showLoginForm)
        {
            int formWidth = windowWidth / 3;
            int formHeight = windowHeight / 3;
            int baseX = (windowWidth - formWidth) / 2;
            int baseY = (windowHeight - formHeight) / 2;

            // Check if username field is clicked
            if (mouseX >= baseX + 40 && mouseX <= baseX + formWidth - 40 &&
                mouseY >= baseY + formHeight - 100 && mouseY <= baseY + formHeight - 65)
            {
                inputFocus = InputField::Username;
            }
            // Check if password field is clicked
            else if (mouseX >= baseX + 40 && mouseX <= baseX + formWidth - 40 &&
                     mouseY >= baseY + formHeight - 180 && mouseY <= baseY + formHeight - 145)
            {
                inputFocus = InputField::Password;
            }
            // Check if login button is clicked
            else if (mouseX >= baseX + formWidth / 2 - 75 && mouseX <= baseX + formWidth / 2 + 75 &&
                     mouseY >= baseY + 40 && mouseY <= baseY + 90)
            {
                if (dbManager.loginUser(inputUsername, inputPassword))
                {
                    isLoggedIn = true;
                    loggedInUsername = inputUsername;
                    startLogIn = false;
                    showLoginForm = false;
                    errorMessage = "";
                }
                else
                {
                    errorMessage = "Invalid username or password";
                }
                inputUsername = "";
                inputPassword = "";
                inputFocus = InputField::None;
                glutPostRedisplay();
                return; // Add this to prevent further processing
            }
        }

        // Handle signup form interactions
        if (showSignupForm)
        {
            int formWidth = windowWidth / 3;
            int formHeight = windowHeight / 2.5;
            int baseX = (windowWidth - formWidth) / 2;
            int baseY = (windowHeight - formHeight) / 2;

            // Check if username field is clicked
            if (mouseX >= baseX + 40 && mouseX <= baseX + formWidth - 40 &&
                mouseY >= baseY + formHeight - 100 && mouseY <= baseY + formHeight - 65)
            {
                inputFocus = InputField::Username;
            }
            // Check if password field is clicked
            else if (mouseX >= baseX + 40 && mouseX <= baseX + formWidth - 40 &&
                     mouseY >= baseY + formHeight - 180 && mouseY <= baseY + formHeight - 145)
            {
                inputFocus = InputField::Password;
            }
            // Check if confirm password field is clicked
            else if (mouseX >= baseX + 40 && mouseX <= baseX + formWidth - 40 &&
                     mouseY >= baseY + formHeight - 260 && mouseY <= baseY + formHeight - 225)
            {
                inputFocus = InputField::ConfirmPassword;
            }
            // Check if signup button is clicked
            else if (mouseX >= baseX + formWidth / 2 - 75 && mouseX <= baseX + formWidth / 2 + 75 &&
                     mouseY >= baseY + 40 && mouseY <= baseY + 90)
            {
                // Trim whitespace from inputs
                inputUsername = trim(inputUsername);
                inputPassword = trim(inputPassword);
                inputConfirmPassword = trim(inputConfirmPassword);

                if (inputUsername.empty() || inputPassword.empty() || inputConfirmPassword.empty())
                {
                    errorMessage = "Please fill in all fields";
                }
                else if (inputPassword != inputConfirmPassword)
                {
                    errorMessage = "Passwords do not match";
                }
                else
                {
                    if (dbManager.registerUser(inputUsername, inputPassword))
                    {
                        isLoggedIn = true;
                        loggedInUsername = inputUsername;
                        startLogIn = false;
                        showSignupForm = false;
                        errorMessage = "";
                    }
                    else
                    {
                        errorMessage = "Registration failed. Username may already exist.";
                    }
                }
                inputUsername = "";
                inputPassword = "";
                inputConfirmPassword = "";
                inputFocus = InputField::None;
                glutPostRedisplay();
                return; // Add this to prevent further processing
            }
        }

        if (menu)
        {

            // Check if login button is clicked
            if (mouseX >= loginButtonX && mouseX <= loginButtonX + authButtonWidth &&
                mouseY >= loginButtonY && mouseY <= loginButtonY + authButtonHeight)
            {
                showLoginForm = true;
                showSignupForm = false;
                isLoggedIn = false;
                startLogIn = true;
                errorMessage = "";
                glutPostRedisplay();
                return;
            }

            // Check if signup button is clicked
            if (mouseX >= signupButtonX && mouseX <= signupButtonX + authButtonWidth &&
                mouseY >= signupButtonY && mouseY <= signupButtonY + authButtonHeight)
            {
                showSignupForm = true;
                showLoginForm = false;
                isLoggedIn = false;
                startLogIn = true;
                errorMessage = "";
                glutPostRedisplay();
                return;
            }
            if (startLogIn)
            {
                if (mouseX >= homeIconX && mouseX <= homeIconX + homeIconSize &&
                    mouseY >= homeIconY && mouseY <= homeIconY + homeIconSize)
                {

                    menu = 1;
                    startLogIn = fail;
                    notfail = 1;
                }
            }
            if (isLoggedIn)
            {
                if (mouseX >= HindiButtonX && mouseX <= HindiButtonX + buttonWidth * 2.3 && mouseY >= HindiButtonY && mouseY <= HindiButtonY + buttonHeight * 1.5)
                {
                    loadHindiValidWords();
                    playon = 1;
                    menu = 0;
                    notfail = 1;
                    hindiGame = 1;
                    gamestate = 1;
                    bhojpuriGame = 0;
                    englishGame = 0;
                    remainingTime = 55;
                    initGame();
                    glutPostRedisplay();
                }
                if (mouseX >= BhojpuriButtonX && mouseX <= BhojpuriButtonX + buttonWidth * 2.3 && mouseY >= BhojpuriButtonY && mouseY <= BhojpuriButtonY + buttonHeight * 1.5)
                {
                    loadBhojpuriValidWords();
                    playon = 1;
                    menu = 0;
                    notfail = 1;
                    hindiGame = 0;
                    bhojpuriGame = 1;
                    gamestate = 1;
                    englishGame = 0;
                    remainingTime = 55;
                    initGame();
                    glutPostRedisplay();
                }
                if (mouseX >= englishButtonX && mouseX <= englishButtonX + buttonWidth * 1.5 && mouseY >= englishButtonY && mouseY <= englishButtonY + buttonHeight * 1.5)
                {
                    loadEnglishValidWords();
                    playon = 1;
                    notfail = 1;
                    menu = 0;
                    gamestate = 1;
                    englishGame = 1;
                    hindiGame = 0;
                    bhojpuriGame = 0;
                    initGame();
                    remainingTime = 55;
                    glutPostRedisplay();
                }
            }
        }

        else if (!notfail)
        {
            if (mouseX >= restartButtonX && mouseX <= restartButtonX + buttonWidth && mouseY >= restartButtonY && mouseY <= restartButtonY + buttonHeight)
            {
                notfail = 1;
                restartGame();
            }

            // Check if quit button is clicked
            if (mouseX >= quitButtonX && mouseX <= quitButtonX + buttonWidth && mouseY >= quitButtonY && mouseY <= quitButtonY + buttonHeight)
            {
                exit(0); // Quit the game
            }
            if (mouseX >= homeIconX && mouseX <= homeIconX + homeIconSize &&
                mouseY >= homeIconY && mouseY <= homeIconY + homeIconSize)
            {
                restartGame();

                menu = 1;
                hindiGame = 0;
                bhojpuriGame = 0;
                englishGame = 0;
                notfail = 1;
            }
        }
        else
        {
            if (gamestate)
            {
                if (mouseX >= 0 && mouseX <= windowWidth &&
                    mouseY >= 0 && mouseY <= windowHeight)
                {
                    gamestate = 0;
                }
            }
            else
            {
                for (Ball &ball : balls)
                {
                    float dist = sqrt((mouseX - ball.x) * (mouseX - ball.x) + (mouseY - ball.y) * (mouseY - ball.y));
                    if (dist < ballRadius && !ball.clicked)
                    {
                        ball.clicked = true;
                        ball.color[0] = 1.0f; // Change color to red
                        ball.color[1] = 0.0f;
                        ball.color[2] = 0.0f;
                        userInput += ball.letter;
                        break;
                    }
                }

                if (mouseX >= backspaceIconX && mouseX <= backspaceIconX + backspaceIconSize &&
                    mouseY >= backspaceIconY && mouseY <= backspaceIconY + backspaceIconSize)
                {
                    handleInput(L'\b', -1); // Perform backspace action
                    return;
                }
            }
        }
    }
    glutPostRedisplay();
}

bool checkCollision(const Ball &newBall, const vector<Ball> &existingBalls)
{
    for (const auto &ball : existingBalls)
    {
        float distance = sqrt(pow(newBall.x - ball.x, 2) + pow(newBall.y - ball.y, 2));
        if (distance < 2 * ballRadius) // Check if balls are overlapping
        {
            return true;
        }
    }
    return false;
}

void initGame()
{

    int ballCount = ballsPerLevel[min(currentLevel - 1, maxLevels - 1)]; // Adjust based on current level
    const int maxAttempts = 100;
    balls.clear();
    restartButtonX = windowWidth / 2 - 450;
    restartButtonY = windowHeight / 2 - 60;
    quitButtonX = windowWidth / 2 + 150;
    quitButtonY = windowHeight / 2 - 60;

    HindiButtonX = windowWidth / 2 - 450;
    HindiButtonY = windowHeight / 2 - 60;
    BhojpuriButtonX = windowWidth / 2 - 150;
    BhojpuriButtonY = windowHeight / 2 - 250;
    englishButtonX = windowWidth / 2 + 150;
    englishButtonY = windowHeight / 2 - 60;

    loginButtonX = windowWidth - 440;
    loginButtonY = windowHeight - 80;
    signupButtonX = windowWidth - 220;
    signupButtonY = windowHeight - 80;

    homeIconX = windowWidth - homeIconSize - 10;
    homeIconY = 10;

    backspaceIconX = windowWidth - backspaceIconSize - 10;
    backspaceIconY = 10;

    // Set the initial time based on the current level
    remainingTime = levelTime[min(currentLevel - 1, maxLevels - 1)];
    fail = false;
    playon = true;

    for (int i = 0; i < ballCount; ++i)
    {
        Ball ball;
        bool validPosition = false;
        int attempts = 0;

        while (!validPosition && attempts < maxAttempts)
        {
            ball.x = rand() % (windowWidth - 2 * static_cast<int>(ballRadius)) + ballRadius;
            ball.y = rand() % (windowHeight - 2 * static_cast<int>(ballRadius)) + ballRadius;

            if (!checkCollision(ball, balls))
            {
                validPosition = true;
            }
            attempts++;
        }

        if (validPosition)
        {

            ball.dx = (rand() % 2 == 0 ? ballSpeed : -ballSpeed);
            ball.dy = (rand() % 2 == 0 ? ballSpeed : -ballSpeed);
            // Assign a random Hindi letter from the array
            if (hindiGame || bhojpuriGame)
                ball.letter = hindiLetters[rand() % numHindiLetters];
            else
                ball.letter = englishLetters[rand() % numEnglishLetters];
            ball.clicked = false;
            ball.color[0] = 0.4f + (static_cast<float>(rand()) / RAND_MAX) * 0.5f; // Red component between 0.5 and 1.0
            ball.color[1] = 0.4f + (static_cast<float>(rand()) / RAND_MAX) * 0.5f; // Green component between 0.5 and 1.0
            ball.color[2] = 0.4f + (static_cast<float>(rand()) / RAND_MAX) * 0.5f; // Blue component between 0.5 and 1.0
            balls.push_back(ball);
        }
        else
        {
            break;
        }
    }
    if (hindiGame || bhojpuriGame)
        initKeyboardMapHindi();
    else
        initKeyboardMapEnglish();

    // Reset the start time to the current time
    startTime = glutGet(GLUT_ELAPSED_TIME);

    // Cancel any existing timer and start a new one
    glutTimerFunc(0, updateTimer, 0);
}
void keyboard(unsigned char key, int x, int y)
{
    if (showLoginForm || showSignupForm)
    {
        if (key == 13)
        { // Enter key
            // Handle form submission
            if (showLoginForm)
            {
                if (dbManager.loginUser(inputUsername, inputPassword))
                {
                    isLoggedIn = true;
                    loggedInUsername = inputUsername;
                    showLoginForm = false;
                    startLogIn = false; // Set startLogIn to false on successful login
                    errorMessage = "";
                }
                else
                {
                    errorMessage = "Invalid username or password";
                }
            }
            else if (showSignupForm)
            {
                if (inputPassword == inputConfirmPassword)
                {
                    if (dbManager.registerUser(inputUsername, inputPassword))
                    {
                        isLoggedIn = true;
                        loggedInUsername = inputUsername;
                        showSignupForm = false;
                        startLogIn = false; // Set startLogIn to false on successful signup
                        errorMessage = "";
                    }
                    else
                    {
                        errorMessage = "Registration failed. Username may already exist.";
                    }
                }
                else
                {
                    errorMessage = "Passwords do not match";
                }
            }
            inputUsername = "";
            inputPassword = "";
            inputConfirmPassword = "";
            inputFocus = InputField::None;
        }
        else if (key == 8)
        { // Backspace
            if (inputFocus == InputField::Username && !inputUsername.empty())
            {
                inputUsername.pop_back();
            }
            else if (inputFocus == InputField::Password && !inputPassword.empty())
            {
                inputPassword.pop_back();
            }
            else if (inputFocus == InputField::ConfirmPassword && !inputConfirmPassword.empty())
            {
                inputConfirmPassword.pop_back();
            }
        }
        else if (key >= 32 && key <= 126)
        { // Printable ASCII characters
            if (inputFocus == InputField::Username)
            {
                inputUsername += key;
            }
            else if (inputFocus == InputField::Password)
            {
                inputPassword += key;
            }
            else if (inputFocus == InputField::ConfirmPassword)
            {
                inputConfirmPassword += key;
            }
        }
        glutPostRedisplay();
    }
    if ((!menu) && (notfail))
    {
        if (key == ' ' || key == 13)
        {
            if (gamestate)
            {
                gamestate = 0;
                ballSpeed = currentBallSpeed + currentLevel * 0.5 * speedIncreaseFactor;
            }
            else
            {
                gamestate = 1;
                if (userInput.empty())
                {
                    // Move to the next level if no letters are selected
                    if (currentLevel < maxLevels)
                    {
                        currentLevel++;
                        initGame(); // Generate new balls for the new level
                    }
                    else
                    {
                        // If the user completes the final level

                        notfail = false; // End the game
                        currentLevel = 1;
                    }
                }
                else
                {
                    // Check if the formed word is valid
                    if ((hindiGame && (hindiWordTrie.search(userInput))) ||
                        (bhojpuriGame && (bhojpuriWordTrie.search(userInput))) ||
                        (englishGame && (englishWordTrie.search(userInput))))
                    {
                        score += userInput.length();
                        notfail = true;

                        // Move to the next level
                        if (currentLevel < maxLevels)
                        {
                            currentLevel++;
                            initGame(); // Generate new balls for the new level
                        }
                        else
                        {
                            // If the user completes the final level
                            //   cout << "Game Completed! Final Score: " << score <<  endl;
                            notfail = false; // End the game
                        }

                        // Reset clicked balls
                        for (Ball &ball : balls)
                        {
                            if (ball.clicked)
                            {
                                ball.clicked = false;
                                ball.color[0] = static_cast<float>(rand()) / RAND_MAX;
                                ball.color[1] = static_cast<float>(rand()) / RAND_MAX;
                                ball.color[2] = static_cast<float>(rand()) / RAND_MAX;
                            }
                        }
                    }
                    else
                    {
                        notfail = false; // Only set to false if the word is invalid
                    }

                    userInput.clear();
                }
            }
        }
        else if (key == 8)
        {
            // Backspace
            if (!userInput.empty())
            {
                userInput.pop_back();
                // Unclick the last clicked ball
                for (auto it = balls.rbegin(); it != balls.rend(); ++it)
                {
                    if (it->clicked)
                    {
                        it->clicked = false;
                        it->color[0] = static_cast<float>(rand()) / RAND_MAX;
                        it->color[1] = static_cast<float>(rand()) / RAND_MAX;
                        it->color[2] = static_cast<float>(rand()) / RAND_MAX;
                        break;
                    }
                }
            }
        }
        else if (keyboardMap.find(key) != keyboardMap.end())
        {
            wchar_t languagechar = keyboardMap[key];
            // Find the first non-clicked ball with this letter
            for (size_t i = 0; i < balls.size(); ++i)
            {
                if (balls[i].letter[0] == languagechar && !balls[i].clicked)
                {
                    handleInput(languagechar, i);
                    break;
                }
            }
        }
    }

    glutPostRedisplay();
}
void init()
{
    glClearColor(1.0, 0.75, 0.8, 0.0); // pink color
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    srand(static_cast<unsigned>(time(0)));
    initGame();

    // Initialize FreeType
    if (FT_Init_FreeType(&ft))
    {
        cerr << "Could not init FreeType library" << endl;
        exit(1);
    }

    // Load a font (ensure this path is correct and the font supports Devanagari)
    if (FT_New_Face(ft, "Rozha_One/RozhaOne-Regular.ttf", 0, &face))
    {
        cerr << "Could not load font" << endl;
        exit(1);
    }
    FT_Set_Pixel_Sizes(face, 0, 48);

    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Ball Word Game");
    bool fullScreen = true; // Set to true for full screen, false for windowed
    if (fullScreen)
    {
        glutFullScreen(); // Set to full screen
        windowWidth = glutGet(GLUT_SCREEN_WIDTH);
        windowHeight = glutGet(GLUT_SCREEN_HEIGHT);
        glViewport(0, 0, windowWidth, windowHeight);
    }

    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    // Cleanup FreeType
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return 0;
}