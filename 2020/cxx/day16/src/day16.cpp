//
//
//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using ull = unsigned long long;

struct bounds_t {
  ull lower;
  ull upper;
};

using rules_t = std::map<std::string, std::pair<bounds_t, bounds_t>>;
using ticket_t = std::vector<ull>;

bool isvalid(ull val, const bounds_t &b) {
  return val >= b.lower && val <= b.upper;
}

ull isvalid(ull val, const rules_t &r) {
  return std::any_of(r.begin(), r.end(), [val](const auto &e) {
    return isvalid(val, e.second.first) || isvalid(val, e.second.second);
  });
}

int main(int argc, char *argv[]) {
  std::ifstream fin;
  fin.open(argv[1]);

  std::string inputval;
  rules_t rules;
  ticket_t own;
  std::vector<ticket_t> nearby;

  enum mode_t { rules_mode, own_mode, nearby_mode } mode = rules_mode;

  while (std::getline(fin, inputval)) {
    if (inputval == "") {
      continue;
    } else if (inputval == "your ticket:") {
      mode = own_mode;
      continue;
    } else if (inputval == "nearby tickets:") {
      mode = nearby_mode;
      continue;
    }

    if (mode == rules_mode) {
      auto input = inputval;
      auto rname{input.substr(0, input.find(':'))};
      input = input.substr(rname.size() + 2);
      auto rule1{input.substr(0, input.find(' '))};
      auto rule2{input.substr(rule1.size() + 3)};

      rules[rname] = {
          {std::stoull(rule1), std::stoull(rule1.substr(rule1.find('-') + 1))},
          {std::stoull(rule2), std::stoull(rule2.substr(rule2.find('-') + 1))}};
    } else if (mode == own_mode) {
      std::stringstream ss{inputval};

      std::string val;
      while (std::getline(ss, val, ',')) {
        own.push_back(std::stoull(val));
      }
    } else if (mode == nearby_mode) {
      std::stringstream ss{inputval};

      std::string val;
      ticket_t t;
      while (std::getline(ss, val, ',')) {
        t.push_back(std::stoull(val));
      }
      nearby.push_back(t);
    }
  }

  ull erate = 0;
  for (auto e : nearby) {
    for (auto k : e) {
      if (!isvalid(k, rules)) {
        erate += k;
      }
    }
  }

  std::cout << erate << std::endl;

  return EXIT_SUCCESS;
}
