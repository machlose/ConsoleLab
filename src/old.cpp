#pragma once
#include <windows.h>
#include <string>
#include <sstream>
#include <vector>
#include <functional>

struct ScreenSizeInfo {
    int width;
    int height;
    ScreenSizeInfo(int width, int height){
        this->width = width;
        this->height = height;
    }
    ScreenSizeInfo(){}
};

struct MouseInfo {
    int x;
    int y;
    bool leftPressed;   // true tylko przez jedną klatkę po wciśnięciu
    bool rightPressed;
    bool leftHeld;      // true dopóki trzymasz
    bool rightHeld;
};

struct Pos {
    int x;
    int y;
};

struct KeyInfo {
    WORD  vkCode;       // virtual key code (VK_*)
    char  ch;           // znak ASCII (0 jeśli nie-drukowany)
    bool  pressed;      // true = wciśnięty, false = puszczony
    bool  ctrl;
    bool  shift;
    bool  alt;
};

// ============================================================ Sprite

struct Sprite {
    int width;
    int height;
    std::vector<char> chars;   // row-major, size = width * height
    std::vector<WORD> attrs;   // atrybuty kolorów, size = width * height

    // Konstruktor z gotowych danych (tablica stringów = wiersze)
    Sprite(std::vector<std::string> rows,
           WORD defaultAttr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
    {
        height = (int)rows.size();
        width  = 0;
        for (auto &r : rows)
            if ((int)r.size() > width) width = (int)r.size();

        chars.assign(width * height, ' ');
        attrs.assign(width * height, defaultAttr);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < (int)rows[y].size(); x++) {
                chars[y * width + x] = rows[y][x];
                attrs[y * width + x] = defaultAttr;
            }
        }
    }

    // Konstruktor pusty — wypełniony spacjami z podanym atrybutem
    Sprite(int w, int h,
           WORD defaultAttr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
        : width(w), height(h),
          chars(w * h, ' '),
          attrs(w * h, defaultAttr)
    {}

    void setAttr(WORD attr) {
        std::fill(attrs.begin(), attrs.end(), attr);
    }

    void resize(int newW, int newH,
                WORD defaultAttr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) {
        std::vector<char> newChars(newW * newH, ' ');
        std::vector<WORD> newAttrs(newW * newH, defaultAttr);

        int copyW = (newW < width)  ? newW : width;
        int copyH = (newH < height) ? newH : height;

        for (int y = 0; y < copyH; y++) {
            for (int x = 0; x < copyW; x++) {
                newChars[y * newW + x] = chars[y * width + x];
                newAttrs[y * newW + x] = attrs[y * width + x];
            }
        }

        width  = newW;
        height = newH;
        chars  = std::move(newChars);
        attrs  = std::move(newAttrs);
    }
};

// ============================================================ Button

class Button {
public:
    Pos    pos;
    Sprite spriteNormal;
    Sprite spriteHover;
    Sprite spritePressed;
    std::function<void()> onClick;

    Button(Pos pos, Sprite normal, Sprite hover, Sprite pressed,
           std::function<void()> onClick = nullptr)
        : pos(pos),
          spriteNormal(normal),
          spriteHover(hover),
          spritePressed(pressed),
          onClick(onClick)
    {}

    // Skrócony konstruktor — jeden sprite, kolory zmieniają się automatycznie
    Button(Pos pos, std::vector<std::string> rows,
           std::function<void()> onClick = nullptr)
        : pos(pos),
          spriteNormal (rows, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE),
          spriteHover  (rows, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE),
          spritePressed(rows, BACKGROUND_BLUE | BACKGROUND_INTENSITY
                            | FOREGROUND_RED  | FOREGROUND_GREEN | FOREGROUND_BLUE),
          onClick(onClick)
    {}

    bool isHovered(const MouseInfo &mouse) const {
        return mouse.x >= pos.x && mouse.x < pos.x + spriteNormal.width
            && mouse.y >= pos.y && mouse.y < pos.y + spriteNormal.height;
    }

    void handleEvents(const MouseInfo &mouse) {
        if (isHovered(mouse) && mouse.leftPressed && onClick)
            onClick();
    }

