#include "env.h"
const char *auto_etc(void)
{
  const char *env;
  if ((env = env_get("EZMLM_ETC")) == 0)
    env = "\057\145\164\143\057\145\172\155\154\155";
  return env;
}
