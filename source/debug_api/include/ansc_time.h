/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/


/**********************************************************************

    module:	ansc_time.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros related to time operation. Instead
        of defining everything in a single file, we simply route
        every function and macro to the corresponding platform-
        dependent prototype definition.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/24/01    initial revision.
        04/01/01    add user mode wrapper functions

**********************************************************************/


#ifndef  _ANSC_TIME_
#define  _ANSC_TIME_


/***********************************************************
            FUNCTION AND MACRO PROTOTYPE ROUTING
***********************************************************/

/*
 * According to the current platform definition, we route the definition to the corresponding
 * header files.
 */

    #define  AnscGetTickInSeconds                   UserGetTickInSeconds2
    #define  AnscGetTickInSecondsAbs                UserGetTickInSecondsAbs     /* linux specific */
    #define  AnscGetTickInMilliSeconds              UserGetTickInMilliSeconds2
    #define  AnscGetTickInMilliSecondsAbs           UserGetTickInMilliSecondsAbs
    #define  AnscSleep                              UserSleepInMilliSeconds
    #define  AnscGetUtcTime                         UserGetUtcTime
    #define  AnscSetUtcTime                         UserSetUtcTime
    #define  AnscGetSystemTime                      UserGetSystemTime
	#define  AnscGetNtpTime                         UserGetNtpTime
    #define  AnscSetSystemTime                      UserSetSystemTime
    #define  AnscGetLocalTime                       UserGetLocalTime
    #define  AnscSetLocalTime                       UserSetLocalTime

	#ifdef	_ANSC_SIMPLE_TIMEZONE

		#define	 AnscSetTimeZone					AnscSetSimpleTimeZone
		#define  AnscGetTimeZoneOffset				AnscGetSimpleTimeZoneOffset
		#define	 AnscEnumTimeZones					AnscEnumSimpleTimeZones

	#else

		#define  AnscSetTimeZone                    UserSetTzString             /* linux specific */
		#define  AnscGetTimeZoneOffset              UserGetTzOffset             /* linux specific */

	#endif

    #define  AnscGetUtcSeconds                      UserGetUtcSeconds           /* linux specific */

    #define  ANSC_TIME_SOURCE_SYS                   USER_TIME_SOURCE_SYS
    #define  ANSC_TIME_SOURCE_NTP                   USER_TIME_SOURCE_NTP


    /*
     * Following data structure is used by objects to require time/date information from either the
     * underlying operationg system, or from the utility objects.
     */
    typedef  struct  _USER_SYSTEM_TIME  ANSC_UNIVERSAL_TIME,  *PANSC_UNIVERSAL_TIME;


/*
 * There're some differences between our definitions of the calendar and the one defined by NTP
 * (Network Time Protocol). For example, NTP defines "Month" in the range of from 0 to 11 while
 * we use 1 to 12.
 */
#define  ANSC_MONTH_JANUARY                         1
#define  ANSC_MONTH_FEBRUARY                        2
#define  ANSC_MONTH_MARCH                           3
#define  ANSC_MONTH_APRIL                           4
#define  ANSC_MONTH_MAY                             5
#define  ANSC_MONTH_JUNE                            6
#define  ANSC_MONTH_JULY                            7
#define  ANSC_MONTH_AUGUST                          8
#define  ANSC_MONTH_SEPTEMBER                       9
#define  ANSC_MONTH_OCTOBER                         10
#define  ANSC_MONTH_NOVEMBER                        11
#define  ANSC_MONTH_DECEMBER                        12

#define  ANSC_WEEKDAY_SUNDAY                        0
#define  ANSC_WEEKDAY_MONDAY                        1
#define  ANSC_WEEKDAY_TUESDAY                       2
#define  ANSC_WEEKDAY_WEDNESDAY                     3
#define  ANSC_WEEKDAY_THURSDAY                      4
#define  ANSC_WEEKDAY_FRIDAY                        5
#define  ANSC_WEEKDAY_SATURDAY                      6

