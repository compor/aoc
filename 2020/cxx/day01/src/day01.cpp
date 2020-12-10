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

#include <tuple>
// using std::tuple

#include <functional>
// using std::get

#include <optional>
// using std::optional

auto find_desired_sum_pair(const std::set<int> &input, int desired_sum,
                           int exclude_val = 0) {
  bool found = false;
  int complement = 0;

  for (auto &e : input) {
    if (e == exclude_val) {
      continue;
    }

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
  if (auto p = find_desired_sum_pair(input, 2020)) {
    return p->first * p->second;
  }

  return 0;
}

std::optional<std::tuple<int, int, int>>
find_desired_sum_triplet(const std::set<int> &input, int desired_sum) {
  int complement = 0;

  for (auto &e : input) {
    complement = desired_sum - e;

    if (auto p = find_desired_sum_pair(input, complement, e)) {
      return std::make_tuple(e, p->first, p->second);
    }
  }

  return {};
}

int part2(const std::set<int> &input) {
  if (auto p = find_desired_sum_triplet(input, 2020)) {
    return std::get<0>(*p) * std::get<1>(*p) * std::get<2>(*p);
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
  std::cout << part2(input) << std::endl;

  return EXIT_SUCCESS;
}
