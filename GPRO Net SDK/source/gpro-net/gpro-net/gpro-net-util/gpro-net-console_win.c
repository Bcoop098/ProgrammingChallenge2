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

	gpro-net-console_win.c
	Console management source for Windows.
*/

#include "gpro-net/gpro-net/gpro-net-util/gpro-net-console.h"
#ifdef _WIN32

#include <io.h>
#include <stdio.h>
#include <Windows.h>


//-----------------------------------------------------------------------------

// redirect with settings
inline void gpro_consoleInternalRedirectToggle(gpro_console* const console, int const redirectInput, int const redirectOutput, int const redirectError)
{
	FILE* str = 0;
	int i = -1, j = -1;

	// redirect input
	i = 0;
	if (redirectInput)
	{
		if (GetStdHandle(STD_INPUT_HANDLE) != INVALID_HANDLE_VALUE && !console->handle[i])
		{
			// flush buffer, duplicate handle and reopen stream to console
			//j = fprintf(stdin, "\n STDIN =/= DEFAULT \n");
			j = fflush(stdin);
			j = _dup(i);
			str = freopen("CONIN$", "r+", stdin);
			if (str)
			{
				// store values and configure
				console->handle[i] = str;
				console->io[i] = j;
				j = setvbuf(stdin, NULL, _IONBF, 0);
				//j = fprintf(stdin, "\n STDIN == CONSOLE \n");
			}
		}
	}
	else
	{
		if (GetStdHandle(STD_INPUT_HANDLE) != INVALID_HANDLE_VALUE && console->handle[i])
		{
			// flush and reopen
			//j = fprintf(stdin, "\n STDIN =/= CONSOLE \n");
			j = fflush(stdin);
			str = freopen("NUL:", "r+", stdin);
			if (str)
			{
				// duplicate handle and reconfigure stream, reset variables
				j = _dup2(console->io[i], i);
				j = setvbuf(stdin, NULL, _IONBF, 0);
				//j = fprintf(stdin, "\n STDIN == DEFAULT \n");
				console->handle[i] = 0;
				console->io[i] = -1;
			}
		}
	}

	// redirect output
	i = 1;
	if (redirectOutput)
	{
		if (GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE && !console->handle[i])
		{
			// flush buffer, duplicate handle and reopen stream to console
			//j = fprintf(stdout, "\n STDOUT =/= DEFAULT \n");
			j = fflush(stdout);
			j = _dup(i);
			str = freopen("CONOUT$", "a+", stdout);
			if (str)
			{
				// store values and configure
				console->handle[i] = str;
				console->io[i] = j;
				j = setvbuf(stdout, NULL, _IONBF, 0);
				//j = fprintf(stdout, "\n STDOUT == CONSOLE \n");
			}
		}
	}
	else
	{
		if (GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE && console->handle[i])
		{
			// flush and reopen
			//j = fprintf(stdout, "\n STDOUT =/= CONSOLE \n");
			j = fflush(stdout);
			str = freopen("NUL:", "a+", stdout);
			if (str)
			{
				// duplicate handle and reconfigure stream, reset variables
				j = _dup2(console->io[i], i);
				j = setvbuf(stdout, NULL, _IONBF, 0);
				//j = fprintf(stdout, "\n STDOUT == DEFAULT \n");
				console->handle[i] = 0;
				console->io[i] = -1;
			}
		}
	}

	// redirect error
	i = 2;
	if (redirectError)
	{
		if (GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE && !console->handle[i])
		{
			// flush buffer, duplicate handle and reopen stream to console
			//j = fprintf(stderr, "\n STDERR =/= DEFAULT \n");
			j = fflush(stderr);
			j = _dup(i);
			str = freopen("CONOUT$", "a+", stderr);
			if (str)
			{
				// store values and configure
				console->handle[i] = str;
				console->io[i] = j;
				j = setvbuf(stderr, NULL, _IONBF, 0);
				//j = fprintf(stderr, "\n STDERR == CONSOLE \n");
			}
		}
	}
	else
	{
		if (GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE && console->handle[i])
		{
			// flush and reopen
			//j = fprintf(stderr, "\n STDERR =/= CONSOLE \n");
			j = fflush(stderr);
			str = freopen("NUL:", "a+", stderr);
			if (str)
			{
				// duplicate handle and reconfigure stream, reset variables
				j = _dup2(console->io[i], i);
				j = setvbuf(stderr, NULL, _IONBF, 0);
				//j = fprintf(stderr, "\n STDERR == DEFAULT \n");
				console->handle[i] = 0;
				console->io[i] = -1;
			}
		}
	}
}


