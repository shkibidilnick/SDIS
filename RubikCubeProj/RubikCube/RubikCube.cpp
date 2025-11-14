#include "RubikCube.h"

#include <algorithm>
#include <fstream>
#include <random>
#include <sstream>
#include <stdexcept>

// Constructor
RubikCube::RubikCube() { initializeSolvedState(); }

void RubikCube::initializeSolvedState() {
  std::array<Color, 6> facetColors = {
      Color::White,   // facet 0 - Front
      Color::Yellow,  // facet 1 - Back
      Color::Orange,  // facet 2 - Left (was Red)
      Color::Red,     // facet 3 - Right (was Orange)
      Color::Blue,    // facet 4 - Up
      Color::Green,   // facet 5 - Down
  };

  for (int facet = 0; facet < 6; facet++) {
    for (int row = 0; row < 3; row++) {
      for (int col = 0; col < 3; col++) {
        cube[facet][row][col] = facetColors[facet];
      }
    }
  }
}

bool RubikCube::operator==(const RubikCube& other) const {
  return this->cube == other.cube;
}

bool RubikCube::operator!=(const RubikCube& other) const {
  return !(*this == other);
}

void RubikCube::randomize() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 5);
  std::uniform_int_distribution<> boolDis(0, 1);

  for (int i = 0; i < 20; i++) {
    int move = dis(gen);
    bool clockwise = boolDis(gen) == 1;
    switch (move) {
      case 0:
        rotateFront(clockwise);
        break;
      case 1:
        rotateBack(clockwise);
        break;
      case 2:
        rotateLeft(clockwise);
        break;
      case 3:
        rotateRight(clockwise);
        break;
      case 4:
        rotateUp(clockwise);
        break;
      case 5:
        rotateDown(clockwise);
        break;
    }
  }
}

bool RubikCube::isSolved() const {
  for (int facet = 0; facet < 6; facet++) {
    Color facetFirstColor = cube[facet][0][0];
    for (int row = 0; row < 3; row++) {
      for (int col = 0; col < 3; col++) {
        if (cube[facet][row][col] != facetFirstColor) {
          return false;
        }
      }
    }
  }
  return true;
}

void RubikCube::rotateFacetClockwise(int facet) {
  std::array<std::array<Color, 3>, 3> temp;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      temp[i][j] = cube[facet][2 - j][i];
    }
  }
  cube[facet] = temp;
  // Example
  /*
  * Before:              After:
    [0,0] [0,1] [0,2]    [2,0] [1,0] [0,0]
    [1,0] [1,1] [1,2] -> [2,1] [1,1] [0,1]
    [2,0] [2,1] [2,2]    [2,2] [1,2] [0,2]
  */
}

void RubikCube::rotateFacetCounterClockwise(int facet) {
  std::array<std::array<Color, 3>, 3> temp;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      temp[i][j] = cube[facet][j][2 - i];
    }
  }
  cube[facet] = temp;
}

void RubikCube::rotateFacetWithRules(int facet, bool clockwise,
                                     const std::array<RotationRule, 4>& rules) {
  // Save all source colors before any changes
  std::array<Color, 12> savedColors;
  int colorIndex = 0;

  if (clockwise) {
    // For clockwise rotation: save from source positions
    for (const auto& rule : rules) {
      for (int i = 0; i < 3; i++) {
        const auto& pos = rule.sourcePositions[i];
        savedColors[colorIndex++] =
            cube[rule.sourceFacet][pos.first][pos.second];
      }
    }

    // Rotate the main facet
    rotateFacetClockwise(facet);

    // Apply saved colors to target positions
    colorIndex = 0;
    for (const auto& rule : rules) {
      for (int i = 0; i < 3; i++) {
        const auto& pos = rule.targetPositions[i];
        cube[rule.targetFacet][pos.first][pos.second] =
            savedColors[colorIndex++];
      }
    }
  } else {
    // For counter-clockwise rotation: save from target positions (in reverse
    // order)
    for (int r = 3; r >= 0; r--) {
      const auto& rule = rules[r];
      for (int i = 2; i >= 0; i--) {
        const auto& pos = rule.targetPositions[i];
        savedColors[colorIndex++] =
            cube[rule.targetFacet][pos.first][pos.second];
      }
    }

    // Rotate the main facet
    rotateFacetCounterClockwise(facet);

    // Apply saved colors to source positions (in reverse order)
    colorIndex = 0;
    for (int r = 3; r >= 0; r--) {
      const auto& rule = rules[r];
      for (int i = 2; i >= 0; i--) {
        const auto& pos = rule.sourcePositions[i];
        cube[rule.sourceFacet][pos.first][pos.second] =
            savedColors[colorIndex++];
      }
    }
  }
}

