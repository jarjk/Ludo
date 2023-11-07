/* so this is ludo / ki nevet a végén / Mensch, ärgere dich nicht!
 * this version finally has coloured output
 * and its code is also a lot better thanks to clang (CLion)
 * for example: the for loops are better
 * the bool end() makes sense
 * and VS2019 said, it's a lot better to use constexpr instead of #define*/

#include <ctime>
#include <iostream>
#include <string>

/* the following are UBUNTU/LINUX, and macOS ONLY terminal color codes,
 * but it works in Windows 10 too under VS2019.*/
constexpr auto RESET = "\033[0m";
// constexpr auto BLACK = "\033[30m"      /* Black */;
constexpr auto RED = "\033[31m"      /* Red */;
constexpr auto GREEN = "\033[32m"      /* Green */;
constexpr auto YELLOW = "\033[33m"      /* Yellow */;
constexpr auto BLUE = "\033[34m"      /* Blue */;
//constexpr auto MAGENTA = "\033[35m"      /* Magenta */;
//constexpr auto CYAN = "\033[36m"      /* Cyan */;
//constexpr auto WHITE = "\033[37m"      /* White */;
//constexpr auto BOLDBLACK = "\033[1m\033[30m"      /* Bold Black */;
constexpr auto BOLDRED = "\033[1m\033[31m"      /* Bold Red */;
constexpr auto BOLDGREEN = "\033[1m\033[32m"      /* Bold Green */;
constexpr auto BOLDYELLOW = "\033[1m\033[33m"      /* Bold Yellow */;
constexpr auto BOLDBLUE = "\033[1m\033[34m"      /* Bold Blue */;
//constexpr auto BOLDMAGENTA = "\033[1m\033[35m"      /* Bold Magenta */;
//constexpr auto BOLDCYAN = "\033[1m\033[36m"      /* Bold Cyan */;
//constexpr auto BOLDWHITE = "\033[1m\033[37m"      /* Bold White */;

// #define RESET   "\033[0m"
// #define BLACK "\033[30m"              /* Black */
// #define RED "\033[31m"                /* Red */
// #define GREEN "\033[32m"              /* Green */
// #define YELLOW "\033[33m"             /* Yellow */
// #define BLUE "\033[34m"               /* Blue */
// #define MAGENTA "\033[35m"            /* Magenta */
// #define CYAN "\033[36m"               /* Cyan */
// #define WHITE "\033[37m"              /* White */
// #define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
// #define BOLDRED "\033[1m\033[31m"     /* Bold Red */
// #define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
// #define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
// #define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
// #define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
// #define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
// #define BOLDWHITE "\033[1m\033[37m"   /* Bold White */

// gameboard, nothing: _ = 0, blue: B = 1, red: R = 2, green: G = 3, yellow: Y = 4
int board[52]{};

struct colour {
    bool waitingboard[5]{}, waitingroom[4]{true, true, true, true}, house[4]{}, was_at_the_end{};
    int current{}, onboard{}, idx{}, idx_house{}, knock_out_num{};
    int start{}, end{}, colour_num{};
    char ch{};
};

colour blue, red, green, yellow;

/* It's a function that writes the game stats to the terminal. */
void out();

/* It's a function that throws the cube. */
void throw_cube(int &current);

/* It's checking if an array of four is empty. */
bool full_four(const bool arr[4]);

/* It's checking if the game is over. */
bool end();

/* It's playing one round. */
void round();

/* It's clearing the terminal. And it's CROSS-PLATFORM! */
void clear_term();

// cross-platform wait
void wait_term();

// is there a player from this colour on the board?
bool is_on_board(const int &colour);

// please kick each other, because there is not enough space for two on only one field of the board
void knock_out(colour &tmp);

// array of five is not empty
bool not_empty_five(const bool array[5]);

// round for red, green, yellow
void rgy(colour &temp);

// needs to go home, because it's been kicked out.
void go_home(colour &col);

