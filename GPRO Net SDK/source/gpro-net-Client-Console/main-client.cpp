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

	main-client.cpp
	Main source for console client application.
*/
#include "gpro-net/gpro-net-client/gpro-net-RakNet-Client.hpp"

#include "gpro-net/gpro-net.h"

#include "../gpro-net-Client-Plugin/gpro-net-Client-Plugin.h"

#if (defined _WINDOWS || defined _WIN32)

//#include <Windows.h>
#pragma comment(lib, "gpro-net-Client-Plugin.lib")
// Description of spatial pose.
struct sSpatialPose
{
	float scale[3];     // non-uniform scale
	float rotate[3];    // orientation as Euler angles
	float translate[3]; // translation

	// read from stream
	RakNet::BitStream& Read(RakNet::BitStream& bitstream)
	{
		bitstream.Read(scale[0]);
		bitstream.Read(scale[1]);
		bitstream.Read(scale[2]);
		bitstream.Read(rotate[0]);
		bitstream.Read(rotate[1]);
		bitstream.Read(rotate[2]);
		bitstream.Read(translate[0]);
		bitstream.Read(translate[1]);
		bitstream.Read(translate[2]);
		return bitstream;
	}

	// write to stream
	RakNet::BitStream& Write(RakNet::BitStream& bitstream) const
	{
		bitstream.Write(scale[0]);
		bitstream.Write(scale[1]);
		bitstream.Write(scale[2]);
		bitstream.Write(rotate[0]);
		bitstream.Write(rotate[1]);
		bitstream.Write(rotate[2]);
		bitstream.Write(translate[0]);
		bitstream.Write(translate[1]);
		bitstream.Write(translate[2]);
		return bitstream;
	}
};

// complete plugin test
int testPlugin()
{
	//HMODULE plugin = LoadLibrary(TEXT("./plugin/gpro-net-Client-Plugin"));
	//if (plugin)
	{
		printf("%d \n", foo(9000));

		// done
		//return FreeLibrary(plugin);
		return 1;
	}
	return -1;
}

#else	// !(defined _WINDOWS || defined _WIN32)

// dummy plugin test
int testPlugin()
{
	return -1;
}

#endif	// (defined _WINDOWS || defined _WIN32)


// utility test (game states, console)
int testUtility()
{
	gpro_battleship battleship;
	gpro_checkers checkers;
	gpro_mancala mancala;

	gpro_battleship_reset(battleship);
	gpro_checkers_reset(checkers);
	gpro_mancala_reset(mancala);

	return gpro_consoleDrawTestPatch();
}


int main(int const argc, char const* const argv[])
{
	float scaleHolder[4];
	float rotationHolder[4];
	float translateHolder[4];
	int scaleInt = 0;
	int rotationInt = 0;
	int translateInt = 0;
	int ourScaleInt;
	int ourRotationInt;
	int ourTranslationInt;
	float ourScale[4];
	float ourRotation[4];
	float ourTranslation[4];
	testUtility();

	testPlugin();

	gproNet::cRakNetClient client;
	RakNet::BitStream bitStream;

	while (1)
	{
		sSpatialPose playerSpatialPose;
		playerSpatialPose.Read(bitStream);
		for (int i = 0; i < 3; i++)
		{
			scaleHolder[i] = playerSpatialPose.scale[i];
			rotationHolder[i] = playerSpatialPose.rotate[i];
			translateHolder[i] = playerSpatialPose.translate[i];
		}

		//brain fart, forgot why this isn't working. Why is it I forget this, but not bitwise
		scaleHolder = decompress(scaleInt, scaleHolder);
		rotationHolder = decompress(rotationInt, rotationHolder);
		translateHolder = decompress(translateInt, translateHolder);
		//run decompress on this info
		/*
		* update pos, scale, and rotation with this value
		*/
		//get our info, compress it, store it in a bitstream

		bitStream.Reset();
		/*
		* playerSpatialPose.scale = compress(ourScale); //this wouldn't work, however the point here is that we need to send the int we get from compression to the bitstream
		* playerSpatialPose.rotate = compress(ourRotation);
		* playerSpatialPose.scale.translate = compress(ourTranslation);
		*/
		//write the info to the bitstream;
		playerSpatialPose.Write(bitStream);
		client.MessageLoop();
		//use messageLoop to send the data, struggling to remember what we need to do other than the bitstream stuff
	}

	printf("\n\n");
	system("pause");

	
}



