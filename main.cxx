/* 2023-08-22
 * This is: ludo / ki nevet a végén / Mensch, ärgere dich nicht!
 * It would be nice to know whether it works or just looks like it does.
 * Requires many many tests which I don't want to perform. Would be nice to have
 * some kind of smooth_out. The pretty_out is now capable of printing houses.
 * TODO: Also should be rewritten to comply to OOP. */

// includes
#include <array>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <thread>

using std::array;
using std::cin;
using std::cout;
using std::string;

/* the following are UBUNTU/LINUX, and macOS ONLY terminal color codes,
 * but it works in Windows 10 too under VS2019.*/
constexpr auto RESET = "\033[0m";   // standard colour
constexpr auto BLUE = "\033[34m";   // Blue
constexpr auto RED = "\033[31m";    // Red
constexpr auto GREEN = "\033[32m";  // Green
constexpr auto YELLOW = "\033[33m"; // Yellow

// gameboard: nothing: _ = 0, Blue: 1, Red: 2, Green: 3, Yellow: 4
array<int, 52> board{};

// the structure that contains data for each colour
struct colour {
    // the place between where their round ends and the board.
    array<bool, 5> waitingboard{};
    /* this is where they're before starting the game. They also get back here
     * if kicked out. starts as full*/
    array<bool, 4> waitingroom{true, true, true, true};
    // the place where they get when finished
    array<bool, 4> house{};
    /* have they been to the end of the board? needed for red, green, yellow*/
    bool was_at_the_end{};
    /* the current number they'll have to go.*/
    int current{};
    /* it shows where each of the players currently are.*/
    int onboard = -1;
    /* */
    int idx{};
    /* */
    int idx_house{};
    /* it shows the number of times they've been knocked out */
    int knock_out_num{};
    /* the startfield on the board*/
    int start{};
    /* the field on which they last step before going to the waitingboard or
     * house.*/
    int end{};
    /* identification of Blue: 1, Red: 2, Green: 3, Yellow: 4 */
    int colour_num{};
};

colour blue, red, green, yellow;

// how should the figures look like on the board
constexpr auto show_as = "▇▇";
// counting the number of rounds
int round_num = 0;

/* functions */

/* It's a function that throws the cube. */
void throw_cube(int &current);

/* It's checking if an array of four is empty. */
bool full_four(array<bool, 4> arr);

/* It's checking if the game is over. */
bool end();

/* It's playing one round. */
void round();

/* It's clearing the terminal. And it's CROSS-PLATFORM! */
void clear_term();

// cross-platform wait
void wait_term();
void wait_term(const int &microseconds);

// is there a player from this colour on the board?
bool is_on_board(const int &color);

// please kick each other, because there is not enough space for two on only one
// field of the board
void knock_out(colour &tmp);

// array of five is not empty
bool not_empty_five(array<bool, 5> arr);

// round for red, green, yellow
void rgy(colour &tmp);

// needs to go home, because it's been kicked out.
void go_home(colour &color);

/* the function for printing the whole stuff to console */
void pretty_out();

/* some kind of fill thing for pretty out. */
string fill(const int &num);

/* print the waitingboard for pretty_out */
string print_wboard(const colour &color, int num);

/* printing board for pretty_out */
string print_board(const int &num);

/* printing waiting rooms */
string print_wroom(const colour &color, int num);

/* printing houses for pretty_out */
string print_house(const colour &color, int num);

int main() {

    // setting stuff should be a kind of constant i guess
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

    clear_term();

    cout << "Are you ready to start this beautiful game? [Y/n] ";
    string choice = "y";
    getline(cin, choice, '\n');
    if (choice.empty() || choice[0] == 'y' || choice[0] == 'Y') {
        choice = "Y";
        cout
            << "Do you want to play a full game without doing anything? [Y/n] ";
        getline(cin, choice, '\n');
        if (choice.empty() || choice[0] == 'y' || choice[0] == 'Y') {
            while (!end()) {
                round();
                pretty_out();
            }
        } else {
            choice = 'y';
            while (!end() &&
                   (choice[0] == 'y' || choice[0] == 'Y' || choice.empty())) {
                round();
                pretty_out();
                wait_term();
                cout << "Can the next round begin? [Y/n] ";
                getline(cin, choice, '\n');
            }
            pretty_out();
        }
    }
    return 0;
}

