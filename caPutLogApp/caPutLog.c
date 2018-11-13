/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* caPutLog.c,v 1.25.2.6 2004/10/07 13:37:34 mrk Exp */
/*
 *      Author:         Jeffrey O. Hill 
 *      Date:           080791 
 */

/*
 * ANSI C
 */
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include <errlog.h>
#include <envDefs.h>
#include <logClient.h>
#include <epicsExit.h>

#define epicsExportSharedSymbols
#include "caPutLogAs.h"
#include "caPutLogTask.h"
#include "caPutLogClient.h"
#include "caPutLog.h"

#ifndef LOCAL
#define LOCAL static
#endif

/*
 *  caPutLogShow ()
 */
void caPutLogShow (int level)
{
    if (level < 0) level = 0;
    if (level > 2) level = 2;
#if 0
    caPutLogAsShow(level);
#endif
    caPutLogClientShow(level);
}

/*
 *  caPutLogReconf()
 */
int caPutLogReconf (int config)
{
#if 0
    caPutLogTaskReconf(config);
#endif
    caPutLogClientFlush();
    return caPutLogSuccess;
}

static void caPutLogExitProc(void *arg)
{
    caPutLogAsStop();
}

/*
 *  caPutLogInit()
 */
int caPutLogInit (const char *addr_str, int config)
{
    int status;

    if (config == caPutLogNone) {
        return caPutLogSuccess;
        errlogSevPrintf(errlogInfo, "caPutLog: disabled\n");
    }

    status = caPutLogClientInit(addr_str);
    if (status) {
        return caPutLogError;
    }

    status = caPutLogTaskStart(config);
    if (status) {
        return caPutLogError;
    }

    status = caPutLogAsInit();
    if (status) {
        return caPutLogError;
    }

    epicsAtExit(caPutLogExitProc, NULL);

    errlogSevPrintf(errlogInfo, "caPutLog: successfully initialized\n");
    return caPutLogSuccess;
}
