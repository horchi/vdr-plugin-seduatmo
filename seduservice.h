/*
 * seduservice.h: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: seduservice.h,v 1.3 2012/11/22 18:02:59 wendel Exp $
 */

#ifndef __SEDU_SERVICE_H
#define __SEDU_SERVICE_H

//***************************************************************************
// Sedu Service
//***************************************************************************

class cSeduService
{
   public:

      enum Cinebars
      {
         cbHorizontal,
         cbVertical,
         cbBoth,
         cbCount
      };

      enum SeduMode
      {
         smMiniDMX,
         smTpm2,
         smCount
      };

      enum ViewMode
      {
         vmAtmo,
         vmFixedCol,
         vmRainbow,
         vmBlack,
         vmDetached,
         vmCount
      };

      enum LedPosition
      {
         lpTop,
         lpLeft,
         lpBottom,
         lpRight
      };

      struct cLed
      {
         int x;
         int toX;
         int y;
         int toY;
         LedPosition lp;
      };

      // static

      static const char* toName(ViewMode vm);
      static const char* viewModes[];
};

#endif // __SEDU_SERVICE_H
