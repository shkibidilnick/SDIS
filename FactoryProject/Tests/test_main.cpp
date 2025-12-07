#include <UnitTest++/UnitTest++.h>

#include <iostream>

int main(int /*argc*/, char* /*argv*/[]) {
  int result = UnitTest::RunAllTests();
  std::cout << "Tests passed. Enter Enter to continue...";
  std::cin.get();

  return result;
}
