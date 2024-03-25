
#pragma once

#include <vector>

namespace Sim::Graphics::Data
{

template <int N>
struct Atlas
{
	struct Pixel
	{
		uint8_t data[N] = {0};

		constexpr Pixel() = default;

		constexpr Pixel(const Pixel& p)
		{
			for(int i = 0; i < N; i++)
			{
				data[i] = p.data[i];
			}
		}

		constexpr Pixel(const uint8_t* p)
		{
			for(int i = 0; i < N; i++)
			{
				data[i] = p[i];
			}
		}
		
		constexpr bool operator == (const Pixel& p) const
		{
			for(int i = 0; i < N; i++)
			{
				if(data[i] != p.data[i])
				{
					return false;
				}
			}
			
			return true;
		}

		constexpr bool operator != (const Pixel& p) const
		{
			return !(*this == p);
		}

		constexpr Pixel& operator = (const Pixel& p)
		{
			for(int i = 0; i < N; i++)
			{
				data[i] = p.data[i];
			}

			return *this;
		}

		constexpr Pixel& operator = (const uint8_t* p)
		{
			for(int i = 0; i < N; i++)
			{
				data[i] = p[i];
			}

			return *this;
		}

		constexpr uint8_t& operator [] (int i)
		{
			return data[i];
		}

		constexpr const uint8_t& operator [] (int i) const
		{
			return data[i];
		}
	};

	const int width;
	const int height;

	std::vector<Pixel> data;

	Atlas(int width, int height)
		: width(width)
		, height(height)
		, data(width * height)
	{
	}

	Pixel& operator()(int x, int y, int z = 0)
	{
		int z_off = z * width * height;

		if(z_off >= data.size())
		{
			data.resize(z_off + width * height);
		}

		return data[z_off + y * width + x];
	}

	const Pixel& operator()(int x, int y, int z = 0) const
	{
		static const Pixel empty;
		int z_off = z * width * height;

		if(z_off >= data.size())
		{
			return empty;
		}

		return data[z_off + y * width + x];
	}

	void draw(const Atlas<N>& src, int x, int y, int z = 0, bool padding = false)
	{
		for(int i = 0; i < src.height; i++)
		{
			for(int j = 0; j < src.width; j++)
			{
				(*this)(x + j, y + i, z) = src(j, i);
			}
		}

		if(padding)
		{
			for(int i = 0; i < src.height; i++)
			{
				(*this)(x - 1, y + i, z) = src(0, i);
				(*this)(x + src.width, y + i, z) = src(src.width - 1, i);
			}

			for(int i = 0; i < src.width; i++)
			{
				(*this)(x + i, y - 1, z) = src(i, 0);
				(*this)(x + i, y + src.height, z) = src(i, src.height - 1);
			}

			(*this)(x - 1, y - 1, z) = src(0, 0);
			(*this)(x + src.width, y - 1, z) = src(src.width - 1, 0);
			(*this)(x - 1, y + src.height, z) = src(0, src.height - 1);
			(*this)(x + src.width, y + src.height, z) = src(src.width - 1, src.height - 1);
		}
	}
};

};