/*
 * Following is the list of all available time-zones (multiple names may be assigned to the same
 * time zone due to that several well-known places belong to the same zone).
 */
#define  ANSC_TIME_ZONE_ENIWETOK                    (-12 * 3600)                            /* GMT - 12:00 */
#define  ANSC_TIME_ZONE_KWAJALEIN                   (-12 * 3600)                            /* GMT - 12:00 */
#define  ANSC_TIME_ZONE_MIDWAY_ISLAND               (-11 * 3600)                            /* GMT - 11:00 */
#define  ANSC_TIME_ZONE_SAMOA                       (-11 * 3600)                            /* GMT - 11:00 */
#define  ANSC_TIME_ZONE_HAWAII                      (-10 * 3600)                            /* GMT - 10:00 */
#define  ANSC_TIME_ZONE_ALASKA                      (-09 * 3600)                            /* GMT - 09:00 */
#define  ANSC_TIME_ZONE_PACIFIC_TIME                (-08 * 3600)                            /* GMT - 08:00 */
#define  ANSC_TIME_ZONE_TIJUANA                     (-08 * 3600)                            /* GMT - 08:00 */
#define  ANSC_TIME_ZONE_ARIZONA                     (-07 * 3600)                            /* GMT - 07:00 */
#define  ANSC_TIME_ZONE_MOUNTAIN_TIME               (-07 * 3600)                            /* GMT - 07:00 */
#define  ANSC_TIME_ZONE_CENTRAL_AMERICA             (-06 * 3600)                            /* GMT - 06:00 */
#define  ANSC_TIME_ZONE_CENTRAL_TIME                (-06 * 3600)                            /* GMT - 06:00 */
#define  ANSC_TIME_ZONE_MEXICO_CITY                 (-06 * 3600)                            /* GMT - 06:00 */
#define  ANSC_TIME_ZONE_SASKATCHEWAN                (-06 * 3600)                            /* GMT - 06:00 */
#define  ANSC_TIME_ZONE_BOGOTA                      (-05 * 3600)                            /* GMT - 05:00 */
#define  ANSC_TIME_ZONE_LIMA                        (-05 * 3600)                            /* GMT - 05:00 */
#define  ANSC_TIME_ZONE_QUITO                       (-05 * 3600)                            /* GMT - 05:00 */
#define  ANSC_TIME_ZONE_EASTERN_TIME                (-05 * 3600)                            /* GMT - 05:00 */
#define  ANSC_TIME_ZONE_INDIANA                     (-05 * 3600)                            /* GMT - 05:00 */
#define  ANSC_TIME_ZONE_ATLANTIC_TIME               (-04 * 3600)                            /* GMT - 04:00 */
#define  ANSC_TIME_ZONE_CARACAS                     (-04 * 3600)                            /* GMT - 04:00 */
#define  ANSC_TIME_ZONE_LA_PAZ                      (-04 * 3600)                            /* GMT - 04:00 */
#define  ANSC_TIME_ZONE_SANTIAGO                    (-04 * 3600)                            /* GMT - 04:00 */
#define  ANSC_TIME_ZONE_NEWFOUNDLAND                (-03 * 3600 - 1800)                     /* GMT - 03:30 */
#define  ANSC_TIME_ZONE_BRASILIA                    (-03 * 3600)                            /* GMT - 03:00 */
#define  ANSC_TIME_ZONE_BUENOS_AIRES                (-03 * 3600)                            /* GMT - 03:00 */
#define  ANSC_TIME_ZONE_GEORGETOWN                  (-03 * 3600)                            /* GMT - 03:00 */
#define  ANSC_TIME_ZONE_GREENLAND                   (-03 * 3600)                            /* GMT - 03:00 */
#define  ANSC_TIME_ZONE_MID_ATLANTIC                (-02 * 3600)                            /* GMT - 02:00 */
#define  ANSC_TIME_ZONE_AZORES                      (-01 * 3600)                            /* GMT - 01:00 */
#define  ANSC_TIME_ZONE_CAPE_VERDE_ISLAND           (-01 * 3600)                            /* GMT - 01:00 */
#define  ANSC_TIME_ZONE_CASABLANCA                  (+00 * 3600)                            /* GMT         */
#define  ANSC_TIME_ZONE_MONROVIA                    (+00 * 3600)                            /* GMT         */
#define  ANSC_TIME_ZONE_GMT                         (+00 * 3600)                            /* GMT         */
#define  ANSC_TIME_ZONE_DUBLIN                      (+00 * 3600)                            /* GMT         */
#define  ANSC_TIME_ZONE_EDINBURGH                   (+00 * 3600)                            /* GMT         */
#define  ANSC_TIME_ZONE_LISBON                      (+00 * 3600)                            /* GMT         */
#define  ANSC_TIME_ZONE_LONDON                      (+00 * 3600)                            /* GMT         */
#define  ANSC_TIME_ZONE_AMSTERDAM                   (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_BERLIN                      (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_BERN                        (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_ROME                        (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_STOCKHOLM                   (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_VIENNA                      (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_BELGRADE                    (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_BRATISLAVA                  (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_BUDAPEST                    (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_LJUBLJANA                   (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_PRAGUE                      (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_BRUSSELS                    (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_COPENHAGEN                  (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_MADRID                      (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_PARIS                       (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_SARAJEVO                    (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_SKOPJE                      (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_SOFIJA                      (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_VILNIUS                     (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_WARSAW                      (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_ZAGREB                      (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_WEST_AFRICA                 (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_CENTRAL_AFRICA              (+01 * 3600)                            /* GMT + 01:00 */
#define  ANSC_TIME_ZONE_ATHENS                      (+02 * 3600)                            /* GMT + 02:00 */
#define  ANSC_TIME_ZONE_ISTANBUL                    (+02 * 3600)                            /* GMT + 02:00 */
#define  ANSC_TIME_ZONE_MINSK                       (+02 * 3600)                            /* GMT + 02:00 */
#define  ANSC_TIME_ZONE_BUCHAREST                   (+02 * 3600)                            /* GMT + 02:00 */
#define  ANSC_TIME_ZONE_CAIRO                       (+02 * 3600)                            /* GMT + 02:00 */
#define  ANSC_TIME_ZONE_HARARE                      (+02 * 3600)                            /* GMT + 02:00 */
#define  ANSC_TIME_ZONE_PRETORIA                    (+02 * 3600)                            /* GMT + 02:00 */
#define  ANSC_TIME_ZONE_HELSINKI                    (+02 * 3600)                            /* GMT + 02:00 */
#define  ANSC_TIME_ZONE_RIGA                        (+02 * 3600)                            /* GMT + 02:00 */
#define  ANSC_TIME_ZONE_TALLINN                     (+02 * 3600)                            /* GMT + 02:00 */
#define  ANSC_TIME_ZONE_JERUSALEM                   (+02 * 3600)                            /* GMT + 02:00 */
#define  ANSC_TIME_ZONE_BAGHDAD                     (+03 * 3600)                            /* GMT + 03:00 */
#define  ANSC_TIME_ZONE_KUWAIT                      (+03 * 3600)                            /* GMT + 03:00 */
#define  ANSC_TIME_ZONE_RIYADH                      (+03 * 3600)                            /* GMT + 03:00 */
#define  ANSC_TIME_ZONE_MOSCOW                      (+03 * 3600)                            /* GMT + 03:00 */
#define  ANSC_TIME_ZONE_ST_PETERSBURG               (+03 * 3600)                            /* GMT + 03:00 */
#define  ANSC_TIME_ZONE_VOLGOGRAD                   (+03 * 3600)                            /* GMT + 03:00 */
#define  ANSC_TIME_ZONE_NAIROBI                     (+03 * 3600)                            /* GMT + 03:00 */
#define  ANSC_TIME_ZONE_TEHRAN                      (+03 * 3600 + 1800)                     /* GMT + 03:30 */
#define  ANSC_TIME_ZONE_ABU_DHABI                   (+04 * 3600)                            /* GMT + 04:00 */
#define  ANSC_TIME_ZONE_MUSCAT                      (+04 * 3600)                            /* GMT + 04:00 */
#define  ANSC_TIME_ZONE_BAKU                        (+04 * 3600)                            /* GMT + 04:00 */
#define  ANSC_TIME_ZONE_TBILISI                     (+04 * 3600)                            /* GMT + 04:00 */
#define  ANSC_TIME_ZONE_YEREVAN                     (+04 * 3600)                            /* GMT + 04:00 */
#define  ANSC_TIME_ZONE_KABUL                       (+04 * 3600 + 1800)                     /* GMT + 04:30 */
#define  ANSC_TIME_ZONE_EKATERINBURG                (+05 * 3600)                            /* GMT + 05:00 */
#define  ANSC_TIME_ZONE_ISLAMABAD                   (+05 * 3600)                            /* GMT + 05:00 */
#define  ANSC_TIME_ZONE_KARACHI                     (+05 * 3600)                            /* GMT + 05:00 */
#define  ANSC_TIME_ZONE_TASHKENT                    (+05 * 3600)                            /* GMT + 05:00 */
#define  ANSC_TIME_ZONE_CALCUTTA                    (+05 * 3600 + 1800)                     /* GMT + 05:30 */
#define  ANSC_TIME_ZONE_CHENNAI                     (+05 * 3600 + 1800)                     /* GMT + 05:30 */
#define  ANSC_TIME_ZONE_MUMBAI                      (+05 * 3600 + 1800)                     /* GMT + 05:30 */
#define  ANSC_TIME_ZONE_NEW_DELHI                   (+05 * 3600 + 1800)                     /* GMT + 05:30 */
#define  ANSC_TIME_ZONE_KATHMANDU                   (+05 * 3600 + 2700)                     /* GMT + 05:45 */
#define  ANSC_TIME_ZONE_ALMATY                      (+06 * 3600)                            /* GMT + 06:00 */
#define  ANSC_TIME_ZONE_NOVOSIBIRSK                 (+06 * 3600)                            /* GMT + 06:00 */
#define  ANSC_TIME_ZONE_ASTANA                      (+06 * 3600)                            /* GMT + 06:00 */
#define  ANSC_TIME_ZONE_DHAKA                       (+06 * 3600)                            /* GMT + 06:00 */
#define  ANSC_TIME_ZONE_SRI_JAYAWARDENEPURA         (+06 * 3600)                            /* GMT + 06:00 */
#define  ANSC_TIME_ZONE_RANGOON                     (+06 * 3600 + 1800)                     /* GMT + 06:30 */
#define  ANSC_TIME_ZONE_BANGKOK                     (+07 * 3600)                            /* GMT + 07:00 */
#define  ANSC_TIME_ZONE_HANOI                       (+07 * 3600)                            /* GMT + 07:00 */
#define  ANSC_TIME_ZONE_JAKARTA                     (+07 * 3600)                            /* GMT + 07:00 */
#define  ANSC_TIME_ZONE_KRASNOYARSK                 (+07 * 3600)                            /* GMT + 07:00 */
#define  ANSC_TIME_ZONE_BEIJING                     (+08 * 3600)                            /* GMT + 08:00 */
#define  ANSC_TIME_ZONE_CHONGQING                   (+08 * 3600)                            /* GMT + 08:00 */
#define  ANSC_TIME_ZONE_HONG_KONG                   (+08 * 3600)                            /* GMT + 08:00 */
#define  ANSC_TIME_ZONE_URUMQI                      (+08 * 3600)                            /* GMT + 08:00 */
#define  ANSC_TIME_ZONE_IRKUTSK                     (+08 * 3600)                            /* GMT + 08:00 */
#define  ANSC_TIME_ZONE_ULAAN_BATAAR                (+08 * 3600)                            /* GMT + 08:00 */
#define  ANSC_TIME_ZONE_KUALA_LUMPUR                (+08 * 3600)                            /* GMT + 08:00 */
#define  ANSC_TIME_ZONE_SINGAPORE                   (+08 * 3600)                            /* GMT + 08:00 */
#define  ANSC_TIME_ZONE_PERTH                       (+08 * 3600)                            /* GMT + 08:00 */
#define  ANSC_TIME_ZONE_TAIPEI                      (+08 * 3600)                            /* GMT + 08:00 */
#define  ANSC_TIME_ZONE_OSAKA                       (+09 * 3600)                            /* GMT + 09:00 */
#define  ANSC_TIME_ZONE_SAPPORO                     (+09 * 3600)                            /* GMT + 09:00 */
#define  ANSC_TIME_ZONE_TOKYO                       (+09 * 3600)                            /* GMT + 09:00 */
#define  ANSC_TIME_ZONE_SEOUL                       (+09 * 3600)                            /* GMT + 09:00 */
#define  ANSC_TIME_ZONE_YAKUTSK                     (+09 * 3600)                            /* GMT + 09:00 */
#define  ANSC_TIME_ZONE_ADELAIDE                    (+09 * 3600 + 1800)                     /* GMT + 09:30 */
#define  ANSC_TIME_ZONE_DARWIN                      (+09 * 3600 + 1800)                     /* GMT + 09:30 */
#define  ANSC_TIME_ZONE_BRISBANE                    (+10 * 3600)                            /* GMT + 10:00 */
#define  ANSC_TIME_ZONE_CANBERRA                    (+10 * 3600)                            /* GMT + 10:00 */
#define  ANSC_TIME_ZONE_MELBOURNE                   (+10 * 3600)                            /* GMT + 10:00 */
#define  ANSC_TIME_ZONE_SYDNEY                      (+10 * 3600)                            /* GMT + 10:00 */
#define  ANSC_TIME_ZONE_GUAM                        (+10 * 3600)                            /* GMT + 10:00 */
#define  ANSC_TIME_ZONE_PORT_MORESBY                (+10 * 3600)                            /* GMT + 10:00 */
#define  ANSC_TIME_ZONE_HOBART                      (+10 * 3600)                            /* GMT + 10:00 */
#define  ANSC_TIME_ZONE_VLADIVOSTOK                 (+10 * 3600)                            /* GMT + 10:00 */
#define  ANSC_TIME_ZONE_MAGADAN                     (+11 * 3600)                            /* GMT + 11:00 */
#define  ANSC_TIME_ZONE_SOLOMON_ISLAND              (+11 * 3600)                            /* GMT + 11:00 */
#define  ANSC_TIME_ZONE_NEW_CALEDONIA               (+11 * 3600)                            /* GMT + 11:00 */
#define  ANSC_TIME_ZONE_AUCKLAND                    (+12 * 3600)                            /* GMT + 12:00 */
#define  ANSC_TIME_ZONE_WELLINGTON                  (+12 * 3600)                            /* GMT + 12:00 */
#define  ANSC_TIME_ZONE_FIJI                        (+12 * 3600)                            /* GMT + 12:00 */
#define  ANSC_TIME_ZONE_KAMCHATKA                   (+12 * 3600)                            /* GMT + 12:00 */
#define  ANSC_TIME_ZONE_MARSHALL_ISLAND             (+12 * 3600)                            /* GMT + 12:00 */
#define  ANSC_TIME_ZONE_NUKUALOFA                   (+13 * 3600)                            /* GMT + 13:00 */


#endif
