/*
 * ledsconf.h: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: ledsconf.h,v 1.9 2012/11/20 14:32:28 wendel Exp $
 */

#ifndef _LED_CONF_H_
#define _LED_CONF_H_

#include <vdr/plugin.h>
#include <vdr/tools.h>

#include "seduservice.h"

//***************************************************************************
// cLedConf
//***************************************************************************

class cLedConf : public cListObject, public cSeduService
{
   public:

      cLedConf() {};

      bool Parse(const char* s);

      int X()       { return x; }
      int ToX()     { return toX; }
      int Y()       { return y; }
      int ToY()     { return toY; }
      int Pos()     { return lp; }
      const char* RgbOrder() { return rgbOrder; }

      int isValid() { return x > na && y > na && lp > na; }

   private:

      bool parseRange(const char*& p, int& from, int& to);
      bool parseOrder(const char*& p, char* order);
      const char* skipWs(const char* p);

      int x {na};
      int toX {na};
      int y {na};
      int toY {na};
      int lp {na};                // LED postition
      char rgbOrder[4] {'\0'};
};

//***************************************************************************
// cLedConfs
//***************************************************************************

class cLedConfs : public cConfig<cLedConf>
{

};

extern cLedConfs ledConfs;

#endif // _LED_CONF_H_