    const Sprite &currentSprite(const MouseInfo &mouse) const {
        if (isHovered(mouse)) {
            if (mouse.leftHeld) return spritePressed;
            return spriteHover;
        }
        return spriteNormal;
    }
};

// ============================================================ TextBox

class TextBox {
public:
    Pos         pos;
    int         width;          // szerokość pola (bez ramki)
    std::string text;           // pełny tekst (może być dłuższy niż width)
    int         cursor;         // pozycja kursora w tekście
    int         scroll;         // offset przewijania (ile znaków z lewej ukryte)
    bool        focused;
    bool        showBorder;
    std::function<void(const std::string &)> onEnter;  // callback na Enter

    // Atrybuty kolorów
    WORD attrNormal  = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    WORD attrFocused = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
                     | BACKGROUND_BLUE;
    WORD attrCursor  = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE
                     | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    WORD attrBorder  = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    TextBox(Pos pos, int width, bool showBorder = true,
            std::function<void(const std::string &)> onEnter = nullptr)
        : pos(pos), width(width), cursor(0), scroll(0),
          focused(false), showBorder(showBorder), onEnter(onEnter)
    {}

    // Kliknięcie — ustaw fokus i pozycję kursora
    void handleMouse(const MouseInfo &mouse) {
        int fieldX = showBorder ? pos.x + 1 : pos.x;
        int fieldY = showBorder ? pos.y + 1 : pos.y;

        if (mouse.leftPressed) {
            bool inBox = mouse.x >= fieldX && mouse.x < fieldX + width
                      && mouse.y == fieldY;
            focused = inBox;
            if (inBox) {
                int clicked = scroll + (mouse.x - fieldX);
                cursor = (clicked < (int)text.size()) ? clicked : (int)text.size();
            }
        }
    }

    // Klawiatura — wywołaj tylko gdy focused == true
    void handleKey(const KeyInfo &key) {
        if (!focused || !key.pressed) return;

        switch (key.vkCode) {
            case VK_BACK:
                if (cursor > 0) {
                    text.erase(cursor - 1, 1);
                    cursor--;
                }
                break;

            case VK_DELETE:
                if (cursor < (int)text.size())
                    text.erase(cursor, 1);
                break;

            case VK_LEFT:
                if (cursor > 0) cursor--;
                break;

            case VK_RIGHT:
                if (cursor < (int)text.size()) cursor++;
                break;

            case VK_HOME:
                cursor = 0;
                break;

            case VK_END:
                cursor = (int)text.size();
                break;

            case VK_RETURN:
                if (onEnter) onEnter(text);
                break;

            case VK_ESCAPE:
                focused = false;
                break;

            default:
                // Wstaw drukowalny znak ASCII
                if (key.ch >= 0x20 && key.ch < 0x7F) {
                    text.insert(cursor, 1, key.ch);
                    cursor++;
                }
                break;
        }

        // Aktualizuj scroll żeby kursor był zawsze widoczny
        if (cursor < scroll)
            scroll = cursor;
        if (cursor >= scroll + width)
            scroll = cursor - width + 1;
    }