void RubikCube::rotateFront(bool clockwise) {
  std::array<RotationRule, 4> rules = {{
      {4, {{{2, 0}, {2, 1}, {2, 2}}}, 3, {{{0, 0}, {1, 0}, {2, 0}}}},
      {3, {{{0, 0}, {1, 0}, {2, 0}}}, 5, {{{0, 2}, {0, 1}, {0, 0}}}},
      {5, {{{0, 2}, {0, 1}, {0, 0}}}, 2, {{{2, 2}, {1, 2}, {0, 2}}}},
      {2, {{{2, 2}, {1, 2}, {0, 2}}}, 4, {{{2, 0}, {2, 1}, {2, 2}}}},
  }};
  rotateFacetWithRules(0, clockwise, rules);
}

void RubikCube::rotateBack(bool clockwise) {
  std::array<RotationRule, 4> rules = {{
      {4, {{{0, 2}, {0, 1}, {0, 0}}}, 2, {{{0, 0}, {1, 0}, {2, 0}}}},
      {2, {{{0, 0}, {1, 0}, {2, 0}}}, 5, {{{2, 0}, {2, 1}, {2, 2}}}},
      {5, {{{2, 2}, {2, 1}, {2, 0}}}, 3, {{{0, 2}, {1, 2}, {2, 2}}}},
      {3, {{{0, 2}, {1, 2}, {2, 2}}}, 4, {{{0, 2}, {0, 1}, {0, 0}}}},
  }};
  rotateFacetWithRules(1, clockwise, rules);
}

void RubikCube::rotateLeft(bool clockwise) {
  std::array<RotationRule, 4> rules = {{
      {4, {{{0, 0}, {1, 0}, {2, 0}}}, 0, {{{0, 0}, {1, 0}, {2, 0}}}},
      {0, {{{0, 0}, {1, 0}, {2, 0}}}, 5, {{{0, 0}, {1, 0}, {2, 0}}}},
      {5, {{{0, 0}, {1, 0}, {2, 0}}}, 1, {{{2, 2}, {1, 2}, {0, 2}}}},
      {1, {{{2, 2}, {1, 2}, {0, 2}}}, 4, {{{0, 0}, {1, 0}, {2, 0}}}},
  }};
  rotateFacetWithRules(2, clockwise, rules);
}

void RubikCube::rotateRight(bool clockwise) {
  std::array<RotationRule, 4> rules = {{
      {4, {{{2, 2}, {1, 2}, {0, 2}}}, 1, {{{0, 0}, {1, 0}, {2, 0}}}},
      {1, {{{0, 0}, {1, 0}, {2, 0}}}, 5, {{{2, 2}, {1, 2}, {0, 2}}}},
      {5, {{{2, 2}, {1, 2}, {0, 2}}}, 0, {{{2, 2}, {1, 2}, {0, 2}}}},
      {0, {{{2, 2}, {1, 2}, {0, 2}}}, 4, {{{2, 2}, {1, 2}, {0, 2}}}},
  }};
  rotateFacetWithRules(3, clockwise, rules);
}

void RubikCube::rotateUp(bool clockwise) {
  std::array<RotationRule, 4> rules = {{
      {1, {{{0, 2}, {0, 1}, {0, 0}}}, 3, {{{0, 2}, {0, 1}, {0, 0}}}},
      {3, {{{0, 2}, {0, 1}, {0, 0}}}, 0, {{{0, 2}, {0, 1}, {0, 0}}}},
      {0, {{{0, 2}, {0, 1}, {0, 0}}}, 2, {{{0, 2}, {0, 1}, {0, 0}}}},
      {2, {{{0, 2}, {0, 1}, {0, 0}}}, 1, {{{0, 2}, {0, 1}, {0, 0}}}},
  }};
  rotateFacetWithRules(4, clockwise, rules);
}

