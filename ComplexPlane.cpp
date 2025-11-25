#include "ComplexPlane.h"

/*
	Private members:
		VertexArray m_vArray;
		State m_State{};
		Vector2f m_mouseLocation, m_plane_center, m_plane_size;
		Vector2i m_pixel_size;
		int m_zoomcount{};
		float m_aspectRatio;

*/


ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) : m_aspectRatio((pixelHeight / 1.f) / pixelWidth),
                                                              m_vArray(Points, pixelWidth* pixelHeight),
                                                              m_State(State::CALCULATING),
                                                               m_mouseLocation(0, 0),
															  m_plane_center(0, 0),
                                                              m_plane_size(BASE_WIDTH, BASE_HEIGHT* m_aspectRatio),
                                                              m_pixel_size(pixelWidth, pixelHeight),
															  m_zoomCount(0){}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const {
	target.draw(m_vArray);
}


void ComplexPlane::concurrentRender(ComplexPlane* objPtr, size_t startRow, size_t endRow) {
	for (size_t i = startRow; i < endRow; i++)
	{
		for (size_t j = 0; j < (size_t)objPtr->m_pixel_size.x; j++)
		{
			//cout << "I" << i << "  : J  " << j << endl;
			objPtr->m_vArray[j + i * objPtr->m_pixel_size.x].position = { (float)j,(float)i };

			Vector2i tempPixel(j, i);

			Vector2f tempVFloat = objPtr->mapPixelToCoords(tempPixel);

			size_t iter = objPtr->countIterations(tempVFloat);

			Uint8 r, g, b;

			objPtr->iterationsToRGB(iter, r, g, b);

			objPtr->m_vArray[j + i * objPtr->m_pixel_size.x].color = { r,g,b };


		}
	}
}

void ComplexPlane::updateRender()
{

	if (m_State == State::CALCULATING)
	{
		size_t THREAD_COUNT = thread::hardware_concurrency(); // returns the amount of threads that can be run concurrently
		if (THREAD_COUNT == 0) THREAD_COUNT = 8;

		vector<thread> threadVect;
		
		// Each thread handles its own horizontal slice of the image
		for (size_t i = 0; i < THREAD_COUNT; ++i) {
			size_t startRow = (m_pixel_size.y / THREAD_COUNT) * i;
			size_t endRow = (i == THREAD_COUNT-1) ? m_pixel_size.y : (m_pixel_size.y / THREAD_COUNT) + startRow;
			threadVect.push_back(thread{ concurrentRender, this, startRow, endRow});
		}

		
		for (size_t i = 0; i < THREAD_COUNT; ++i) {
			threadVect.at(i).join();
		}

		m_State = State::DISPLAYING;
	}
}

void ComplexPlane::zoomIn() {
	++m_zoomCount;
	float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);

	m_plane_size.x = xSize;
	m_plane_size.y = ySize;
	m_State = State::CALCULATING;
}

void ComplexPlane::zoomOut() {
	--m_zoomCount;
	float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);

	m_plane_size.x = xSize;
	m_plane_size.y = ySize;
	m_State = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel) {
	m_plane_center = mapPixelToCoords(mousePixel);
	m_State = State::CALCULATING;

}

void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
	m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text)
{
	stringstream ss;

	ss << "Mandelbrot Set" << endl;
	ss << "Center: (" << m_plane_center.x << "," << m_plane_center.y << ")" << endl;
	ss << "Cursor: (" << m_mouseLocation.x << "," << m_mouseLocation.y << ")" << endl;
	ss << "Left-click to Zoom in" << endl;
	ss << "Right-click to Zoom out" << endl;

	text.setString(ss.str());
}


/*
		Thus, plotting the Mandelbrot set is done as follows:
	1) map each pixel on the screen to a complex number c,
	2)check if it belongs to the set by iterating the formula, and
	3)color the pixel black if it does and a diffrerent color if it does not.

	The absolute value of z may never exceed 2.0, so we set a maximum number of iterations,
	in our case we will use 64.
*/

