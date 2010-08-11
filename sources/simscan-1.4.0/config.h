/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.in by autoheader.  */

/* Path to the clamav databases binary */
#define CLAMAVDBPATH "/var/lib/clamav"

/* "clamdscan program" */
#define CLAMDSCAN "/usr/bin/clamdscan"

/* "working directory" */
#define CONTROLDIR "/var/qmail/control"

/* run ripmime program */
#define DO_RIPMIME 1

/* "dspam program" */
/* #undef DSPAM */

/* "dspam arguments" */
/* #undef DSPAM_ARGS */

/* enable attachment scanning */
#define ENABLE_ATTACH 1

/* enable clamav scanning */
#define ENABLE_CLAMAV /**/

/* enable custom smtp reject message with virus name */
#define ENABLE_CUSTOM_SMTP_REJECT /**/

/* enable drop message */
/* #undef ENABLE_DROPMSG */

/* enable dspam scanning */
/* #undef ENABLE_DSPAM */

/* dspam --user user@domain option. */
/* #undef ENABLE_DSPAM_USER */

/* enable per domain checking */
#define ENABLE_PER_DOMAIN /**/

/* enable received header */
#define ENABLE_RECEIVED 1

/* enable regex scanner */
#define ENABLE_REGEX 1

/* enable spam scanning */
#define ENABLE_SPAM 1

/* spamc -u user@domain option. */
/* #undef ENABLE_SPAMC_USER */

/* scan authenticated users for spam. */
/* #undef ENABLE_SPAM_AUTH_USER */

/* pass spam through to user */
/* #undef ENABLE_SPAM_PASSTHRU */

/* enable trophie scanning */
/* #undef ENABLE_TROPHIE */

/* "system supports nproc ulimit" */
#define HAS_ULIMIT_NPROC 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strsep' function. */
#define HAVE_STRSEP 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Name of package */
#define PACKAGE "simscan"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* "qmail directory" */
#define QMAILDIR "/var/qmail"

/* "qmail-queue program" */
#define QMAILQUEUE "/var/qmail/bin/qmail-tail"

/* "quarantine directory" */
/* #undef QUARANTINEDIR */

/* key for attachment scanner */
#define RCVD_ATTACH_KEY "attach"

/* key for clamav */
#define RCVD_CLAM_KEY "clamav"

/* key for dspam */
#define RCVD_DSPAM_KEY "dspam"

/* key for regex scanner */
#define RCVD_REGEX_KEY "regex"

/* key for spamassassin */
#define RCVD_SPAM_KEY "spam"

/* key for clamav */
#define RCVD_TROPHIE_KEY "trophie"

/* "ripmime program" */
#define RIPMIME "/usr/local/bin/ripmime"

/* Path to the sigtool binary. */
#define SIGTOOLPATH "/usr/bin/sigtool"

/* Path to the spamassassin binary */
#define SPAMASSASSINPATH "/usr/bin/spamassassin"

/* "spamc program" */
#define SPAMC "/usr/bin/spamc"

/* "spamc arguments" */
#define SPAMC_ARGS ""

/* "spam hits" */
/* #undef SPAM_HITS */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* "path to trophie binary" */
/* #undef TROPHIEBINARY */

/* "path to trophie socket" */
/* #undef TROPHIESOCKET */

/* Version number of package */
#define VERSION "1.4.0"

/* one or more virusscanners are active */
#define VIRUSSCANNER 1

/* "working directory" */
#define WORKDIR "/var/qmail/simscan"
