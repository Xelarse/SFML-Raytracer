#include "..\include\App.h"

App::App()
{
}

App::~App()
{
}

void App::Run()
{
    _pWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(1280, 720), "SFML-Raytracer");
    _pEventHander = std::make_unique<EventHandler>();
    _pAppClock = std::make_unique<sf::Clock>();
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
