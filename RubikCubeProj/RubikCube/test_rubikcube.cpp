#include <UnitTest++/UnitTest++.h>

#include <fstream>
#include <map>
#include <sstream>

#include "../RubikCube/RubikCube.h"

// Test 1: Default constructor creates solved cube
TEST(DefaultConstructor_CreatesSolvedCube) {
  RubikCube cube;
  CHECK(cube.isSolved());
}

// Test 2: Copy constructor
TEST(CopyConstructor_CopiesCorrectly) {
  RubikCube cube1;
  cube1.rotateFront(true);
  RubikCube cube2(cube1);
  CHECK(cube1 == cube2);
}

// Test 3: Assignment operator
TEST(AssignmentOperator_CopiesCorrectly) {
  RubikCube cube1;
  cube1.rotateFront(true);
  RubikCube cube2;
  cube2 = cube1;
  CHECK(cube1 == cube2);
}

// Test 4: Equality operator for solved cubes
TEST(EqualityOperator_SolvedCubes_AreEqual) {
  RubikCube cube1;
  RubikCube cube2;
  CHECK(cube1 == cube2);
}

// Test 5: Inequality operator for different cubes
TEST(InequalityOperator_DifferentCubes_AreNotEqual) {
  RubikCube cube1;
  RubikCube cube2;
  cube2.rotateFront(true);
  CHECK(cube1 != cube2);
}

// Test 6.1: Front clockwise rotation moves colors correctly for all adjacent
// faces
TEST(RotateFront_Clockwise_MovesColorsCorrectly) {
  RubikCube cube;

  Color up_bottom_left = cube.getColor(4, 2, 0);    // Up[2,0]
  Color up_bottom_center = cube.getColor(4, 2, 1);  // Up[2,1]
  Color up_bottom_right = cube.getColor(4, 2, 2);   // Up[2,2]

  Color right_left_top = cube.getColor(3, 0, 0);     // Right[0,0]
  Color right_left_middle = cube.getColor(3, 1, 0);  // Right[1,0]
  Color right_left_bottom = cube.getColor(3, 2, 0);  // Right[2,0]

  Color down_top_right = cube.getColor(5, 0, 2);   // Down[0,2]
  Color down_top_center = cube.getColor(5, 0, 1);  // Down[0,1]
  Color down_top_left = cube.getColor(5, 0, 0);    // Down[0,0]

  Color left_right_bottom = cube.getColor(2, 2, 2);  // Left[2,2]
  Color left_right_middle = cube.getColor(2, 1, 2);  // Left[1,2]
  Color left_right_top = cube.getColor(2, 0, 2);     // Left[0,2]

  cube.rotateFront(true);

  // Up → Right
  CHECK(cube.getColor(3, 0, 0) == up_bottom_left);    // Up[2,0] → Right[0,0]
  CHECK(cube.getColor(3, 1, 0) == up_bottom_center);  // Up[2,1] → Right[1,0]
  CHECK(cube.getColor(3, 2, 0) == up_bottom_right);   // Up[2,2] → Right[2,0]

  // Right → Down
  CHECK(cube.getColor(5, 0, 2) == right_left_top);     // Right[0,0] → Down[0,2]
  CHECK(cube.getColor(5, 0, 1) == right_left_middle);  // Right[1,0] → Down[0,1]
  CHECK(cube.getColor(5, 0, 0) == right_left_bottom);  // Right[2,0] → Down[0,0]

  // Down → Left
  CHECK(cube.getColor(2, 2, 2) == down_top_right);   // Down[0,2] → Left[2,2]
  CHECK(cube.getColor(2, 1, 2) == down_top_center);  // Down[0,1] → Left[1,2]
  CHECK(cube.getColor(2, 0, 2) == down_top_left);    // Down[0,0] → Left[0,2]

  // Left → Up
  CHECK(cube.getColor(4, 2, 0) == left_right_bottom);  // Left[2,2] → Up[2,0]
  CHECK(cube.getColor(4, 2, 1) == left_right_middle);  // Left[1,2] → Up[2,1]
  CHECK(cube.getColor(4, 2, 2) == left_right_top);     // Left[0,2] → Up[2,2]
}

// Test 6.2: Front counter-clockwise rotation moves colors correctly
TEST(RotateFront_CounterClockwise_MovesColorsCorrectly) {
  RubikCube cube;

  Color up_bottom_left = cube.getColor(4, 2, 0);    // Up[2,0]
  Color up_bottom_center = cube.getColor(4, 2, 1);  // Up[2,1]
  Color up_bottom_right = cube.getColor(4, 2, 2);   // Up[2,2]

  Color right_left_top = cube.getColor(3, 0, 0);     // Right[0,0]
  Color right_left_middle = cube.getColor(3, 1, 0);  // Right[1,0]
  Color right_left_bottom = cube.getColor(3, 2, 0);  // Right[2,0]

  Color down_top_right = cube.getColor(5, 0, 2);   // Down[0,2]
  Color down_top_center = cube.getColor(5, 0, 1);  // Down[0,1]
  Color down_top_left = cube.getColor(5, 0, 0);    // Down[0,0]

  Color left_right_bottom = cube.getColor(2, 2, 2);  // Left[2,2]
  Color left_right_middle = cube.getColor(2, 1, 2);  // Left[1,2]
  Color left_right_top = cube.getColor(2, 0, 2);     // Left[0,2]

  cube.rotateFront(false);

  // Right → Up
  CHECK(cube.getColor(4, 2, 0) == right_left_top);     // Right[0,0] → Up[2,0]
  CHECK(cube.getColor(4, 2, 1) == right_left_middle);  // Right[1,0] → Up[2,1]
  CHECK(cube.getColor(4, 2, 2) == right_left_bottom);  // Right[2,0] → Up[2,2]

  // Down → Right
  CHECK(cube.getColor(3, 0, 0) == down_top_right);   // Down[0,2] → Right[0,0]
  CHECK(cube.getColor(3, 1, 0) == down_top_center);  // Down[0,1] → Right[1,0]
  CHECK(cube.getColor(3, 2, 0) == down_top_left);    // Down[0,0] → Right[2,0]

  // Left → Down
  CHECK(cube.getColor(5, 0, 2) == left_right_bottom);  // Left[2,2] → Down[0,2]
  CHECK(cube.getColor(5, 0, 1) == left_right_middle);  // Left[1,2] → Down[0,1]
  CHECK(cube.getColor(5, 0, 0) == left_right_top);     // Left[0,2] → Down[0,0]

  // Up → Left
  CHECK(cube.getColor(2, 2, 2) == up_bottom_left);    // Up[2,0] → Left[2,2]
  CHECK(cube.getColor(2, 1, 2) == up_bottom_center);  // Up[2,1] → Left[1,2]
  CHECK(cube.getColor(2, 0, 2) == up_bottom_right);   // Up[2,2] → Left[0,2]
}

