#ifndef RUBIKCUBE_H
#define RUBIKCUBE_H

#include <array>
#include <iostream>
#include <string>

enum class Color { White, Yellow, Red, Orange, Blue, Green };

class RubikCube {
 private:
  std::array<std::array<std::array<Color, 3>, 3>, 6> cube;

  // Private helper methods
  void initializeSolvedState();
  void rotateFacetClockwise(int facet);
  void rotateFacetCounterClockwise(int facet);

  struct RotationRule {
    int sourceFacet;
    std::array<std::pair<int, int>, 3> sourcePositions;
    int targetFacet;
    std::array<std::pair<int, int>, 3> targetPositions;
  };

  void rotateFacetWithRules(int facet, bool clockwise,
                            const std::array<RotationRule, 4>& rules);

  std::string getColorSymbol() const;
  std::string getColorCode(Color color) const;

 public:
  // Constructors
  RubikCube();
  RubikCube(const RubikCube& other) = default;

  // Operators
  RubikCube& operator=(const RubikCube& other) = default;
  bool operator==(const RubikCube& other) const;
  bool operator!=(const RubikCube& other) const;

  // General methods
  void randomize();
  bool isSolved() const;

  void rotateFront(bool clockwise);
  void rotateBack(bool clockwise);
  void rotateLeft(bool clockwise);
  void rotateRight(bool clockwise);
  void rotateUp(bool clockwise);
  void rotateDown(bool clockwise);

  // Work with files
  friend std::ostream& operator<<(std::ostream& os, const RubikCube& cube);
  friend std::istream& operator>>(std::istream& is, RubikCube& cube);

  bool loadFromFile(const std::string& filename);
  bool saveToFile(const std::string& filename) const;

  // Method get
  Color getColor(int facet, int row, int col) const;

  // Method Set
  void setColor(int facet, int row, int col, Color color);

  void displayColored(std::ostream& os) const;
};

#endif
