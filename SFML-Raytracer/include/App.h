#pragma once
#include <SFML/Graphics.hpp>
#include "EventHandler.h"
#include "Utilities.h"
#include "Camera.h"
#include "Sphere.h"
#include "Box.h"
#include "Hittables.h"
#include "Mesh.h"

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

	sf::Color CalculatePixel(const double& u, const double& v);
	void UpdateRenderTexture();
	void CreateImage();
	void GetColour(const double& u, const double& v, sf::Color& colOut);
	void GetColourAntiAliasing(const double& u, const double& v, sf::Color& colOut);

	AA::Vec3 BackgroundGradientCol(const AA::Ray& ray);


	int _width = 800;
	int _height = 600;
	bool _antiAliasing = false;
	
	double _cameraXBound = 10.0;
	double _cameraPanSpeed = 2.0;

	std::unique_ptr<sf::RenderWindow> _pWindow;
	std::unique_ptr<EventHandler> _pEventHander;
	std::unique_ptr<sf::Clock> _pAppClock;

	Box* _testBox = nullptr;

	sf::RectangleShape _renderTarget;
	std::unique_ptr<sf::Texture> _renderTexture;

	std::unique_ptr<AA::ColourArray> _pixelColourBuffer;
	std::unique_ptr<Camera> _cam;
	std::unique_ptr<Hittables> _world;
};

