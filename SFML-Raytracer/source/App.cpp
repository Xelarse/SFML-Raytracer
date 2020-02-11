#include "..\include\App.h"
#include <iostream>
#include <random>

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
    InitCoreSystems();
    InitScene();

    while (_pWindow->isOpen())
    {
        float deltaTimeMs = _pAppClock->restart().asMilliseconds();
        Tick(deltaTimeMs);
        std::cout << "FPS: " << floor(1000 / deltaTimeMs) << std::endl;
    }

}

void App::InitCoreSystems()
{
    //SFML related inits
    _pWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(_width, _height), "SFML-Raytracer");
    _pEventHander = std::make_unique<EventHandler>();
    _pAppClock = std::make_unique<sf::Clock>();
    sf::View view = _pWindow->getView();
    view.setSize(_width, -_height);
    _pWindow->setView(view);
    _renderTexture = std::make_unique<sf::Texture>();
    _renderTarget = sf::RectangleShape(sf::Vector2f(_width, _height));

    //Raytracer related inits
    _pixelColourBuffer = std::make_unique<AA::ColourArray>(_width, _height);
    _world = std::make_unique<Hittables>();
    

    AA::Vec3 lookFrom = AA::Vec3(0, 3, 5);
    AA::Vec3 lookAt = AA::Vec3(0, 0.5, 0);
    double vFov = 70;
    _cam = std::make_unique<Camera>(lookFrom, lookAt, AA::Vec3(0, 1, 0), vFov, (_width / _height));
}

void App::InitScene()
{
    //Add a static sphere with no specific colour and one with a colour for backdrop
    _world->_hittableObjects.push_back(new Sphere(AA::Vec3(0, 0.5, -1), 0.8, true, sf::Color(0, 0, 0, 255), false));
    _world->_hittableObjects.push_back(new Sphere(AA::Vec3(0, -static_cast<double>(_height) - 1, -1), _height, true, sf::Color(102, 0, 102, 255), true));

    //Add a box that can be moved with WASD, TODO potentially remove later
    _world->_hittableObjects.push_back(new Box(AA::Vec3(2, 0.5, -0.5), AA::Vec3(1, 1, 2), false, sf::Color(0, 0, 0, 255), false));
    _testBox = dynamic_cast<Box*>(_world->_hittableObjects.back());



    //Add a bunch of spheres using random dist
    //std::random_device rd;
    //std::mt19937 gen(rd());
    //std::uniform_real_distribution<double> xDist(-5.0, 5.0);
    //std::uniform_real_distribution<double> yDist(0.0, 5.0);
    //std::uniform_real_distribution<double> zDist(-12.0, -5.0);
    //std::uniform_real_distribution<double> rad(0.1, 0.8);

    //for (int i = 0; i < 50; ++i)
    //{
    //    _world->_hittableObjects.emplace_back(new Sphere(AA::Vec3(xDist(gen), yDist(gen), zDist(gen)), rad(gen), sf::Color(0, 0, 0, 255)));
    //}

    //_world->_hittableObjects.push_back(new Mesh(
    //    "assets/cube.obj",
    //    //"assets/KennyPirate/pirate_captain.obj",
    //    AA::Vec3(-2, 0.5, 0),
    //    AA::Vec3(0.3, 0.3, 0.3)
    //    )
    //);

    //Calculate the scene bvh
    if (_bvhEnabled)
    {
        _sceneBvh = std::make_unique<BvhNode>(_world->_hittableObjects, 0, 0);
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
        AA::Vec3 previous = _testBox->GetPosition();
        previous[2] -= 0.25;
        previous[2] = previous.Z() < -3.0 ? -3.0 : previous.Z();
        _testBox->MoveBox(previous);
    }
    else if (_pEventHander->IsKeyPressed(sf::Keyboard::S))
    {
        AA::Vec3 previous = _testBox->GetPosition();
        previous[2] += 0.25;
        previous[2] = previous.Z() > 3 ? 3 : previous.Z();
        _testBox->MoveBox(previous);
    }
    if (_pEventHander->IsKeyPressed(sf::Keyboard::A))
    {
        AA::Vec3 previous = _testBox->GetPosition();
        previous[0] -= 0.25;
        previous[0] = previous.X() < -4.0 ? -4.0 : previous.X();
        _testBox->MoveBox(previous);
    }
    else if (_pEventHander->IsKeyPressed(sf::Keyboard::D))
    {
        AA::Vec3 previous = _testBox->GetPosition();
        previous[0] += 0.25;
        previous[0] = previous.X() > 4.0 ? 4.0 : previous.X();
        _testBox->MoveBox(previous);
    }

    if (_pEventHander->IsKeyPressed(sf::Keyboard::Up))
    {
        double previous = _cam->GetVFov();
        previous += 2;
        previous = previous > 90 ? 90 : previous;
        _cam->SetVFov(previous);
    }
    else if (_pEventHander->IsKeyPressed(sf::Keyboard::Down))
    {
        double previous = _cam->GetVFov();
        previous -= 2;
        previous = previous < 20 ? 20 : previous;
        _cam->SetVFov(previous);
    }

    if (_camLeft)
    {
        //Take the current camera position
        AA::Vec3 newPos = _cam->GetLookFrom();
        //Add to it to make it go left
        newPos[0] += (dt / 1000) * _cameraPanSpeed;
        //Check if it being left is outside the current set bounds
        if (newPos.X() > _cameraXBound)
        {
            newPos[0] = _cameraXBound;
            _camLeft = !_camLeft;
        }
        _cam->SetLookFrom(newPos);
    }
    else
    {
        //Take the current camera position
        AA::Vec3 newPos = _cam->GetLookFrom();
        //Add to it to make it go left
        newPos[0] -= (dt / 1000) *_cameraPanSpeed;
        //Check if it being left is outside the current set bounds
        if (newPos.X() < -_cameraXBound)
        {
            newPos[0] = -_cameraXBound;
            _camLeft = !_camLeft;
        }
        _cam->SetLookFrom(newPos);
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

sf::Color App::CalculatePixel(const double& u, const double& v)
{
    sf::Color pixelColour = sf::Color::Black;

    if (_antiAliasing)
    {
        GetColourAntiAliasing(u, v, pixelColour);
    }
    else
    {
        GetColour(u, v, pixelColour);
    }

    return pixelColour;
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
            double u = double(x / double(_width));
            double v = double(y / double(_height));
            _pixelColourBuffer->ColourPixelAtPosition(x, y, CalculatePixel(u, v));
        }
    }

    //Write to the texture to display
    UpdateRenderTexture();
}

