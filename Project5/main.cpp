#include <SFML/Graphics.hpp>
#include <math.h>
#include "Vectors.h"
#include "buttonInput.h"

#define WINDOWX 800
#define WINDOWY 800

#define XY(v) (v).x,(v).y

#define getrandom(min, max) ((rand()%(int)(((max) + 1) - (min)))+ (min))


class Object;
typedef std::vector<Object> VectorArray;
class Object {
public:
	sf::CircleShape rect;
	bool initalized = false;

	float radius = 6.f;
	double mass = 100000;
	double G = 0.000001;
	Vector2 position = Vector2((float)getrandom(0, WINDOWX), (float)getrandom(0, WINDOWY));
	Vector2 velocity = Vector2(0.f, 0.f);

	Object() {
		if (!initalized)
			rect.setRadius(radius);
	}
	void Init(double nmass, float nradius) {
		mass = nmass;
		radius = nradius;
		rect.setOrigin(radius, radius);
		rect.setRadius(radius);
	}

	void CalculateObject(float dt, VectorArray &obArray) {

		// To add
		// Collision thats hard enough fuses two objects together into a larger one.

		for (int i = 0; i < obArray.size(); i++)
		{
			if (&obArray[i] == this)
				continue;

			Object &obj = obArray[i];

			double d = position.distanceTo(obj.position);

			double accel = (G * obj.mass) / (obj.radius * obj.radius);
			Vector2 dir = obj.position - position;
			dir.normalized();

			if (((d - radius) - obj.radius) <= 0) {
				Vector2 v1 = (velocity * (mass - obj.mass) + (obj.velocity * (obj.mass * 2))) / (mass + obj.mass);
				Vector2 v2 = (obj.velocity * (obj.mass - mass) + (velocity * (mass * 2))) / (mass + obj.mass);

				rect.setFillColor(sf::Color::Red);
				obj.rect.setFillColor(sf::Color::Red);

				velocity = v1;
				obj.velocity = v2;
			}
			else {
				velocity = velocity + (dir * accel);

			} 
		}
		position = position + (velocity * dt);
	}
	void addForce(Vector2 vector) {
		velocity = velocity + vector;
	}
};

class Camera {
public:
	Vector2 position = Vector2(0.f, 0.f);
	float z = 1.f;
};

void UpdateObjects(VectorArray &obArray, float deltatime) {
	for (int i = 0; i < obArray.size(); i++) {
		obArray[i].CalculateObject(deltatime, obArray);
	}
}

void RenderObjects(VectorArray &obArray, sf::RenderWindow &window, Camera c) {
	for (int i = 0; i < obArray.size(); i++) {
		Object &obj = obArray[i];
		Vector2 newpos = obj.position + c.position;

		obj.rect.setPosition(newpos.asSF());
		window.draw(obj.rect);
	}
}


int main() {
	sf::RenderWindow window(sf::VideoMode(WINDOWX, WINDOWY), "Project5 Gravity", sf::Style::Close);
	sf::Clock clock;

	Camera mycamera;

	VectorArray objectArray;
	objectArray.resize(25);  // Start with 25 objects

	//Mouse 
	newPressMouse leftclick = newPressMouse(sf::Mouse::Left); // Place

	// Keyboard
	//newPressKey leftkey = newPressKey(sf::Keyboard::Left);
	newPressKey spacekey = newPressKey(sf::Keyboard::Space); // Start sim
	newToggleKey buildkey = newToggleKey(sf::Keyboard::B); // Build mode
	newPressKey reset = newPressKey(sf::Keyboard::R); // Reset 

	bool startSim = false;

	float scrollradius = 1.f;
	sf::CircleShape buildrect;

	// This is all SFML Garbage
	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseWheelMoved) { // Get scroll change
				scrollradius += (float) event.mouseWheel.delta;
				if (scrollradius < 1.f)
					scrollradius = 1.f;
			}
		}// This is all SFML Garbage

		clock.restart();
		window.clear(sf::Color::Black);

		float deltatime = clock.getElapsedTime().asSeconds();  // Our Delta time.

		/*
		if (leftkey.isPressed()) { // Add a Force to a random object
			int ran = getrandom(0, objectArray.size() - 1);
			objectArray[ran].addForce(Vector2(15000.f, 100.f));
			objectArray[ran].rect.setFillColor(sf::Color::Blue);
		}*/
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
			mycamera.position = mycamera.position + Vector2(50000.f, 0.f) * deltatime;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
			mycamera.position = mycamera.position + Vector2(0.f, 50000.f) * deltatime;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
			mycamera.position = mycamera.position + Vector2(0.f, -50000.f) * deltatime;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
			mycamera.position = mycamera.position + Vector2(-50000.f, 0.f) * deltatime;
		}

		if (buildkey.isToggled()) {  // Place mode
			buildrect.setRadius(scrollradius);
			buildrect.setPosition(sf::Vector2f(window.mapPixelToCoords(sf::Mouse::getPosition(window))));
			buildrect.setOrigin(scrollradius, scrollradius);
			window.draw(buildrect);

			
			if (leftclick.isPressed()) { // Create new ball on click
				objectArray.resize(objectArray.size() + 1);
				sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				Object &obj = objectArray[objectArray.size() - 1];  // This object will have collision problems...
				
				obj.radius = scrollradius;
				obj.rect.setRadius(scrollradius);

				obj.position = asVector2(worldPos - mycamera.position.asSF());
				obj.mass = 10000 * pow(scrollradius, 3);

				//obj.rect.setPosition(obj.position.asSF());
				obj.rect.setOrigin(scrollradius, scrollradius);

			}
		}

		if (startSim)
			UpdateObjects(objectArray, deltatime); 
		if (spacekey.isPressed()) //Basically press space to start the sim.
			startSim = true;

		if (reset.isPressed()) {
			startSim = false;
			objectArray.resize(0);
		}

		RenderObjects(objectArray, window, mycamera);
		window.display();
	}
	return 0;
}