//-----------------------------------------------------------------------------

int gpro_consoleCreateMain(gpro_console* const console)
{
	if (console)
	{
		// if console not already open
		void* handle = GetConsoleWindow();
		if (!handle && !console->handle[3])
		{
			// allocate and show console
			if (AllocConsole())
			{
				// reset flags
				console->handle[0] = console->handle[1] = console->handle[2] = 0;
				console->io[0] = console->io[1] = console->io[2] = -1;

				// init flag
				console->handle[3] = handle = GetConsoleWindow();

				// disable closing console manually because doing this kills 
				//	the whole application; could also start a new process, 
				//	but then there's also that to manage
				DeleteMenu(GetSystemMenu(handle, FALSE), SC_CLOSE, MF_BYCOMMAND);

				// redirect to new console (in/out, not err)
				gpro_consoleInternalRedirectToggle(console, 1, 1, 0);

				// done
				return 0;
			}
			return -2;
		}
		return +1;
	}
	return -1;
}


int gpro_consoleRedirectMain(gpro_console* const console, int const redirectInput, int const redirectOutput, int const redirectError)
{
	if (console)
	{
		// if console exists
		void const* const handle = GetConsoleWindow();
		if ((console->handle[3] == handle) && handle)
		{
			// redirect toggle
			gpro_consoleInternalRedirectToggle(console, redirectInput, redirectOutput, redirectError);

			// done
			return 0;
		}
		return -2;
	}
	return -1;
}


int gpro_consoleReleaseMain(gpro_console* const console)
{
	if (console)
	{
		// if console exists
		void const* const handle = GetConsoleWindow();
		if ((console->handle[3] == handle) && handle)
		{
			// reset to original standard i/o
			gpro_consoleInternalRedirectToggle(console, 0, 0, 0);

			// delete console instance
			// console will hide when all standard handles are closed
			if (FreeConsole())
			{
				// reset
				console->handle[3] = 0;

				// done
				return 0;
			}
			return -2;
		}
		return +1;
	}
	return -1;
}


//-----------------------------------------------------------------------------

int gpro_consoleGetCursor(short* const x_out, short* const y_out)
{
	if (x_out && y_out)
	{
		CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo[1];
		HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
		if (stdHandle && console &&
			GetConsoleScreenBufferInfo(stdHandle, screenBufferInfo))
		{
			*x_out = screenBufferInfo->dwCursorPosition.X;
			*y_out = screenBufferInfo->dwCursorPosition.Y;
			return 0;
		}
		return -2;
	}
	return -1;
}


int gpro_consoleSetCursor(short const x, short const y)
{
	COORD const pos = { x, y };
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
	if (stdHandle && console &&
		SetConsoleCursorPosition(stdHandle, pos))
	{
		return 0;
	}
	return -2;
}


int gpro_consoleToggleCursor(int const visible)
{
	CONSOLE_CURSOR_INFO cursorInfo[1];
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
	if (stdHandle && console &&
		GetConsoleCursorInfo(stdHandle, cursorInfo))
	{
		cursorInfo->bVisible = visible;
		if (SetConsoleCursorInfo(stdHandle, cursorInfo))
		{
			return 0;
		}
	}
	return -2;
}


int gpro_consoleGetColor(gpro_consoleColor* const fg_out, gpro_consoleColor* const bg_out)
{
	if (fg_out && bg_out)
	{
		CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo[1];
		HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
		if (stdHandle && console &&
			GetConsoleScreenBufferInfo(stdHandle, screenBufferInfo))
		{
			*fg_out = (gpro_consoleColor)(screenBufferInfo->wAttributes & 0xf);
			*bg_out = (gpro_consoleColor)(screenBufferInfo->wAttributes >> 4 & 0xf);
			return 0;
		}
		return -2;
	}
	return -1;
}


