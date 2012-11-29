/*
 * seduservice.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: seduservice.c,v 1.3 2012/11/22 18:02:59 wendel Exp $
 */

#include "seduservice.h"

//***************************************************************************
// View Modes
//***************************************************************************

const char* cSeduService::viewModes[] =
{
   "atmo",
   "fixed color",
   "rainbow",
   "black",
   "detached",

   0
};

const char* cSeduService::toName(ViewMode vm)
{
   return viewModes[vm];
}