    // Zbuduj sprite TextBoxa do narysowania
    Sprite buildSprite() const {
        int sprW = showBorder ? width + 2 : width;
        int sprH = showBorder ? 3         : 1;

        Sprite spr(sprW, sprH, attrNormal);
        WORD   fieldAttr = focused ? attrFocused : attrNormal;

        if (showBorder) {
            // Górna ramka
            spr.chars[0]         = '+';
            spr.chars[width + 1] = '+';
            for (int x = 1; x <= width; x++) spr.chars[x] = '-';

            // Dolna ramka
            int bot = 2 * sprW;
            spr.chars[bot]         = '+';
            spr.chars[bot+width+1] = '+';
            for (int x = 1; x <= width; x++) spr.chars[bot + x] = '-';

            // Boczne ramki
            spr.chars[sprW]          = '|';
            spr.chars[sprW+width+1]  = '|';

            for (int x = 0; x < sprW; x++) {
                spr.attrs[x]           = attrBorder;
                spr.attrs[bot + x]     = attrBorder;
            }
            spr.attrs[sprW]         = attrBorder;
            spr.attrs[sprW+width+1] = attrBorder;

            // Pole tekstowe — środkowy wiersz, offset 1
            for (int x = 0; x < width; x++) {
                int tx = scroll + x;
                char c = (tx < (int)text.size()) ? text[tx] : ' ';

                bool isCursor = focused && (tx == cursor);
                spr.chars[sprW + 1 + x] = isCursor ? (c == ' ' ? '_' : c) : c;
                spr.attrs[sprW + 1 + x] = isCursor ? attrCursor : fieldAttr;
            }
        } else {
            // Bez ramki — sam wiersz tekstu
            for (int x = 0; x < width; x++) {
                int tx = scroll + x;
                char c = (tx < (int)text.size()) ? text[tx] : ' ';

                bool isCursor = focused && (tx == cursor);
                spr.chars[x] = isCursor ? (c == ' ' ? '_' : c) : c;
                spr.attrs[x] = isCursor ? attrCursor : fieldAttr;
            }
        }

        return spr;
    }
};

// ============================================================ UI

class UI {
public:
    static HANDLE hOut;
    static HANDLE hIn;

    static ScreenSizeInfo     screenSize;
    static MouseInfo          mouse;
    static std::vector<KeyInfo> keys;   // klawisze wciśnięte w tej klatce

    static void init() {
        hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        hIn  = GetStdHandle(STD_INPUT_HANDLE);

        DWORD mode;
        GetConsoleMode(hIn, &mode);
        mode |=  ENABLE_MOUSE_INPUT;
        mode |=  ENABLE_WINDOW_INPUT;
        mode &= ~ENABLE_QUICK_EDIT_MODE;
        SetConsoleMode(hIn, mode);

        CONSOLE_CURSOR_INFO cci{ 1, FALSE };
        SetConsoleCursorInfo(hOut, &cci);

        screenSize = getScreenSize();
    }

    static ScreenSizeInfo getScreenSize() {
        ScreenSizeInfo output;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hOut, &csbi);

