/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef UTIL_IMAGE_H
#define UTIL_IMAGE_H

#include <algorithm>
#include <cstdint>
#include <cstring>

template<typename Tin, typename Tout>
void convertImageData(Tin *dataIn, Tout *dataOut, uint64_t samples)
{
	Tout output;
	
	for (uint64_t i = 0; i < samples; ++i)
	{
		output = static_cast<Tout>(*dataIn);
		*dataOut = output;
		++dataIn;
		++dataOut;
	}
}

template<>
void convertImageData<uint8_t, uint8_t>(uint8_t *dataIn, uint8_t *dataOut, uint64_t samples)
{
	memcpy(dataOut, dataIn, (size_t) (samples * sizeof(uint8_t)));
}

template<>
void convertImageData<uint16_t, uint8_t>(uint16_t *dataIn, uint8_t *dataOut, uint64_t samples)
{
	uint8_t output;
	
	for (uint64_t i = 0; i < samples; ++i)
	{
		output = (*dataIn) >> 8;
		*dataOut = output;
		++dataIn;
		++dataOut;
	}
}

template<>
void convertImageData<int16_t, uint8_t>(int16_t *dataIn, uint8_t *dataOut, uint64_t samples)
{
	int32_t input;
	uint8_t output;
	
	for (uint64_t i = 0; i < samples; ++i)
	{
		input = (static_cast<int32_t>(*dataIn) + 32768);
		output = input >> 8;
		*dataOut = output;
		++dataIn;
		++dataOut;
	}
}

template<>
void convertImageData<uint32_t, uint8_t>(uint32_t *dataIn, uint8_t *dataOut, uint64_t samples)
{
	uint8_t output;
	
	for (uint64_t i = 0; i < samples; ++i)
	{
		output = (*dataIn) >> 24;
		*dataOut = output;
		++dataIn;
		++dataOut;
	}
}

#endif
