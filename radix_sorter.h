#ifndef RDESTL_RADIX_SORTER_H
#define RDESTL_RADIX_SORTER_H

#include "rdestl/vector.h"

namespace rde
{
	template<typename T>
	struct radix_sorter
	{
		static const size_t kHistogramSize	= 256;

		enum data_type
		{
			data_unsigned = false,
			data_signed,
		};

		void Resize(int num)
		{
			m_dst.resize(num);
		}
		RDE_FORCEINLINE void CalculateOffsets(rde::uint32_t* histogram)
		{
			rde::uint32_t offsets[4] = { 1, 1, 1, 1 };
			for (int i = 0; i < kHistogramSize; ++i)
			{
				rde::uint32_t temp = histogram[i] + offsets[0];
				histogram[i] = offsets[0] - 1;
				offsets[0] = temp;

				temp = histogram[i + kHistogramSize] + offsets[1];
				histogram[i + kHistogramSize] = offsets[1] - 1;
				offsets[1] = temp;

				temp = histogram[i + kHistogramSize*2] + offsets[2];
				histogram[i + kHistogramSize*2] = offsets[2] - 1;
				offsets[2] = temp;

				temp = histogram[i + kHistogramSize*3] + offsets[3];
				histogram[i + kHistogramSize*3] = offsets[3] - 1;
				offsets[3] = temp;
			}
		}
		RDE_FORCEINLINE void CalculateOffsetsSigned(rde::uint32_t* histogram)
		{
			rde::uint32_t offsets[4] = { 1, 1, 1, 1 };
			int numNeg(0);
			for (int i = 0; i < kHistogramSize; ++i)
			{
				rde::uint32_t temp = histogram[i] + offsets[0];
				histogram[i] = offsets[0] - 1;
				offsets[0] = temp;

				temp = histogram[i + kHistogramSize] + offsets[1];
				histogram[i + kHistogramSize] = offsets[1] - 1;
				offsets[1] = temp;

				temp = histogram[i + kHistogramSize*2] + offsets[2];
				histogram[i + kHistogramSize*2] = offsets[2] - 1;
				offsets[2] = temp;

				if (i >= kHistogramSize/2)
					numNeg += histogram[i + kHistogramSize*3];
			}
			rde::uint32_t* h3 = &histogram[kHistogramSize*3];
			offsets[3] = numNeg + 1;
			for (int i = 0; i < kHistogramSize / 2; ++i)
			{
				rde::uint32_t temp = h3[i] + offsets[3];
				h3[i] = offsets[3] - 1;
				offsets[3] = temp;
			}
			offsets[3] = 1;
			for (int i = kHistogramSize / 2; i < kHistogramSize; ++i)
			{
				rde::uint32_t temp = h3[i] + offsets[3];
				h3[i] = offsets[3] - 1;
				offsets[3] = temp;
			}
		}

		template<data_type TDataType, typename TFunc>
		void Sort(T* src, int num, const TFunc& func)
		{
			if (num == 0)
				return;
			if (num > m_dst.size())
				Resize(num);

			rde::uint32_t	m_histogram[kHistogramSize * 4];
			rde::Sys::MemSet(m_histogram, 0, sizeof(m_histogram));

			rde::uint32_t* h1 = &m_histogram[0] + kHistogramSize;
			rde::uint32_t* h2 = h1 + kHistogramSize;
			rde::uint32_t* h3 = h2 + kHistogramSize;

			bool alreadySorted(true);
			rde::uint32_t prevValue = func(src[0]);

			int k(0);
			for (int i = 0; i < num; ++i)
			{
				k = i;
				const rde::uint32_t x = func(src[i]);
				if (alreadySorted && x < prevValue)
				{
					alreadySorted = false;
					break;
				}
				const rde::uint8_t* px = (const rde::uint8_t*)&x;

				++m_histogram[*px];
				++h1[px[1]];
				++h2[px[2]];
				++h3[px[3]];

				prevValue = x;
			}
			if (alreadySorted)
				return;

			for (; k < num; ++k)
			{
				const rde::uint32_t x = func(src[k]);
				const rde::uint8_t* px = (const rde::uint8_t*)&x;

				++m_histogram[*px];
				++h1[px[1]];
				++h2[px[2]];
				++h3[px[3]];
			}

			const bool canBreakAfter16Bits = (h2[0] == (rde::uint32_t)num);
			(void)canBreakAfter16Bits;

			if (TDataType == data_signed)
				CalculateOffsetsSigned(m_histogram);
			else
				CalculateOffsets(m_histogram);

			for (int i = 0; i < num; ++i)
			{
				const rde::uint32_t pos = func(src[i]) & 0xFF;
				m_dst[m_histogram[pos]++] = src[i];
			}
			for (int i = 0; i < num; ++i)
			{
				const rde::uint32_t pos = (func(m_dst[i]) >> 8) & 0xFF;
				src[h1[pos]++] = m_dst[i];
			}
			if (TDataType == data_unsigned && canBreakAfter16Bits)
				return;
			for (int i = 0; i < num; ++i)
			{
				const rde::uint32_t pos = (func(src[i]) >> 16) & 0xFF;
				m_dst[h2[pos]++] = src[i];
			}
			for (int i = 0; i < num; ++i)
			{
				const rde::uint32_t pos = (func(m_dst[i]) >> 24) & 0xFF;
				src[h3[pos]++] = m_dst[i];
			}
		}		
		rde::vector<T> m_dst;
	};
}

#endif