size_t ComplexPlane::countIterations(Vector2f coord)
{

	/// 
	/// Grabbed this "void testconvergence" from the complex example in canvas. just gotta refine it a bit.
	/// 


	complex<double> c(coord.x, coord.y);			//MAP given pixel coord to c


	complex<double> z(0, 0);

	//cout << "abs(z) " << abs(z) << endl;
	size_t i;

	for (i = 0; i < MAX_ITER && abs(z) < 2.0; ++i)
	//check if it belongs by iterating
	{
		z = z * z + c;
		//cout << "z_" << i << "= " << z << endl;
		//cout << "|z| = " << abs(z) << endl
	}

	return i;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
	if (count == MAX_ITER)
	{
		r = 0;
		g = 0;
		b = 0;
		//BLACK
	}
	else if (count >= 0 && count <= 22)
	{
		size_t rangePos = count; // current position in the color range
		size_t rangeLen = 22; //total len of color range
		float prog = (float)rangePos / (float)rangeLen; // prog tells you how far you are into the gradient (0=beginning, 1=end)

		r = 191 + (int)(80 * prog);
		g = 0;
		b = 255;
		//PURPLE
	}
	else if (count >= 23 && count <= 46)
	{
		size_t rangePos = count - 23;
		size_t rangeLen = 46 - 23;
		float prog = (float)rangePos / (float)rangeLen;

		r = 72;
		g = 209 + (int)(80 * prog);
		b = 204;
		//MEDIUM TURQUOISE
	}
	else if (count >= 47 && count <= 70)
	{
		size_t rangePos = count - 47;
		size_t rangeLen = 70 - 47;
		float prog = (float)rangePos / (float)rangeLen;

		r = 0;
		g = 255;
		b = 127 + (int)(80 * prog);
		//SPRING GREEN
	}
	else if (count >= 71 && count <= 94)
	{
		size_t rangePos = count - 71;
		size_t rangeLen = 94 - 71;
		float prog = (float)rangePos / (float)rangeLen;

		r = 255 + (int)(80 * prog);
		g = 255 + (int)(80 * prog);
		b = 0;
		//YELLOW
	}
	else if (count >= 95 && count <= 127)
	{
		size_t rangePos = count - 95;
		size_t rangeLen = 127 - 95;
		float prog = (float)rangePos / (float)rangeLen;
		r = 220;
		g = 20 + (int)(80 * prog);
		b = 60 + (int)(80 * prog);
		//CRIMSON
	}

	/*
	*
	You can create a color "sliding" effect to differentiate more colors by adding or subtracting
	the iteration count to one color within a region
	You can experiment with HSL color mapping to see which values to assign for each region
	Set S to 100% and L to 50% and slide the H (see Canvas for "Color Mapping" Link)
	*
	*/
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
	float ax = 0, bx = m_pixel_size.x;	//X Range
	float ay = m_pixel_size.y, by = 0;	//Y range
	float nx = mousePixel.x, ny = mousePixel.y;
	/*
		Assuming your monitor is something like 1080p, the default range of the display pixels are:
		x => [0, 1920]
		y => [1080, 0]

		The general formula to map a value n from range [a,b] into range [c,d] is
		((n  - a) / (b - a)) * (d - c) + c

		The magnitude (d - c) is always equal to either m_plane_size.x or m_plane_size.y,
		depending on which direction you are calculating

		The offset of +c is always equal to either (m_plane_center.x - m_plane_size.x / 2.0) or
		(m_plane_center.y - m_plane_size.y / 2.0)
	*
	*/
	Vector2f val;

	val.x = ((nx - ax) / (bx - ax)) * (m_plane_size.x) + (m_plane_center.x - m_plane_size.x / 2.0);
	val.y = ((ny - ay) / (by - ay)) * (m_plane_size.y) + (m_plane_center.y - m_plane_size.y / 2.0);

	return val;
}
