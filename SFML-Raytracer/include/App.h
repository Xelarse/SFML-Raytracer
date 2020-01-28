#pragma once
#include <SFML/Graphics.hpp>
#include "EventHandler.h"
#include "Utilities.h"
#include "Camera.h"
#include "Sphere.h"

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

	void CalculatePixel(int x, int y);
	void UpdateRenderTexture();
	void CreateImage();

	sf::Color BackgroundGradientCol(const AA::Ray& ray);


	int _width = 800;
	int _height = 600;

	std::unique_ptr<sf::RenderWindow> _pWindow;
	std::unique_ptr<EventHandler> _pEventHander;
	std::unique_ptr<sf::Clock> _pAppClock;

	sf::RectangleShape _renderTarget;
	std::unique_ptr<sf::Texture> _renderTexture;

	std::unique_ptr<AA::ColourArray> _pixelColourBuffer;
	std::unique_ptr<Camera> _cam;
	std::vector<std::unique_ptr<Hittable>> _hittables;
};

