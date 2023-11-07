/* 2023-04-24 Copyright (c) 2023 Jero. All Rights Reserved.
 * so this is ludo / ki nevet a végén / Mensch, ärgere dich nicht!
 * this version includes a bug fix! which was pretty bad and also very obvious, but no worries I fixed it.
 */

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

/* the following are UBUNTU/LINUX, and macOS ONLY terminal color codes,
 * but it works in Windows 10 too under VS2019.*/
constexpr auto RESET = "\033[0m";

constexpr auto RED = "\033[31m" /* Red */;
constexpr auto GREEN = "\033[32m" /* Green */;
constexpr auto YELLOW = "\033[33m" /* Yellow */;
constexpr auto BLUE = "\033[34m" /* Blue */;

constexpr auto BOLDRED = "\033[1m\033[31m" /* Bold Red */;
constexpr auto BOLDGREEN = "\033[1m\033[32m" /* Bold Green */;
constexpr auto BOLDYELLOW = "\033[1m\033[33m" /* Bold Yellow */;
constexpr auto BOLDBLUE = "\033[1m\033[34m" /* Bold Blue */;

// gameboard, nothing: _ = 0, blue: B = 1, red: R = 2, green: G = 3, yellow: Y =
// 4
int board[52]{};

struct colour {
    bool waitingboard[5]{}, waitingroom[4]{true, true, true, true}, house[4]{},
        was_at_the_end{};
    int current{}, onboard{}, idx{}, idx_house{}, knock_out_num{};
    int start{}, end{}, colour_num{};
    char ch{};
    std::string upper, lower;
};

colour blue, red, green, yellow;
int round_num = 0;

/* functions */
/* It's a function that writes the game stats to the terminal. */
void out();

/* It should be the newer version of out() and be able to show the game as if it
 * was real */
void pretty_out();

/* some kind of fill thing for pretty out. */
std::string fill(const int &num, const bool &basic, const std::string &str);

/* fill the waitingboard in pretty_out */
std::string print_wboard(const colour &cl, int num, const bool &first);

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

// please kick each other, because there is not enough space for two on only one
// field of the board
void knock_out(colour &tmp);

// array of five is not empty
bool not_empty_five(const bool array[5]);

// round for red, green, yellow
void rgy(colour &temp);

// needs to go home, because it's been kicked out.
void go_home(colour &col);

/* cout board */
std::string print_board(const int &num, const bool &first);

/* printing waiting rooms */
std::string print_wroom(const colour &cl, int num, const bool &first);

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

    blue.upper = "██";
    // blue.lower = "█▄█";

    red.upper = "██";
    // red.lower = "█▀▄";

    green.upper = "██";
    // green.lower = "▀▄█";

    yellow.upper = "██";
    // yellow.lower = " █ ";

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
                pretty_out();
            }
        } else {
            choice[0] = 'y';
            while (!end() &&
                   (choice[0] == 'y' || choice[0] == 'Y' || choice.empty())) {
                round();
                pretty_out();
                wait_term();
                std::cout << "Can the next round begin? [Y/n] ";
                // std::cin >> choice[0];
                getline(std::cin, choice, '\n');
            }
            pretty_out();
        }
    }
    return 0;
}

// writes game stats to terminal
void out() {
    wait_term();
    clear_term();

    std::cout << "board:\n";

    for (int i : board) {
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
        std::cout << BLUE << "\tblue:\t" << blue.current << RED << "\n\tred:\t"
                  << red.current << GREEN << "\n\tgreen:\t" << green.current
                  << YELLOW << "\n\tyellow:\t" << yellow.current << RESET;
    }

    std::cout << "\n\nwaiting rooms:\n";
    {
        std::cout << BLUE << "\tblue:\t";
        for (bool i : blue.waitingroom) {
            std::cout << i << " ";
        }
        std::cout << RED << "\n\tred:\t";
        for (bool i : red.waitingroom) {
            std::cout << i << " ";
        }
        std::cout << GREEN << "\n\tgreen:\t";
        for (bool i : green.waitingroom) {
            std::cout << i << " ";
        }
        std::cout << YELLOW << "\n\tyellow:\t";
        for (bool i : yellow.waitingroom) {
            std::cout << i << " ";
        }
        std::cout << RESET;
    }

    std::cout << "\n\nwaiting boards:\n";
    {
        std::cout << BLUE << "\tblue:\t";
        for (bool i : blue.waitingboard) {
            std::cout << i << " ";
        }
        std::cout << RED << "\n\tred:\t";
        for (bool i : red.waitingboard) {
            std::cout << i << " ";
        }
        std::cout << GREEN << "\n\tgreen:\t";
        for (bool i : green.waitingboard) {
            std::cout << i << " ";
        }
        std::cout << YELLOW << "\n\tyellow:\t";
        for (bool i : yellow.waitingboard) {
            std::cout << i << " ";
        }
    }

    std::cout << RESET << "\n\nhouses:\n";
    {
        std::cout << BLUE << "\tblue:\t";
        for (bool i : blue.house) {
            std::cout << i << " ";
        }
        std::cout << RED << "\n\tred:\t";
        for (bool i : red.house) {
            std::cout << i << " ";
        }
        std::cout << GREEN << "\n\tgreen:\t";
        for (bool i : green.house) {
            std::cout << i << " ";
        }
        std::cout << YELLOW << "\n\tyellow:\t";
        for (bool i : yellow.house) {
            std::cout << i << " ";
        }
    }

    std::cout << RESET << "\n\nknockouts:\n";
    std::cout << BLUE << "\tblue:\t" << blue.knock_out_num << RED
              << "\n\tred:\t" << red.knock_out_num << GREEN << "\n\tgreen:\t"
              << green.knock_out_num << YELLOW << "\n\tyellow:\t"
              << yellow.knock_out_num << RESET << std::endl;
}

