//
//
//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <regex>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

const char floor_v = '.';
const char occupied_v = '#';
const char unoccupied_v = 'L';

struct seat_t {
  int row;
  int col;
  char state;
};

using row_t = std::vector<seat_t>;
using seat_layout_t = std::vector<row_t>;

bool is_floor(const seat_t &seat) { return seat.state == floor_v; }

bool is_occupied(const seat_t &seat) { return seat.state == occupied_v; }

bool is_unoccupied(const seat_t &seat) {
  return is_floor(seat) || seat.state == unoccupied_v;
}

void flip_seat(seat_t &seat) {
  if (!is_floor(seat)) {
    seat.state = is_occupied(seat) ? unoccupied_v : occupied_v;
  }
}

std::ostream &operator<<(std::ostream &os, const seat_layout_t &layout) {
  for (const auto &e : layout) {
    for (const auto &k : e) {
      os << k.state;
    }
    os << '\n';
  }

  return os;
}

std::vector<std::reference_wrapper<const seat_t>>
get_adjacent_seats_part1(const seat_layout_t &layout, const seat_t &seat) {
  std::vector<std::reference_wrapper<const seat_t>> seats;

  // ABOVE
  if (seat.row - 1 >= 0) {
    // UP
    seats.push_back(layout[seat.row - 1][seat.col]);

    // RIGHT
    if (seat.col - 1 >= 0) {
      seats.push_back(layout[seat.row - 1][seat.col - 1]);
    }

    // LEFT
    if (seat.col + 1 < layout[seat.row - 1].size()) {
      seats.push_back(layout[seat.row - 1][seat.col + 1]);
    }
  }

  // BELOW
  if (seat.row + 1 < layout.size()) {
    // DOWN
    seats.push_back(layout[seat.row + 1][seat.col]);

    // RIGHT
    if (seat.col - 1 >= 0) {
      seats.push_back(layout[seat.row + 1][seat.col - 1]);
    }

    // LEFT
    if (seat.col + 1 < layout[seat.row + 1].size()) {
      seats.push_back(layout[seat.row + 1][seat.col + 1]);
    }
  }

  // DIRECTLY RIGHT
  if (seat.col - 1 >= 0) {
    seats.push_back(layout[seat.row][seat.col - 1]);
  }
  // DIRECTLY LEFT
  if (seat.col + 1 < layout[seat.row].size()) {
    seats.push_back(layout[seat.row][seat.col + 1]);
  }

  return seats;
}

std::vector<std::reference_wrapper<const seat_t>>
get_adjacent_seats_part2(const seat_layout_t &layout, const seat_t &seat) {
  std::vector<std::reference_wrapper<const seat_t>> seats;

  // ABOVE
  for (int i = seat.row - 1; i >= 0; --i) {
    // UP
    if (is_floor(layout[i][seat.col]))
      continue;

    seats.push_back(layout[i][seat.col]);
    break;
  }

  // ABOVE
  for (int i = seat.row - 1, j = seat.col - 1; i >= 0 && j >= 0; --i, --j) {
    // LEFT
    if (is_floor(layout[i][j]))
      continue;

    seats.push_back(layout[i][j]);
    break;
  }

  // ABOVE
  for (int i = seat.row - 1, j = seat.col + 1; i >= 0 && j < layout[i].size();
       --i, ++j) {
    // RIGHT
    if (is_floor(layout[i][j]))
      continue;

    seats.push_back(layout[i][j]);
    break;
  }

  // BELOW
  for (int i = seat.row + 1; i < layout.size(); ++i) {
    // UP
    if (is_floor(layout[i][seat.col]))
      continue;

    seats.push_back(layout[i][seat.col]);
    break;
  }

  // BELOW
  for (int i = seat.row + 1, j = seat.col - 1; i < layout.size() && j >= 0;
       ++i, --j) {
    // LEFT
    if (is_floor(layout[i][j]))
      continue;

    seats.push_back(layout[i][j]);
    break;
  }

  // BELOW
  for (int i = seat.row + 1, j = seat.col + 1;
       i < layout.size() && j < layout[i].size(); ++i, ++j) {
    // RIGHT
    if (is_floor(layout[i][j]))
      continue;

    seats.push_back(layout[i][j]);
    break;
  }

  // DIRECTLY LEFT
  for (int j = seat.col - 1; j >= 0; --j) {
    if (is_floor(layout[seat.row][j]))
      continue;

    seats.push_back(layout[seat.row][j]);
    break;
  }
  // DIRECTLY RIGHT
  for (int j = seat.col + 1; j < layout[seat.row].size(); ++j) {
    if (is_floor(layout[seat.row][j]))
      continue;

    seats.push_back(layout[seat.row][j]);
    break;
  }

  return seats;
}

