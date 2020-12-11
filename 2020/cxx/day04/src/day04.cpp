//
//
//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <regex>
#include <sstream>
#include <utility>
#include <vector>

using passport_key_t = std::string;
using passport_val_t = std::string;

using passport_t = std::map<passport_key_t, passport_val_t>;

using batch_file_t = std::vector<std::string>;

passport_t parse_entry(const std::string &entry) {
  std::regex entry_regex{"\\s*(.*):(.*)\\s*"};
  std::smatch matches;
  passport_t passport;
  std::string entryval;
  std::istringstream iss(entry);

  while (std::getline(iss, entryval, ' ')) {
    if (std::regex_match(entryval, matches, entry_regex)) {
      passport.insert({matches[1].str(), matches[2].str()});
    }
  }

  return passport;
}

auto parse_batch_file(const batch_file_t &bf) {
  std::vector<passport_t> passports;
  std::string current = "";

  for (auto &e : bf) {
    if (e == "") {
      passports.push_back(parse_entry(current));
      current = "";
    }

    current += " " + e;
  }

  passports.push_back(parse_entry(current));

  return passports;
}

bool is_valid(const passport_t &passport) {
  return passport.count("cid") ? passport.size() == 8 : passport.size() == 7;
}

auto count_valid_passports(const std::vector<passport_t> &passports) {
  return std::count_if(passports.begin(), passports.end(), is_valid);
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
  batch_file_t input;

  while (std::getline(fin, inputval)) {
    input.push_back(inputval);
  }

  const auto &passports = parse_batch_file(input);

  const auto &part1 = count_valid_passports;
  std::cout << part1(passports) << std::endl;

  return EXIT_SUCCESS;
}