void App::GetColour(const double& u, const double& v, sf::Color& colOut)
{
    Hittable::HitResult res;
    AA::Ray ray = _cam->GetRay(u, v);

    if (_bvhEnabled && _sceneBvh->IntersectedRay(ray, 0.0, 20000.0, res))
    {
        colOut = res.col;
    }
    else if (_world->IntersectedRay(ray, 0.0, 20000.0, res))
    {
        colOut = res.col;
    }
    else
    {
        colOut = BackgroundGradientCol(ray).Vec3ToCol();
    }
}

void App::GetColourAntiAliasing(const double& u, const double& v, sf::Color& colOut)
{
    AA::Vec3 tempColValues = AA::Vec3(0,0,0);
    Hittable::HitResult res;

    //Iterate with slight variance around the set X and Y position of the ray, get the colour data and add it to the temp
    for (size_t i = 0; i < _perPixelAA; i++)
    {
        double rayU = ((u * _width) + AA::RanDouble()) / _width;
        double rayV = ((v * _height) + AA::RanDouble()) / _height;
        AA::Ray ray = _cam->GetRay(rayU, rayV);

        if (_world->IntersectedRay(ray, 0.0, 20000.0, res))
        {
            tempColValues += AA::Vec3(res.col.r / 255.0, res.col.g / 255.0, res.col.b / 255.0);
        }
        else
        {
            tempColValues += BackgroundGradientCol(ray);
        }
    }

    tempColValues /= static_cast<double>(_perPixelAA);
    colOut = tempColValues.Vec3ToCol();
}

AA::Vec3 App::BackgroundGradientCol(const AA::Ray& ray)
{
    AA::Vec3 top = AA::Vec3(0.0, 0.2, 1.0);
    AA::Vec3 bottom = AA::Vec3(1.0, 1.0, 1.0);

    AA::Vec3 unitDir = ray._dir.UnitVector();
    float t = 0.5 * (unitDir.Y() + 1.0);

    return AA::LinearLerp(top, bottom, t);
}
