/***
 * hesperus: SimpleImage.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

namespace hesp {

//#################### CONSTRUCTORS ####################
template <typename Pixel>
SimpleImage<Pixel>::SimpleImage(int width, int height)
:	Image(width, height)
{
	int size = width * height;
	Pixel *pixels = new Pixel[size]();	// <-- Note that this *default constructs* every Pixel in the array.
	m_pixels.reset(pixels);
}

template <typename Pixel>
SimpleImage<Pixel>::SimpleImage(Pixel *pixels, int width, int height)
:	Image(width, height), m_pixels(pixels)
{}

template <typename Pixel>
SimpleImage<Pixel>::SimpleImage(const shared_array<Pixel>& pixels, int width, int height)
:	Image(width, height), m_pixels(pixels)
{}

template <typename Pixel>
SimpleImage<Pixel>::SimpleImage(const Image_Ptr& rhs)
:	Image(rhs->width(), rhs->height())
{
	int size = m_width * m_height;
	Pixel *pixels = new Pixel[size];
	for(int i=0; i<size; ++i) pixels[i] = rhs(i);
	m_pixels.reset(pixels);
}

//#################### PUBLIC OPERATORS ####################
template <typename Pixel>
const Pixel SimpleImage<Pixel>::operator()(int n) const
{
	return m_pixels[n];
}

template <typename Pixel>
const Pixel SimpleImage<Pixel>::operator()(int x, int y) const
{
	return m_pixels[y*m_width+x];
}

//#################### PUBLIC METHODS ####################
template <typename Pixel>
void SimpleImage<Pixel>::set(int n, const Pixel& pixel)
{
	m_pixels[n] = pixel;
}

template <typename Pixel>
void SimpleImage<Pixel>::set(int x, int y, const Pixel& pixel)
{
	m_pixels[y*m_width+x] = pixel;
}

}
