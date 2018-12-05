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

#include <algorithm>
// using std::remove_if

#include <set>
// using std::set

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

std::set<char> units;

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

  for (const auto &c : polymer) {
    units.insert(std::tolower(c));
  }

  auto most_problematic_unit = *(units.begin());
  auto resulting_length = polymer.length();

  for (const auto &c : units) {
    auto test_polymer = polymer;

    test_polymer.erase(
        std::remove_if(test_polymer.begin(), test_polymer.end(),
                       [c](const auto &e) { return std::tolower(e) == c; }),
        test_polymer.end());

    while (collapse(test_polymer)) {
    }

    // std::cout << "unit: " << c << " length: " << test_polymer.length() <<
    // '\n';

    if (resulting_length > test_polymer.length()) {
      most_problematic_unit = c;
      resulting_length = test_polymer.length();
    }
  }

  // std::cout << polymer << '\n';
  std::cout << "result: " << resulting_length << '\n';

  return 0;
}