int main() {
    srand(time(nullptr));

    // setting stuff
    blue.start = 0;
    red.start = 13;
    green.start = 26;
    yellow.start = 39;

    blue.end = 50;
    red.end = 11;
    green.end = 24;
    yellow.end = 37;

    blue.colour_num = 1;
    red.colour_num = 2;
    green.colour_num = 3;
    yellow.colour_num = 4;

    blue.ch = 'B';
    red.ch = 'R';
    green.ch = 'G';
    yellow.ch = 'Y';

    clear_term();

    std::cout << "Are you ready to start this beautiful game? [Y/n] ";
    // char choice = '\0';
    std::string choice = "y";
    // std::cin >> choice;
    getline(std::cin, choice, '\n');
    if (choice.empty() || choice[0] == 'y' || choice[0] == 'Y') {
        choice = "Y";
        std::cout
                << "Do you want to play a full game without doing anything? [Y/n] ";
        getline(std::cin, choice, '\n');
        if (choice.empty() || choice[0] == 'y' || choice[0] == 'Y') {
            while (!end()) {
                round();
                out();
            }
        } else {
            choice[0] = 'y';
            while (!end() && (choice[0] == 'y' || choice[0] == 'Y' || choice[0] == '\n')) {
                round();
                out();
                std::cout << "Can the next round begin? [Y/n] ";
                std::cin >> choice[0];
            }
            out();
        }
    }

    return 0;
}

// writes game stats to terminal
void out() {
    wait_term();
    clear_term();

    std::cout << "board:\n";

    for (int i: board) {
        if (i == 0)
            std::cout << "_  ";
        else if (i == blue.colour_num)
            std::cout << BOLDBLUE << blue.ch << "  " << RESET;
        else if (i == red.colour_num)
            std::cout << BOLDRED << red.ch << "  " << RESET;
        else if (i == green.colour_num)
            std::cout << BOLDGREEN << green.ch << "  " << RESET;
        else if (i == yellow.colour_num)
            std::cout << BOLDYELLOW << yellow.ch << "  " << RESET;
        else
            std::cout << "___!!!error!!!___";
    }
    // numbers under board
    std::cout << '\n';
    for (int i = 0; i < 52; i++) {
        if (i < 10)
            std::cout << i << "  ";
        else
            std::cout << i << " ";
    }
    // showing current numbers of the dice
    std::cout << "\n\ndice:\n";
    {
        std::cout << BLUE << "\tblue:\t" << blue.current << RED << "\n\tred:\t" << red.current << GREEN
                  << "\n\tgreen:\t" << green.current << YELLOW << "\n\tyellow:\t"
                  << yellow.current << RESET;
    }

    std::cout << "\n\nwaiting rooms:\n";
    {
        std::cout << BLUE << "\tblue:\t";
        for (bool i: blue.waitingroom) {
            std::cout << i << " ";
        }
        std::cout << RED << "\n\tred:\t";
        for (bool i: red.waitingroom) {
            std::cout << i << " ";
        }
        std::cout << GREEN << "\n\tgreen:\t";
        for (bool i: green.waitingroom) {
            std::cout << i << " ";
        }
        std::cout << YELLOW << "\n\tyellow:\t";
        for (bool i: yellow.waitingroom) {
            std::cout << i << " ";
        }
        std::cout << RESET;
    }

    std::cout << "\n\nwaiting boards:\n";
    {
        std::cout << BLUE << "\tblue:\t";
        for (bool i: blue.waitingboard) {
            std::cout << i << " ";
        }
        std::cout << RED << "\n\tred:\t";
        for (bool i: red.waitingboard) {
            std::cout << i << " ";
        }
        std::cout << GREEN << "\n\tgreen:\t";
        for (bool i: green.waitingboard) {
            std::cout << i << " ";
        }
        std::cout << YELLOW << "\n\tyellow:\t";
        for (bool i: yellow.waitingboard) {
            /*if (yellow.waitingboard[i] == true)
    std::cout << "Y ";
    else
    std::cout << "_ ";
    */
            std::cout << i << " ";
        }
    }

    std::cout << RESET << "\n\nhouses:\n";
    {
        std::cout << BLUE << "\tblue:\t";
        for (bool i: blue.house) {
            std::cout << i << " ";
        }
        std::cout << RED << "\n\tred:\t";
        for (bool i: red.house) {
            std::cout << i << " ";
        }
        std::cout << GREEN << "\n\tgreen:\t";
        for (bool i: green.house) {
            std::cout << i << " ";
        }
        std::cout << YELLOW << "\n\tyellow:\t";
        for (bool i: yellow.house) {
            std::cout << i << " ";
        }
        // std::cout << std::endl << std::endl;
    }

    std::cout << RESET << "\n\nknockouts:\n";
    std::cout << BLUE << "\tblue:\t" << blue.knock_out_num << RED << "\n\tred:\t" << red.knock_out_num << GREEN
              << "\n\tgreen:\t" << green.knock_out_num << YELLOW << "\n\tyellow:\t" << yellow.knock_out_num << RESET
              << std::endl;
}

