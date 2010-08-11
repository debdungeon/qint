#include "env.h"
const char *auto_bin(void)
{
  const char *env;
  if ((env = env_get("EZMLM_BIN")) == 0)
    env = "\057\165\163\162\057\154\157\143\141\154\057\142\151\156\057\145\172\155\154\155";
  return env;
}
