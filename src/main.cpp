#include "compiler.h"

int main()
{
    bccc::compile("../examples/return_2.c", "../examples/return_2.s");

    return 0;
}