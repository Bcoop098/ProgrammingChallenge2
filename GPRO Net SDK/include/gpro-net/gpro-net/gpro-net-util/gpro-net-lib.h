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

	gpro-net-lib.h
	Header for common library macros.
*/

#ifndef _GPRO_NET_LIB_H_
#define _GPRO_NET_LIB_H_


// universal export/import tag definition
#ifdef GPRO_NET_EXPORTS
#define GPRO_NET_SYMBOL __declspec(dllexport)
#else	// !GPRO_NET_EXPORTS
#ifdef	GPRO_NET_IMPORTS
#define GPRO_NET_SYMBOL __declspec(dllimport)
#else	// !GPRO_NET_IMPORTS
#define GPRO_NET_SYMBOL 
#endif	// GPRO_NET_IMPORTS
#endif	// GPRO_NET_EXPORTS


#endif	// !_GPRO_NET_LIB_H_