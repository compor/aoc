//
//
//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>

struct pos_t {
  int x;
  int y;
  int direction;
};

struct cmd_t {
  char action;
  int val;
};

using instructions_t = std::vector<cmd_t>;

void process_part1(pos_t &pos, const cmd_t &cmd);

void move(pos_t &pos, const pos_t &relative_move) {
  pos.x += relative_move.x;
  pos.y += relative_move.y;
}

void forward(pos_t &pos, const cmd_t &cmd) {
  switch (pos.direction) {
  case 0:
    process_part1(pos, {'E', cmd.val});
    break;
  case 1:
    process_part1(pos, {'N', cmd.val});
    break;
  case 2:
    process_part1(pos, {'W', cmd.val});
    break;
  case 3:
    process_part1(pos, {'S', cmd.val});
    break;
  default:
    std::cerr << "Unsupported direction " << pos.direction << "!\n";
    std::exit(EXIT_FAILURE);
    break;
  }
}

void turn(pos_t &pos, const cmd_t &cmd) {
  int turn = 0;

  if (cmd.action == 'R') {
    turn = (cmd.val / 90) % 4;
    pos.direction = (pos.direction - turn) % 4;
    if (pos.direction < 0) {
      pos.direction += 4;
    }
  } else {
    turn = (cmd.val / 90) % 4;
    pos.direction = (pos.direction + turn) % 4;
  }
}

void process_part1(pos_t &pos, const cmd_t &cmd) {
  switch (cmd.action) {
  case 'N':
    move(pos, {0, cmd.val, 0});
    break;
  case 'S':
    move(pos, {0, -1 * cmd.val, 0});
    break;
  case 'E':
    move(pos, {cmd.val, 0, 0});
    break;
  case 'W':
    move(pos, {-1 * cmd.val, 0, 0});
    break;
  case 'F':
    forward(pos, cmd);
    break;
  case 'R':
  case 'L':
    turn(pos, cmd);
    break;
  default:
    std::cerr << "Unsupported command " << cmd.action << "!\n";
    std::exit(EXIT_FAILURE);
    break;
  }
}

void process_part2(pos_t &ship, pos_t &waypoint, const cmd_t &cmd) {
  switch (cmd.action) {
  case 'N':
  case 'S':
  case 'E':
  case 'W':
    break;
  default:
    break;
  }
}

int glasgow_distance(const pos_t &pos1, const pos_t &pos2) {
  return std::abs(pos2.x - pos1.x) + std::abs(pos2.y - pos1.y);
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
  instructions_t input;

  while (std::getline(fin, inputval)) {
    input.push_back({inputval[0], std::atoi(inputval.c_str() + 1)});
  }

  pos_t pos{0, 0, 0};
  pos_t start = pos;

  for (const auto &e : input) {
    process_part1(pos, e);
  }

  std::cout << glasgow_distance(pos, start) << std::endl;

  return EXIT_SUCCESS;
}
