#include "auto_home.h"

void hier()
{
  h(auto_home,-1,-1,02755);

  d(auto_home,"bin",-1,-1,02755);
  d(auto_home,"doc",-1,-1,02755);
  d(auto_home,"man",-1,-1,02755);
  d(auto_home,"man/man1",-1,-1,02755);
  d(auto_home,"man/cat1",-1,-1,02755);

  c(auto_home,"doc","MATCHUP",-1,-1,0644);
  c(auto_home,"doc","ACCOUNTING",-1,-1,0644);

  c(auto_home,"man/man1","matchup.1",-1,-1,0644);
  c(auto_home,"man/cat1","matchup.0",-1,-1,0644);
  c(auto_home,"man/man1","xqp.1",-1,-1,0644);
  c(auto_home,"man/cat1","xqp.0",-1,-1,0644);
  c(auto_home,"man/man1","xsender.1",-1,-1,0644);
  c(auto_home,"man/cat1","xsender.0",-1,-1,0644);
  c(auto_home,"man/man1","xrecipient.1",-1,-1,0644);
  c(auto_home,"man/cat1","xrecipient.0",-1,-1,0644);
  c(auto_home,"man/man1","columnt.1",-1,-1,0644);
  c(auto_home,"man/cat1","columnt.0",-1,-1,0644);

  c(auto_home,"bin","matchup",-1,-1,0755);
  c(auto_home,"bin","columnt",-1,-1,0755);
  c(auto_home,"bin","zoverall",-1,-1,0755);
  c(auto_home,"bin","zsendmail",-1,-1,0755);

  c(auto_home,"bin","xqp",-1,-1,0755);
  c(auto_home,"bin","xsender",-1,-1,0755);
  c(auto_home,"bin","xrecipient",-1,-1,0755);

  c(auto_home,"bin","ddist",-1,-1,0755);
  c(auto_home,"bin","deferrals",-1,-1,0755);
  c(auto_home,"bin","failures",-1,-1,0755);
  c(auto_home,"bin","successes",-1,-1,0755);
  c(auto_home,"bin","rhosts",-1,-1,0755);
  c(auto_home,"bin","recipients",-1,-1,0755);
  c(auto_home,"bin","rxdelay",-1,-1,0755);
  c(auto_home,"bin","senders",-1,-1,0755);
  c(auto_home,"bin","suids",-1,-1,0755);

  c(auto_home,"bin","zddist",-1,-1,0755);
  c(auto_home,"bin","zdeferrals",-1,-1,0755);
  c(auto_home,"bin","zfailures",-1,-1,0755);
  c(auto_home,"bin","zsuccesses",-1,-1,0755);
  c(auto_home,"bin","zrhosts",-1,-1,0755);
  c(auto_home,"bin","zrecipients",-1,-1,0755);
  c(auto_home,"bin","zrxdelay",-1,-1,0755);
  c(auto_home,"bin","zsenders",-1,-1,0755);
  c(auto_home,"bin","zsuids",-1,-1,0755);
}
