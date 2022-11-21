/*
 * config.h: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: config.h,v 1.28 2012/11/28 06:29:24 wendel Exp $
 */

#ifndef __SEDU_CONFIG_H
#define __SEDU_CONFIG_H

#include "common.h"
#include "ledsconf.h"
#include "seduservice.h"

//***************************************************************************
// Configuration
//***************************************************************************

class cSeduConfig : public cSeduService
{
   public:

      cSeduConfig();
      ~cSeduConfig();

      void setTvIp(const char* p)           { free(tvIp); tvIp = strdup(p); }

      // geometry

      int grabWidth {na};
      int grabHeight {na};

      // adjust

      int threshold {17};
      int adjGreen {100};
      int adjRed {57};
      int adjBlue {70};
      int frequence {25};
      unsigned int average {10};
      int gamma {14};
      int xDeep {2};
      int yDeep {1};
      int black {0};

      // technical

      ViewMode viewMode {vmAtmo};
      int fixedR {111};
      int fixedG {101};
      int fixedB {0};
      int effectSpeed {500};

      int showMainmenu {yes};
      char seduRGBOrder[4] {};
      SeduMode seduMode {smMiniDMX};
      Cinebars detectCineBars {cbBoth};

      char* tvIp {};
      int loglevel {0};

      cLed* leds {};
      size_t ledCount {0};

      // functions

      cLed* createLeds(cLedConfs* conf);
      void copyLeds(cSeduConfig* c)
      {
         ledCount = c->ledCount;
         leds = new cLed[ledCount];
         memset(leds, 0, ledCount*sizeof(cLed));

         for (int i = 0; i < ledCount; i++)
            leds[i] = c->leds[i];
      }
};

//***************************************************************************
// Global Configuration
//***************************************************************************

extern cSeduConfig cfg;

#endif // __SEDU_CONFIG_H