template <typename SeatContainerIteratorT>
std::vector<std::reference_wrapper<const seat_t>>
update(const seat_t &seat, const int occupied_tolerance,
       SeatContainerIteratorT begin, SeatContainerIteratorT end) {
  std::vector<std::reference_wrapper<const seat_t>> to_update;

  if (is_floor(seat)) {
    return to_update;
  }

  if (is_unoccupied(seat)) {
    if (0 == std::count_if(begin, end,
                           [](const auto &e) { return is_occupied(e); })) {
      to_update.push_back(seat);
    }
  } else {
    if (std::count_if(begin, end, [](const auto &e) {
          return is_occupied(e);
        }) >= occupied_tolerance) {
      to_update.push_back(seat);
    }
  }

  return to_update;
}

std::vector<std::reference_wrapper<const seat_t>>
sim_step_part1(const seat_layout_t &layout, const seat_t &seat) {
  std::vector<std::reference_wrapper<const seat_t>> to_update;

  if (is_floor(seat)) {
    return to_update;
  }

  const auto &adj_seats = get_adjacent_seats_part1(layout, seat);
  to_update = update(seat, 4, adj_seats.begin(), adj_seats.end());

  return to_update;
}

std::vector<std::reference_wrapper<const seat_t>>
sim_step_part2(const seat_layout_t &layout, const seat_t &seat) {
  std::vector<std::reference_wrapper<const seat_t>> to_update;

  if (is_floor(seat)) {
    return to_update;
  }

  const auto &adj_seats = get_adjacent_seats_part2(layout, seat);
  to_update = update(seat, 5, adj_seats.begin(), adj_seats.end());

  return to_update;
}

template <typename StepFuncT>
void sim(seat_layout_t &layout, StepFuncT &step, bool debug = false) {
  int row = 0, col = 0;
  bool changed = true;

  while (changed) {
    std::set<std::pair<int, int>> to_update_from_step;

    for (row = 0; row < layout.size(); ++row) {
      for (col = 0; col < layout[row].size(); ++col) {
        auto to_update = step(layout, layout[row][col]);

        for (const auto &e : to_update) {
          to_update_from_step.insert(std::make_pair(e.get().row, e.get().col));
        }
      }
    }

    changed = false;
    for (auto &e : to_update_from_step) {
      flip_seat(layout[e.first][e.second]);
      changed = true;
    }

    if (debug && changed) {
      std::cout << layout << '\n';
    }
  }

  return;
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
  seat_layout_t input;
  int row = 0, col = 0;

  while (std::getline(fin, inputval)) {
    row_t row_seats;
    col = 0;
    for (auto &e : inputval) {
      row_seats.push_back({row, col, e});
      col++;
    }
    row++;

    input.push_back(row_seats);
  }

  auto layout_part1 = std::move(input);
  auto layout_part2 = layout_part1;
  sim(layout_part1, sim_step_part1);
  sim(layout_part2, sim_step_part2);

  const auto &count_occupied = [](const auto &layout) {
    int acc = 0;
    std::for_each(layout.begin(), layout.end(), [&acc](const auto &row) {
      acc += std::count_if(row.begin(), row.end(), [](const auto &e) {
        return !is_floor(e) && is_occupied(e);
      });
    });

    return acc;
  };
  std::cout << count_occupied(layout_part1) << std::endl;
  std::cout << count_occupied(layout_part2) << std::endl;

  return EXIT_SUCCESS;
}
