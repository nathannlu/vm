#ifndef COMPILER_H
#define COMPILER_H

#include "parser.h"
#include "op_code.h"
#include <iostream>
#include <fstream>


/**
 * code example:
 * --
 *(+ 3 2)
 *
 */
/*
// sample ast
$1 = {
  type = ExpType::LIST,
  number = 32767,
  string = "",
  list = std::vector of length 3,
  capacity 3 = {
    {
      type = ExpType::SYMBOL,
      number = 32767,
      string = "+",
      list = std::vector of length 0,
      capacity 0
    },
    {
      type = ExpType::NUMBER,
      number = 3,
      string = "", 
      list = std::vector of length 0,
      capacity 0
    },
    {
      type = ExpType::NUMBER,
      number = 2,
      string = "",
      list = std::vector of length 0, capacity 0
    }
  }
}
*/
class compiler {
  public:
    compiler() {}

    std::vector<uint8_t> bytecode;
    std::vector<uint8_t> constants;

    void write() {
      // Open a file for writing
      std::ofstream file("data.bin", std::ios::binary);

      // Write the vector data to the file
      file.write(reinterpret_cast<char*>(bytecode.data()), bytecode.size());

      // Close the file
      file.close();
    }

    void compile(const Exp& exp) {
      std::cout << "compile shitttt\n";

      switch(exp.type){
        case ExpType::NUMBER:
          std::cout << "number\n";
          break;

        case ExpType::STRING:
          std::cout << "string\n";
          break;

        case ExpType::SYMBOL:
          std::cout << "symbol\n";
          break;

        case ExpType::LIST:
          std::cout << "list\n";

          auto tag = exp.list[0];

          if(tag.type == ExpType::SYMBOL) {
            auto op = tag.string;

            // check if next 2 values are both numbers or strings
            // if one of them is a list, calculate that first

            if (op == "+") {
              emit(OP_CONST);
              emit(0);
              // @todo add to constant pool

              emit(OP_CONST);
              emit(1);
              // @todo add to constant pool

              emit(OP_ADD);
            }
          }
          
          // if it is a list
          // that means it has children. loop inside and generate 
          //
          // loop through exp.list (which is a vector)
          // and run gen() on its children

          break;
      }

      emit(OP_HALT);
      std::printf("{}", bytecode);
    }

    void emit(uint8_t op_code) {
      std::cout << "emitting: ";
      std::cout << unsigned(op_code);
      std::cout << "\n";
      bytecode.push_back(op_code);
    }
};

#endif