// plays one round, in the future it should not be so long, it rather should be made by making a function, that does it for each colour
void round() {
    throw_cube(blue.current);
    // blue
    {
        // its waiting board is not empty
        if (not_empty_five(blue.waitingboard)) {
            if (blue.onboard + blue.current >= 5 - blue.onboard) {
                blue.waitingboard[blue.onboard] = false;
                blue.house[blue.idx_house] = true;
                blue.onboard = 0;
                blue.idx_house++;
            } else {
                blue.waitingboard[blue.onboard] = false;
                blue.onboard = blue.current + blue.onboard;
                blue.waitingboard[blue.onboard] = true;
            }
        }
            // board is empty
        else if (!is_on_board(blue.colour_num)) {
            // can go to the board from its waiting room
            if (blue.current == 6) {
                blue.onboard = blue.start;
                knock_out(blue);
                board[blue.onboard] = blue.colour_num;
                blue.waitingroom[blue.idx] = false;
                blue.idx++;
            }
        }
            // waitingroom not full_four
            // not near its house
        else if (blue.onboard + blue.current <= blue.end) {
            board[blue.onboard] = 0;
            blue.onboard += blue.current;
            knock_out(blue);
            board[blue.onboard] = blue.colour_num;
        }
            // so cool, that from the board it goes directly into the house
        else if (blue.onboard + blue.current == blue.end + 6) {
            board[blue.onboard] = 0;
            blue.house[blue.idx_house] = true;
            blue.idx_house++;
        }
            // near the house, needs to go to waitingboard (pl.:48-ason van 5ot dobott)
        else if (blue.onboard + blue.current <= blue.end + 5) {
            board[blue.onboard] = 0;
            blue.onboard = blue.current - (blue.end - blue.onboard) - 1;
            blue.waitingboard[blue.onboard] = true;
        }
    }
    rgy(red);
    rgy(green);
    rgy(yellow);
}