int gpro_consoleSetColor(gpro_consoleColor const fg, gpro_consoleColor const bg)
{
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
	if (stdHandle && console &&
		SetConsoleTextAttribute(stdHandle, (short)(fg | bg << 4)))
	{
		return 0;
	}
	return -2;
}


int gpro_consoleResetColor()
{
	return gpro_consoleSetColor(gpro_consoleColor_white, gpro_consoleColor_black);
}


int gpro_consoleGetCursorColor(short* const x_out, short* const y_out, gpro_consoleColor* const fg_out, gpro_consoleColor* const bg_out)
{
	if (x_out && y_out && fg_out && bg_out)
	{
		CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo[1];
		HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
		if (stdHandle && console &&
			GetConsoleScreenBufferInfo(stdHandle, screenBufferInfo))
		{
			*x_out = screenBufferInfo->dwCursorPosition.X;
			*y_out = screenBufferInfo->dwCursorPosition.Y;
			*fg_out = (gpro_consoleColor)(screenBufferInfo->wAttributes & 0xf);
			*bg_out = (gpro_consoleColor)(screenBufferInfo->wAttributes >> 4 & 0xf);
			return 0;
		}
		return -2;
	}
	return -1;
}


int gpro_consoleSetCursorColor(short const x, short const y, gpro_consoleColor const fg, gpro_consoleColor const bg)
{
	COORD const pos = { x, y };
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
	if (stdHandle && console &&
		SetConsoleCursorPosition(stdHandle, pos) &&
		SetConsoleTextAttribute(stdHandle, (short)(fg | bg << 4)))
	{
		return 0;
	}
	return -2;
}


int gpro_consoleDrawTestPatch()
{
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
	if (stdHandle && console)
	{
		// test all colors and shifts
		short x, y;
		gpro_consoleColor fg, bg;
		for (y = 0; y < 16; ++y)
		{
			for (x = 0; x < 16; ++x)
			{
				fg = (gpro_consoleColor)y;
				bg = (gpro_consoleColor)x;
				gpro_consoleSetColor(fg, bg);
				gpro_consoleSetCursor(x * 2, y);
				printf("%x", (int)x);
				gpro_consoleSetCursorColor(x * 2 + 1, y, fg, bg);
				printf("%x", (int)y);
			}
		}
		gpro_consoleGetCursor(&x, &y);
		gpro_consoleGetColor(&fg, &bg);
		gpro_consoleGetCursorColor(&x, &y, &fg, &bg);
		gpro_consoleResetColor();
		printf("[]=(%d, %d) \n", (int)x, (int)y);

		// done
		return 0;
	}
	return -2;
}


int gpro_consoleClear()
{
	// help to avoid using system("cls"): https://docs.microsoft.com/en-us/windows/console/clearing-the-screen 
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
	if (stdHandle && console)
	{
		// simple clear
		//system("cls");

		CONSOLE_SCREEN_BUFFER_INFO buffer[1];
		if (GetConsoleScreenBufferInfo(stdHandle, buffer))
		{
			COORD const coord = { 0, 0 };
			DWORD const sz = (buffer->dwSize.X * buffer->dwSize.Y);
			DWORD write[1] = { 0 };
			if (FillConsoleOutputCharacterA(stdHandle, ' ', sz, coord, write) &&
				GetConsoleScreenBufferInfo(stdHandle, buffer) &&
				FillConsoleOutputAttribute(stdHandle, buffer->wAttributes, sz, coord, write) &&
				SetConsoleCursorPosition(stdHandle, coord))
			{
				// done
				return 0;
			}
		}
	}
	return -2;
}


//-----------------------------------------------------------------------------

int gpro_consolePrintDebug(char const* const format, ...)
{
	if (format)
	{
		unsigned char str[256] = { 0 };
		va_list args = 0;
		int result = 0;

		// fill buffer with formatted arguments
		va_start(args, format);
		result = _vsnprintf(str, sizeof(str), format, args);
		va_end(args);

		// internal print
		OutputDebugStringA(str);

		// return length
		return result;
	}
	return -1;
}


//-----------------------------------------------------------------------------


#endif	// _WIN32