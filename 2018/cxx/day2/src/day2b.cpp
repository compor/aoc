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

#include <bitset>
// using std::bitset

#include <string>
// using std::string

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

  std::string found;
  std::bitset<64> diff;
  for (unsigned i = 0; i < boxids.size(); i++) {
    for (unsigned j = i + 1; j < boxids.size(); j++) {
      diff.reset();

      for (unsigned k = 0; k < boxids[i].size(); k++) {
        if (boxids[i][k] != boxids[j][k]) {
          diff.set(k);
        }
      }

      if (diff.count() == 1) {
        int pos = 0;
        while (diff.count()) {
          diff >>= 1;
          pos++;
        }
        found = boxids[i].erase(pos - 1, 1);
      }
    }
  }

  std::cout << found << '\n';

  return 0;
}
