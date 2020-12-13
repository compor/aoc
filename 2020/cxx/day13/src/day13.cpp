//
//
//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

using busid_t = long long int;

using schedule_t = std::vector<busid_t>;

long int calc_wait(busid_t min_departure, busid_t bus) {
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
    input.push_back(inputval == "x" ? 0 : std::atoi(inputval.c_str()));
  }

  long int min_wait = 0;
  busid_t min_busid = *std::min_element(
      input.begin(), input.end(),
      [&min_departure, &min_wait](const auto &e1, const auto &e2) {
        if (!e1 || !e2)
          return false;

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
