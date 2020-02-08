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
    //SFML related inits
    _pWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(_width, _height), "SFML-Raytracer");
    _pEventHander = std::make_unique<EventHandler>();
    _pAppClock = std::make_unique<sf::Clock>();

    //Raytracer related inits
    _pixelColourBuffer = std::make_unique<AA::ColourArray>(_width, _height);
    _world = std::make_unique<Hittables>();
    _renderTexture = std::make_unique<sf::Texture>();

    AA::Vec3 lookFrom = AA::Vec3(0, -3, 5);
    AA::Vec3 lookAt = AA::Vec3(0, -0.5, 0);
    double vFov = 70;
    _cam = std::make_unique<Camera>(lookFrom, lookAt, AA::Vec3(0,1,0), vFov, (_width / _height));



    _renderTarget = sf::RectangleShape(sf::Vector2f(_width, _height));

    //Add a couple of spheres to the world
    _world->AddHittable(std::make_unique<Sphere>(
            AA::Vec3(0, -0.5, -1), 0.8, sf::Color(0, 0, 0, 255)
        )
    );

    _world->AddHittable(std::make_unique<Sphere>(
        AA::Vec3(0, _height + 0.5, -1), _height, sf::Color(0, 0, 0, 255)
        )
    );

    _world->AddHittable(std::make_unique<Mesh>(
        //"D:\\Alex\\Documents\\ProjectsAndWork\\ThirdYear\\SFML-Raytracer\\SFML-Raytracer\\assets\\cube.obj",
        AA::Vec3(-1, 0, -0.2)
        )
    );

    auto box = std::make_unique<Box>(
        AA::Vec3(2, -0.5, -0.5), 1, 1, 2, sf::Color(0, 0, 0, 255)
        );

    //TODO remove this pointer later, just for moving cube independantly from the world hittables
    _testBox = box.get();
    _world->AddHittable(std::move(box));

    //TODO remove later, model loading test
    //Mesh msh = Mesh("D:\\Alex\\Documents\\ProjectsAndWork\\ThirdYear\\SFML-Raytracer\\SFML-Raytracer\\assets\\KennyPirate\\ship_light.obj");

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
        AA::Vec3 previous = _testBox->GetPosition();
        previous._z -= 0.25;
        previous._z = previous._z < -3.0 ? -3.0 : previous._z;
        _testBox->MoveBox(previous);
    }
    else if (_pEventHander->IsKeyPressed(sf::Keyboard::S))
    {
        AA::Vec3 previous = _testBox->GetPosition();
        previous._z += 0.25;
        previous._z = previous._z > 3 ? 3 : previous._z;
        _testBox->MoveBox(previous);
    }
    if (_pEventHander->IsKeyPressed(sf::Keyboard::A))
    {
        AA::Vec3 previous = _testBox->GetPosition();
        previous._x -= 0.25;
        previous._x = previous._x < -4.0 ? -4.0 : previous._x;
        _testBox->MoveBox(previous);
    }
    else if (_pEventHander->IsKeyPressed(sf::Keyboard::D))
    {
        AA::Vec3 previous = _testBox->GetPosition();
        previous._x += 0.25;
        previous._x = previous._x > 4.0 ? 4.0 : previous._x;
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
        newPos._x += (dt / 1000) * _cameraPanSpeed;
        //Check if it being left is outside the current set bounds
        if (newPos._x > _cameraXBound)
        {
            newPos._x = _cameraXBound;
            _camLeft = !_camLeft;
        }
        _cam->SetLookFrom(newPos);
    }
    else
    {
        //Take the current camera position
        AA::Vec3 newPos = _cam->GetLookFrom();
        //Add to it to make it go left
        newPos._x -= (dt / 1000) *_cameraPanSpeed;
        //Check if it being left is outside the current set bounds
        if (newPos._x < -_cameraXBound)
        {
            newPos._x = -_cameraXBound;
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

    if (_world->IntersectedRay(ray, 0.0, 20000.0, res))
    {
        //Colour the pixel
        AA::Vec3 norm = res.normal;
        norm += 1;
        norm *= 0.5;
        colOut = norm.Vec3ToCol();
    }
    else
    {
        colOut = BackgroundGradientCol(ray).Vec3ToCol();
    }
}

void App::GetColourAntiAliasing(const double& u, const double& v, sf::Color& colOut)
{
    AA::Vec3 tempColValues = AA::Vec3(0,0,0);
    int pixelIterations = 10;
    Hittable::HitResult res;

    //Iterate with slight variance around the set X and Y position of the ray, get the colour data and add it to the temp
    for (size_t i = 0; i < pixelIterations; i++)
    {
        double rayU = ((u * _width) + AA::RanDouble()) / _width;
        double rayV = ((v * _height) + AA::RanDouble()) / _height;
        AA::Ray ray = _cam->GetRay(rayU, rayV);

        if (_world->IntersectedRay(ray, 0.0, 20000.0, res))
        {
            //Colour the pixel
            AA::Vec3 norm = res.normal;
            norm += 1;
            norm *= 0.5;
            tempColValues += norm;
        }
        else
        {
            tempColValues += BackgroundGradientCol(ray);
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
