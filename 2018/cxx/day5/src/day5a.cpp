//
//
//

#include <iostream>
// using std::cout
// using std::cerr

#include <string>
// using std::string

#include <cctype>
// using std::lower

#include <iterator>
// using std::next

#include <fstream>
// using std::ifstream

bool are_reacting(char c1, char c2) {
  if (c1 != c2 && std::tolower(c1) == std::tolower(c2)) {
    return true;
  }

  return false;
}

decltype(auto) collapse(std::string &p) {
  unsigned long reactions = 0;

  for (auto it = p.begin(); it != p.end();) {
    auto it_next = std::next(it);

    if (it_next == p.end()) {
      break;
    }

    if (are_reacting(*it, *it_next)) {
      it = p.erase(it, ++it_next);
      reactions++;

      continue;
    }

    ++it;
  }

  return reactions;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "incorrect number of arguments provided!\n";
    std::exit(-1);
  }

  std::ifstream fin;
  fin.open(argv[1]);

  if (!fin) {
    std::cerr << "unable to open file: " << argv[1] << "!\n";
    std::exit(-1);
  }

  std::string polymer;

  fin >> polymer;
  // std::cout << polymer << '\n';

  while (collapse(polymer)) {
  }

  // std::cout << polymer << '\n';
  std::cout << "result: " << polymer.length() << '\n';

  return 0;
}
