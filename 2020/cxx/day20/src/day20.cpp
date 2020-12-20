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
using sll = long long;

using data_t = std::vector<std::vector<bool>>;

struct tile_t {
  ull id;
  data_t data;
};

using neighbours_t = std::map<ull, std::set<ull>>;

auto get_edges(const tile_t &t) {
  data_t edges;

  edges.push_back(t.data.front());
  edges.push_back(t.data.back());

  std::vector<bool> left, right;

  for (const auto &r : t.data) {
    left.push_back(r.front());
    right.push_back(r.back());
  }

  edges.push_back(left);
  edges.push_back(right);

  return edges;
}

bool are_neighbours(const tile_t &t1, const tile_t &t2) {
  auto edges1 = get_edges(t1);
  auto edges2 = get_edges(t2);

  for (auto &e1 : edges1) {
    for (const auto &e2 : edges2) {
      if (e1 == e2)
        return true;
      std::reverse(e1.begin(), e1.end());
      if (e1 == e2)
        return true;
    }
  }

  return false;
}

int main(int argc, char *argv[]) {
  std::ifstream fin;
  fin.open(argv[1]);

  std::string inputval;
  std::vector<tile_t> tiles;

  while (std::getline(fin, inputval)) {
    if (inputval == "") {
      continue;
    } else if (inputval.substr(0, 4) == "Tile") {
      std::stringstream ss(inputval);
      std::string input;
      std::getline(ss, input, ' ');
      std::getline(ss, input, ' ');

      tiles.push_back({std::stoull(input), {}});
    } else {
      tiles.back().data.push_back({});
      for (auto c : inputval) {
        tiles.back().data.back().push_back(c == '#' ? 1 : 0);
      }
    }
  }

  neighbours_t neighbours;

  for (auto i = 0ull; i < tiles.size(); ++i) {
    for (auto j = 0ull; j < tiles.size(); ++j) {
      if (i == j || (neighbours.count(tiles[i].id) &&
                     neighbours[tiles[i].id].count(tiles[j].id))) {
        continue;
      }

      if (are_neighbours(tiles[i], tiles[j])) {
        neighbours[tiles[i].id].insert(tiles[j].id);
      }
    }
  }

  // for (auto e : neighbours) {
  // std::cout << "tile " << e.first << ": ";
  // for (const auto &n : e.second) {
  // std::cout << n << " ";
  //}
  // std::cout << std::endl;
  //}

  const auto &part1 = [](const auto &neighbours) {
    ull m = 1;

    for (const auto &e : neighbours) {
      if (e.second.size() == 2) {
        m *= e.first;
      }
    }

    return m;
  };

  std::cout << part1(neighbours) << '\n';

  return EXIT_SUCCESS;
}
