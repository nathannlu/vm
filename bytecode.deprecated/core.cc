#include "parser.h"
#include "compiler.h"
#include <fstream>
using namespace syntax;


void write_to_file() {
  // Open an output file
  std::ofstream outputFile("output.txt");

  // Check if the file is open before writing
  if (outputFile.is_open()) {
    // Write the parsed result to the file
    //outputFile << "Parsed result: " << ast << std::endl;

    // Close the file
    outputFile.close();
  } else {
    // Handle file opening error
    std::cerr << "Error: Unable to open the output file." << std::endl;
  }
}



int main() {
  parser p;
  compiler c;

  auto ast = p.parse(R"(
    (+ 3 2)
  )");

  c.compile(ast);
  c.write();

  return 0;
}
