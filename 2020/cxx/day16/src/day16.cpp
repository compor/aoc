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

struct bounds_t {
  ull lower;
  ull upper;
};

using rules_t = std::map<std::string, std::pair<bounds_t, bounds_t>>;
using ticket_t = std::vector<ull>;

bool isvalid(ull val, const bounds_t &b) {
  return val >= b.lower && val <= b.upper;
}

bool isvalid(ull val, const bounds_t &b1, const bounds_t &b2) {
  return isvalid(val, b1) || isvalid(val, b2);
}

ull isvalid(ull val, const rules_t &r) {
  return std::any_of(r.begin(), r.end(), [val](const auto &e) {
    return isvalid(val, e.second.first, e.second.second);
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

  std::vector<ticket_t> valid;
  ull erate = 0;
  for (auto e : nearby) {
    bool noerrors = true;
    for (auto k : e) {
      if (!isvalid(k, rules)) {
        erate += k;
        noerrors = false;
      }
    }
    if (noerrors)
      valid.push_back(e);
  }

  valid.push_back(own);

  std::map<ull, std::set<std::string>> f2r;

  for (ull i = 0; i < own.size(); ++i) {
    // f2r[i] = {};
    for (auto &k : rules) {
      f2r[i].insert(k.first);
    }
  }

  for (const auto &e : valid) {
    ull i = 0;
    for (const auto &f : e) {
      for (const auto &r : rules) {
        if (!isvalid(f, r.second.first, r.second.second)) {
          f2r[i].erase(r.first);
        }
      }
      i++;
    }
  }

  std::set<ull> visited;
  while (visited.size() < f2r.size()) {
    ull pos;
    bool found = false;
    for (const auto &e : f2r) {
      if (e.second.size() <= 1 && !visited.count(e.first)) {
        pos = e.first;
        visited.insert(pos);
        found = true;
        break;
      }
    }

    if (found) {
      for (ull i = 0; i < f2r.size(); i++) {
        if (i == pos)
          continue;

        f2r[i].erase(*f2r[pos].begin());
      }
    }
  }

  for (auto &e : f2r) {
    std::cout << "field " << e.first << " with potential rules: ";
    for (auto &k : e.second) {
      std::cout << k << " ";
    }
    std::cout << std::endl;
  }

  const auto &part2 = [&f2r](const ticket_t &t) {
    ull p = 1;

    ull i = 0;
    for (const auto &e : t) {
      if (f2r[i++].begin()->find("departure") != std::string::npos) {
        p *= e;
      }
    }

    return p;
  };

  std::cout << erate << std::endl;
  std::cout << part2(own) << std::endl;

  return EXIT_SUCCESS;
}
