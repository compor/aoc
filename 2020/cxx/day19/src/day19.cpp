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
using sll = long long;

using conjuction_t = std::vector<ull>;

struct rule_t {
  char c;
  std::vector<conjuction_t> disjunctions;
};

std::ostream &operator<<(std::ostream &os, const rule_t &r) {
  if (r.c != '0') {
    os << r.c;
  } else {
    for (const auto &e : r.disjunctions) {
      for (const auto &k : e) {
        os << k << ' ';
      }
      os << "| ";
    }
  }

  return os;
}

ull cmatch(ull r, const std::vector<rule_t> &rules, const std::string &s,
           ull pos);

ull cmatch(const std::vector<ull> &all, const std::vector<rule_t> &rules,
           const std::string &s, ull pos) {
  ull newpos = pos;
  bool checked = false;

  for (auto &e : all) {
    checked = true;

    if (newpos >= s.size()) {
      return pos;
    }

    ull newpos2 = cmatch(e, rules, s, newpos);

    if (newpos2 <= newpos) {
      return pos;
    }

    newpos = newpos2;
  }

  return checked ? newpos : pos;
}

ull cmatch(ull ridx, const std::vector<rule_t> &rules, const std::string &s,
           ull pos) {
  if (pos >= s.size()) {
    return pos;
  }

  if (rules[ridx].c != '0' &&
      (rules[ridx].c == s[pos] || rules[ridx].c == s[pos])) {
    return ++pos;
  } else {
    ull newpos = pos;
    for (const auto &e : rules[ridx].disjunctions) {
      newpos = cmatch(e, rules, s, pos);

      if (newpos != pos) {
        return newpos;
      }
    }
  }

  return pos;
}

int main(int argc, char *argv[]) {
  std::ifstream fin;
  fin.open(argv[1]);

  std::string inputval;
  std::vector<rule_t> rules;

  while (std::getline(fin, inputval)) {
    if (inputval == "")
      break;

    std::string inputval2;
    for (auto &c : inputval) {
      if (c == '\"') {
        inputval2.push_back(' ');
      } else if (c == ':') {
        inputval2.push_back(' ');
        inputval2.push_back('|');
      } else {
        inputval2.push_back(c);
      }
    }
    inputval = inputval2;

    std::istringstream ss(inputval);
    std::string stok;

    std::getline(ss, stok, ' ');
    rules.push_back({'0', {}});

    while (std::getline(ss, stok, ' ')) {
      if (stok[0] == 'a' || stok[0] == 'b') {
        rules.back().c = stok[0];
        break;
      } else if (std::isdigit(stok[0])) {
        ull r = std::stoull(stok);
        rules.back().disjunctions.back().push_back(r);
      } else if (stok[0] == '|') {
        rules.back().disjunctions.push_back({});
      }
    }
  }

  std::vector<std::string> input;
  while (std::getline(fin, inputval)) {
    input.push_back(inputval);
  }

  const auto &part1 = [&rules](const int ridx, const auto &msgs) {
    return std::count_if(msgs.begin(), msgs.end(),
                         [ridx, &rules](const auto &e) {
                           return e.size() == cmatch(ridx, rules, e, 0);
                         });
  };

  std::cout << part1(0, input) << '\n';

  return EXIT_SUCCESS;
}