// Test 6.3: Back clockwise rotation moves colors correctly
TEST(RotateBack_Clockwise_MovesColorsCorrectly) {
  RubikCube cube;

  Color up_top_left = cube.getColor(4, 0, 0);    // Up[0,0]
  Color up_top_center = cube.getColor(4, 0, 1);  // Up[0,1]
  Color up_top_right = cube.getColor(4, 0, 2);   // Up[0,2]

  Color left_left_top = cube.getColor(2, 0, 0);     // Left[0,0]
  Color left_left_middle = cube.getColor(2, 1, 0);  // Left[1,0]
  Color left_left_bottom = cube.getColor(2, 2, 0);  // Left[2,0]

  Color down_bottom_right = cube.getColor(5, 2, 2);   // Down[2,2]
  Color down_bottom_center = cube.getColor(5, 2, 1);  // Down[2,1]
  Color down_bottom_left = cube.getColor(5, 2, 0);    // Down[2,0]

  Color right_right_bottom = cube.getColor(3, 2, 2);  // Right[2,2]
  Color right_right_middle = cube.getColor(3, 1, 2);  // Right[1,2]
  Color right_right_top = cube.getColor(3, 0, 2);     // Right[0,2]

  cube.rotateBack(true);

  // Up → Left
  CHECK(cube.getColor(2, 0, 0) == up_top_left);    // Up[0,0] → Left[0,0]
  CHECK(cube.getColor(2, 1, 0) == up_top_center);  // Up[0,1] → Left[1,0]
  CHECK(cube.getColor(2, 2, 0) == up_top_right);   // Up[0,2] → Left[2,0]

  // Left → Down
  CHECK(cube.getColor(5, 2, 2) == left_left_top);     // Left[0,0] → Down[2,2]
  CHECK(cube.getColor(5, 2, 1) == left_left_middle);  // Left[1,0] → Down[2,1]
  CHECK(cube.getColor(5, 2, 0) == left_left_bottom);  // Left[2,0] → Down[2,0]

  // Down → Right
  CHECK(cube.getColor(3, 2, 2) == down_bottom_right);  // Down[2,2] → Right[2,2]
  CHECK(cube.getColor(3, 1, 2) ==
        down_bottom_center);                          // Down[2,1] → Right[1,2]
  CHECK(cube.getColor(3, 0, 2) == down_bottom_left);  // Down[2,0] → Right[0,2]

  // Right → Up
  CHECK(cube.getColor(4, 0, 0) == right_right_bottom);  // Right[2,2] → Up[0,0]
  CHECK(cube.getColor(4, 0, 1) == right_right_middle);  // Right[1,2] → Up[0,1]
  CHECK(cube.getColor(4, 0, 2) == right_right_top);     // Right[0,2] → Up[0,2]
}

// Test 6.4: Back counter-clockwise rotation moves colors correctly (полная
// версия)
TEST(RotateBack_CounterClockwise_MovesColorsCorrectly_Full) {
  RubikCube cube;

  Color up_top_left = cube.getColor(4, 0, 0);    // Up[0,0]
  Color up_top_center = cube.getColor(4, 0, 1);  // Up[0,1]
  Color up_top_right = cube.getColor(4, 0, 2);   // Up[0,2]

  Color left_left_top = cube.getColor(2, 0, 0);     // Left[0,0]
  Color left_left_middle = cube.getColor(2, 1, 0);  // Left[1,0]
  Color left_left_bottom = cube.getColor(2, 2, 0);  // Left[2,0]

  Color down_bottom_right = cube.getColor(5, 2, 2);   // Down[2,2]
  Color down_bottom_center = cube.getColor(5, 2, 1);  // Down[2,1]
  Color down_bottom_left = cube.getColor(5, 2, 0);    // Down[2,0]

  Color right_right_bottom = cube.getColor(3, 2, 2);  // Right[2,2]
  Color right_right_middle = cube.getColor(3, 1, 2);  // Right[1,2]
  Color right_right_top = cube.getColor(3, 0, 2);     // Right[0,2]

  cube.rotateBack(false);

  // Right → Down
  CHECK(cube.getColor(5, 2, 2) ==
        right_right_bottom);  // Right[2,2] → Down[2,2]
  CHECK(cube.getColor(5, 2, 1) ==
        right_right_middle);                         // Right[1,2] → Down[2,1]
  CHECK(cube.getColor(5, 2, 0) == right_right_top);  // Right[0,2] → Down[2,0]

  // Down → Left
  CHECK(cube.getColor(2, 0, 0) == down_bottom_right);   // Down[2,2] → Left[0,0]
  CHECK(cube.getColor(2, 1, 0) == down_bottom_center);  // Down[2,1] → Left[1,0]
  CHECK(cube.getColor(2, 2, 0) == down_bottom_left);    // Down[2,0] → Left[2,0]

  // Left → Up
  CHECK(cube.getColor(4, 0, 0) == left_left_top);     // Left[0,0] → Up[0,0]
  CHECK(cube.getColor(4, 0, 1) == left_left_middle);  // Left[1,0] → Up[0,1]
  CHECK(cube.getColor(4, 0, 2) == left_left_bottom);  // Left[2,0] → Up[0,2]

  // Up → Right
  CHECK(cube.getColor(3, 2, 2) == up_top_left);    // Up[0,0] → Right[2,2]
  CHECK(cube.getColor(3, 1, 2) == up_top_center);  // Up[0,1] → Right[1,2]
  CHECK(cube.getColor(3, 0, 2) == up_top_right);   // Up[0,2] → Right[0,2]
}

