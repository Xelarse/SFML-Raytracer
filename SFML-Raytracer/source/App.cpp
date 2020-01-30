#include "..\include\App.h"
#include <iostream>

//https://raytracing.github.io/books/RayTracingInOneWeekend.html up to antialisaing
//https://github.com/RayTracing/raytracing.github.io

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
    _world = std::make_unique<Hittables>();
    _renderTexture = std::make_unique<sf::Texture>();

    _renderTarget = sf::RectangleShape(sf::Vector2f(_width, _height));

    //Add a couple of spheres to the world
    _world->AddHittable(std::make_unique<Sphere>(
            AA::Vec3(0, 0, -1), 0.8, sf::Color(0, 0, 0, 255)
        )
    );

    _world->AddHittable(std::make_unique<Sphere>(
        AA::Vec3(0, _height + 0.5, -1), _height, sf::Color(0, 0, 0, 255)
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
    sf::Color pixelColour = sf::Color::Black;

    if (_antiAliasing)
    {
        GetColourAntiAliasing(x, y, pixelColour);
    }
    else
    {
        GetColour(x, y, pixelColour);
    }
    _pixelColourBuffer->ColourPixelAtPosition(x, y, pixelColour);
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

void App::GetColour(const int& x, const int& y, sf::Color& colOut)
{
    AA::Ray pixelRay(_cam->Origin(), _cam->GetDir(x, y, _width, _height));

    Hittable::HitResult res;

    if (_world->IntersectedRay(pixelRay, 0.0, 20000.0, res))
    {
        //Colour the pixel
        AA::Vec3 norm = res.normal;
        norm += 1;
        norm *= 0.5;
        colOut = norm.Vec3ToCol();
    }
    else
    {
        colOut = BackgroundGradientCol(pixelRay).Vec3ToCol();
    }
}

void App::GetColourAntiAliasing(const int& x, const int& y, sf::Color& colOut)
{
    AA::Vec3 tempColValues = AA::Vec3(0,0,0);
    int pixelIterations = 100;
    Hittable::HitResult res;

    //Iterate with slight variance around the set X and Y position of the ray, get the colour data and add it to the temp
    for (size_t i = 0; i < pixelIterations; i++)
    {
        double xVarience = AA::RanDouble();
        double yVarience = AA::RanDouble();
        AA::Ray pixelRay(_cam->Origin(), _cam->GetDir(x + xVarience, y + yVarience, _width, _height));

        if (_world->IntersectedRay(pixelRay, 0.0, 20000.0, res))
        {
            //Colour the pixel
            AA::Vec3 norm = res.normal;
            norm += 1;
            norm *= 0.5;
            tempColValues += norm;
        }
        else
        {
            tempColValues += BackgroundGradientCol(pixelRay);
        }
    }

    tempColValues /= static_cast<double>(pixelIterations);
    colOut = tempColValues.Vec3ToCol();
}

AA::Vec3 App::BackgroundGradientCol(const AA::Ray& ray)
{
    AA::Vec3 top = AA::Vec3(0.0, 0.2, 1.0);
    AA::Vec3 bottom = AA::Vec3(1.0, 1.0, 1.0);

    AA::Vec3 unitDir = ray._dir.UnitVector();
    float t = 0.5 * (unitDir._y + 1.0);

    return AA::LinearLerp(top, bottom, t);
}
