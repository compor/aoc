//
//
//

#include <cstdlib>
// using std::exit

#include <iostream>
// using std::cout
// using std::cerr

#include <vector>
// using std::vector

#include <string>
// using std::string

#include <map>
// using std::map

#include <regex>
// using std::regex

#include <fstream>
// using std::ifstream

struct claim {
  using num_t = unsigned long;

  num_t id;
  num_t x;
  num_t y;
  num_t width;
  num_t height;

  claim(const claim &) = default;
  claim() : id(0), x(0), y(0), width(0), height(0) {}
  claim(num_t id, num_t x, num_t y, num_t width, num_t height)
      : id(id), x(x), y(y), width(width), height(height) {}
};

std::ostream &operator<<(std::ostream &os, const claim &c) {
  os << c.id << ' ' << c.x << ' ' << c.y << ' ' << c.width << ' ' << c.height;

  return os;
}

constexpr size_t fabric_dimension = 1000;
unsigned long fabric[fabric_dimension][fabric_dimension] = {0};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "incorrect number of arguments provided!\n";
    std::exit(-1);
  }

  std::ifstream fin;
  fin.open(argv[1]);

  if (!fin) {
    std::cerr << "unable to open file: " << argv[1] << "!\n";
    std::exit(-1);
  }

  std::string raw_claim;
  std::vector<std::string> raw_claims;

  while (std::getline(fin, raw_claim)) {
    raw_claims.push_back(raw_claim);
  }

  std::vector<claim> claims;
  claims.reserve(raw_claim.size());

  std::regex claim_regex{"#([[:digit:]]+) @ ([[:digit:]]+),([[:digit:]]+): "
                         "([[:digit:]]+)x([[:digit:]]+)"};
  std::smatch matches;

  for (const auto &e : raw_claims) {
    if (std::regex_match(e, matches, claim_regex)) {
      if (matches.size() == 6) {
        char *c;
        claims.emplace_back(std::strtoul(matches[1].str().c_str(), &c, 10),
                            std::strtoul(matches[2].str().c_str(), &c, 10),
                            std::strtoul(matches[3].str().c_str(), &c, 10),
                            std::strtoul(matches[4].str().c_str(), &c, 10),
                            std::strtoul(matches[5].str().c_str(), &c, 10));
      }
    }
  }

  for (const auto &c : claims) {
    for (auto i = 0u; i < c.width; i++) {
      for (auto j = 0u; j < c.height; j++) {
        fabric[c.x + i][c.y + j]++;
      }
    }
  }

  unsigned long count = 0;
  for (auto i = 0u; i < fabric_dimension; i++) {
    for (auto j = 0u; j < fabric_dimension; j++) {
      if (fabric[i][j] >= 2) {
        count++;
      }
    }
  }

  std::cout << count << '\n';

  return 0;
}
