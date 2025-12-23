#include "as/as.h"

void as(str path)
{
   fstream_t fs;
   fsopen(fs, path, FMODE_RT);

   as_fetch_tokens(addr fs);

   fsclose(fs);
}
