//
//
//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using ull = unsigned long long;

const ull max_turn = 2020;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "incorrect number of arguments provided!\n";
    std::exit(EXIT_FAILURE);
  }

  std::ifstream fin;
  fin.open(argv[1]);

  if (!fin) {
    std::cerr << "unable to open file: " << argv[1] << "!\n";
    std::exit(EXIT_FAILURE);
  }

  std::string inputval;
  // of the pair first -> second to last turn spoken, second -> last turn spoken
  std::map<ull, std::pair<ull, ull>> spoken;
  ull turn = 1;
  ull last = 0;

  while (std::getline(fin, inputval, ',')) {
    last = std::stoull(inputval);
    spoken[last] = {0, turn};
    turn++;
  }

  for (; turn <= max_turn; ++turn) {
    last = spoken[last].first ? spoken[last].second - spoken[last].first : 0;

    // update last spoken
    if (spoken.count(last)) {
      spoken[last].first = spoken[last].second;
      spoken[last].second = turn;
    } else {
      spoken[last] = {0, turn};
    }
  }

  std::cout << last << std::endl;

  return EXIT_SUCCESS;
}
