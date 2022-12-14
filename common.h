/*
 * common.h: EPG2VDR plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __COMMON_H
#define __COMMON_H

//***************************************************************************
//
//***************************************************************************

enum Misc
{
   success = 0,
   done    = success,
   fail    = -1,
   ignore  = -2,
   na      = -1,
   yes     = 1,
   on      = 1,
   off     = 0,
   no      = 0,
   TB      = 1,

   tmeSecondsPerMinute = 60,
   tmeSecondsPerHour = tmeSecondsPerMinute * 60,
   tmeSecondsPerDay = 24 * tmeSecondsPerHour,
   tmeUsecondsPerSecond = 1000 * 1000
};

//***************************************************************************
// Misc ..
//***************************************************************************

int isEmpty(const char* str);
int minMax(int x, int min, int max);
int getrand(int min, int max);
double min(double a, double b);
double max(double a, double b);
int isAlive(const char* address);
int ping(const char* address);

//***************************************************************************
// Time
//***************************************************************************

typedef unsigned long long MsTime;

MsTime msNow();

//***************************************************************************
// Tell
//***************************************************************************

void __attribute__ ((format(printf, 2, 3))) tell(int eloquence, const char* format, ...);
int error(const char* format, ...);

//***************************************************************************
#endif //___COMMON_H
