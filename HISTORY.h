/*
 * ----------------------------------------------
 * seduatmo Plugin - Revision History
 * ----------------------------------------------
 *
 *  (c) 2012-2018 Jörg Wendel
 *
 */

#define _VERSION      "0.0.10"
#define VERSION_DATE  "21.08.2020"

#ifdef GIT_REV
#  define VERSION _VERSION "-GIT" GIT_REV
#else
#  define VERSION _VERSION
#endif

/*
 * ------------------------------------

2020-08-21: Version 0.0.10
  - added: Auto powerof by TV state (option -t <ip|dns>)

2019-12-20: Version 0.0.9
     Added config RGB order for each LED

2019-12-20: Version 0.0.8
     Added support of softhdvaapi and softhddrm plugin

2018-10-14: Version 0.0.7
  - added: Plugin service interface to change mode

2018-10-04: Version 0.0.6
     Added support of softhdcuvid plugin

2018-09-24: Version 0.0.5
     Auto device detection configurable

2018-02-29: Version 0.0.4
      Improved recover handling

2015-10-11: Version 0.0.3
      Fixed menu handling

2012-11-26: Version 0.0.2

2012-11-07: Version 0.0.1
  - Initial revision.

 * ------------------------------------
 */
