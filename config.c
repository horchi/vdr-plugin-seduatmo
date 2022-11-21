/*
 * seduthred.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: config.c,v 1.27 2012/11/28 06:29:24 wendel Exp $
 */

#include <string.h>

#include "config.h"
#include "common.h"

//***************************************************************************
// Global Configuration
//***************************************************************************

cSeduConfig cfg;

//***************************************************************************
// cConfigData
//***************************************************************************

cSeduConfig::cSeduConfig()
{
   strcpy(seduRGBOrder, "BGR");
}

cSeduConfig::~cSeduConfig()
{
   free(tvIp);
   delete leds;
}

//***************************************************************************
// Create Leds from config
//***************************************************************************

cSeduConfig::cLed* cSeduConfig::createLeds(cLedConfs* conf)
{
   int seq = 0;
   delete leds;

   grabWidth = 0;
   grabHeight = 0;

   ledCount = conf->Count();
   leds = new cLed[ledCount];
   memset(leds, 0, ledCount*sizeof(cLed));

   for (cLedConf* l = conf->First(); l; l = conf->Next(l))
   {
      tell(1, "led%d (%d) %d/%d  %d/%d", seq, l->Pos(),
           l->X(), l->Y(),
           l->ToX(), l->ToY());

      if (l->isValid())
      {
         // calc size of led matrix

         if (grabWidth < l->X())
            grabWidth = l->X();
         if (grabWidth < l->ToX())
            grabWidth = l->ToX();
         if (grabHeight < l->Y())
            grabHeight = l->Y();
         if (grabHeight < l->ToY())
            grabHeight = l->ToY();

         leds[seq].lp  = (LedPosition)l->Pos();
         leds[seq].x   = l->X();
         leds[seq].y   = l->Y();
         leds[seq].toX = l->ToX();
         leds[seq].toY = l->ToY();

         strcpy(leds[seq].rgbOrder, l->RgbOrder());

         seq++;
      }
   }

   ledCount = seq;
   grabWidth++;
   grabHeight++;

   return leds;
}