int compress(float *floatArray)//float[4] to int (w = 1?)
{
	//float array represents scale, rotate, or translate
	int maxIndex = 0;
	float maxIndexValue = 0;
	int out;
	float newQuaternion[4];
	float scale = 0.5f; //this value should be a number to assist with normalizing the information.
	for (int i = 0; i < 4; i++)
	{
		newQuaternion[i] = floatArray[i];
	}
	//find max index
	int arrSize = sizeof(floatArray) / sizeof(floatArray[0]);
	for (int i = 0; i < arrSize; i++)
	{
		if (maxIndexValue < floatArray[i]) //if the float value for the maxIndex is less than the value we are checking
		{
			maxIndexValue = floatArray[i];
			maxIndex = i; //update max index position
		}
	}
	//run compression algorithm
	
	//int is 32 bits 0000 0000 0000 0000 0000 0000 0000 0000
	out += (maxIndex << 30); //index of max value, only care about the first 2 bits, since those represent 0,1,2,3
	int bitOffset = 20; //has to start at 20, decrease it each loop
	for (int i = 0; i < 4; i++)
	{
		if (i != maxIndex)
		{
			bool negative = newQuaternion[i]; //we want to see if we have to set a negative flag later, this was discussed by a few of us on friday (might have been saturday, head is a mess)
			//from what I recall, someone (i think scott) mentioned that it was easier to set a flag that a value was negative, by taking the int value for an input and adding 512 to it to set the flag
			int newInput = abs(newQuaternion[i]) * scale; //float to int, less data, also needed
			if (negative)
			{
				newInput += 512; //sets the flag for negative at the 0 value of a 10-bit array(?) or whatever we are calling the way we store this information
			}
			out += (newInput << bitOffset); //shift new input left by the offset. this is the 511 part.
			bitOffset -= 10; //move to the next set of bits
		}
	}
	return out; //return the int with the offset completed
}

float * decompress(int quaternionInt, float* array)//int to float4 (w = 1?)
{
	
	int maxIndex = (quaternionInt >> 30) & 3; //only care about the first 2 bits
	int currentIndex = 0;
	float inverseScale = .5f; //rescales the values to be usable, forget how I get this
	for (int i = 20; i >= 0; i -= 10) //this is basically undoing the previous code, and since we care about the bits and not the index of the array, hence the 20 to start (the offset)
	{
		if (currentIndex == maxIndex)
		{
			currentIndex++;
		}
		int everythingButLeading = (quaternionInt >> i); //shift quaternionInt right by the value of I, then set everythingButLeading to it
		everythingButLeading &= 1023; //run AND on every number now, get 1 where both are 1, only want 10 bits, this helps to get that //1111 1111 11 (10 bits)
		(array)[currentIndex] = everythingButLeading & 511; //set the arrays value at the current index to be the return value of everythingButLeading AND 511, not 512, since the flag comes next
		if ((everythingButLeading & 512) != 0) //get the flag from the bit signifying the sign, first bit out of 10
		{
			(array)[currentIndex] *= -1; //if the negative flag is there, then we want to multiply the value by negative 1
		}
		currentIndex++;
	}
	//undo the square root
	float squareSum = 0;
	for (int i = 0; i < 4; i++)
	{
		if (i != maxIndex)
		{
			(array)[i] *= inverseScale;
			squareSum += ((array)[i] * (array)[i]);
		}
	}
	float remainder = 1.0f - squareSum; //squareSum should be less than 1
	(array)[maxIndex] = sqrt(remainder);
	return (array);
}