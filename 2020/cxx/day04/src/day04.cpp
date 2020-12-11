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
#include <set>
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

bool is_valid_part1(const passport_t &passport) {
  return passport.count("cid") ? passport.size() == 8 : passport.size() == 7;
}

bool is_valid_byr(const std::string &byr) {
  unsigned int y = std::atoi(byr.c_str());

  return y >= 1920 && y <= 2002;
}

bool is_valid_iyr(const std::string &iyr) {
  unsigned int y = std::atoi(iyr.c_str());

  return y >= 2010 && y <= 2020;
}

bool is_valid_eyr(const std::string &eyr) {
  unsigned int y = std::atoi(eyr.c_str());

  return y >= 2020 && y <= 2030;
}

bool is_valid_hgt(const std::string &hgt) {
  std::regex entry_regex{"([0123456789]+)(cm|in)"};
  std::smatch matches;

  std::regex_match(hgt, matches, entry_regex);
  if (matches.size() != 3)
    return false;

  auto h = std::atoi(matches[1].str().c_str());

  return matches[2].str() == "cm" ? h >= 150 && h <= 193 : h >= 59 && h <= 76;
}

bool is_valid_hcl(const std::string &hcl) {
  std::regex entry_regex{"#([0123456789abcdef]{6})"};
  std::smatch matches;

  std::regex_match(hcl, matches, entry_regex);

  return matches.size() == 2;
}

bool is_valid_ecl(const std::string &ecl) {
  const std::set<std::string> valids{"amb", "blu", "brn", "gry",
                                     "grn", "hzl", "oth"};

  return valids.count(ecl) != 0;
}

bool is_valid_pid(const std::string &pid) {
  std::regex entry_regex{"(\\d{9})"};
  std::smatch matches;

  std::regex_match(pid, matches, entry_regex);

  return matches.size() == 2;
}

bool is_valid_part2(const passport_t &passport) {
  if (!is_valid_part1(passport)) {
    return false;
  }

  return is_valid_byr(passport.at("byr")) && is_valid_iyr(passport.at("iyr")) &&
         is_valid_eyr(passport.at("eyr")) && is_valid_hgt(passport.at("hgt")) &&
         is_valid_hcl(passport.at("hcl")) && is_valid_ecl(passport.at("ecl")) &&
         is_valid_pid(passport.at("pid"));

  return true;
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

  const auto &part1 = [&passports]() {
    return std::count_if(passports.begin(), passports.end(), is_valid_part1);
  };

  const auto &part2 = [&passports]() {
    return std::count_if(passports.begin(), passports.end(), is_valid_part2);
  };

  std::cout << part1() << std::endl;
  std::cout << part2() << std::endl;

  return EXIT_SUCCESS;
}