// plays one round, made by making a function, that does it for each colour
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

// one round, unfortunately doesn't work for blue, so its red, green, yellow
void rgy(colour &tmp) {
    throw_cube(tmp.current);
    // its waiting board is not empty
    if (not_empty_five(tmp.waitingboard)) {
        // goes directly into its house
        if (tmp.onboard + tmp.current >= 5) {
            tmp.waitingboard[tmp.onboard] = false;
            tmp.house[tmp.idx_house] = true;
            tmp.onboard = 0;
            tmp.idx_house++;
        }
        // goes into its house slowly
        else {
            tmp.waitingboard[tmp.onboard] = false;
            tmp.onboard = tmp.onboard + tmp.current;
            tmp.waitingboard[tmp.onboard] = true;
        }
    }
    // board is empty
    else if (!is_on_board(tmp.colour_num)) {
        // can go to the board from its waiting room
        if (tmp.current == 6) {
            tmp.onboard = tmp.start;
            knock_out(tmp);
            board[tmp.onboard] = tmp.colour_num;
            tmp.waitingroom[tmp.idx] = false;
            tmp.idx++;
        }
        // board is not empty
    } else if (!tmp.was_at_the_end) {
        // start movement
        if (tmp.onboard + tmp.current < 52) {
            board[tmp.onboard] = 0;
            tmp.onboard += tmp.current;
            knock_out(tmp);
            board[tmp.onboard] = tmp.colour_num;
        }
        // needs to go to the start of the board
        else if (tmp.onboard + tmp.current > 51) {
            board[tmp.onboard] = 0;
            tmp.onboard = tmp.current - (52 - tmp.onboard);
            knock_out(tmp);
            board[tmp.onboard] = tmp.colour_num;
            tmp.was_at_the_end = true;
        }
    }
    // start movement until it reaches the waiting board
    else if (tmp.onboard + tmp.current <= tmp.end) {
        board[tmp.onboard] = 0;
        tmp.onboard += tmp.current;
        knock_out(tmp);
        board[tmp.onboard] = tmp.colour_num;
    }
    // goes right into house
    else if (tmp.onboard + tmp.current == tmp.end + 6) {
        board[tmp.onboard] = 0;
        tmp.house[tmp.idx_house] = true;
        tmp.idx_house++;
        tmp.was_at_the_end = false;
    }
    // goes into its waiting board
    else if (tmp.onboard + tmp.current <= tmp.end + 5) {
        board[tmp.onboard] = 0;
        tmp.onboard = tmp.current - (tmp.end - tmp.onboard) - 1;
        tmp.waitingboard[tmp.onboard] = true;
        tmp.was_at_the_end = false;
    }
}

// clears terminal
void clear_term() {
    cout << "\033[2J\033[1;1H";
    /* #ifdef _WIN32
       system("CLS");
  #else
  system("clear");
  #endif */
}

// cross-platform wait
void wait_term() {
    std::this_thread::sleep_for(std::chrono::microseconds(4000));
}
void wait_term(const int &microseconds) {
    std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
}

// is there a figure from this colour on the board?
bool is_on_board(const int &color) {
    int idx = 0;
    while (board[idx] != color && idx < 52) {
        idx++;
    }
    return idx < 52;
}

// checks if an array of four is full
bool full_four(array<bool, 4> arr) {
    return arr[0] && arr[1] && arr[2] && arr[3];
}

// checks if an array of five is not empty
bool not_empty_five(array<bool, 5> arr) {
    return arr[0] || arr[1] || arr[2] || arr[3] || arr[4];
}

