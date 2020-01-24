#pragma once
#include <SFML/Graphics.hpp>
#include "EventHandler.h"

class App
{

public:
	App();
	~App();
	
	void Run();


private:
	//dt in milliseconds
	void Tick(float dt);

	void Update(float dt);
	void Draw();

	std::unique_ptr<sf::RenderWindow> _pWindow;
	std::unique_ptr<EventHandler> _pEventHander;
	std::unique_ptr<sf::Clock> _pAppClock;
	sf::CircleShape _testCircle;
};

