#include "..\include\App.h"

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

    //Add a sphere to the vector
    _spheres.emplace_back(
        AA::Sphere(
            AA::Vec(_width / 2, _height / 2, 100), 
            20, 
            AA::Colour(255, 255, 255)
        )
    );

    _testCircle = sf::CircleShape(100.f);
    _testCircle.setFillColor(sf::Color::Green);

    while (_pWindow->isOpen())
    {
        float deltaTimeMs = _pAppClock->restart().asMilliseconds();
        Tick(deltaTimeMs);
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
    if (_pEventHander->IsKeyPressed(sf::Keyboard::Key::W))
    {
        _testCircle.move(sf::Vector2f(0, -1));
    }

    if (_pEventHander->IsKeyPressed(sf::Keyboard::Key::S))
    {
        _testCircle.move(sf::Vector2f(0, 1));
    }

    if (_pEventHander->IsKeyPressed(sf::Keyboard::Key::A))
    {
        _testCircle.move(sf::Vector2f(-1, 0));
    }

    if (_pEventHander->IsKeyPressed(sf::Keyboard::Key::D))
    {
        _testCircle.move(sf::Vector2f(1, 0));
    }
}

void App::Draw()
{
    //Clear previous screen
    _pWindow->clear();

    //Draw the drawables
    _pWindow->draw(_testCircle);

    //present
    _pWindow->display();
}

void App::CalculatePixel(int x, int y)
{
    //Create a ray that originates from the pixel position and goes forwards in the Z
    AA::Ray pixelRay(AA::Vec(x, y, 0), AA::Vec(0, 0, 1));

    //Check for an intersection if it finds one,
    double t = 20000;   //Upper limit for the check?

    //TODO change later to loop over all spheres to check against, for now dirty just one
    if (AA::RayIntersectSphere(pixelRay, _spheres.back(), t))
    {
        //Colour the pixel
        _pixelColourBuffer->ColourPixelAtPosition(x, y, AA::Colour(255, 255, 255));
    }

    //Fire another ray based on the angle of incidence to see where it bounces next
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
}