// checks if it's the end of the game
bool end() {
    if (full_four(blue.house) || full_four(red.house) ||
        full_four(green.house) || full_four(yellow.house)) {
        cout << "\nONE OF THE ANIMALS GOT TO THE END, HOORAY, "
                "HOORAY!\nNUMBER OF ALL KNOCKOUTS: "
             << blue.knock_out_num + red.knock_out_num + green.knock_out_num +
                    yellow.knock_out_num
             << "\n";
        return true;
    }
    return false;
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
            cout << "___ERROR!___\n";
            break;
        }
        cout << "\nA KNOCKOUT HAS HAPPENED (KO) \n";
        cout << "number of current knockouts: " << BLUE << blue.knock_out_num
             << " " << RED << red.knock_out_num << " " << GREEN
             << green.knock_out_num << " " << YELLOW << yellow.knock_out_num
             << "\n";
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        wait_term(1000000);
    }
}

// if a figure's been kicked out, it needs to go back to its waitingroom
void go_home(colour &color) {
    color.was_at_the_end = false;
    color.onboard = 0;
    color.idx--;
    color.waitingroom[color.idx] = true;
    color.knock_out_num++;
}

// it throws the dice, can easily be changed to manual dice roll
void throw_cube(int &current) {
    std::mt19937 mt{std::random_device{}()};
    std::uniform_int_distribution<> die6{1, 6};
    // current = rand() % 6 + 1;
    current = die6(mt);

    // current = random_device() * std::random_device() *
    // random_device() ;
    /* current = 0;
       do {
       cout << "What now: ";
       cin >> current;
    // 2147483647
    } while (current < 1 || current > 6); */
}

// pretty out
void pretty_out() {
    wait_term();
    clear_term();

    // 0
    // cout << "╔" << fill(60, false, "═") << "╗\n";
    // 1: 1/2
    cout /*<< "║"*/ << RED << " " << red.current << RESET << "  " << fill(5)
                    << print_board(23) << print_board(24) << print_board(25)
                    << fill(5) << "  " << GREEN << green.current << " " << RESET
                    << "\t" << round_num << "\n\n";

    // 2: 1/2
    cout << fill(6) << print_board(22) << print_wboard(green, 0)
         << print_board(26) << fill(6) << "\n\n";

    // 3: 1/2
    cout << fill(2) << print_wroom(red, 0) << print_wroom(red, 1) << fill(2)
         << print_board(21) << print_wboard(green, 1) << print_board(27)
         << fill(2) << print_wroom(green, 0) << print_wroom(green, 1) << fill(2)
         << "\n\n";

    // 4: 1/2
    cout << fill(2) << print_wroom(red, 2) << print_wroom(red, 3) << fill(2)
         << print_board(20) << print_wboard(green, 2) << print_board(28)
         << fill(2) << print_wroom(green, 2) << print_wroom(green, 3) << fill(2)
         << "\n\n";

    // 5: 1/2
    cout << fill(6) << print_board(19) << print_wboard(green, 3)
         << print_board(29) << fill(6) << "\n\n";

    // 6: 1/2
    cout << fill(6) << print_board(18) << print_wboard(green, 4)
         << print_board(30) << fill(6) << "\n\n";

    // 7: 1/2
    cout << print_board(12) << print_board(13) << print_board(14)
         << print_board(15) << print_board(16) << print_board(17)
         << print_house(green, 0) << print_house(green, 1)
         << print_house(green, 2) << print_house(green, 3) << " "
         << print_house(yellow, 0) << print_board(31) << print_board(32)
         << print_board(33) << print_board(34) << print_board(35)
         << print_board(36) << "\n"
         << fill(6) << print_house(red, 0) << "       "
         << print_house(yellow, 1) << "\n";

    // 8: 1/2
    cout << print_board(11);
    for (int i = 0; i < 5; i++) {
        cout << print_wboard(red, i);
    }
    cout << print_house(red, 1) << "       " << print_house(yellow, 2);
    for (int i = 5 - 1; i >= 0; i--) {
        cout << print_wboard(yellow, i);
    }
    cout << print_board(37) << "\n"
         << fill(6) << print_house(red, 2) << "       "
         << print_house(yellow, 3) << "\n";

    // 9: 1/2
    cout << print_board(10) << print_board(9) << print_board(8)
         << print_board(7) << print_board(6) << print_board(5)
         << print_house(red, 3) << " " << print_house(blue, 0)
         << print_house(blue, 1) << print_house(blue, 2) << print_house(blue, 3)
         << print_board(43) << print_board(42) << print_board(41)
         << print_board(40) << print_board(39) << print_board(38) << "\n\n";

    // 10: 1/2
    cout << fill(6) << print_board(4) << print_wboard(blue, 4)
         << print_board(44) << fill(6) << "\n\n";

    // 11: 1/2
    cout << fill(6) << print_board(3) << print_wboard(blue, 3)
         << print_board(45) << fill(6) << "\n\n";

    // 12: 1/2
    cout << fill(2) << print_wroom(blue, 0) << print_wroom(blue, 1) << fill(2)
         << print_board(2) << print_wboard(blue, 2) << print_board(46)
         << fill(2) << print_wroom(yellow, 0) << print_wroom(yellow, 1)
         << fill(2) << "\n\n";

    // 13: 1/2
    cout << fill(2) << print_wroom(blue, 2) << print_wroom(blue, 3) << fill(2)
         << print_board(1) << print_wboard(blue, 1) << print_board(47)
         << fill(2) << print_wroom(yellow, 2) << print_wroom(yellow, 3)
         << fill(2) << "\n\n";

    // 14: 1/2
    cout << fill(6) << print_board(0) << print_wboard(blue, 0)
         << print_board(48) << fill(6) << "\n\n";

    // 15: 1/2
    cout << " " << BLUE << blue.current << RESET << "  " << fill(5)
         << print_board(51) << print_board(50) << print_board(49) << fill(5)
         << YELLOW << "  " << yellow.current << RESET << " "
         << "\n\n";
}

