//
//
//

#include <cstdlib>
// using std::exit

#include <iostream>
// using std::cout
// using std::cerr

#include <fstream>
// using std::ifstream

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

  int frequency = 0, change = 0;

  while (fin >> change) {
    frequency += change;
  }

  std::cout << "resulting frequency: " << frequency << "\n";

  return 0;
}
