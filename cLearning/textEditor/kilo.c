/*** includes ***/
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <stdlib.h>
#include <errno.h>

/*** defines ***/
#define CTRL_KEY(k) ((k) & 0x1f) // the CRTL key in the terminal strips the 5th and 6th bits from the other key you press with it.
#define ABUF_INIT {NULL, 0}

#define APP_VERSION "0.0.1"

enum editorKey {
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    //ARROW_LEFT = 'h',
    //ARROW_RIGHT = 'l',
    //ARROW_UP = 'k',
    //ARROW_DOWN = 'j',
    PAGE_UP,
    PAGE_DOWN,
    HOME_KEY,
    END_KEY,
};

/*** data ***/
typedef struct erow {
    int size;
    char* chars;
} erow;

struct editorConfig {
    int cx, cy; // cursor positions
    int screenCols;
    int screenRows;
    int numrows;
    erow row;
    struct termios originalTermios;
};

struct abuf {
    char *b;
    int len;
};

void abAppend(struct abuf *ab, const char* s, int len) {
    char* new = realloc(ab->b, ab->len + len);

    if (new == NULL) return;
    memcpy(&new[ab->len], s, len);
    ab->b = new;
    ab->len += len;
}

void abFree(struct abuf* ab) {
    free(ab->b);
}

struct editorConfig E;

/*** terminal ***/
void die(const char* s) {
    write(STDOUT_FILENO, "\x1b[1J", 4);
    write(STDOUT_FILENO, "\x1b[2;1H", 6);
    // perror looks at the global errno that is set by most C lib functions
    perror(s);
    exit(1); // exit with failure
}

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.originalTermios) == -1) {
        die("tcsetattr");
    }
}

void enableRawMode() {
    struct termios raw;
    if (tcgetattr(STDIN_FILENO, &raw) == -1) die("tcgetattr");
    E.originalTermios = raw;
    atexit(disableRawMode);
    // ~ is bitwise not. We are ANDing the NOT of ECHO with the control flag
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    // Control Characters field
    raw.c_cc[VMIN] = 0; // number of bytes needed before read
    raw.c_cc[VTIME] = 1; // max time to wait before read() returns in tenths of a sec
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

int editorReadKey() {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if ((nread == -1) && (errno != EAGAIN)) die("read");
    }

    if (c == '\x1b') {
        char seq[3];

        if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

        if (seq[0] == '[') {
            if (seq[1] >= '0' && seq[1] <= '9') {
                if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
                if (seq[2] == '~') {
                    switch (seq[1]) {
                        case '1': return HOME_KEY;
                        case '3': return DEL_KEY;
                        case '4': return END_KEY;
                        case '5': return PAGE_UP;
                        case '6': return PAGE_DOWN;
                        case '7': return HOME_KEY;
                        case '8': return END_KEY;
                    }
                }
            } else {
                switch (seq[1]) {
                    case 'A' : return ARROW_UP;
                    case 'B' : return ARROW_DOWN;
                    case 'C' : return ARROW_RIGHT;
                    case 'D' : return ARROW_LEFT;
                    case 'F' : return END_KEY;
                    case 'H' : return HOME_KEY;
                    case 'O' :
                        {
                            switch (seq[2]) {
                                case 'H': return HOME_KEY;
                                case 'F': return END_KEY;
                            }
                        }
                }
            }
        }
        return '\x1b';
    } else {
        return c;
    }
}


int getCursorPosition(int *cols, int *rows) {
    const unsigned int bufSize = 32;
    char buf[bufSize];
    unsigned int i = 0;

    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;

    for (; i < bufSize; i++) {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
        if (buf[i] == 'R') break;
    }
    buf[i] = '\0'; // assign line ending so printf knows when it ends
    if (buf[0] != '\x1b' || buf[1] != '[') return -1;
    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;
    return 0;
}

int getWindowSize(int *cols, int *rows) {
    struct winsize ws; 
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if (write(STDOUT_FILENO, "\x1b[999B\x1b[999C", 12) != 12) return -1;
        return getCursorPosition(cols, rows);    
    } else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}

/*** output ***/
void editorDrawRows(struct abuf *ab) {
    int y;
    for (y = 0; y < E.screenRows; y++) {
        if (y == E.screenRows / 3) {
            char welcome[80];
            int welcomeLen = snprintf(welcome, sizeof(welcome), "Jvim editor -- version %s", APP_VERSION); 
            if (welcomeLen > E.screenCols) welcomeLen = E.screenCols;
            // Centres the welcome message
            int padding = (E.screenCols - welcomeLen) / 2;
            // Add the tilde in front of the welcome line
            if (padding) {
                abAppend(ab, "~", 1);
                padding--;
            }
            while (padding--) abAppend(ab, " ", 1); // keep padding the buffer for this line until it is halfway across the screen
            abAppend(ab, welcome, welcomeLen); // append the welcome message halfway
        } else {
            abAppend(ab, "~", 1);
        }
        abAppend(ab, "\x1b[K", 3); // Clear screen
        if (y < E.screenRows - 1) {
            abAppend(ab, "\r\n", 2);
        }
    }
}

void editorRefreshScreen() {
    // \x is hex in a string 
    // write 4 bytes, \x1b = escape, \x1b[ is the escape sequence
    struct abuf ab = ABUF_INIT;

    abAppend(&ab, "\x1b[?25l", 6); // hide cursor
    abAppend(&ab, "\x1b[H", 3); // Move cursor to the top

    editorDrawRows(&ab);

    abAppend(&ab, "\x1b[?25h", 6); // show cursor
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1); 
    abAppend(&ab, buf, strlen(buf));
    write(STDOUT_FILENO, ab.b, ab.len);
    abFree(&ab);
}


/*** input ***/ 
void editorMoveCursor(int key) {
    switch(key) {
        case ARROW_LEFT:
            if (E.cx > 0) E.cx--;
            break;
        case ARROW_DOWN:
            if (E.cy < E.screenRows - 1) E.cy++;
            break;
        case ARROW_UP:
            if (E.cy > 0) E.cy--;
            break;
        case ARROW_RIGHT:
            if (E.cx < E.screenCols - 1) E.cx++;
            break;
    }
}

void editorProcessKeyPress() {
    int c = editorReadKey();

    switch (c) {
        case CTRL_KEY('q'):
            write(STDOUT_FILENO, "\x1b[2J", 4);
            write(STDOUT_FILENO, "\x1b[H", 3);
            exit(0);
            break;
        case PAGE_UP:
        case PAGE_DOWN:
            {
                int times = E.screenRows;
                while (times--)
                    editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
            }
            break;
        case HOME_KEY:
            E.cx = 0;
            break;
        case END_KEY:
            E.cx = E.screenCols - 1;
            break;
        case ARROW_LEFT:
        case ARROW_DOWN:
        case ARROW_UP:
        case ARROW_RIGHT:
            editorMoveCursor(c);
            break;
    }
}


/*** init ***/
void initEditor() {
    E.cx = 0;
    E.cy = 0;
    E.numrows = 0;
    if (getWindowSize(&E.screenCols, &E.screenRows) == -1) die("getWindowSize");
}

int main() {
    enableRawMode();
    initEditor();
    // reading from stdin, using a buffer of size 1, reading each character
    while (1) {
        editorRefreshScreen();
        editorProcessKeyPress();
    }
    return 0;
}
