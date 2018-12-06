//
//
//

#include <iostream>
// using std::cout
// using std::cerr

#include <iomanip>
// using std::setw

#include <functional>
// using std::reference_wrapper

#include <string>
// using std::string

#include <vector>
// using std::vector

#include <map>
// using std::map

#include <queue>
// using std::queue

#include <limits>
// using std::numeric_limits

#include <algorithm>
// using std::remove_if

#include <fstream>
// using std::ifstream

using distance_t = unsigned long;
using area_t = unsigned long;
using coord_t = unsigned long;
using locid_t = unsigned long;

struct location {
  locid_t id;

  coord_t x;
  coord_t y;

  location(locid_t id, coord_t x, coord_t y) : id(id), x(x), y(y) {}
  location(coord_t x, coord_t y) : location(0, x, y) {}
};

struct tile {
  locid_t id;
  distance_t distance;

  coord_t x;
  coord_t y;

  bool visited;

  tile(locid_t id = 0, distance_t d = std::numeric_limits<distance_t>::max())
      : id(id), distance(d), x(0), y(0), visited(false) {}
};

struct plane {
  std::vector<std::vector<tile>> space;

  plane(coord_t max_x, coord_t max_y) : space(max_x + 1) {
    for (auto &e : space) {
      e.resize(max_y + 1);
    }

    for (auto x = 0u; x < space.size(); ++x) {
      for (auto y = 0u; y < space[0].size(); ++y) {
        space[x][y].x = x;
        space[x][y].y = y;
      }
    }
  }

  decltype(auto) max_x() const { return space.size() - 1; }
  decltype(auto) max_y() const { return space[0].size() - 1; }
};

unsigned long int manhattan(const location &l1, const location &l2) {
  distance_t distance = 0;

  distance += l1.x > l2.x ? l1.x - l2.x : l2.x - l1.x;
  distance += l1.y > l2.y ? l1.y - l2.y : l2.y - l1.y;

  return distance;
}

template <typename IteratorT>
void set_locations(plane &p, IteratorT begin, IteratorT end) {
  for (auto it = begin; it != end; ++it) {
    p.space[it->x][it->y].id = it->id;
    p.space[it->x][it->y].distance = 0;
  }
}

template <typename IteratorT>
void calculate_location_distances(plane &p, IteratorT begin, IteratorT end) {
  for (auto it = begin; it != end; ++it) {
    for (auto cur_x = 0lu; cur_x <= p.max_x(); cur_x++) {
      for (auto cur_y = 0lu; cur_y <= p.max_y(); cur_y++) {
        auto d = manhattan({cur_x, cur_y}, *it);

        if (d < p.space[cur_x][cur_y].distance) {
          p.space[cur_x][cur_y].id = it->id;
          p.space[cur_x][cur_y].distance = d;
        }
      }
    }
  }
}

template <typename IteratorT>
void find_tied_distances(plane &p, IteratorT begin, IteratorT end) {
  for (auto it = begin; it != end; ++it) {
    for (auto cur_x = 0lu; cur_x <= p.max_x(); cur_x++) {
      for (auto cur_y = 0lu; cur_y <= p.max_y(); cur_y++) {
        if (p.space[cur_x][cur_y].id != 0 &&
            it->id != p.space[cur_x][cur_y].id) {
          auto d = manhattan({cur_x, cur_y}, *it);

          if (d == p.space[cur_x][cur_y].distance) {
            p.space[cur_x][cur_y].id = 0;
            p.space[cur_x][cur_y].distance = 0;
          }
        }
      }
    }
  }
}

