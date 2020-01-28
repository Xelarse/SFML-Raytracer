#include "..\include\App.h"
#include <iostream>

App::App()
{
}

App::~App()
{
}

void App::Run()
{
    _pWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(_width, _height), "SFML-Raytracer");
    _pEventHander = std::make_unique<EventHandler>();
    _pAppClock = std::make_unique<sf::Clock>();
    _pixelColourBuffer = std::make_unique<AA::ColourArray>(_width, _height);
    _cam = std::make_unique<Camera>(_width, _height);
    _renderTexture = std::make_unique<sf::Texture>();

    _renderTarget = sf::RectangleShape(sf::Vector2f(_width, _height));

    //Add a sphere to the vector
    _hittables.emplace_back(std::make_unique<Sphere>(
            AA::Vec3(0, 0, -1), 0.99, sf::Color(0, 0, 0, 255)
        )
    );

    while (_pWindow->isOpen())
    {
        float deltaTimeMs = _pAppClock->restart().asMilliseconds();
        Tick(deltaTimeMs);
        std::cout << "FPS: " << floor(1000 / deltaTimeMs) << std::endl;
    }

}

void App::Tick(float dt)
{
    //Process window events
    _pEventHander->ProcessEvents(_pWindow.get());

    //Update App logic
    Update(dt);

    //Draw to screen
    Draw();
}

void App::Update(float dt)
{
    //if (_pEventHander->IsKeyPressed(sf::Keyboard::W))
    //{
    //    _spheres.front()._origin._z += 10;
    //}
    //else if (_pEventHander->IsKeyPressed(sf::Keyboard::S))
    //{
    //    _spheres.front()._origin._z -= 10;
    //}
    //else if (_pEventHander->IsKeyPressed(sf::Keyboard::A))
    //{
    //    _spheres.front()._radius += 10;
    //}
    //else if (_pEventHander->IsKeyPressed(sf::Keyboard::D))
    //{
    //    _spheres.front()._radius -= 10;
    //}

    CreateImage();
}

void App::Draw()
{
    //Clear previous screen
    _pWindow->clear();

    //Draw the render target to screen
    _pWindow->draw(_renderTarget);

    //present
    _pWindow->display();
}

void App::CalculatePixel(int x, int y)
{
    //Use the camera to translate the x and y to viewport and draw a ray from it
    AA::Ray pixelRay(_cam->Origin(), _cam->GetDir(x, y, _width, _height));
    sf::Color pixelColour = BackgroundGradientCol(pixelRay);

    //TODO change later to loop over all spheres to check against, for now dirty just one
    if (_hittables.back()->IntersectedRay(pixelRay))
    {
        //Colour the pixel
        pixelColour = _hittables.back()->Colour();
    }

    _pixelColourBuffer->ColourPixelAtPosition(x, y, pixelColour);

    //Fire another ray based on the angle of incidence to see where it bounces next
}

void App::UpdateRenderTexture()
{
    //Create an image that uses the raytraced pixel data
    sf::Image renderData;
    renderData.create(_width, _height, reinterpret_cast<sf::Uint8*>(_pixelColourBuffer->GetDataBasePointer()));

    //renderData.saveToFile("C:\\Users\\Xelarse\\Desktop\\test.png");

    //TODO potentially update instead of loading every tick for better performance
    if (_renderTexture->loadFromImage(renderData))
    {
        _renderTarget.setTexture(_renderTexture.get());
    }
}

void App::CreateImage()
{
    //Draw a ray for each pixel, store the resultant colour
    for (int x = 0; x < _width; x++)
    {
        for (int y = 0; y < _height; y++)
        {
            CalculatePixel(x, y);
        }
    }

    //Write to the texture to display
    UpdateRenderTexture();
}

sf::Color App::BackgroundGradientCol(const AA::Ray& ray)
{
    sf::Color top = sf::Color::Blue;
    sf::Color bottom = sf::Color::White;

    AA::Vec3 unitDir = ray._dir.UnitVector();
    float t = 0.5 * (unitDir._y + 1.0);

    return AA::LinearLerp(top, bottom, t);
}
