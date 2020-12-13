//
//
//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

using busid_t = int;

using schedule_t = std::vector<busid_t>;

int calc_wait(busid_t min_departure, busid_t bus) {
  return bus - (min_departure % bus);
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
  schedule_t input;
  busid_t min_departure = 0;

  std::getline(fin, inputval);
  min_departure = std::atoi(inputval.c_str());

  while (std::getline(fin, inputval, ',')) {
    if (inputval != "x") {
      input.push_back(std::atoi(inputval.c_str()));
    }
  }

  int min_wait = 0;
  int min_busid = *std::min_element(
      input.begin(), input.end(),
      [&min_departure, &min_wait](const auto &e1, const auto &e2) {
        auto w1 = calc_wait(min_departure, e1);
        auto w2 = calc_wait(min_departure, e2);
        auto c = w1 < w2;
        if (c) {
          min_wait = w1;
        }
        return c;
      });

  std::cout << min_busid << ' ' << min_wait << ' ' << min_busid * min_wait
            << '\n';

  return EXIT_SUCCESS;
}