void clear_term() {
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

bool is_on_board(const int &colour) {
    // k is needed in order not to conflict with temporary i-s
    int k = 0;
    while (board[k] != colour && k < 52)
        k++;
    return k < 52;
}

// checks if an array of four is full
bool full_four(const bool arr[4]) {
    if (arr[0] && arr[1] && arr[2] && arr[3])
        return true;
    else
        return false;
}

// checks if it's the end of the game
bool end() {
    if (blue.current == 6 && red.current == 6 && green.current == 6 &&
        yellow.current == 6) {
        wait_term();
        std::cout << "\nHooray! Everyone throw a six! This means everyone has won!\n";
        return true;
    } else if (full_four(blue.house) || full_four(red.house) || full_four(green.house) || full_four(yellow.house)) {
        // clear_term();
        std::cout << "\nONE OF THE ANIMALS GOT TO THE END, HOORAY, HOORAY!\nNUMBER OF ALL KNOCKOUTS: " <<
                  blue.knock_out_num + red.knock_out_num + green.knock_out_num + yellow.knock_out_num << "\n\n";
        return true;
    } else
        return false;
}

void wait_term() {
#ifdef _WIN32
    system("timeout \t 1");
#else
    system("sleep 0.08");
#endif
}

void knock_out(colour &tmp) {
    if (board[tmp.onboard] != 0) {
        /*board[tmp.onboard] = 5;
      tmp.onboard = tmp.start;
      tmp.idx--;
      tmp.waitingroom[tmp.idx] = true;
      */
        switch (board[tmp.onboard]) {
            case 1:
                go_home(blue);
                // blue.onboard = blue.start - 1;
                // blue.idx--;
                // blue.waitingroom[blue.idx] = true;
                // blue.knock_out_num++;
                break;
            case 2:
                red.onboard = red.start - 1;
                red.idx--;
                red.waitingroom[red.idx] = true;
                red.knock_out_num++;
                break;
            case 3:
                green.onboard = green.start - 1;
                green.idx--;
                green.waitingroom[green.idx] = true;
                green.knock_out_num++;
                break;
            case 4:
                yellow.onboard = yellow.start - 1;
                yellow.idx--;
                yellow.waitingroom[yellow.idx] = true;
                yellow.knock_out_num++;
                // go_home(yellow);
                break;
            default:
                std::cout << "___ERROR!___\n";
                break;
        }
        std::cout << "\nA KNOCKOUT HAS HAPPENED (KO) \n";
        wait_term();
    }
}

bool not_empty_five(const bool array[5]) {
    if (array[0] || array[1] || array[2] || array[3] || array[4])
        return true;
    else
        return false;
}

void rgy(colour &temp) {
    throw_cube(temp.current);
    // its waiting board is not empty
    if (not_empty_five(temp.waitingboard)) {
        // goes directly into its house
        if (temp.onboard + temp.current >= 5 - temp.onboard) {
            temp.waitingboard[temp.onboard] = false;
            temp.house[temp.idx_house] = true;
            temp.onboard = 0;
            temp.idx_house++;
        }
            // goes into its house slowly
        else {
            temp.waitingboard[temp.onboard] = false;
            temp.onboard = temp.onboard + temp.current;
            temp.waitingboard[temp.onboard] = true;
        }
    }
        // board is empty
    else if (!is_on_board(temp.colour_num)) {
        // can go to the board from its waiting room
        if (temp.current == 6) {
            temp.onboard = temp.start;
            knock_out(temp);
            board[temp.onboard] = temp.colour_num;
            temp.waitingroom[temp.idx] = false;
            temp.idx++;
        }
    } else if (!temp.was_at_the_end) {
        // basic movement
        if (temp.onboard + temp.current < 52) {
            board[temp.onboard] = 0;
            temp.onboard += temp.current;
            knock_out(temp);
            board[temp.onboard] = temp.colour_num;
        }
            // needs to go to the start of the board
        else if (temp.onboard + temp.current > 51) {
            board[temp.onboard] = 0;
            temp.onboard = temp.current - (52 - temp.onboard);
            knock_out(temp);
            board[temp.onboard] = temp.colour_num;
            temp.was_at_the_end = true;
        }
    }
        // basic movement until it reaches the waiting board
    else if (temp.onboard + temp.current <= temp.end) {
        board[temp.onboard] = 0;
        temp.onboard += temp.current;
        knock_out(temp);
        board[temp.onboard] = temp.colour_num;
    }
        // goes right into house
    else if (temp.onboard + temp.current == temp.end + 6) {
        board[temp.onboard] = 0;
        temp.house[temp.idx_house] = true;
        temp.idx_house++;
        temp.was_at_the_end = false;
    }
        // goes into its waiting board
    else if (temp.onboard + temp.current <= temp.end + 5) {
        board[temp.onboard] = 0;
        temp.onboard = temp.current - (temp.end - temp.onboard) - 1;
        temp.waitingboard[temp.onboard] = true;
        temp.was_at_the_end = false;
    }
}

void go_home(colour &col) {
    col.onboard = col.start - 1;
    col.idx--;
    col.waitingroom[col.idx] = true;
    col.knock_out_num++;
}

void throw_cube(int &current) {
    current = rand() % 6 + 1;
}
