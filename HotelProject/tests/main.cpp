#include <UnitTest++/UnitTest++.h>

#include <iostream>

int main() {
  int result = UnitTest::RunAllTests();

  std::cout << "\nPress Enter to continue...";
  std::cin.get();

  return result;
}
