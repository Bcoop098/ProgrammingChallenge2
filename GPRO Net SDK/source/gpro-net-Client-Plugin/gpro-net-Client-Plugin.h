/*
   Copyright 2021 Daniel S. Buckstein

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	GPRO Net SDK: Networking framework.
	By Daniel S. Buckstein

	gpro-net-Client-Plugin.h
	Main interface for plugin; wraps C++ features with C prototypes.

	****THIS FILE MUST NOT CONTAIN C++ KEYWORDS****
*/

#ifndef _GPRO_NET_PLUGIN_CLIENT_H_
#define _GPRO_NET_PLUGIN_CLIENT_H_


#include "gpro-net/gpro-net/gpro-net-util/gpro-net-lib.h"


#ifdef __cplusplus
extern "C" {
#endif	// __cplusplus


GPRO_NET_SYMBOL int foo(int bar);


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !_GPRO_NET_PLUGIN_CLIENT_H_