// Test 6.5: Left clockwise rotation moves colors correctly
TEST(RotateLeft_Clockwise_MovesColorsCorrectly) {
  RubikCube cube;

  // Запоминаем цвета
  Color up_left_top = cube.getColor(4, 0, 0);     // Up[0,0]
  Color up_left_middle = cube.getColor(4, 1, 0);  // Up[1,0]
  Color up_left_bottom = cube.getColor(4, 2, 0);  // Up[2,0]

  Color front_left_top = cube.getColor(0, 0, 0);     // Front[0,0]
  Color front_left_middle = cube.getColor(0, 1, 0);  // Front[1,0]
  Color front_left_bottom = cube.getColor(0, 2, 0);  // Front[2,0]

  Color down_left_top = cube.getColor(5, 0, 0);     // Down[0,0]
  Color down_left_middle = cube.getColor(5, 1, 0);  // Down[1,0]
  Color down_left_bottom = cube.getColor(5, 2, 0);  // Down[2,0]

  Color back_right_bottom = cube.getColor(1, 2, 2);  // Back[2,2]
  Color back_right_middle = cube.getColor(1, 1, 2);  // Back[1,2]
  Color back_right_top = cube.getColor(1, 0, 2);     // Back[0,2]

  cube.rotateLeft(true);

  // Up → Front
  CHECK(cube.getColor(0, 0, 0) == up_left_top);     // Up[0,0] → Front[0,0]
  CHECK(cube.getColor(0, 1, 0) == up_left_middle);  // Up[1,0] → Front[1,0]
  CHECK(cube.getColor(0, 2, 0) == up_left_bottom);  // Up[2,0] → Front[2,0]

  // Front → Down
  CHECK(cube.getColor(5, 0, 0) == front_left_top);     // Front[0,0] → Down[0,0]
  CHECK(cube.getColor(5, 1, 0) == front_left_middle);  // Front[1,0] → Down[1,0]
  CHECK(cube.getColor(5, 2, 0) == front_left_bottom);  // Front[2,0] → Down[2,0]

  // Down → Back
  CHECK(cube.getColor(1, 2, 2) == down_left_top);     // Down[0,0] → Back[2,2]
  CHECK(cube.getColor(1, 1, 2) == down_left_middle);  // Down[1,0] → Back[1,2]
  CHECK(cube.getColor(1, 0, 2) == down_left_bottom);  // Down[2,0] → Back[0,2]

  // Back → Up
  CHECK(cube.getColor(4, 0, 0) == back_right_bottom);  // Back[2,2] → Up[0,0]
  CHECK(cube.getColor(4, 1, 0) == back_right_middle);  // Back[1,2] → Up[1,0]
  CHECK(cube.getColor(4, 2, 0) == back_right_top);     // Back[0,2] → Up[2,0]
}

// Test 6.6: Left counter-clockwise rotation moves colors correctly (полная
// версия)
TEST(RotateLeft_CounterClockwise_MovesColorsCorrectly_Full) {
  RubikCube cube;

  Color up_left_top = cube.getColor(4, 0, 0);     // Up[0,0]
  Color up_left_middle = cube.getColor(4, 1, 0);  // Up[1,0]
  Color up_left_bottom = cube.getColor(4, 2, 0);  // Up[2,0]

  Color front_left_top = cube.getColor(0, 0, 0);     // Front[0,0]
  Color front_left_middle = cube.getColor(0, 1, 0);  // Front[1,0]
  Color front_left_bottom = cube.getColor(0, 2, 0);  // Front[2,0]

  Color down_left_top = cube.getColor(5, 0, 0);     // Down[0,0]
  Color down_left_middle = cube.getColor(5, 1, 0);  // Down[1,0]
  Color down_left_bottom = cube.getColor(5, 2, 0);  // Down[2,0]

  Color back_right_bottom = cube.getColor(1, 2, 2);  // Back[2,2]
  Color back_right_middle = cube.getColor(1, 1, 2);  // Back[1,2]
  Color back_right_top = cube.getColor(1, 0, 2);     // Back[0,2]

  cube.rotateLeft(false);

  // Back → Down
  CHECK(cube.getColor(5, 0, 0) == back_right_bottom);  // Back[2,2] → Down[0,0]
  CHECK(cube.getColor(5, 1, 0) == back_right_middle);  // Back[1,2] → Down[1,0]
  CHECK(cube.getColor(5, 2, 0) == back_right_top);     // Back[0,2] → Down[2,0]

  // Down → Front
  CHECK(cube.getColor(0, 0, 0) == down_left_top);     // Down[0,0] → Front[0,0]
  CHECK(cube.getColor(0, 1, 0) == down_left_middle);  // Down[1,0] → Front[1,0]
  CHECK(cube.getColor(0, 2, 0) == down_left_bottom);  // Down[2,0] → Front[2,0]

  // Front → Up
  CHECK(cube.getColor(4, 0, 0) == front_left_top);     // Front[0,0] → Up[0,0]
  CHECK(cube.getColor(4, 1, 0) == front_left_middle);  // Front[1,0] → Up[1,0]
  CHECK(cube.getColor(4, 2, 0) == front_left_bottom);  // Front[2,0] → Up[2,0]

  // Up → Back
  CHECK(cube.getColor(1, 2, 2) == up_left_top);     // Up[0,0] → Back[2,2]
  CHECK(cube.getColor(1, 1, 2) == up_left_middle);  // Up[1,0] → Back[1,2]
  CHECK(cube.getColor(1, 0, 2) == up_left_bottom);  // Up[2,0] → Back[0,2]
}

