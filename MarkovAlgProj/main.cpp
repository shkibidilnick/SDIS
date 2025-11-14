#include <fstream>
#include <iostream>
#include <limits>
#include <string>

#include "MarkovAlg.h"
#include "RuleParser.h"

void printMenu() {
  std::cout << "\n=== Markov Algorithm Normal Form ===\n";
  std::cout << "1. Load rules from file\n";
  std::cout << "2. Enter rules manually\n";
  std::cout << "3. Set input string\n";
  std::cout << "4. Execute one step\n";
  std::cout << "5. Run to completion\n";
  std::cout << "6. Show current state\n";
  std::cout << "7. Exit\n";
  std::cout << "Choose option: ";
}

void loadFromFile(MarkovAlgorithm& algo) {
  std::string filename;
  std::cout << "Enter filename: ";

  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::getline(std::cin, filename);

  try {
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cout << "Error opening file!\n";
      return;
    }

    algo = MarkovAlgorithm();
    algo.loadRulesFromStream(file);
    std::cout << "Rules loaded successfully!\n";
  } catch (const std::exception& e) {
    std::cout << "Error loading rules: " << e.what() << "\n";
  } catch (...) {
    std::cout << "Unknown error occurred while loading rules!\n";
  }
}

void enterRulesManually(MarkovAlgorithm& algo) {
  std::cout << "Enter rules (one per line).\n";
  std::cout << "Format: pattern -> replacement [final]\n";
  std::cout << "Examples:\n";
  std::cout << "  a -> b\n";
  std::cout << "  ab -> x final\n";
  std::cout << "  x -> EMPTY\n";
  std::cout << "Enter empty line to finish\n";

  std::string line;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  algo = MarkovAlgorithm();

  while (true) {
    std::cout << "> ";
    std::getline(std::cin, line);

    if (line.empty()) break;

    Rule rule;
    std::string errorMessage;

    if (RuleParser::tryParse(line, rule, errorMessage)) {
      algo.addRule(rule);
      std::cout << "Added rule: '" << rule.getPattern() << "' -> '"
                << rule.getReplacement() << "' "
                << (rule.isFinal() ? "(final)" : "(non-final)") << "\n";
    } else {
      std::cout << "Error: " << errorMessage << "\n";
      std::cout << "Please use format: pattern -> replacement [final]\n";
    }
  }
}

void setInputString(MarkovAlgorithm& algo) {
  std::string input;
  std::cout << "Enter input string: ";

  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  std::getline(std::cin, input);

  MarkovTape tape(input);
  algo.setTape(tape);
  algo.reset();
  std::cout << "String set: " << input << "\n";
}

int main() {
  MarkovAlgorithm algo;
  int choice;

  do {
    printMenu();
    std::cin >> choice;

    switch (choice) {
      case 1:
        loadFromFile(algo);
        break;
      case 2:
        enterRulesManually(algo);
        break;
      case 3:
        setInputString(algo);
        break;
      case 4:
        if (algo.isHalted()) {
          std::cout << "Algorithm already halted!\n";
        } else {
          algo.step();
          std::cout << "Step executed. Current state: " << algo.getCurrentTape()
                    << "\n";
        }
        break;
      case 5:
        algo.run();
        std::cout << "Algorithm completed. Result: " << algo.getCurrentTape()
                  << "\n";
        break;
      case 6:
        std::cout << "Current state: " << algo.getCurrentTape()
                  << " (halted: " << std::boolalpha << algo.isHalted() << ")\n";
        break;
      case 7:
        std::cout << "Exiting...\n";
        break;
      default:
        std::cout << "Invalid choice!\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  } while (choice != 7);

  return 0;
}
