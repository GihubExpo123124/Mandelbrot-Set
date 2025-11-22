#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <string>
using namespace sf;
using namespace std;

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

enum class State {
	CALCULATING,DISPLAYING
};

class ComplexPlane : public Drawable {
	VertexArray m_vArray;
	State m_State{};
	Vector2f m_mouseLocation, m_plane_center, m_plane_size;
	Vector2i m_pixel_size;
	int m_zoomcount{};
	float m_aspectRatio;

	public:
		ComplexPlane(int pixelWidth, int pixelHeight);
		void zoomIn();
		void zoomOut();
		void draw(RenderTarget& target, RenderStates states) const;
		void updateRender();
		void setCenter(Vector2i mousePixel);
		void setMouseLocation(Vector2i mousePixel);
		void loadText(Text& text);
		size_t countIterations(Vector2f coord);
		void iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b);
		Vector2f mapPixelToCoords(Vector2i mousePixel);

};