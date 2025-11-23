#include "ComplexPlane.h"

int main() {
	RenderWindow window(VideoMode(VideoMode::getDesktopMode().width/3, VideoMode::getDesktopMode().height/3), "Mandelbrot test");
	ComplexPlane plane(VideoMode::getDesktopMode().width / 3, VideoMode::getDesktopMode().height / 3);

	Font font;
	Text text;
	if (!font.loadFromFile("calibri.ttf")) {
		cerr << "Calibri failed to load." << endl;
		return 1;
	}

	text.setFont(font);
	plane.loadText(text);
	text.setCharacterSize(15);
	text.setFillColor(Color::White);
	text.setStyle(Text::Italic | Text::Underlined | Text::Bold);

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

	
		plane.updateRender();
		
		plane.loadText(text);

		window.clear();
		window.draw(plane);
		window.draw(text);
		window.display();
	}

	return 0;
}