// Test 6.7: Right clockwise rotation moves colors correctly
TEST(RotateRight_Clockwise_MovesColorsCorrectly) {
  RubikCube cube;

  // Запоминаем цвета
  Color up_right_top = cube.getColor(4, 0, 2);     // Up[0,2]
  Color up_right_middle = cube.getColor(4, 1, 2);  // Up[1,2]
  Color up_right_bottom = cube.getColor(4, 2, 2);  // Up[2,2]

  Color back_left_bottom = cube.getColor(1, 2, 0);  // Back[2,0]
  Color back_left_middle = cube.getColor(1, 1, 0);  // Back[1,0]
  Color back_left_top = cube.getColor(1, 0, 0);     // Back[0,0]

  Color down_right_top = cube.getColor(5, 0, 2);     // Down[0,2]
  Color down_right_middle = cube.getColor(5, 1, 2);  // Down[1,2]
  Color down_right_bottom = cube.getColor(5, 2, 2);  // Down[2,2]

  Color front_right_top = cube.getColor(0, 0, 2);     // Front[0,2]
  Color front_right_middle = cube.getColor(0, 1, 2);  // Front[1,2]
  Color front_right_bottom = cube.getColor(0, 2, 2);  // Front[2,2]

  cube.rotateRight(true);

  // Up → Back
  CHECK(cube.getColor(1, 2, 0) == up_right_top);     // Up[0,2] → Back[2,0]
  CHECK(cube.getColor(1, 1, 0) == up_right_middle);  // Up[1,2] → Back[1,0]
  CHECK(cube.getColor(1, 0, 0) == up_right_bottom);  // Up[2,2] → Back[0,0]

  // Back → Down
  CHECK(cube.getColor(5, 0, 2) == back_left_bottom);  // Back[2,0] → Down[0,2]
  CHECK(cube.getColor(5, 1, 2) == back_left_middle);  // Back[1,0] → Down[1,2]
  CHECK(cube.getColor(5, 2, 2) == back_left_top);     // Back[0,0] → Down[2,2]

  // Down → Front
  CHECK(cube.getColor(0, 0, 2) == down_right_top);     // Down[0,2] → Front[0,2]
  CHECK(cube.getColor(0, 1, 2) == down_right_middle);  // Down[1,2] → Front[1,2]
  CHECK(cube.getColor(0, 2, 2) == down_right_bottom);  // Down[2,2] → Front[2,2]

  // Front → Up
  CHECK(cube.getColor(4, 0, 2) == front_right_top);     // Front[0,2] → Up[0,2]
  CHECK(cube.getColor(4, 1, 2) == front_right_middle);  // Front[1,2] → Up[1,2]
  CHECK(cube.getColor(4, 2, 2) == front_right_bottom);  // Front[2,2] → Up[2,2]
}

// Test 6.8: Right counter-clockwise rotation moves colors correctly (полная
// версия)
TEST(RotateRight_CounterClockwise_MovesColorsCorrectly_Full) {
  RubikCube cube;

  Color up_right_top = cube.getColor(4, 0, 2);     // Up[0,2]
  Color up_right_middle = cube.getColor(4, 1, 2);  // Up[1,2]
  Color up_right_bottom = cube.getColor(4, 2, 2);  // Up[2,2]

  Color back_left_bottom = cube.getColor(1, 2, 0);  // Back[2,0]
  Color back_left_middle = cube.getColor(1, 1, 0);  // Back[1,0]
  Color back_left_top = cube.getColor(1, 0, 0);     // Back[0,0]

  Color down_right_top = cube.getColor(5, 0, 2);     // Down[0,2]
  Color down_right_middle = cube.getColor(5, 1, 2);  // Down[1,2]
  Color down_right_bottom = cube.getColor(5, 2, 2);  // Down[2,2]

  Color front_right_top = cube.getColor(0, 0, 2);     // Front[0,2]
  Color front_right_middle = cube.getColor(0, 1, 2);  // Front[1,2]
  Color front_right_bottom = cube.getColor(0, 2, 2);  // Front[2,2]

  cube.rotateRight(false);

  // Front → Down
  CHECK(cube.getColor(5, 0, 2) == front_right_top);  // Front[0,2] → Down[0,2]
  CHECK(cube.getColor(5, 1, 2) ==
        front_right_middle);  // Front[1,2] → Down[1,2]
  CHECK(cube.getColor(5, 2, 2) ==
        front_right_bottom);  // Front[2,2] → Down[2,2]

  // Down → Back
  CHECK(cube.getColor(1, 2, 0) == down_right_top);     // Down[0,2] → Back[2,0]
  CHECK(cube.getColor(1, 1, 0) == down_right_middle);  // Down[1,2] → Back[1,0]
  CHECK(cube.getColor(1, 0, 0) == down_right_bottom);  // Down[2,2] → Back[0,0]

  // Back → Up
  CHECK(cube.getColor(4, 0, 2) == back_left_bottom);  // Back[2,0] → Up[0,2]
  CHECK(cube.getColor(4, 1, 2) == back_left_middle);  // Back[1,0] → Up[1,2]
  CHECK(cube.getColor(4, 2, 2) == back_left_top);     // Back[0,0] → Up[2,2]

  // Up → Front
  CHECK(cube.getColor(0, 0, 2) == up_right_top);     // Up[0,2] → Front[0,2]
  CHECK(cube.getColor(0, 1, 2) == up_right_middle);  // Up[1,2] → Front[1,2]
  CHECK(cube.getColor(0, 2, 2) == up_right_bottom);  // Up[2,2] → Front[2,2]
}

