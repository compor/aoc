//
//
//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include <utility>
#include <vector>

struct pwd_policy_t {
  int lower;
  int upper;
  char c;
};

using password_t = std::string;

using record_t = std::pair<pwd_policy_t, password_t>;
using db_t = std::vector<record_t>;

std::optional<record_t> parse_line(std::string &line) {
  std::regex entry_regex{"(\\d+)-(\\d+) (.): (.*)"};
  std::smatch matches;

  if (std::regex_match(line, matches, entry_regex)) {
    if (matches.size() == 5) {
      return std::make_optional(
          std::make_pair(pwd_policy_t{std::atoi(matches[1].str().c_str()),
                                      std::atoi(matches[2].str().c_str()),
                                      matches[3].str()[0]},
                         matches[4].str()));
    }
  }

  return std::nullopt;
}

bool is_valid(const record_t &r) {
  auto n = std::count(r.second.begin(), r.second.end(), r.first.c);
  return n >= r.first.lower && n <= r.first.upper;
}

auto part1(const db_t &db) {
  return std::count_if(db.begin(), db.end(), is_valid);
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
  db_t input;

  while (std::getline(fin, inputval)) {
    if (auto record = parse_line(inputval)) {
      input.push_back(*record);
    }
  }

  std::cout << part1(input) << std::endl;

  return EXIT_SUCCESS;
}