void RubikCube::rotateDown(bool clockwise) {
  std::array<RotationRule, 4> rules = {{
      {0, {{{2, 0}, {2, 1}, {2, 2}}}, 3, {{{2, 0}, {2, 1}, {2, 2}}}},
      {3, {{{2, 0}, {2, 1}, {2, 2}}}, 1, {{{2, 0}, {2, 1}, {2, 2}}}},
      {1, {{{2, 0}, {2, 1}, {2, 2}}}, 2, {{{2, 0}, {2, 1}, {2, 2}}}},
      {2, {{{2, 0}, {2, 1}, {2, 2}}}, 0, {{{2, 0}, {2, 1}, {2, 2}}}},
  }};
  rotateFacetWithRules(5, clockwise, rules);
}

bool RubikCube::loadFromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    return false;
  }

  try {
    file >> *this;
    return true;
  } catch (...) {
    return false;
  }
}

bool RubikCube::saveToFile(const std::string& filename) const {
  std::ofstream file(filename);
  if (!file.is_open()) {
    return false;
  }

  file << *this;
  return file.good();
}

Color RubikCube::getColor(int facet, int row, int col) const {
  if (facet < 0 || facet >= 6 || row < 0 || row >= 3 || col < 0 || col >= 3) {
    throw std::out_of_range("Invalid facet, row, column index");
  }
  return cube[facet][row][col];
}

void RubikCube::setColor(int facet, int row, int col, Color color) {
  if (facet < 0 || facet >= 6 || row < 0 || row >= 3 || col < 0 || col >= 3) {
    throw std::out_of_range("Invalid facet, row or column index");
  }
  cube[facet][row][col] = color;
}

std::string RubikCube::getColorSymbol() const { return "\xFE"; }

std::string RubikCube::getColorCode(Color color) const {
  switch (color) {
    case Color::White:
      return "\033[97m";
    case Color::Yellow:
      return "\033[93m";
    case Color::Red:
      return "\033[91m";
    case Color::Orange:
      return "\033[38;5;208m";
    case Color::Blue:
      return "\033[94m";
    case Color::Green:
      return "\033[92m";
    default:
      return "\033[0m";
  }
}

void RubikCube::displayColored(std::ostream& os) const {
  std::string reset = "\033[0m";

  auto getFaceRow = [&](int facet, int row) -> std::string {
    std::string result;
    for (int col = 0; col < 3; col++) {
      Color currentColor = getColor(facet, row, col);
      std::string colorCode = getColorCode(currentColor);
      std::string symbol = getColorSymbol();
      result += colorCode + symbol + reset + " ";
    }
    return result;
  };

  os << "       ";
  os << getFaceRow(4, 0) << "\n";

  os << "       ";
  os << getFaceRow(4, 1) << "\n";

  os << "       ";
  os << getFaceRow(4, 2) << "\n";

  os << "\n";

  for (int row = 0; row < 3; row++) {
    os << getFaceRow(2, row) << " ";
    os << getFaceRow(0, row) << " ";
    os << getFaceRow(3, row) << " ";
    os << getFaceRow(1, row);
    os << "\n";
  }

  os << "\n";

  os << "       ";
  os << getFaceRow(5, 0) << "\n";

  os << "       ";
  os << getFaceRow(5, 1) << "\n";

  os << "       ";
  os << getFaceRow(5, 2) << "\n";
}
std::ostream& operator<<(std::ostream& os, const RubikCube& cube) {
  for (int facet = 0; facet < 6; facet++) {
    os << "Facet" << facet << ": ";
    for (int row = 0; row < 3; row++) {
      for (int col = 0; col < 3; col++) {
        os << static_cast<int>(cube.getColor(facet, row, col));
        if (row != 2 || col != 2) {
          os << " ";
        }
      }
      if (row != 2) {
        os << " | ";
      }
    }
    os << "\n";
  }
  return os;
}

std::istream& operator>>(std::istream& is, RubikCube& cube) {
  std::string line;
  for (int facet = 0; facet < 6; facet++) {
    std::getline(is, line);
    std::stringstream ss(line.substr(line.find(':') + 1));

    for (int row = 0; row < 3; row++) {
      for (int col = 0; col < 3; col++) {
        int colorValue;
        ss >> colorValue;
        if (col == 2 && row != 2) {
          char separator;
          ss >> separator;
        }
        cube.setColor(facet, row, col, static_cast<Color>(colorValue));
      }
    }
  }
  return is;
}
