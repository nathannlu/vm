#include "compiler.h"
#include <stdlib.h>

void initialize_compiler(struct compiler* c, struct vm_value* constants, struct globals* globals, struct locals* locals) {
  // malloc bytecode;
  c->count = 0;
  c->scope_level = 0;

  // create the main code object
  struct code_object* main_code = new_code_object("main");
  c->co = main_code;

  // compiler should not own these. these
  // should be taken from the code object
  /*
  c->constants = constants;
  c->globals = globals;
  c->locals = locals;     // this array is only used in compiler to temporarily keep track of scope
  */
}

// add op_code to bytecode
void compiler_emit(struct compiler* c, uint8_t op_code) {
  if(c->count < MAX_BYTECODE_LENGTH) {
    c->co->bytecode[c->count] = op_code;
    c->count++; // always points to next empty slot in arr to be filled
  } else {
    printf("Max bytecode length reached\n");
  }
}

static void compiler_scope_enter(struct compiler* c) {
  c->scope_level++;
}

static void compiler_scope_exit(struct compiler* c) {
  int local_var_count = 0;

  // Go through locals array and count how many
  // had been defined during this scope
  for(int i = 0; i < c->co->locals->count; i++) {
    if (c->co->locals->locals[i].scope_level == c->scope_level) {
      local_var_count++;
    }
  }
  // Delete locals
  c->co->locals->count = c->co->locals->count - local_var_count;

  if (local_var_count > 0) {
    compiler_emit(c, OP_SCOPE_EXIT);
    compiler_emit(c, local_var_count);
  }

  c->scope_level--;
}

/*
static int compiler_add_constant(struct compiler* c, const struct vm_value constant) {
  if(c->constants_length == MAX_CONSTANT_LENGTH) {
    // realloc
  }

  int index = c->constants_length;
  c->co->constants[index] = constant;

  printf("adding constant: %f to index %d\n", constant.number, index);

  c->constants_length++;

  return index;
}
*/

static void compiler_patch_jmp_address(struct compiler* c, int tmp_addr_index, uint16_t patched_index) {
  c->co->bytecode[tmp_addr_index] = (patched_index >> 8) & 0xff;
  c->co->bytecode[tmp_addr_index+1] = patched_index & 0xff;
}

