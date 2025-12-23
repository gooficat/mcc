#include "as/as.h"
#include "as/as_tk.h"

int main()
{
   init_fancc;
   fancc(YELLOW, "Hello, World!\n");

   as("../tests/test1.s");

   return 0;
}
