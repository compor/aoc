//
//
//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using ull = unsigned long long;
using sll = long long;

struct token_t {
  char type;
  ull val;
};

void process(std::vector<token_t> &rpn) {
  while (rpn.size() > 1) {
    if (rpn.back().type == '0') {
      return;
    } else {
      auto c = rpn.back();
      rpn.pop_back();

      process(rpn);
      ull val1 = rpn.back().val;
      rpn.pop_back();
      process(rpn);
      ull val2 = rpn.back().val;
      rpn.pop_back();

      if (c.type == '+') {
        rpn.push_back({'0', val1 + val2});
      } else {
        rpn.push_back({'0', val1 * val2});
      }
    }
  }
}

void to_rpn(const std::vector<token_t> &tokens, std::vector<token_t> &rpn) {
  std::vector<token_t> ops;

  for (auto &c : tokens) {
    if (c.type == '0') {
      rpn.push_back(c);
    } else if (c.type == '(') {
      ops.push_back(c);
    } else if (c.type == '+' || c.type == '*') {
      if (ops.size() && ops.back().type != '(') {
        rpn.push_back(ops.back());
        ops.pop_back();
      }
      ops.push_back(c);
    } else if (c.type == ')') {
      while (ops.back().type != '(') {
        rpn.push_back(ops.back());
        ops.pop_back();
      }
      ops.pop_back();
    }
  }

  for (sll i = ops.size() - 1; i >= 0; --i) {
    rpn.push_back(ops[i]);
  }
}

int main(int argc, char *argv[]) {
  std::ifstream fin;
  fin.open(argv[1]);

  std::string inputval;
  std::vector<std::vector<token_t>> expr;

  while (std::getline(fin, inputval)) {
    std::string input;

    for (auto c : inputval) {
      if (c == '(') {
        input.push_back(c);
        input.push_back(' ');
      } else if (c == ')') {
        input.push_back(' ');
        input.push_back(c);
      } else {
        input.push_back(c);
      }
    }

    std::vector<token_t> tokens;
    std::istringstream ss(input);
    std::string stok;
    while (std::getline(ss, stok, ' ')) {
      if (stok == "(" || stok == ")" || stok == "+" || stok == "*") {
        tokens.push_back({stok[0], 0});
      } else {
        tokens.push_back({'0', std::stoull(stok)});
      }
    }

    expr.push_back(tokens);
  }

  auto part1 = std::accumulate(expr.begin(), expr.end(), 0ull,
                               [](ull acc, const auto &e) {
                                 std::vector<token_t> rpn;
                                 to_rpn(e, rpn);
                                 process(rpn);
                                 return acc + rpn.back().val;
                               });

  std::cout << part1 << '\n';

  return EXIT_SUCCESS;
}
