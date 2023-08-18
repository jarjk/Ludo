#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
// the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define RESET "\033[0m"
#define BLACK "\033[30m" /* Black */
// #define RED "\033[31m"                /* Red */
constexpr auto RED = "\033[31m" /* Red */;
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
// #define BOLDWHITE \
//     "\033[1m\033[37m" /* Bold White */ \
//     }
constexpr auto GREEN = "\033[32m";              /* Green */
constexpr auto YELLOW = "\033[33m";             /* Yellow */
constexpr auto BLUE = "\033[34m";               /* Blue */
constexpr auto MAGENTA = "\033[35m";            /* Magenta */
constexpr auto CYAN = "\033[36m";               /* Cyan */
constexpr auto WHITE = "\033[37m";              /* White */
constexpr auto BOLDBLACK = "\033[1m\033[30m";   /* Bold Black */
constexpr auto BOLDRED = "\033[1m\033[31m";     /* Bold Red */
constexpr auto BOLDGREEN = "\033[1m\033[32m";   /* Bold Green */
constexpr auto BOLDYELLOW = "\033[1m\033[33m";  /* Bold Yellow */
constexpr auto BOLDBLUE = "\033[1m\033[34m";    /* Bold Blue */
constexpr auto BOLDMAGENTA = "\033[1m\033[35m"; /* Bold Magenta */
constexpr auto BOLDCYAN = "\033[1m\033[36m";    /* Bold Cyan */
constexpr auto BOLDWHITE = "\033[1m\033[37m";   /* Bold White */

int main() {
  srand(time(nullptr));
  std::vector<int> m;

  std::cout << "How many numbers? ";
  int num = 0;
  std::cin >> num;

  // init
  m.reserve(num);
  for (int i = 0; i < num; i++) {
    m.push_back(std::rand() % 2);
  }
  for (int i : m) {
    std::cout << BOLDGREEN << i
              << " " /*  << i << " " << i << " " << i << " " << i << " " <<
                        i << " " << i << " " << i << "\n" */
        ;
  }
  std::cout << "\n" << RESET;
  /* std::cout << GREEN << "\nHello " << RESET << "World" << RED << "!\n";
  std::cout << RESET << "Okay, now this is basic.\n"
            << BLACK << "but this is said to be black\n"
            << BOLDCYAN << "its not only cyan, but its also bold.\n";
  std::string str = GREEN;
  std::string stred = RED;
  stred += "now id like youtu wrkplease ";
  stred += GREEN;
  stred += " greenworks GMBH thankyou.\n";
  std::cout << str + "noway that #define worksbutconstexpr doesnt.\t" +
                   WHITE + "am i as white as i sound?\n" + BOLDMAGENTA
            << "HelloWorld(magentabold)" << stred << "\tred with constexpr\n"
            << RESET << "RESETed text now."; */

  return 0;
}