// Test 6.9: Up clockwise rotation moves colors correctly
TEST(RotateUp_Clockwise_MovesColorsCorrectly) {
  RubikCube cube;

  Color back_top_left = cube.getColor(1, 0, 0);    // Back[0,0]
  Color back_top_center = cube.getColor(1, 0, 1);  // Back[0,1]
  Color back_top_right = cube.getColor(1, 0, 2);   // Back[0,2]

  Color right_top_left = cube.getColor(3, 0, 0);    // Right[0,0]
  Color right_top_center = cube.getColor(3, 0, 1);  // Right[0,1]
  Color right_top_right = cube.getColor(3, 0, 2);   // Right[0,2]

  Color front_top_left = cube.getColor(0, 0, 0);    // Front[0,0]
  Color front_top_center = cube.getColor(0, 0, 1);  // Front[0,1]
  Color front_top_right = cube.getColor(0, 0, 2);   // Front[0,2]

  Color left_top_left = cube.getColor(2, 0, 0);    // Left[0,0]
  Color left_top_center = cube.getColor(2, 0, 1);  // Left[0,1]
  Color left_top_right = cube.getColor(2, 0, 2);   // Left[0,2]

  cube.rotateUp(true);

  // Back → Right
  CHECK(cube.getColor(3, 0, 0) == back_top_left);    // Back[0,0] → Right[0,0]
  CHECK(cube.getColor(3, 0, 1) == back_top_center);  // Back[0,1] → Right[0,1]
  CHECK(cube.getColor(3, 0, 2) == back_top_right);   // Back[0,2] → Right[0,2]

  // Right → Front
  CHECK(cube.getColor(0, 0, 0) == right_top_left);    // Right[0,0] → Front[0,0]
  CHECK(cube.getColor(0, 0, 1) == right_top_center);  // Right[0,1] → Front[0,1]
  CHECK(cube.getColor(0, 0, 2) == right_top_right);   // Right[0,2] → Front[0,2]

  // Front → Left
  CHECK(cube.getColor(2, 0, 0) == front_top_left);    // Front[0,0] → Left[0,0]
  CHECK(cube.getColor(2, 0, 1) == front_top_center);  // Front[0,1] → Left[0,1]
  CHECK(cube.getColor(2, 0, 2) == front_top_right);   // Front[0,2] → Left[0,2]

  // Left → Back
  CHECK(cube.getColor(1, 0, 0) == left_top_left);    // Left[0,0] → Back[0,0]
  CHECK(cube.getColor(1, 0, 1) == left_top_center);  // Left[0,1] → Back[0,1]
  CHECK(cube.getColor(1, 0, 2) == left_top_right);   // Left[0,2] → Back[0,2]
}

// Test 6.10: Up counter-clockwise rotation moves colors correctly (полная
// версия)
TEST(RotateUp_CounterClockwise_MovesColorsCorrectly_Full) {
  RubikCube cube;

  Color back_top_left = cube.getColor(1, 0, 0);    // Back[0,0]
  Color back_top_center = cube.getColor(1, 0, 1);  // Back[0,1]
  Color back_top_right = cube.getColor(1, 0, 2);   // Back[0,2]

  Color right_top_left = cube.getColor(3, 0, 0);    // Right[0,0]
  Color right_top_center = cube.getColor(3, 0, 1);  // Right[0,1]
  Color right_top_right = cube.getColor(3, 0, 2);   // Right[0,2]

  Color front_top_left = cube.getColor(0, 0, 0);    // Front[0,0]
  Color front_top_center = cube.getColor(0, 0, 1);  // Front[0,1]
  Color front_top_right = cube.getColor(0, 0, 2);   // Front[0,2]

  Color left_top_left = cube.getColor(2, 0, 0);    // Left[0,0]
  Color left_top_center = cube.getColor(2, 0, 1);  // Left[0,1]
  Color left_top_right = cube.getColor(2, 0, 2);   // Left[0,2]

  cube.rotateUp(false);

  // Left → Front
  CHECK(cube.getColor(0, 0, 0) == left_top_left);    // Left[0,0] → Front[0,0]
  CHECK(cube.getColor(0, 0, 1) == left_top_center);  // Left[0,1] → Front[0,1]
  CHECK(cube.getColor(0, 0, 2) == left_top_right);   // Left[0,2] → Front[0,2]

  // Front → Right
  CHECK(cube.getColor(3, 0, 0) == front_top_left);    // Front[0,0] → Right[0,0]
  CHECK(cube.getColor(3, 0, 1) == front_top_center);  // Front[0,1] → Right[0,1]
  CHECK(cube.getColor(3, 0, 2) == front_top_right);   // Front[0,2] → Right[0,2]

  // Right → Back
  CHECK(cube.getColor(1, 0, 0) == right_top_left);    // Right[0,0] → Back[0,0]
  CHECK(cube.getColor(1, 0, 1) == right_top_center);  // Right[0,1] → Back[0,1]
  CHECK(cube.getColor(1, 0, 2) == right_top_right);   // Right[0,2] → Back[0,2]

  // Back → Left
  CHECK(cube.getColor(2, 0, 0) == back_top_left);    // Back[0,0] → Left[0,0]
  CHECK(cube.getColor(2, 0, 1) == back_top_center);  // Back[0,1] → Left[0,1]
  CHECK(cube.getColor(2, 0, 2) == back_top_right);   // Back[0,2] → Left[0,2]
}

