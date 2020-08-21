/*
 * common.c: EPG2VDR plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include <sys/time.h>
#include <sys/param.h>
#include <netinet/in_systm.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>

#include <stdarg.h>
#include <string.h>
#include <syslog.h>

#include <vdr/thread.h>

#include "common.h"
#include "config.h"

cMutex logMutex;

//***************************************************************************
// Tell
//***************************************************************************

void tell(int eloquence, const char* format, ...)
{
   if (cfg.loglevel < eloquence)
      return ;

   const int sizeBuffer = 100000;
   char t[sizeBuffer+100]; *t = 0;
   va_list ap;

   cMutexLock lock(&logMutex);

   va_start(ap, format);

   snprintf(t, sizeBuffer, "SEDUATMO: ");
   vsnprintf(t+strlen(t), sizeBuffer-strlen(t), format, ap);

   syslog(LOG_ERR, "%s", t);

   va_end(ap);
}

//***************************************************************************
// Error
//***************************************************************************

int error(const char* format, ...)
{
   const int sizeBuffer = 100000;
   char t[sizeBuffer+100]; *t = 0;
   va_list ap;

   cMutexLock lock(&logMutex);

   va_start(ap, format);

   snprintf(t, sizeBuffer, "SEDUATMO: ");
   vsnprintf(t+strlen(t), sizeBuffer-strlen(t), format, ap);

   syslog(LOG_ERR, "%s", t);

   va_end(ap);

   return fail;
}

//***************************************************************************
// is Empty
//***************************************************************************

int isEmpty(const char* str)
{
   return !str || !*str;
}

//***************************************************************************
// msNow
//***************************************************************************

MsTime msNow()
{
   timeval tv;

   gettimeofday(&tv, 0);

   return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

//***************************************************************************
// Misc. Functions
//***************************************************************************

int minMax(int x, int min, int max)
{
   if (x < min)
      return min;

   if (max < x)
      return max;

   return x;
}

double min(double a, double b)
{
    return a < b ? a : b;
}

double max(double a, double b)
{
    return a >= b ? a : b;
}

int getrand(int min, int max)
{
    srand(time(0));
    return rand() % (max-min) + min;
}

//***************************************************************************
// Is Alive  (fork ping therefore no root permissions needed)
//***************************************************************************

int isAlive(const char* address)
{
   return ping(address) == success;
}

uint16_t cksum(uint16_t *addr, unsigned len)
{
   uint16_t answer = 0;
   uint32_t sum = 0;

   while (len > 1)
   {
      sum += *addr++;
      len -= 2;
   }

   if (len == 1)
   {
      *(unsigned char *)&answer = *(unsigned char *)addr ;
      sum += answer;
   }

   sum = (sum >> 16) + (sum & 0xffff);
   sum += (sum >> 16);
   answer = ~sum;

   return answer;
}

int ping(const char* target)
{
   const size_t DEFDATALEN = (64-ICMP_MINLEN);
   const size_t MAXIPLEN = 60;
   const size_t MAXICMPLEN = 76;
   const size_t MAXPACKET = (65536 - 60 - ICMP_MINLEN);

   int i, cc, packlen, datalen = DEFDATALEN;
	struct hostent *hp;
	struct sockaddr_in to, from;

	u_char *packet, outpack[MAXPACKET];
	char hnamebuf[MAXHOSTNAMELEN];
   std::string hostname;
	struct icmp *icp;
	int ret, fromlen, hlen;
	fd_set rfds;
	struct timeval tv;

	to.sin_family = AF_INET;
	to.sin_addr.s_addr = inet_addr(target);

	if (to.sin_addr.s_addr != (u_int)-1)
		hostname = target;
	else
	{
		if (!(hp = gethostbyname(target)))
		{
			tell(0, "unknown host '%s'", target);
			return fail;
		}

		to.sin_family = hp->h_addrtype;
		bcopy(hp->h_addr, (caddr_t)&to.sin_addr, hp->h_length);
		strncpy(hnamebuf, hp->h_name, sizeof(hnamebuf) - 1);
		hostname = hnamebuf;
	}

	packlen = datalen + MAXIPLEN + MAXICMPLEN;
	packet = (u_char*)malloc((u_int)packlen);

   int sd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (sd < 0)
	{
      tell(0, "Error: sokett failed due to '%s'", strerror(errno));
		return fail;   // Needs to run as root ore set rights "setcap cap_net_raw+ep /usr/bin/vdr"
	}

	icp = (struct icmp*)outpack;
	icp->icmp_type = ICMP_ECHO;
	icp->icmp_code = 0;
	icp->icmp_cksum = 0;
	icp->icmp_seq = 12345;
	icp->icmp_id = getpid();

	cc = datalen + ICMP_MINLEN;
	icp->icmp_cksum = cksum((unsigned short *)icp,cc);
	i = sendto(sd, (char *)outpack, cc, 0, (struct sockaddr*)&to, (socklen_t)sizeof(struct sockaddr_in));

   if (i < 0)
      tell(0, "Error: Sendto '%s'", strerror(errno));

	FD_ZERO(&rfds);
	FD_SET(sd, &rfds);
	tv.tv_sec = 0;
	tv.tv_usec = 50000;

	while (true)
	{
		int retval = select(sd+1, &rfds, NULL, NULL, &tv);

		if (retval <= 0)
		{
         if (retval == -1)
            tell(0, "Error: select() '%s'", strerror(errno));

         close(sd);
			return fail;
		}

      fromlen = sizeof(sockaddr_in);

      if ((ret = recvfrom(sd, (char*)packet, packlen, 0, (struct sockaddr*)&from, (socklen_t*)&fromlen)) < 0)
      {
         tell(0, "Error: recvfrom '%s'", strerror(errno));
         close(sd);
         return fail;
      }

      // Check the IP header

      hlen = sizeof(struct ip);

      if (ret < (hlen + ICMP_MINLEN))
      {
         tell(0,  "Packet too short (%d) bytes from '%s'", ret, hostname.c_str());
         close(sd);
         return fail;
      }

      // Now the ICMP part

      icp = (struct icmp*)(packet + hlen);

      if (icp->icmp_type == ICMP_ECHOREPLY)
      {
         if (icp->icmp_seq == 12345 && icp->icmp_id == getpid())
         {
            close(sd);
            return success;
         }
      }
   }

   close(sd);

	return fail;
}
