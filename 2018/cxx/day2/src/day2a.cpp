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

#include <fstream>
// using std::ifstream

using BoxIDCountMapTy = std::map<char, unsigned>;

void init_map_count(BoxIDCountMapTy &m) {
  BoxIDCountMapTy::key_type c = 'a';

  while (c <= 'z') {
    m[c++] = 0;
  }
}

bool has_exactly(const BoxIDCountMapTy &m,
                 const BoxIDCountMapTy::mapped_type count) {
  for (auto &e : m) {
    if (e.second == count) {
      return true;
    }
  }

  return false;
}

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

  std::string id;
  std::vector<std::string> boxids;

  while (fin >> id) {
    boxids.push_back(id);
  }

  BoxIDCountMapTy count;

  unsigned exactly_two = 0;
  unsigned exactly_three = 0;

  for (const auto &id : boxids) {
    init_map_count(count);

    for (auto &c : id) {
      count[c]++;
    }

    if (has_exactly(count, 2)) {
      exactly_two++;
    }

    if (has_exactly(count, 3)) {
      exactly_three++;
    }
  }

  std::cout << exactly_two * exactly_three << '\n';

  return 0;
}