// Test 6.11: Down clockwise rotation moves colors correctly
TEST(RotateDown_Clockwise_MovesColorsCorrectly) {
  RubikCube cube;

  Color front_bottom_left = cube.getColor(0, 2, 0);    // Front[2,0]
  Color front_bottom_center = cube.getColor(0, 2, 1);  // Front[2,1]
  Color front_bottom_right = cube.getColor(0, 2, 2);   // Front[2,2]

  Color right_bottom_left = cube.getColor(3, 2, 0);    // Right[2,0]
  Color right_bottom_center = cube.getColor(3, 2, 1);  // Right[2,1]
  Color right_bottom_right = cube.getColor(3, 2, 2);   // Right[2,2]

  Color back_bottom_left = cube.getColor(1, 2, 0);    // Back[2,0]
  Color back_bottom_center = cube.getColor(1, 2, 1);  // Back[2,1]
  Color back_bottom_right = cube.getColor(1, 2, 2);   // Back[2,2]

  Color left_bottom_left = cube.getColor(2, 2, 0);    // Left[2,0]
  Color left_bottom_center = cube.getColor(2, 2, 1);  // Left[2,1]
  Color left_bottom_right = cube.getColor(2, 2, 2);   // Left[2,2]

  cube.rotateDown(true);

  // Front → Right
  CHECK(cube.getColor(3, 2, 0) ==
        front_bottom_left);  // Front[2,0] → Right[2,0]
  CHECK(cube.getColor(3, 2, 1) ==
        front_bottom_center);  // Front[2,1] → Right[2,1]
  CHECK(cube.getColor(3, 2, 2) ==
        front_bottom_right);  // Front[2,2] → Right[2,2]

  // Right → Back
  CHECK(cube.getColor(1, 2, 0) == right_bottom_left);  // Right[2,0] → Back[2,0]
  CHECK(cube.getColor(1, 2, 1) ==
        right_bottom_center);  // Right[2,1] → Back[2,1]
  CHECK(cube.getColor(1, 2, 2) ==
        right_bottom_right);  // Right[2,2] → Back[2,2]

  // Back → Left
  CHECK(cube.getColor(2, 2, 0) == back_bottom_left);    // Back[2,0] → Left[2,0]
  CHECK(cube.getColor(2, 2, 1) == back_bottom_center);  // Back[2,1] → Left[2,1]
  CHECK(cube.getColor(2, 2, 2) == back_bottom_right);   // Back[2,2] → Left[2,2]

  // Left → Front
  CHECK(cube.getColor(0, 2, 0) == left_bottom_left);  // Left[2,0] → Front[2,0]
  CHECK(cube.getColor(0, 2, 1) ==
        left_bottom_center);                           // Left[2,1] → Front[2,1]
  CHECK(cube.getColor(0, 2, 2) == left_bottom_right);  // Left[2,2] → Front[2,2]
}

// Test 6.12: Down counter-clockwise rotation moves colors correctly
TEST(RotateDown_CounterClockwise_MovesColorsCorrectly_Full) {
  RubikCube cube;

  Color front_bottom_left = cube.getColor(0, 2, 0);    // Front[2,0]
  Color front_bottom_center = cube.getColor(0, 2, 1);  // Front[2,1]
  Color front_bottom_right = cube.getColor(0, 2, 2);   // Front[2,2]

  Color right_bottom_left = cube.getColor(3, 2, 0);    // Right[2,0]
  Color right_bottom_center = cube.getColor(3, 2, 1);  // Right[2,1]
  Color right_bottom_right = cube.getColor(3, 2, 2);   // Right[2,2]

  Color back_bottom_left = cube.getColor(1, 2, 0);    // Back[2,0]
  Color back_bottom_center = cube.getColor(1, 2, 1);  // Back[2,1]
  Color back_bottom_right = cube.getColor(1, 2, 2);   // Back[2,2]

  Color left_bottom_left = cube.getColor(2, 2, 0);    // Left[2,0]
  Color left_bottom_center = cube.getColor(2, 2, 1);  // Left[2,1]
  Color left_bottom_right = cube.getColor(2, 2, 2);   // Left[2,2]

  cube.rotateDown(false);

  // Right → Front
  CHECK(cube.getColor(0, 2, 0) ==
        right_bottom_left);  // Right[2,0] → Front[2,0]
  CHECK(cube.getColor(0, 2, 1) ==
        right_bottom_center);  // Right[2,1] → Front[2,1]
  CHECK(cube.getColor(0, 2, 2) ==
        right_bottom_right);  // Right[2,2] → Front[2,2]

  // Back → Right
  CHECK(cube.getColor(3, 2, 0) == back_bottom_left);  // Back[2,0] → Right[2,0]
  CHECK(cube.getColor(3, 2, 1) ==
        back_bottom_center);                           // Back[2,1] → Right[2,1]
  CHECK(cube.getColor(3, 2, 2) == back_bottom_right);  // Back[2,2] → Right[2,2]

  // Left → Back
  CHECK(cube.getColor(1, 2, 0) == left_bottom_left);    // Left[2,0] → Back[2,0]
  CHECK(cube.getColor(1, 2, 1) == left_bottom_center);  // Left[2,1] → Back[2,1]
  CHECK(cube.getColor(1, 2, 2) == left_bottom_right);   // Left[2,2] → Back[2,2]

  // Front → Left
  CHECK(cube.getColor(2, 2, 0) == front_bottom_left);  // Front[2,0] → Left[2,0]
  CHECK(cube.getColor(2, 2, 1) ==
        front_bottom_center);  // Front[2,1] → Left[2,1]
  CHECK(cube.getColor(2, 2, 2) ==
        front_bottom_right);  // Front[2,2] → Left[2,2]
}

