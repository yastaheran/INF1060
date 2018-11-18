/**
* En av headerene som blir brukt av ifish.c, og history.c
* 
*/

#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>

#define PROGNAME            "ifish"

#define err(fmt, args...)   fprintf(stderr, fmt, ## args)

// Debugging: hvis definert, skal det gjøres noe. Ellers skal det erstattes med NULL 
//og la kompilatoren fjerne det
#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "DEBUG: " fmt, ## args)
#else
#define debug(fmt, args...)
#endif

//default paths
#ifndef WCPATH
#define WCPATH ""
#endif

#ifndef PSPATH
#define PSPATH ""
#endif

#endif