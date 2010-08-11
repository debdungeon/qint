#include "env.h"
const char *auto_lib(void)
{
  const char *env;
  if ((env = env_get("EZMLM_LIB")) == 0)
    env = "\057\165\163\162\057\154\157\143\141\154\057\154\151\142\057\145\172\155\154\155";
  return env;
}
