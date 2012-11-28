/*
 * ledsconf.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: ledsconf.c,v 1.18 2012/11/20 19:04:09 wendel Exp $
 */

#include <ctype.h>

#include "common.h"
#include "config.h"
#include "ledsconf.h"

//***************************************************************************
// cLedConf
//***************************************************************************
//***************************************************************************
// Object
//***************************************************************************

cLedConf::cLedConf() 
{ 
   x = na;
   toX = na;
   y = na;
   toY = na;
   lp = na;
}

//***************************************************************************
// Parse like like "led 0-1 14-17"
//***************************************************************************

bool cLedConf::Parse(const char* s)
{
   const char* p = s;

   p = skipWs(p);

   // check keyword

   if (strncasecmp(p, "led ", 4) != 0)
      return false;

   p += 4;
   skipWs(p);

   // LED Position

   if (strncasecmp(p, "top ", 4) == 0)
      lp = lpTop;
   else if (strncasecmp(p, "left ", 5) == 0)
      lp = lpLeft;
   else if (strncasecmp(p, "bot ", 4) == 0)
      lp = lpBottom;
   else if (strncasecmp(p, "bottom ", 7) == 0)
      lp = lpBottom;
   else if (strncasecmp(p, "right ", 6) == 0)
      lp = lpRight;
   else
      return error("Missing location {top,left,bot(tom),right}");
   
   // skip to delemiter
   
   while (*p && *p != ' ' && *p != '\t')
      p++;

   // check

   if (!*p)
      return false;

   skipWs(p);

   // parse X 

   if (!parseRange(p, x, toX))
      return false;

   // parse Y
   
   if (!parseRange(p, y, toY))
      return false;
   
   return true;
}

//***************************************************************************
// Parse Range like "12-26"
//***************************************************************************

bool cLedConf::parseRange(const char*& p, int& from, int& to)
{
   p = skipWs(p);

   if (!isdigit(*p))
      return false;

   from = to = strtol(p, (char**)&p, 0);

   p = skipWs(p);

   if (*p != '-')
      return true;

   p++;

   if (!isdigit(*p))
      return false;

   to = strtol(p, (char**)&p, 0);

   return true;
}

//***************************************************************************
// Skip Whitespaces
//***************************************************************************

const char* cLedConf::skipWs(const char* p)
{
   while (*p && (*p == ' ' || *p == '\t'))
      p++;

   return p;
}
