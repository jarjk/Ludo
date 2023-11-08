/* so this is ludo / ki nevet a végén / Mensch, ärgere dich nicht! */

#include <ctime>
#include <iostream>
#include <string>

// gameboard, nothing: _ = 0, blue: B = 1, red: R = 2, green: G = 3, yellow: Y = 4
int board[52]{};

struct colour {
    bool waitingboard[5]{}, waitingroom[4]{true, true, true, true}, house[4]{},
        was_at_the_end{};
    int current{}, onboard{}, idx{}, idx_house{};
    int start{}, end{}, colour_num{};
    char ch{};
};

colour blue, red, green, yellow;

/* It's a function that writes the game stats to the terminal. */
void out();
/* It's a function that throws the cube. */
void throw_cube(int &current);
/* It's checking if an array of four is empty. */
bool full_four(bool arr[4]);
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
int knockout_num = 0;
// array of five is not empty
bool not_empty_five(bool array[5]);
// round for red, green, yellow
void rgy(colour &temp);
// needs to go home, because it's been kicked out.
void go_home(colour col);

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
    if (choice == "" || choice[0] == 'y' || choice[0] == 'Y') {
        choice = "";
        std::cout
            << "Do you want to play a full game without doing anything? [Y/n] ";
        getline(std::cin, choice, '\n');
        if (choice == "" || choice[0] == 'y' || choice[0] == 'Y') {
            while (!end()) {
                round();
                out();
            }
        } else {
            choice[0] = 'y';
            while (!end() && (choice[0] == 'y' || choice[0] == 'Y' ||
                              choice[0] == '\n')) {
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

    for (int i = 0; i < 52; i++) {
        if (board[i] == 0)
            std::cout << "_  ";
        else if (board[i] == blue.colour_num)
            std::cout << blue.ch << "  ";
        else if (board[i] == red.colour_num)
            std::cout << red.ch << "  ";
        else if (board[i] == green.colour_num)
            std::cout << green.ch << " ";
        else if (board[i] == yellow.colour_num)
            std::cout << yellow.ch << "  ";
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
        std::cout << "\tblue:\t" << blue.current << "\n\tred:\t" << red.current
                  << "\n\tgreen:\t" << green.current << "\n\tyellow:\t"
                  << yellow.current;
    }

    std::cout << "\n\nwaiting rooms:\n";
    {
        std::cout << "\tblue:\t";
        for (int i = 0; i < 4; i++) {
            std::cout << blue.waitingroom[i] << " ";
        }
        std::cout << "\n\tred:\t";
        for (int i = 0; i < 4; i++) {
            std::cout << red.waitingroom[i] << " ";
        }
        std::cout << "\n\tgreen:\t";
        for (int i = 0; i < 4; i++) {
            std::cout << green.waitingroom[i] << " ";
        }
        std::cout << "\n\tyellow:\t";
        for (int i = 0; i < 4; i++) {
            std::cout << yellow.waitingroom[i] << " ";
        }
    }

    std::cout << "\n\nwaiting boards:\n";
    {
        std::cout << "\tblue:\t";
        for (int i = 0; i < 5; i++) {
            /*if (blue.waitingboard[i] == true)
              {
              std::cout << "B ";
              }
              else
              std::cout << "_ ";
              */
            std::cout << blue.waitingboard[i] << " ";
        }
        std::cout << "\n\tred:\t";
        for (int i = 0; i < 5; i++) {
            std::cout << red.waitingboard[i] << " ";
        }
        std::cout << "\n\tgreen:\t";
        for (int i = 0; i < 5; i++) {
            std::cout << green.waitingboard[i] << " ";
        }
        std::cout << "\n\tyellow:\t";
        for (int i = 0; i < 5; i++) {
            /*if (yellow.waitingboard[i] == true)
              std::cout << "Y ";
              else
              std::cout << "_ ";
              */
            std::cout << yellow.waitingboard[i] << " ";
        }
    }

    std::cout << "\n\nhouses:\n";
    {
        std::cout << "\tblue:\t";
        for (int i = 0; i < 4; i++) {
            std::cout << blue.house[i] << " ";
        }
        std::cout << "\n\tred:\t";
        for (int i = 0; i < 4; i++) {
            std::cout << red.house[i] << " ";
        }
        std::cout << "\n\tgreen:\t";
        for (int i = 0; i < 4; i++) {
            std::cout << green.house[i] << " ";
        }
        std::cout << "\n\tyellow:\t";
        for (int i = 0; i < 4; i++) {
            std::cout << yellow.house[i] << " ";
        }
        std::cout << std::endl << std::endl;
    }
}

// plays one round, in the future it should not be so long, it rather should be
// made by making a function, that does it for each colour
void round() {
    throw_cube(blue.current);
    // blue
    {
        // its waiting board is not empty
        if (not_empty_five(blue.waitingboard)) {
            if (blue.onboard + blue.current >= 5 - blue.onboard) {
                blue.waitingboard[blue.onboard] = 0;
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
        // not near it's house
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
    rgy(red);
    rgy(green);
    rgy(yellow);
}

void clear_term() {
#ifdef _WIN64
    system("CLS");
#elif _WIN32
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

void throw_cube(int &current) { current = rand() % 6 + 1; }
// needed for function testing:
// current = 6;

// checks if an array of four is full
bool full_four(bool arr[4]) {
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
        std::cout
            << "Hooray! Everyone throw a six! This means everyone has won!\n";
        exit(0);
        return 0;
    } else if (full_four(blue.house) || full_four(red.house) ||
               full_four(green.house) || full_four(yellow.house)) {
        // clear_term();
        std::cout << "\nONE OF THE ANIMALS GOT TO THE END, HOORAY, "
                     "HOORAY!\nNUMBER OF ALL KNOCKOUTS: "
                  << knockout_num << "\n\n";
        exit(0);
        return 0;
    } else
        return false;
}

void wait_term() {
#ifdef _WIN64
    system("timeout \t 1");
#elif _WIN32
    system("timeout \t 1");
#else
    system("sleep 0.02");
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
            blue.onboard = blue.start - 1;
            blue.idx--;
            blue.waitingroom[blue.idx] = true;
            break;
        case 2:
            red.onboard = red.start - 1;
            red.idx--;
            red.waitingroom[red.idx] = true;
            break;
        case 3:
            green.onboard = green.start - 1;
            green.idx--;
            green.waitingroom[green.idx] = true;
            break;
        case 4:
            yellow.onboard = yellow.start - 1;
            yellow.idx--;
            yellow.waitingroom[yellow.idx] = true;
            // go_home(yellow);
            break;
        default:
            std::cout << "___ERROR!___\n";
            break;
        }
        knockout_num++;
        std::cout << "\nA KNOCKOUT HAS HAPPENED (KO) \n";
        wait_term();
    }
}

bool not_empty_five(bool array[5]) {
    if (array[0] == true || array[1] == true || array[2] == true ||
        array[3] == true || array[4] == true)
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
            temp.waitingboard[temp.onboard] = 0;
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

void go_home(colour col) {
    col.onboard = col.start - 1;
    col.idx--;
    col.waitingroom[col.idx] = true;
}
