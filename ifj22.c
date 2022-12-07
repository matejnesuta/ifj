#include "codegen.h"
#include "include.h"
#include "parser.h"

int main() {
  Parser *parser = StartParsing();
  codegen(parser->root);
}
