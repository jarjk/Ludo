#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

using std::cout;
using std::string;
using std::vector;

class Ludo {
  private:
    vector<int> board;
    int round_num = 0;
    const string show_as;

  public:
    Ludo(const vector<int> &tmp_board, const string &tmp_show_as)
        : board(tmp_board), show_as(tmp_show_as) {}
    void set_board(const int &at, const int &num);
    int get_board(const int &at);
    class Functions {
      public:
        static void clear_term();
        static void wait(const int &time);
        static void change_colour(const int &colour_num);
        static void change_colour(const string &colour);
    };
    class Colours {
      private:
        // The number used for identifying the colour.
        const int colour_num;
        // Start on the board.
        const int start;
        // End on the board.
        const int end;
        string colour_name;
        // It shows where each of the players currently are.
        int onboard = -1;

        // The number of steps it has to go.
        int current_throw{};
        /* This is where they're before starting the game. They also get back
         * here if kicked out. Starts as full. */
        vector<bool> waitingroom = {true, true, true, true};
        int waitingroom_idx{};
        // the place they have to reach
        vector<bool> house;
        int house_idx{};
        // place between house and board
        vector<bool> waitingboard;

        bool was_at_the_end{};
        int knockout_num{};

      public:
        Colours(const int &tmp_start, const int &tmp_end,
                const int &tmp_colour_num)
            : start(tmp_start), end(tmp_end), colour_num(tmp_colour_num) {}
        void throw_die();
        bool full_house();
        bool not_empty_waitingboard();
        bool is_on_board();
    };
};

int main() {
    vector<int> board_vect(52, 0);
    Ludo ludo(board_vect, "▇▇");

    Ludo::Colours Red(1, 0, 50);
    Ludo::Colours Green(2, 13, 11);
    Ludo::Colours Yellow(3, 26, 24);
    Ludo::Colours Blue(4, 39, 37);

    cout << "can we start?";
    std::cin.get();

    // Colours::Functions::clear_term();
    // Colours::Functions::change_colour("blue");
    // cout << "oyy\nwhatup\nis this good fun?";
    // Colours::Functions::wait(1000000);
    // Colours::Functions::clear_term();
    // Colours::Functions::change_colour(1);
    // cout << "cleared?\n";
    // Colours::Functions::change_colour("green");
    // cout << "changed colour?\n";
    // Colours::Functions::change_colour("yellow");
    // cout << "if yes, it\'s pretty cool!";
    // Colours::Functions::change_colour(4);
    // cout << "\t\"ain\'t it?\"";
    // Colours::Functions::change_colour(0);

    cout << "\n";
    return 0;
}

void Ludo::Functions::wait(const int &time) {
    std::this_thread::sleep_for(std::chrono::microseconds(time));
}

void Ludo::Colours::throw_die() {
    std::mt19937 mt{std::random_device{}()};
    std::uniform_int_distribution<> die6{1, 6};
    current_throw = die6(mt);
}

void Ludo::Functions::clear_term() { cout << "\033[2J\033[1;1H"; }

void Ludo::Functions::change_colour(const int &colour_num) {
    switch (colour_num) {
    case 0:
        // reset
        cout << "\033[0m";
        break;
    case 1:
        // red
        cout << "\033[31m";
        break;
    case 2:
        // green
        cout << "\033[32m";
        break;
    case 3:
        // yellow
        cout << "\033[33m";
        break;
    case 4:
        // blue
        cout << "\033[34m";
        break;
    default:
        cout << "not good colour!";
    }
}

void Ludo::Functions::change_colour(const string &colour) {
    if (colour == "reset") {
        cout << "\033[0m";
    } else if (colour == "red") {
        cout << "\033[31m";
    } else if (colour == "green") {
        cout << "\033[32m";
    } else if (colour == "yellow") {
        cout << "\033[33m";
    } else if (colour == "blue") {
        cout << "\033[34m";
    } else {
        cout << "not good colour!";
    }
}

bool Ludo::Colours::full_house() {
    if (house.size() == 4) {
        return house.at(0) && house.at(1) && house.at(2) && house.at(3);
    }
    cout << "something wronk with houses size";
    std::cin.get();
    return false;
}

void Ludo::set_board(const int &at, const int &num) { board.at(at) = num; }

int Ludo::get_board(const int &at) { return board.at(at); }

bool Ludo::Colours::not_empty_waitingboard() {
    if (waitingboard.size() == 5) {
        return waitingboard.at(0) || waitingboard.at(1) || waitingboard.at(2) ||
               waitingboard.at(3) || waitingboard.at(4);
    }
    cout << "something wronk with waitingboards size";
    std::cin.get();
    return false;
}

bool Ludo::Colours::is_on_board() {
    int i = 0;
    while (i < 51 && get_board(i) != colour_num) {
    }
}