// Test 7: Four rotations of any face in both directions return to original
TEST(FourRotations_ReturnToOriginal) {
  RubikCube original;

  typedef void (*RotationFunction)(RubikCube&, bool);

  struct RotationTest {
    RotationFunction func;
    const char* name;
  };

  RotationTest tests[] = {
      {[](RubikCube& cube, bool clockwise) { cube.rotateFront(clockwise); },
       "Front"},
      {[](RubikCube& cube, bool clockwise) { cube.rotateBack(clockwise); },
       "Back"},
      {[](RubikCube& cube, bool clockwise) { cube.rotateLeft(clockwise); },
       "Left"},
      {[](RubikCube& cube, bool clockwise) { cube.rotateRight(clockwise); },
       "Right"},
      {[](RubikCube& cube, bool clockwise) { cube.rotateUp(clockwise); }, "Up"},
      {[](RubikCube& cube, bool clockwise) { cube.rotateDown(clockwise); },
       "Down"}};

  const int numTests = sizeof(tests) / sizeof(tests[0]);

  for (int i = 0; i < numTests; i++) {
    const RotationTest& test = tests[i];

    RubikCube cube1;
    for (int j = 0; j < 4; j++) {
      test.func(cube1, true);
    }
    CHECK(original == cube1);

    RubikCube cube2;
    for (int j = 0; j < 4; j++) {
      test.func(cube2, false);
    }
    CHECK(original == cube2);
  }
}

// Test 8: Front clockwise then counterclockwise returns to original
TEST(ClockwiseCounterclockwise_ReturnsOriginal) {
  RubikCube original;
  RubikCube cube;

  cube.rotateFront(true);
  cube.rotateFront(false);

  CHECK(original == cube);
}

// Test 9: Left rotation moves edge pieces correctly
TEST(RotateLeft_MovesEdgePiecesCorrectly) {
  RubikCube cube;

  Color front_left_top = cube.getColor(0, 0, 0);
  Color up_left_top = cube.getColor(4, 0, 0);

  cube.rotateLeft(true);

  CHECK(cube.getColor(5, 0, 0) == front_left_top);  // Front[0,0] → Down[0,0]
  CHECK(cube.getColor(0, 0, 0) == up_left_top);     // Up[0,0] → Front[0,0]
}

// Test 10: Right rotation is reversible
TEST(RotateRight_IsReversible) {
  RubikCube original;
  RubikCube cube;

  cube.rotateRight(true);
  cube.rotateRight(false);

  CHECK(original == cube);
}

// Test 11: Down rotation changes state
TEST(RotateDown_ChangesState) {
  RubikCube cube;
  cube.rotateDown(true);
  CHECK(!cube.isSolved());
}

// Test 12: Randomize creates unsolved cube
TEST(Randomize_CreatesUnsolvedCube) {
  RubikCube cube;
  cube.randomize();
  CHECK(!cube.isSolved());
}

// Test 13: GetColor returns correct colors for solved cube
TEST(GetColor_ReturnsCorrectColors) {
  RubikCube cube;

  for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      CHECK(cube.getColor(0, row, col) == Color::White);
      CHECK(cube.getColor(1, row, col) == Color::Yellow);
      CHECK(cube.getColor(2, row, col) == Color::Orange);
      CHECK(cube.getColor(3, row, col) == Color::Red);
      CHECK(cube.getColor(4, row, col) == Color::Blue);
      CHECK(cube.getColor(5, row, col) == Color::Green);
    }
  }
}

// Test 14: SetColor changes specific cell
TEST(SetColor_ChangesSpecificCell) {
  RubikCube cube;
  cube.setColor(0, 1, 1, Color::Yellow);
  CHECK(cube.getColor(0, 1, 1) == Color::Yellow);
  CHECK(!cube.isSolved());
}

// Test 15: Stream output contains all facets
TEST(StreamOutput_ContainsAllFacets) {
  RubikCube cube;
  std::stringstream ss;
  ss << cube;

  std::string output = ss.str();

  CHECK(output.find("Facet0:") != std::string::npos);
  CHECK(output.find("Facet1:") != std::string::npos);
  CHECK(output.find("Facet2:") != std::string::npos);
  CHECK(output.find("Facet3:") != std::string::npos);
  CHECK(output.find("Facet4:") != std::string::npos);
  CHECK(output.find("Facet5:") != std::string::npos);
}

// Test 16: Stream input operator works correctly
TEST(StreamInput_Operator_ReadsCorrectly) {
  RubikCube cube1;
  cube1.rotateFront(true);
  cube1.rotateRight(false);

  std::stringstream ss;
  ss << cube1;

  RubikCube cube2;
  ss >> cube2;

  CHECK(cube1 == cube2);
}

// Test 17: Save and load to file preserves state
TEST(SaveAndLoad_File_PreservesState) {
  RubikCube original;
  original.rotateFront(true);
  original.rotateRight(false);
  original.rotateUp(true);

  CHECK(original.saveToFile("test_cube.txt"));

  RubikCube loaded;
  CHECK(loaded.loadFromFile("test_cube.txt"));

  CHECK(original == loaded);
}

