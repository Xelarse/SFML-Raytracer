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
    _pixelColourBuffer = std::make_unique<AA::ColourArray>(_height, _width);
    _renderTexture = std::make_unique<sf::Texture>();

    _renderTarget = sf::RectangleShape(sf::Vector2f(_width, _height));

    //Add a sphere to the vector
    _spheres.emplace_back(
        AA::Sphere(
            AA::Vec3(_width / 2, _height / 2, 10), 
            200, 
            sf::Color(255, 255, 255, 255)
        )
    );

    AA::Vec3 v1 = AA::Vec3(2, 1, 2);
    AA::Vec3 v2 = AA::Vec3(1, 1, 2);
    AA::Vec3 v3 = v1;
    v3 *= 5;

    AA::Ray r1(v1, v2);
    v3 = r1.GetPointAlongRay(10);


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
    if (_pEventHander->IsKeyPressed(sf::Keyboard::W))
    {
        _spheres.front()._origin._z += 10;
    }
    else if (_pEventHander->IsKeyPressed(sf::Keyboard::S))
    {
        _spheres.front()._origin._z -= 10;
    }
    else if (_pEventHander->IsKeyPressed(sf::Keyboard::A))
    {
        _spheres.front()._radius += 10;
    }
    else if (_pEventHander->IsKeyPressed(sf::Keyboard::D))
    {
        _spheres.front()._radius -= 10;
    }

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
    //Create a ray that originates from the pixel position and goes forwards in the Z
    AA::Ray pixelRay(AA::Vec3(x, y, 0), AA::Vec3(0, 0, 1));
    sf::Color pixelColour = sf::Color(0, 0, 0, 255);

    //Check for an intersection if it finds one,
    double t = 20000;   //Upper limit for the check?

    //TODO change later to loop over all spheres to check against, for now dirty just one
    if (AA::RayIntersectSphere(pixelRay, _spheres.back(), t))
    {
        //Colour the pixel
        pixelColour = sf::Color::White;
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
            //Calculate the colour for the pixel, store it in a buffer to write to GPU
            CalculatePixel(x, y);
        }
    }

    //Write to the texture to display
    UpdateRenderTexture();
}
