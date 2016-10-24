//==============================================================================================================
//BitArray.hpp
//by Albert Chen Feb-15-2016.
//==============================================================================================================

#pragma once

#ifndef _included_BitArray__
#define _included_BitArray__

//#include "Engine/Core/Utilities.hpp"
//#include "Engine/Core/BinaryUtils.hpp"

//===========================================================================================================

class BitArray {
public:
	BitArray(void* buffer, size_t byteLen, bool initVal) {
		m_buffer = (Byte*)buffer;
		m_bufferLen = byteLen;
		Reset(initVal);
	}

	void Reset(bool val) {

		if (val) {
			memset(m_buffer, 0xff, m_bufferLen);
		}
		else {
			memset(m_buffer, 0x00, m_bufferLen);
		}

	}
	void Set(size_t idx) {

		size_t byteIdx = idx / 8;
		size_t bitIdx = idx % 8;

		Byte b = m_buffer[byteIdx];
		b = b | (1 << bitIdx);

		m_buffer[byteIdx] = b;


	}
	void Unset(size_t idx) {

		size_t byteIdx = idx / 8;
		size_t bitIdx = idx % 8;
		Byte b = m_buffer[byteIdx];
		b = b & ~(1 << bitIdx);

	}
	bool Get(size_t idx) {
		size_t byteIdx = idx / 8;
		size_t bitIdx = idx % 8;
		Byte b = m_buffer[byteIdx];

		return (b & 1 << bitIdx) != 0;

	}

protected:
	Byte* m_buffer;
	size_t m_bufferLen;

};

template < size_t BITSIZE >
class TBitArray : public BitArray {

public:
	TBitArray(bool initialValue) : BitArray(m_fixedBuffer, (BITSIZE + 7) / 8, initialValue){

	}

	TBitArray() : BitArray(m_fixedBuffer, (BITSIZE + 7) / 8, false){

	}

	//vars
	Byte m_fixedBuffer[(BITSIZE + 7) / 8];

};


//===========================================================================================================

#endif //__includedBitArray__
