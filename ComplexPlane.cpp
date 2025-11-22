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
ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) : m_pixel_size(pixelWidth,pixelHeight),
															  m_aspectRatio((pixelHeight / 1.f) / pixelWidth),
															  m_plane_size(BASE_WIDTH, BASE_HEIGHT * m_aspectRatio),
															   m_vArray(Points, pixelWidth * pixelHeight){}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const {
	target.draw(m_vArray);
}

void ComplexPlane::zoomIn() {
	++m_zoomcount;
	float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomcount);
	float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomcount);

	m_plane_size.x = xSize;
	m_plane_size.y = ySize;
	m_State = State::CALCULATING;
}

void ComplexPlane::zoomOut(){
	--m_zoomcount;
	float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomcount);
	float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomcount);

	m_plane_size.x = xSize;
	m_plane_size.y = ySize;
	m_State = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel) {
	m_plane_center = mapPixelToCoords(mousePixel);

}

void ComplexPlane::loadText(Text& text) {
	istringstream iis;
}