// recursive function to recursively generate
// bytecode from ast
void compiler_gen(struct compiler* c, struct ast_node* ast) {

  // variables for temp data storage inside
  // switch statement
  int index;
  struct ast_node* current;

  switch(ast->type) {
    case Program:
      current = ast->Program.body;
      while (current != NULL) {

        bool is_last = current->next == NULL;

        // Compile node inside linked list
        compiler_gen(c, current);
        current = current->next;

        if(!is_last) {
          compiler_emit(c, OP_POP);
        }
      }

      break;

    case BlockStatement:
      // On enter block, scope level should
      // increase
      compiler_scope_enter(c);

      current = ast->BlockStatement.body;
      while (current != NULL) {
        // Compile node inside linked list
        compiler_gen(c, current);
        current = current->next;
      }

      // Decrease scope level when leaving
      // block
      compiler_scope_exit(c);

      break;

    case BinaryExpression:

      // Evaluate children first
      compiler_gen(c, ast->BinaryExpression.left);
      compiler_gen(c, ast->BinaryExpression.right);

      switch(ast->BinaryExpression.op) {
        // Calculations
        case ADD:
          compiler_emit(c, OP_ADD);
          break;
        case SUB:
          compiler_emit(c, OP_SUB);
          break;
        case MUL:
          compiler_emit(c, OP_MUL);
          break;
        case DIV:
          compiler_emit(c, OP_DIV);
          break;

        /**
         * BOOLEAN MAP
         * - 0 is <
         * - 1 is >
         * - 2 is ==
         * - 3 is >=
         * - 4 is <= 
         * - 5 is !=
         */
        case LESS_THAN:
          compiler_emit(c, OP_COMPARE);
          compiler_emit(c, 0);
          break;
        case GREATER_THAN:
          compiler_emit(c, OP_COMPARE);
          compiler_emit(c, 1);
          break;
        case EQUAL:
          compiler_emit(c, OP_COMPARE);
          compiler_emit(c, 2);
          break;
        case GREATER_EQUAL_THAN:
          compiler_emit(c, OP_COMPARE);
          compiler_emit(c, 3);
          break;
        case LESS_EQUAL_THAN:
          compiler_emit(c, OP_COMPARE);
          compiler_emit(c, 4);
          break;
        case NOT_EQUAL:
          compiler_emit(c, OP_COMPARE);
          compiler_emit(c, 5);
          break;
        /*
        case default:
          printf("BinaryExpression error: unknown operation");
          break;
        */
      }

      break;


    case VariableDeclaration:

      // generate the initializer bytecode
      compiler_gen(c, ast->VariableDeclaration.init);

      bool is_global_scope = c->scope_level == 0;
      if (is_global_scope) {
        // add to globals array
        index = define_global(c->co->globals, ast->VariableDeclaration.id->Identifier.name);

        compiler_emit(c, OP_SET_GLOBAL);
        compiler_emit(c, index);
      } else {
        // add to globals array
        index = add_local(c->co->locals, ast->VariableDeclaration.id->Identifier.name, c->scope_level);

        compiler_emit(c, OP_SET_LOCAL);
        compiler_emit(c, index);
      }

      break;

    case FunctionDeclaration:
      // When a function is declared, a new
      // code object needs to be created for that 
      // function.
      
      // Save currrent code object, and generate
      // bytecode on the new code object for the
      // function body.
      struct code_object* prev_code = c->co;
      int prev_bytecode_count = c->count;

      // Create new code object for function
      struct code_object* new_co = new_code_object(ast->FunctionDeclaration.id->Identifier.name);
      c->co = new_co;
      c->count = 0;

      // Add this as a local in itself
      // (so it can call itself recursively)
      index = add_local(c->co->locals, ast->FunctionDeclaration.id->Identifier.name, c->scope_level);

      // @todo
      // pop parameters and scope exit

      // Generate bytecode in the new code object
      compiler_gen(c, ast->FunctionDeclaration.body);
      struct vm_value func_1 = object_to_vm_value(new_object_code_object((void*)c->co));

      compiler_emit(c, OP_RETURN);

      // Restore original code object and bytecode
      // counter. Add function to the constants
      c->co = prev_code;
      c->count = prev_bytecode_count;
      index = add_constant(c->co->constants, &func_1);

      // On runtime, push this onto the stack
      compiler_emit(c, OP_CONST);
      compiler_emit(c, index); 

      // and set it as either a global or local
      is_global_scope = c->scope_level == 0;
      if (is_global_scope) {
        // add to globals array
        index = define_global(c->co->globals, ast->VariableDeclaration.id->Identifier.name);

        compiler_emit(c, OP_SET_GLOBAL);
        compiler_emit(c, index);
      } else {
        // add to local array
        index = add_local(c->co->locals, ast->VariableDeclaration.id->Identifier.name, c->scope_level);

        compiler_emit(c, OP_SET_LOCAL);
        compiler_emit(c, index);
      }

      break;

    case CallExpression:

      // Fetch function and push it to top of stack
      compiler_gen(c, ast->CallExpression.callee);

      // Count the arity
      int arity = 0;
      /*
      while(ast->CallExpression.arguments->next != NULL) {
        arity++;
      }
      */

      // Call the function
      compiler_emit(c, OP_CALL);
      compiler_emit(c, 0);        // arg count

      break;

    case Identifier:
      
      // 1. Check if you can access variable
      // locally
      index = get_local_index(c->co->locals, ast->Identifier.name);
      if(index != -1) {
        compiler_emit(c, OP_GET_LOCAL);
        compiler_emit(c, index);      // the index on the stack (relative to bp)
      }

      // 2. Else retrieve it from globals
      else {
        index = get_global_index(c->co->globals, ast->Identifier.name);
        if(index == -1) {
          printf("Identifier error: cannot find global\n");
          exit(EXIT_FAILURE);
        }

        compiler_emit(c, OP_GET_GLOBAL);
        compiler_emit(c, index);
      }
      
      break;

    // this can only be ran for code that has
    // their variable declared
    // e.g. 
    // var j = 0;
    // j = 10;
    case AssignmentExpression:
      // Generate bytecode for the right hand side
      compiler_gen(c, ast->AssignmentExpression.right);

      // 1. Check if you can access variable
      // locally
      index = get_local_index(c->co->locals, ast->Identifier.name);
      if(index != -1) {
        compiler_emit(c, OP_SET_LOCAL);
        compiler_emit(c, index);
      }

      // 2. Else attempt to set global
      else {
        // get the index of var name from globals array
        index = get_global_index(c->co->globals, ast->AssignmentExpression.left->Identifier.name);
        if(index == -1) {
          printf("AssignmentExpression error: cannot find global\n");
          exit(EXIT_FAILURE);
        }
       
        // get name of identifier
        compiler_emit(c, OP_SET_GLOBAL);
        compiler_emit(c, index);
      }

      break;

    case IfStatement:
      // handle binary expression
      compiler_gen(c, ast->IfStatement.test);

      // Because OP_JMP_IF_FALSE looks ahead
      // in the bytecode for two address bits that
      // represent the index in the bytecode,
      // we emit two 0 bits as placeholder.
      // 
      // The placeholder bits will be patched
      // later in the code
      compiler_emit(c, OP_JMP_IF_FALSE);
      int jmp_to_alternate_code_index = c->count;      // this points to the beginning of the patch
      compiler_emit(c, 0);
      compiler_emit(c, 0);

      // (1)
      // Generate the consequent bytecode
      compiler_gen(c, ast->IfStatement.consequent);

      // this int describes the index where
      // the consequent code ends
      int consequent_code_end_index = c->count;

      // in consequent, you need to emit values that
      // jump past alternate
      compiler_emit(c, OP_JMP);
      int skip_alternate_code_index = c->count;      // this points to the beginning of the patch
      compiler_emit(c, 0);
      compiler_emit(c, 0);

      // (2)
      // Generate the alternate bytecode
      if (c->count > UINT16_MAX) {
        printf("IfStatement error: index larger than uint16_t\n");
      }
      uint16_t alternate_code_start_index = (uint16_t)c->count;

      compiler_gen(c, ast->IfStatement.alternate);

      // this int describes the index where
      // the alternate code ends
      if (c->count > UINT16_MAX) {
        printf("IfStatement error: index larger than uint16_t\n");
      }
      uint16_t alternate_code_end_index = (uint16_t)c->count;

      // (3)
      // Patching
      // make if jump false, jump to start of alternate code
      compiler_patch_jmp_address(c, jmp_to_alternate_code_index, alternate_code_start_index);

      // make if consequent code's jump, skip
      // alternate code, and jump to where it
      // ends
      compiler_patch_jmp_address(c, skip_alternate_code_index, alternate_code_end_index);

      break;

    case NumericLiteral:
      compiler_emit(c, OP_CONST);

      // add to constant array
      // constants.push
      index = add_constant(c->co->constants, &NUMBER(ast->NumericLiteral.number));

      // emit the index to reference that variable
      compiler_emit(c, index);

      break;

    case StringLiteral:
      break;
  }
}

// @todo free mem of compiler


