//
//
//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include <utility>
#include <vector>

const char tree_v = '#';

struct position_t {
  unsigned int row;
  unsigned int col;
};

using row_t = std::string;
using map_t = std::vector<row_t>;

position_t slope_step(const position_t &pos, const position_t &step,
                      const unsigned int row_length) {
  return {pos.row + step.row, (pos.col + step.col) % row_length};
}

auto count_trees(const map_t &map, const position_t &start_pos,
                 const position_t &step) {
  unsigned trees = 0;
  auto pos = slope_step(start_pos, step, map[start_pos.row].size());

  while (pos.row < map.size()) {
    if (map[pos.row][pos.col] == tree_v)
      trees++;

    pos = slope_step(pos, step, map[pos.row].size());
  }

  return trees;
}

auto multiply_trees_per_slope(const map_t &map, const position_t &start_pos,
                              const std::vector<position_t> &steps) {
  unsigned prod = 1;

  for (const auto &step : steps) {
    prod *= count_trees(map, start_pos, step);
  }

  return prod;
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
  map_t input;

  while (std::getline(fin, inputval)) {
    input.push_back(inputval);
  }

  position_t pos{0, 0};
  position_t step{1, 3};

  const auto &part1 = count_trees;

  std::vector<position_t> steps = {{1, 1}, {1, 3}, {1, 5}, {1, 7}, {2, 1}};

  const auto &part2 = multiply_trees_per_slope;

  std::cout << part1(input, pos, step) << std::endl;
  std::cout << part2(input, pos, steps) << std::endl;

  return EXIT_SUCCESS;
}
