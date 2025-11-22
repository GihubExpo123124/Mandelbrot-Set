#include "ComplexPlane.h"

int main() {
	RenderWindow window(VideoMode(50, 80), "Mandelbrot test");
	ComplexPlane plane(50, 80);

	while (window.isOpen()) {
		Event event;

		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}

			if (event.type == Event::MouseButtonPressed) {
				if (event.mouseButton.button == Mouse::Button::Left) {
					plane.zoomIn();
					plane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
				}
				else if (event.mouseButton.button == Mouse::Right) {
					plane.zoomOut();
					plane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
				}
			}

			if (event.type == Event::MouseMoved) {
				plane.setMouseLocation(Vector2i(event.mouseMove.x, event.mouseMove.y));

			}

			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				window.close();
			}
		}

		window.clear();
		window.draw(plane);
		window.display();
	}

	return 0;
}