// Test 18: Load from non-existent file returns false
TEST(Load_NonExistentFile_ReturnsFalse) {
  RubikCube cube;
  CHECK(!cube.loadFromFile("nonexistent_file.txt"));
}

// Test 19: Complex rotation sequence produces expected pattern
TEST(ComplexRotationSequence_ProducesExpectedPattern) {
  RubikCube cube;

  cube.rotateFront(true);
  cube.rotateUp(false);
  cube.rotateRight(true);

  CHECK(cube.getColor(0, 0, 0) != Color::White);
  CHECK(cube.getColor(4, 2, 2) != Color::Blue);

  CHECK(!cube.isSolved());
}

// Test 20: All rotation types maintain cube integrity(целостность)
TEST(AllRotationTypes_MaintainCubeIntegrity) {
  RubikCube cube;

  std::map<Color, int> colorCounts;
  for (int f = 0; f < 6; f++) {
    for (int r = 0; r < 3; r++) {
      for (int c = 0; c < 3; c++) {
        colorCounts[cube.getColor(f, r, c)]++;
      }
    }
  }

  cube.rotateFront(true);
  cube.rotateBack(false);
  cube.rotateLeft(true);
  cube.rotateRight(false);
  cube.rotateUp(true);
  cube.rotateDown(false);

  std::map<Color, int> newColorCounts;
  for (int f = 0; f < 6; f++) {
    for (int r = 0; r < 3; r++) {
      for (int c = 0; c < 3; c++) {
        newColorCounts[cube.getColor(f, r, c)]++;
      }
    }
  }

  CHECK(colorCounts == newColorCounts);
  CHECK(!cube.isSolved());
}

// Test 21: Reverse rotation sequence returns to original
TEST(ReverseRotationSequence_ReturnsOriginal) {
  RubikCube original;
  RubikCube cube;

  cube.rotateFront(true);
  cube.rotateRight(false);
  cube.rotateUp(true);
  cube.rotateBack(false);
  cube.rotateLeft(true);
  cube.rotateDown(false);

  cube.rotateDown(true);
  cube.rotateLeft(false);
  cube.rotateBack(true);
  cube.rotateUp(false);
  cube.rotateRight(true);
  cube.rotateFront(false);

  CHECK(original == cube);
}

// Test 22: Center pieces remain in place after rotations
TEST(CenterPieces_RemainInPlace) {
  RubikCube cube;

  Color centers[6];
  for (int i = 0; i < 6; i++) {
    centers[i] = cube.getColor(i, 1, 1);
  }

  cube.randomize();

  for (int i = 0; i < 6; i++) {
    CHECK(cube.getColor(i, 1, 1) == centers[i]);
  }
}

// Test 23: Display colored produces non-empty output
TEST(DisplayColored_ProducesNonEmptyOutput) {
  RubikCube cube;
  std::stringstream ss;

  cube.displayColored(ss);
  std::string output = ss.str();

  CHECK(!output.empty());
  CHECK(std::count(output.begin(), output.end(), '\n') >= 8);
}

// Test 24: Multiple randomizations produce different states
TEST(MultipleRandomizations_ProduceDifferentStates) {
  RubikCube cube1, cube2, cube3;

  cube1.randomize();
  cube2.randomize();
  cube3.randomize();

  bool allDifferent = (cube1 != cube2) && (cube2 != cube3) && (cube1 != cube3);
  CHECK(allDifferent);
}

// Test 25: Mixed rotation directions create unique state
TEST(MixedRotationDirections_CreateUniqueState) {
  RubikCube cube1, cube2;

  cube1.rotateFront(true);
  cube1.rotateBack(false);
  cube1.rotateLeft(true);

  cube2.rotateFront(false);
  cube2.rotateBack(true);
  cube2.rotateLeft(false);

  CHECK(cube1 != cube2);
}

// Test 26: Complex operation chain maintains validity
TEST(ComplexOperationChain_MaintainsValidity) {
  RubikCube cube;

  for (int i = 0; i < 3; i++) {
    cube.rotateFront(true);
    cube.rotateRight(false);
    cube.rotateUp(true);
  }

  std::map<Color, int> colorCounts;
  for (int f = 0; f < 6; f++) {
    for (int r = 0; r < 3; r++) {
      for (int c = 0; c < 3; c++) {
        colorCounts[cube.getColor(f, r, c)]++;
      }
    }
  }

  for (const auto& pair : colorCounts) {
    CHECK(pair.second == 9);
  }
}

// Test 27: Multiple rotations of same face are equivalent to module 4
TEST(MultipleRotations_SameFace_EquivalentToModulo4) {
  RubikCube cube1, cube2;

  for (int i = 0; i < 10; i++) {
    cube1.rotateFront(true);
  }

  for (int i = 0; i < 2; i++) {
    cube2.rotateFront(true);
  }

  CHECK(cube1 == cube2);
}

// Test 28: Comprehensive workflow test
TEST(ComprehensiveWorkflow_AllOperationsWork) {
  RubikCube cube;

  CHECK(cube.isSolved());

  cube.rotateFront(true);
  CHECK(!cube.isSolved());

  RubikCube copy = cube;
  CHECK(copy == cube);

  cube.randomize();
  CHECK(!cube.isSolved());

  std::stringstream ss;
  ss << cube;
  CHECK(!ss.str().empty());

  cube.displayColored(ss);
  CHECK(ss.str().length() > 100);

  CHECK(cube.saveToFile("workflow_test.txt"));

  RubikCube loaded;
  CHECK(loaded.loadFromFile("workflow_test.txt"));
  CHECK(loaded == cube);
}

int main() {
  int result = UnitTest::RunAllTests();

  std::cout << "\nPress Enter to continue...";
  std::cin.get();

  return result;
}
