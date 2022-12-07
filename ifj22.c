#include "codegen.h"
#include "include.h"
#include "parser.h"

/**
 * @brief Main function
 *
 * @return int
 */
int main() {
  Parser *parser = StartParsing();
  codegen(parser->root);
  return 0;
}