// plays one round, in the future it should not be so long, it rather should be
// made by making a function, that does it for each colour
void round() {
    throw_cube(blue.current);
    // blue
    {
        // its waiting board is not empty
        if (not_empty_five(blue.waitingboard)) {
            if (blue.onboard + blue.current >= 5) {
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
        // near the house, needs to go to waitingboard (pl.:48-ason van 5ot
        // dobott)
        else if (blue.onboard + blue.current <= blue.end + 5) {
            board[blue.onboard] = 0;
            blue.onboard = blue.current - (blue.end - blue.onboard) - 1;
            blue.waitingboard[blue.onboard] = true;
        }
    }
    pretty_out();
    rgy(red);
    pretty_out();
    rgy(green);
    pretty_out();
    rgy(yellow);
    round_num++;
}

// clears terminal
void clear_term() {
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

// is there a figure from this colour on the board?
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
    /* if (blue.current == 6 && red.current == 6 && green.current == 6 &&
        yellow.current == 6) {
        wait_term();
        std::cout << "\nHooray! Everyone throw a six! This means everyone has "
                     "won !\n ";
        return true;
    } else */
    if (full_four(blue.house) || full_four(red.house) ||
        full_four(green.house) || full_four(yellow.house)) {
        // clear_term();
        std::cout << "\nONE OF THE ANIMALS GOT TO THE END, HOORAY, "
                     "HOORAY!\nNUMBER "
                     "OF ALL KNOCKOUTS: "
                  << blue.knock_out_num + red.knock_out_num +
                         green.knock_out_num + yellow.knock_out_num
                  << "\n";
        return true;
    } else
        return false;
}

// cross-platform wait
void wait_term() {
#ifdef _WIN32
    system("timeout \t 1");
#else
    system("sleep 0.08");
#endif
}

// functions that doesn't let two figures be on the same field
void knock_out(colour &tmp) {
    if (board[tmp.onboard] != 0) {
        switch (board[tmp.onboard]) {
        case 1:
            go_home(blue);
            break;
        case 2:
            go_home(red);
            break;
        case 3:
            go_home(green);
            break;
        case 4:
            go_home(yellow);
            break;
        default:
            std::cout << "___ERROR!___\n";
            break;
        }
        std::cout << "\nA KNOCKOUT HAS HAPPENED (KO) \n";
        wait_term();
    }
}

// checks if an array of five is not empty
bool not_empty_five(const bool array[5]) {
    if (array[0] || array[1] || array[2] || array[3] || array[4])
        return true;
    else
        return false;
}

// one round, unfortunately doesn't work for blue, so its red, green, yellow
void rgy(colour &temp) {
    throw_cube(temp.current);
    // its waiting board is not empty
    if (not_empty_five(temp.waitingboard)) {
        // goes directly into its house
        if (temp.onboard + temp.current >= 5) {
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
        // board is not empty
    } else if (!temp.was_at_the_end) {
        // start movement
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
    // start movement until it reaches the waiting board
    else if (temp.onboard + temp.current <= temp.end) {
        board[temp.onboard] = 0;
        temp.onboard += temp.current;
        knock_out(temp);
        ;
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

// if a figure's been kicked out, it needs to go back to its waitingroom
void go_home(colour &col) {
    // col.onboard = col.start;
    col.was_at_the_end = false;
    col.onboard = 0;
    col.idx--;
    col.waitingroom[col.idx] = true;
    col.knock_out_num++;
}

// it throws the dice, can easily be changed to manual dice roll
void throw_cube(int &current) {
    current = rand() % 6 + 1;
    /* current = 0;
    do {
        std::cout << "What now: ";
        std::cin >> current;
        // 2147483647
    } while (current < 1 || current > 6); */
}

// pretty out
void pretty_out() {
    wait_term();
    clear_term();

    // 1: 1/2
    std::cout << RED << " " << red.current << RESET << "  " << fill(5, true, "")
              << print_board(23, true) << print_board(24, true)
              << print_board(25, true) << fill(5, true, "") << "  " << GREEN
              << green.current << " " << RESET << "\t" << round_num << "\n\n";

    // 2: 1/2
    std::cout << fill(6, true, "") << print_board(22, true)
              << print_wboard(green, 0, true) << print_board(26, true)
              << fill(6, true, "") << "\n\n";

    // 3: 1/2
    std::cout << fill(2, true, "") << print_wroom(red, 0, true)
              << print_wroom(red, 1, true) << fill(2, true, "")
              << print_board(21, true) << print_wboard(green, 1, true)
              << print_board(27, true) << fill(2, true, "")
              << print_wroom(green, 0, true) << print_wroom(green, 1, true)
              << fill(2, true, "") << "\n\n";

    // 4: 1/2
    std::cout << fill(2, true, "") << print_wroom(red, 2, true)
              << print_wroom(red, 3, true) << fill(2, true, "")
              << print_board(20, true) << print_wboard(green, 2, true)
              << print_board(28, true) << fill(2, true, "")
              << print_wroom(green, 2, true) << print_wroom(green, 3, true)
              << fill(2, true, "") << "\n\n";

    // 5: 1/2
    std::cout << fill(6, true, "") << print_board(19, true)
              << print_wboard(green, 3, true) << print_board(29, true)
              << fill(6, true, "") << "\n\n";

    // 6: 1/2
    std::cout << fill(6, true, "") << print_board(18, true)
              << print_wboard(green, 4, true) << print_board(30, true)
              << fill(6, true, "") << "\n\n";

    // 7: 1/2
    std::cout << print_board(12, true) << print_board(13, true)
              << print_board(14, true) << print_board(15, true)
              << print_board(16, true) << print_board(17, true)
              << fill(3, true, "") << print_board(31, true)
              << print_board(32, true) << print_board(33, true)
              << print_board(34, true) << print_board(35, true)
              << print_board(36, true) << "\n\n";

    // 8: 1/2
    std::cout << print_board(11, true);
    for (int i = 0; i < 5; i++) {
        std::cout << print_wboard(red, i, true);
    }
    std::cout << fill(3, true, "");
    for (int i = 5 - 1; i >= 0; i--) {
        std::cout << print_wboard(yellow, i, true);
    }
    std::cout << print_board(37, true) << "\n\n";

    // 9: 1/2
    std::cout << print_board(10, true) << print_board(9, true)
              << print_board(8, true) << print_board(7, true)
              << print_board(6, true) << print_board(5, true)
              << fill(3, true, "") << print_board(43, true)
              << print_board(42, true) << print_board(41, true)
              << print_board(40, true) << print_board(39, true)
              << print_board(38, true) << "\n\n";

    // 10: 1/2
    std::cout << fill(6, true, "") << print_board(4, true)
              << print_wboard(blue, 4, true) << print_board(44, true)
              << fill(6, true, "") << "\n\n";

    // 11: 1/2
    std::cout << fill(6, true, "") << print_board(3, true)
              << print_wboard(blue, 3, true) << print_board(45, true)
              << fill(6, true, "") << "\n\n";

    // 12: 1/2
    std::cout << fill(2, true, "") << print_wroom(blue, 0, true)
              << print_wroom(blue, 1, true) << fill(2, true, "")
              << print_board(2, true) << print_wboard(blue, 2, true)
              << print_board(46, true) << fill(2, true, "")
              << print_wroom(yellow, 0, true) << print_wroom(yellow, 1, true)
              << fill(2, true, "") << "\n\n";

    // 13: 1/2
    std::cout << fill(2, true, "") << print_wroom(blue, 2, true)
              << print_wroom(blue, 3, true) << fill(2, true, "")
              << print_board(1, true) << print_wboard(blue, 1, true)
              << print_board(47, true) << fill(2, true, "")
              << print_wroom(yellow, 2, true) << print_wroom(yellow, 3, true)
              << fill(2, true, "") << "\n\n";

    // 14: 1/2
    std::cout << fill(6, true, "") << print_board(0, true)
              << print_wboard(blue, 0, true) << print_board(48, true)
              << fill(6, true, "") << "\n\n";

    // 15: 1/2
    std::cout << " " << BLUE << blue.current << RESET << "  "
              << fill(5, true, "") << print_board(51, true)
              << print_board(50, true) << print_board(49, true)
              << fill(5, true, "") << YELLOW << "  " << yellow.current << RESET
              << " "
              << "\n\n";
}

std::string fill(const int &num, const bool &basic, const std::string &str) {
    std::string fill_with;
    if (basic) {
        for (int i = 0; i < num; i++) {
            // fill_with += " ████ ";
            // fill_with += " ░░ ";
            fill_with += "    ";
            // fill_with += " ▆▆ ";
        }
    } else {
        for (int i = 0; i < num; i++) {
            fill_with += str;
        }
    }
    return fill_with;
}

std::string print_board(const int &num, const bool &first) {
    std::string print_this = " ";
    if (first) {
        switch (board[num]) {
        case 0:
            print_this = " __";
            break;
        case 1:
            print_this += BLUE;
            print_this += blue.upper;
            break;
        case 2:
            print_this += RED;
            print_this += red.upper;
            break;
        case 3:
            print_this += GREEN;
            print_this += green.upper;
            break;
        case 4:
            print_this += YELLOW;
            print_this += yellow.upper;
            break;
        default:
            print_this = "___!!!error!!!___";
        }
    } else {
        switch (board[num]) {
        case 0:
            print_this = " __";
            break;
        case 1:
            print_this += BLUE;
            print_this += blue.lower;
            break;
        case 2:
            print_this += RED;
            print_this += red.lower;
            break;
        case 3:
            print_this += GREEN;
            print_this += green.lower;
            break;
        case 4:
            print_this += YELLOW;
            print_this += yellow.lower;
            break;
        default:
            print_this = "___!!!error!!!___";
        }
    }
    print_this += " ";
    print_this += RESET;
    return print_this;
}

std::string print_wboard(const colour &cl, int num, const bool &first) {
    std::string print_this = " ..";
    if (cl.waitingboard[num]) {
        if (first) {
            print_this = " ";
            switch (cl.colour_num) {
            case 1:
                print_this += BLUE;
                print_this += blue.upper;
                break;
            case 2:
                print_this += RED;
                print_this += red.upper;
                break;
            case 3:
                print_this += GREEN;
                print_this += green.upper;
                break;
            case 4:
                print_this += YELLOW;
                print_this += yellow.upper;
                break;
            default:
                print_this = "___!!!error!!!___";
            }
        } else {
            print_this = " ";
            switch (cl.colour_num) {
            case 1:
                print_this += BLUE;
                print_this += blue.lower;
                break;
            case 2:
                print_this += RED;
                print_this += red.lower;
                break;
            case 3:
                print_this += GREEN;
                print_this += green.lower;
                break;
            case 4:
                print_this += YELLOW;
                print_this += yellow.lower;
                break;
            default:
                print_this = "___!!!error!!!___";
            }
        }
    }
    print_this += " ";
    print_this += RESET;
    return print_this;
}

std::string print_wroom(const colour &cl, int num, const bool &first) {
    std::string print_this = " ..";
    if (cl.waitingroom[num]) {
        if (first) {
            print_this = " ";
            switch (cl.colour_num) {
            case 1:
                print_this += BLUE;
                print_this += blue.upper;
                break;
            case 2:
                print_this += RED;
                print_this += red.upper;
                break;
            case 3:
                print_this += GREEN;
                print_this += green.upper;
                break;
            case 4:
                print_this += YELLOW;
                print_this += yellow.upper;
                break;
            default:
                print_this = "___!!!error!!!___";
            }
        } else {
            print_this = " ";
            switch (cl.colour_num) {
            case 1:
                print_this += BLUE;
                print_this += blue.lower;
                break;
            case 2:
                print_this += RED;
                print_this += red.lower;
                break;
            case 3:
                print_this += GREEN;
                print_this += green.lower;
                break;
            case 4:
                print_this += YELLOW;
                print_this += yellow.lower;
                break;
            default:
                print_this = "___!!!error!!!___";
            }
        }
    }
    print_this += " ";
    print_this += RESET;
    return print_this;
}
