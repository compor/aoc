//
//
//

#include <cstdlib>
// using std::exit
// using std::atoi

#include <iostream>
// using std::cout
// using std::cerr

#include <fstream>
// using std::ifstream

#include <set>
// using std::set

#include <optional>
// using std::optional

auto find_desired_sum_parts(const std::set<int> &input, int desired_sum) {
  bool found = false;
  int complement = 0;

  for (auto &e : input) {
    complement = desired_sum - e;

    if (input.count(complement)) {
      found = true;
      break;
    }
  }

  return found ? std::make_optional(
                     std::make_pair(desired_sum - complement, complement))
               : std::nullopt;
}

int part1(const std::set<int> &input) {
  if (auto p = find_desired_sum_parts(input, 2020)) {
    return p->first * p->second;
  }

  return 0;
}

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
  std::set<int> input;

  while (std::getline(fin, inputval)) {
    input.insert(std::atoi(inputval.c_str()));
  }

  std::cout << part1(input) << std::endl;

  return EXIT_SUCCESS;
}