string fill(const int &num) {
    string fill_with;
    for (int i = 0; i < num; i++) {
        // fill_with += " ██ ";
        // fill_with += " ░░ ";
        // fill_with += " ▆▆ ";
        fill_with += "    ";
    }
    return fill_with;
}

string print_board(const int &num) {
    string print_this = " ";
    switch (board[num]) {
    case 0:
        print_this = " __";
        break;
    case 1:
        print_this += BLUE;
        print_this += show_as;
        break;
    case 2:
        print_this += RED;
        print_this += show_as;
        break;
    case 3:
        print_this += GREEN;
        print_this += show_as;
        break;
    case 4:
        print_this += YELLOW;
        print_this += show_as;
        break;
    default:
        print_this = "___!!!error!!!___";
    }
    print_this += " ";
    print_this += RESET;
    return print_this;
}

string print_wboard(const colour &color, int num) {
    string print_this = " ..";
    if (color.waitingboard[num]) {
        print_this = " ";
        switch (color.colour_num) {
        case 1:
            print_this += BLUE;
            print_this += show_as;
            break;
        case 2:
            print_this += RED;
            print_this += show_as;
            break;
        case 3:
            print_this += GREEN;
            print_this += show_as;
            break;
        case 4:
            print_this += YELLOW;
            print_this += show_as;
            break;
        default:
            print_this = "___!!!error!!!___";
        }
    }
    print_this += " ";
    print_this += RESET;
    return print_this;
}

string print_wroom(const colour &color, int num) {
    string print_this = " ..";
    if (color.waitingroom[num]) {
        print_this = " ";
        switch (color.colour_num) {
        case 1:
            print_this += BLUE;
            print_this += show_as;
            break;
        case 2:
            print_this += RED;
            print_this += show_as;
            break;
        case 3:
            print_this += GREEN;
            print_this += show_as;
            break;
        case 4:
            print_this += YELLOW;
            print_this += show_as;
            break;
        default:
            print_this = "___!!!error!!!___";
        }
    }
    print_this += " ";
    print_this += RESET;
    return print_this;
}

string print_house(const colour &color, int num) {
    string print_this = "  ";
    if (color.house[num]) {
        print_this = " ";
        switch (color.colour_num) {
        case 1:
            print_this += BLUE;
            print_this += "▄";
            break;
        case 2:
            print_this += RED;
            print_this += "▄";
            break;
        case 3:
            print_this += GREEN;
            print_this += "▄";
            break;
        case 4:
            print_this += YELLOW;
            print_this += "▄";
            break;
        default:
            print_this = "___!!!error!!!___";
        }
    }
    print_this += RESET;
    return print_this;
}