        output.width  = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
        output.height = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;
        return output;
    }

    static void resizeWindow(ScreenSizeInfo newDimensions) {
        
    }

    static int getFontSize() {
        CONSOLE_FONT_INFOEX cfi{};
        cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
        GetCurrentConsoleFontEx(hOut, FALSE, &cfi);
        return cfi.dwFontSize.Y;
    }

    static void handleEvents() {
        mouse.leftPressed  = false;
        mouse.rightPressed = false;
        keys.clear();

        INPUT_RECORD rec;
        DWORD count;

        while (PeekConsoleInput(hIn, &rec, 1, &count) && count > 0) {
            ReadConsoleInput(hIn, &rec, 1, &count);

            if (rec.EventType == WINDOW_BUFFER_SIZE_EVENT) {
                screenSize = getScreenSize();
            }

            if (rec.EventType == MOUSE_EVENT) {
                auto &m = rec.Event.MouseEvent;
                mouse.x = m.dwMousePosition.X;
                mouse.y = m.dwMousePosition.Y;

                bool lDown = m.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED;
                bool rDown = m.dwButtonState & RIGHTMOST_BUTTON_PRESSED;

                if (lDown && !mouse.leftHeld)  mouse.leftPressed  = true;
                if (rDown && !mouse.rightHeld) mouse.rightPressed = true;

                mouse.leftHeld  = lDown;
                mouse.rightHeld = rDown;
            }

            if (rec.EventType == KEY_EVENT) {
                auto &k = rec.Event.KeyEvent;
                KeyInfo ki;
                ki.vkCode  = k.wVirtualKeyCode;
                ki.ch      = (char)k.uChar.AsciiChar;
                ki.pressed = k.bKeyDown;
                ki.ctrl    = (k.dwControlKeyState & (LEFT_CTRL_PRESSED  | RIGHT_CTRL_PRESSED))  != 0;
                ki.shift   = (k.dwControlKeyState & SHIFT_PRESSED)  != 0;
                ki.alt     = (k.dwControlKeyState & (LEFT_ALT_PRESSED   | RIGHT_ALT_PRESSED))   != 0;
                keys.push_back(ki);
            }
        }
    }

    // --------------------------------------------------------- draw

    static void draw(std::vector<char> &chars, std::vector<WORD> &attrs,
                     const Sprite &sprite, Pos pos)
    {
        const int W = screenSize.width;
        const int H = screenSize.height;

        for (int y = 0; y < sprite.height; y++) {
            int sy = pos.y + y;
            if (sy < 0 || sy >= H) continue;

            for (int x = 0; x < sprite.width; x++) {
                int sx = pos.x + x;
                if (sx < 0 || sx >= W) continue;

                chars[sy * W + sx] = sprite.chars[y * sprite.width + x];
                attrs[sy * W + sx] = sprite.attrs[y * sprite.width + x];
            }
        }
    }

    // Przeciążenie — rysuje button z właściwym spritem (normal/hover/pressed)
    static void draw(std::vector<char> &chars, std::vector<WORD> &attrs,
                     const Button &button)
    {
        draw(chars, attrs, button.currentSprite(mouse), button.pos);
    }

    // Przeciążenie — rysuje TextBox (buduje sprite wewnętrznie)
    static void draw(std::vector<char> &chars, std::vector<WORD> &attrs,
                     const TextBox &tb)
    {
        Sprite spr = tb.buildSprite();
        draw(chars, attrs, spr, tb.pos);
    }

    // --------------------------------------------------------- render

    static void render() {
        const int W = screenSize.width;
        const int H = screenSize.height;
        const int total = W * H;

        std::string chars(total, ' ');
        std::vector<WORD> attrs(total, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        // Podświetl komórkę pod myszką
        if (mouse.x >= 0 && mouse.x < W && mouse.y >= 0 && mouse.y < H) {
            int idx = mouse.y * W + mouse.x;

            if (mouse.leftHeld) {
                attrs[idx] = BACKGROUND_BLUE | BACKGROUND_INTENSITY
                           | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            } else if (mouse.rightHeld) {
                attrs[idx] = BACKGROUND_RED | BACKGROUND_INTENSITY
                           | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            } else {
                attrs[idx] = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
            }
        }

        // Rozmiar konsoli w lewym górnym rogu
        std::ostringstream oss;
        oss << "[" << W << "x" << H << "]";
        std::string label = oss.str();
        for (int i = 0; i < (int)label.size() && i < W; i++) {
            chars[i] = label[i];
            attrs[i] = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        }

        for (int y = 0; y < H; y++) {
            COORD pos{ 0, (SHORT)y };
            DWORD written;
            WriteConsoleOutputCharacterA(hOut, chars.data() + y * W, W, pos, &written);
            WriteConsoleOutputAttribute (hOut, attrs.data() + y * W, W, pos, &written);
        }
    }

    // render z gotowym buforem — użyj gdy sam budujesz scenę draw()
    static void render(std::vector<char> &chars, std::vector<WORD> &attrs) {
        const int W = screenSize.width;
        const int H = screenSize.height;

        for (int y = 0; y < H; y++) {
            COORD pos{ 0, (SHORT)y };
            DWORD written;
            WriteConsoleOutputCharacterA(hOut, chars.data() + y * W, W, pos, &written);
            WriteConsoleOutputAttribute (hOut, attrs.data() + y * W, W, pos, &written);
        }
    }

    // Pomocnik — wypełnia bufor spacjami z domyślnym atrybutem
    static void clearBuffer(std::vector<char> &chars, std::vector<WORD> &attrs,
                            WORD defaultAttr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
    {
        int total = screenSize.width * screenSize.height;
        chars.assign(total, ' ');
        attrs.assign(total, defaultAttr);
    }
};

HANDLE              UI::hOut = nullptr;
HANDLE              UI::hIn  = nullptr;
ScreenSizeInfo      UI::screenSize{};
MouseInfo           UI::mouse{ -1, -1, false, false, false, false };
std::vector<KeyInfo> UI::keys{};