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

    module:	ansc_element_definitions.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines some of the element object and
        data structures used across Ansc platform.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/21/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_ELEMENT_DEFINITIONS_
#define  _ANSC_ELEMENT_DEFINITIONS_


/***********************************************************
            RFC 822 EMAIL BOX OBJECT DEFINITIONS
***********************************************************/

/*
 * A mailbox receives mail. It is a conceptual entity which does not necessarily pertain to file
 * storage. For example, some sites may choose to print mail on their line printer and deliver the
 * output to the addressee's desk.
 *
 * A mailbox specification comprises a person, system or process name reference, a domain-dependent
 * string, and a name-domain reference. The name reference is optional and is usually used to
 * indicate the human name of a recipient. The name-domain reference specifies a sequence of sub-
 * domains.  The domain-dependent string is uninterpreted, except by the final sub-domain; the rest
 * of the mail service merely transmits it as a literal string.
 */
#define  ANSC_MAX_RFC822_LPART_SIZE                 128
#define  ANSC_MAX_RFC822_DNAME_SIZE                 ANSC_DOMAIN_NAME_SIZE

typedef  struct
_ANSC_RFC822_MAILBOX
{
    char                            LocalPart [ANSC_MAX_RFC822_LPART_SIZE];
    char                            DomainName[ANSC_MAX_RFC822_DNAME_SIZE];
}
ANSC_RFC822_MAILBOX,  *PANSC_RFC822_MAILBOX;


#endif
