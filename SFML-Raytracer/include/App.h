#pragma once
#include <SFML/Graphics.hpp>
#include <atomic>
#include <mutex>
#include "EventHandler.h"
#include "Utilities.h"
#include "Camera.h"
#include "Sphere.h"
#include "Box.h"
#include "Hittables.h"
#include "Mesh.h"
#include "BvhNode.h"
#include "JobManager.h"

class App
{

public:
	App();
	~App();
	
	void Run();

private:

	void InitCoreSystems();
	void InitScene();

	//dt in milliseconds
	void Tick(float dt);

	void Update(float dt);
	void Draw();

	sf::Color CalculatePixel(const double& u, const double& v);
	void UpdateRenderTexture();
	void CreateImage();
	void CreateImageSegment();
	void GetColour(const double& u, const double& v, sf::Color& colOut);
	void GetColourAntiAliasing(const double& u, const double& v, sf::Color& colOut);

	AA::Vec3 BackgroundGradientCol(const AA::Ray& ray);

	//SFML Stuff
	const int _width = 800;
	const int _height = 600;
	const int _totalPixels;
	std::unique_ptr<sf::RenderWindow> _pWindow;
	std::unique_ptr<EventHandler> _pEventHander;
	std::unique_ptr<sf::Clock> _pAppClock;
	std::unique_ptr<sf::Texture> _renderTexture;
	sf::RectangleShape _renderTarget;


	//Ray Stuff
	bool _antiAliasing = false;
	int _perPixelAA = 10;

	bool _useBvh = true;

	double _cameraXBound = 8.0;
	double _cameraPanSpeed = 4.0;
	bool _camLeft = true;


	Box* _testBox = nullptr;
	std::unique_ptr<AA::ColourArray> _pixelColourBuffer;
	std::unique_ptr<Camera> _cam;
	std::unique_ptr<Hittables> _staticHittables;
	std::unique_ptr<Hittables> _dynamicHittables;

	//Job system stuff
	const bool _isThreaded = true;
	std::mutex _divisionMutex;
	int _currentDivision;
	const int _totalThreads = 30;	//MUST BE A CLEAN DIVISION OF _width * _height //30 max for laptop 160 for desktop
	const int _calcsPerDivision;
	std::unique_ptr<JobManager> _jobManager;
};

