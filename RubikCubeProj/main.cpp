#include <cstdlib>
#include <iostream>
#include <limits>

#include "RubikCube.h"

void displayMainMenu() {
  std::cout << "\n=== RUBIK'S CUBE ===\n";
  std::cout << "1. Display current state (colored)\n";
  std::cout << "2. Display current state (numerical)\n";
  std::cout << "3. Reset to solved state\n";
  std::cout << "4. Randomize cube\n";
  std::cout << "5. Rotate a face\n";
  std::cout << "6. Check if solved\n";
  std::cout << "7. Save current state to file\n";
  std::cout << "8. Load state from file\n";
  std::cout << "0. Exit\n";
  std::cout << "Choose an option: ";
}

void displayRotationMenu() {
  std::cout << "\n--- ROTATE FACE ---\n";
  std::cout << "1.  Front (clockwise)\n";
  std::cout << "2.  Front (counter-clockwise)\n";
  std::cout << "3.  Back (clockwise)\n";
  std::cout << "4.  Back (counter-clockwise)\n";
  std::cout << "5.  Left (clockwise)\n";
  std::cout << "6.  Left (counter-clockwise)\n";
  std::cout << "7.  Right (clockwise)\n";
  std::cout << "8.  Right (counter-clockwise)\n";
  std::cout << "9.  Up (clockwise)\n";
  std::cout << "10. Up (counter-clockwise)\n";
  std::cout << "11. Down (clockwise)\n";
  std::cout << "12. Down (counter-clockwise)\n";
  std::cout << "0.  Back to main menu\n";
  std::cout << "Choose rotation: ";
}

void performRotation(RubikCube& cube, int choice) {
  switch (choice) {
    case 1:
      cube.rotateFront(true);
      break;
    case 2:
      cube.rotateFront(false);
      break;
    case 3:
      cube.rotateBack(true);
      break;
    case 4:
      cube.rotateBack(false);
      break;
    case 5:
      cube.rotateLeft(true);
      break;
    case 6:
      cube.rotateLeft(false);
      break;
    case 7:
      cube.rotateRight(true);
      break;
    case 8:
      cube.rotateRight(false);
      break;
    case 9:
      cube.rotateUp(true);
      break;
    case 10:
      cube.rotateUp(false);
      break;
    case 11:
      cube.rotateDown(true);
      break;
    case 12:
      cube.rotateDown(false);
      break;
    default:
      std::cout << "Invalid rotation choice!\n";
      return;
  }
  std::cout << "Rotation completed successfully!\n";
}

void clearInputBuffer() {
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
  RubikCube cube;
  int choice;

  do {
    displayMainMenu();
    std::cin >> choice;

    if (std::cin.fail()) {
      std::cout << "Invalid input! Please enter a number.\n";
      clearInputBuffer();
      continue;
    }

    switch (choice) {
      case 1: {
        std::cout << "\n--- CURRENT STATE (COLORED) ---\n";
        cube.displayColored(std::cout);
        break;
      }

      case 2: {
        std::cout << "\n--- CURRENT STATE (NUMERICAL) ---\n";
        std::cout << cube;
        std::cout
            << "\nColor codes: 0 = White, 1 = Yellow, 2 = Red, 3 = Orange, "
               "4 = Blue, 5 = Green\n";
        break;
      }

      case 3: {
        cube = RubikCube();
        std::cout << "Cube reset to solved state.\n";
        break;
      }

      case 4: {
        cube.randomize();
        std::cout << "Cube randomized with 20 random moves.\n";
        break;
      }

      case 5: {
        int rotationChoice;
        do {
          displayRotationMenu();
          std::cin >> rotationChoice;

          if (std::cin.fail()) {
            std::cout << "Invalid input! Please enter a number.\n";
            clearInputBuffer();
            continue;
          }

          if (rotationChoice >= 1 && rotationChoice <= 12) {
            performRotation(cube, rotationChoice);
          } else if (rotationChoice != 0) {
            std::cout << "Invalid choice! Please try again.\n";
          }

        } while (rotationChoice != 0);
        break;
      }

      case 6: {
        if (cube.isSolved()) {
          std::cout << "The cube is SOLVED.\n";
        } else {
          std::cout << "The cube is NOT solved.\n";
        }
        break;
      }

      case 7: {
        std::string filename;
        std::cout << "Enter filename to save (e.g., 'my_cube.txt'): ";
        std::cin >> filename;

        if (cube.saveToFile(filename)) {
          std::cout << " Cube state successfully saved to: " << filename
                    << "\n";
        } else {
          std::cout << " ERROR: Could not save to file: " << filename << "\n";
        }
        break;
      }

      case 8: {
        std::string filename;
        std::cout << "Enter filename to load (e.g., 'my_cube.txt'): ";
        std::cin >> filename;

        if (cube.loadFromFile(filename)) {
          std::cout << " Cube state successfully loaded from: " << filename
                    << "\n";
        } else {
          std::cout << " ERROR: Could not load from file: " << filename << "\n";
          std::cout
              << "Make sure the file exists and has the correct format.\n";
        }
        break;
      }

      case 0: {
        std::cout << "Exiting...\n";
        break;
      }

      default: {
        std::cout << "Invalid option! Please choose 0-8.\n";
        break;
      }
    }

    clearInputBuffer();

  } while (choice != 0);

  return 0;
}
