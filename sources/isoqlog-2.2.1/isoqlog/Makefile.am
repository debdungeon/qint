bin_PROGRAMS = isoqlog

isoqlog_SOURCES = Store.c Parser.c main.c loadconfig.c LangCfg.c Html.c Dir.c Data.c 
isoqlog_LDADD   = 

EXTRA_DIST = Data.h Dir.h Html.h Global.h LangCfg.h loadconfig.h Parser.h Store.h isoqlog.conf.sample1 isoqlog.conf.sample2 isoqlog.conf.sample3

# set the include path found by configure
INCLUDES= $(all_includes)

# the library search path.
isoqlog_LDFLAGS = $(all_libraries) 