decltype(auto) get_adjacent(tile &t, plane &p) {
  std::vector<std::reference_wrapper<tile>> tiles;

  if (t.x > 0) {
    tiles.emplace_back(p.space[t.x - 1][t.y]);

    if (t.y < p.max_y()) {
      tiles.emplace_back(p.space[t.x - 1][t.y + 1]);
    }
  }

  if (t.x < p.max_x()) {
    tiles.emplace_back(p.space[t.x + 1][t.y]);
  }

  if (t.y < p.max_y()) {
    tiles.emplace_back(p.space[t.x][t.y + 1]);
  }

  if (t.y > 0) {
    tiles.emplace_back(p.space[t.x][t.y - 1]);

    if (t.x < p.max_x()) {
      tiles.emplace_back(p.space[t.x + 1][t.y - 1]);
    }
  }

  if (t.x > 0 && t.y > 0) {
    tiles.emplace_back(p.space[t.x - 1][t.y - 1]);
  }
  if (t.x < p.max_x() && t.y < p.max_y()) {
    tiles.emplace_back(p.space[t.x + 1][t.y + 1]);
  }

  return tiles;
}

decltype(auto) calculate_area(const location &loc, plane &p) {
  std::queue<std::reference_wrapper<tile>> workqueue;
  area_t a = 0;

  workqueue.push(p.space[loc.x][loc.y]);
  while (!workqueue.empty()) {
    auto &cur_tile = workqueue.front();

    if (!p.space[cur_tile.get().x][cur_tile.get().y].visited &&
        cur_tile.get().id == loc.id) {
      p.space[cur_tile.get().x][cur_tile.get().y].visited = true;
      a++;

      for (auto &adj : get_adjacent(cur_tile.get(), p)) {
        if (adj.get().id == cur_tile.get().id && !adj.get().visited) {
          workqueue.push(adj.get());
        }
      }
    }

    workqueue.pop();
  }

  return a;
}

bool is_location_infinite(const id_t &id, plane &p) {
  auto max_x = p.max_x();
  auto max_y = p.max_y();

  for (auto x = 0u; x <= max_x; ++x) {
    if (p.space[x][0].id == id || p.space[x][max_y].id == id) {
      return true;
    }
  }

  for (auto y = 0u; y <= max_y; ++y) {
    if (p.space[0][y].id == id || p.space[max_x][y].id == id) {
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

  locid_t id = 0;
  coord_t x, max_x = 0;
  coord_t y, max_y = 0;
  std::vector<location> locations;

  char comma;
  while (fin >> x >> comma >> y) {
    locations.emplace_back(++id, x, y);

    max_x = std::max(x, max_x);
    max_y = std::max(y, max_y);
  }
  std::cout << max_x << " " << max_y << '\n';

  plane p(max_x, max_y);

  set_locations(p, locations.begin(), locations.end());

  for (auto x = 0lu; x <= max_x; x++) {
    for (auto y = 0lu; y <= max_y; y++) {
      std::cout << std::setw(2) << p.space[x][y].id << ','
                << p.space[x][y].distance << " ";
    }
    std::cout << '\n';
  }

  std::cout << '\n';
  calculate_location_distances(p, locations.begin(), locations.end());
  find_tied_distances(p, locations.begin(), locations.end());

  for (auto x = 0lu; x <= max_x; x++) {
    for (auto y = 0lu; y <= max_y; y++) {
      std::cout << std::setw(2) << p.space[x][y].id << ','
                << p.space[x][y].distance << " ";
    }
    std::cout << '\n';
  }

  std::map<id_t, area_t> areas;
  for (const auto &loc : locations) {
    areas[loc.id] = calculate_area(loc, p);
    std::cout << "area: " << areas[loc.id] << " for location: " << loc.id
              << '\n';
  }

  std::vector<id_t> to_delete;
  for (auto &e : areas) {
    if (is_location_infinite(e.first, p)) {
      to_delete.push_back(e.first);
    }
  }

  for (auto &e : to_delete) {
    areas.erase(e);
  }

  area_t max_area = 0;
  id_t max_area_id = 0;
  for (auto &e : areas) {
    if (e.second > max_area) {
      max_area = e.second;
      max_area_id = e.first;
    }
  }

  std::cout << "result: " << max_area_id << " " << max_area << '\n';

  return 0;
}
