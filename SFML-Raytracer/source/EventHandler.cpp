#include "..\include\EventHandler.h"

EventHandler::EventHandler()
{
}

EventHandler::~EventHandler()
{
}

void EventHandler::ProcessEvents(sf::RenderWindow* window)
{
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window->close();
        }
    }
}

bool EventHandler::IsKeyPressed(sf::Keyboard::Key keycode)
{
    return sf::Keyboard::isKeyPressed(keycode);
}

bool EventHandler::IsMousePressed(sf::Mouse::Button button)
{
    return sf::Mouse::isButtonPressed(button);
}
