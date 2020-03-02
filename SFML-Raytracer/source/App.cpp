#include "..\include\App.h"
#include <iostream>
#include <random>
#include <functional>

#include "Diffuse.h"
#include "Mirror.h"

//https://raytracing.github.io/books/RayTracingInOneWeekend.html up to antialisaing
//https://github.com/RayTracing/raytracing.github.io

App::App() : _calcsPerDivision((_width * _height) / _totalThreads), _totalPixels(_width * _height)
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
        std::cout << "FPS: " << floor(1000 / deltaTimeMs) << ", Frametime in MS: " << deltaTimeMs << std::endl;
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
    _staticHittables = std::make_unique<Hittables>(true, _useBvh, _useSAH);
    _dynamicHittables = std::make_unique<Hittables>(false, _useBvh, _useSAH);

    if (_lightingEnabled)
    {
        //Light values ---- white: (255,255,255,255) --- Yellow: (255,243,71,255) --- Purple: (239,29,242,255) --- Dark teal: (9,97,88,255) --- Gray(123,123,123,255)
        _sceneLight = std::make_unique<PointLight>(_staticHittables.get(), _dynamicHittables.get(), AA::Vec3(0, 3, 0), sf::Color(255, 255, 255, 255), 20.0, _lightingDebug);
        //_sceneLight = std::make_unique<AreaLight>(
        //    _staticHittables.get(),
        //    _dynamicHittables.get(),
        //    AA::Vec3(0.0, 4.0, -6.0),
        //    AA::Vec2(5.0, 5.0),
        //    1,
        //    sf::Color(255, 255, 255, 255),
        //    100.0,
        //    _lightingDebug
        //);
        //_sceneLight = std::make_unique<VolumeLight>(
        //    _staticHittables.get(),
        //    _dynamicHittables.get(),
        //    AA::Vec3(0.0, 4.0, -10.0),
        //    AABB(AA::Vec3(-2.0, -2.0, -2.0), AA::Vec3(2.0, 2.0, 2.0)),
        //    2,
        //    sf::Color(255, 255, 255, 255),
        //    0.8,
        //    _lightingDebug
        //);
    }

    AA::Vec3 lookFrom = AA::Vec3(0, 4, -5);
    AA::Vec3 lookAt = AA::Vec3(0, 1, 0);
    double vFov = 80;
    _cam = std::make_unique<Camera>(lookFrom, lookAt, AA::Vec3(0, 1, 0), vFov, (_width / _height));

    //Job system Inits
    if (_isThreaded)
    {
        _jobManager = std::make_unique<JobManager>(_totalThreads);
    }
}

void App::InitScene()
{
    //SpawnBase();
    //SpawnMovable();
    //SpawnSphereStress();
    //SpawnMeshes();
    //SpawnMeshStress();
    SpawnLightTest();

    //Prompt the hittables to construt their BVH's
    if (_useBvh)
    {
        _staticHittables->ConstructBvh();
        _dynamicHittables->ConstructBvh();
    }
}

void App::SpawnBase()
{
    //Add a static sphere with no specific colour and one with a colour for backdrop
    _staticHittables->_hittableObjects.push_back(new Sphere(AA::Vec3(0, -static_cast<double>(_height) - 1, -1), _height, true, new Material(sf::Color(102, 0, 102, 255), true), _sceneLight.get()));

    _staticHittables->_hittableObjects.push_back(new Sphere(AA::Vec3(0, 0.5, -1), 0.8, true, new Material(sf::Color(0, 0, 0, 255), false), _sceneLight.get()));
}

void App::SpawnSphereStress()
{
    //Add a static sphere with no specific colour and one with a colour for backdrop
    _staticHittables->_hittableObjects.push_back(new Sphere(AA::Vec3(0, -static_cast<double>(_height) - 1, -1), _height, true, new Material(sf::Color(102, 0, 102, 255), true), _sceneLight.get()));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> xDist(-5.0, 5.0);
    std::uniform_real_distribution<double> yDist(0.0, 5.0);
    std::uniform_real_distribution<double> zDist(12.0, 5.0);
    std::uniform_real_distribution<double> rad(0.1, 0.8);

    for (int i = 0; i < 3000; ++i)
    {
        _staticHittables->_hittableObjects.push_back(new Sphere(AA::Vec3(xDist(gen), yDist(gen), zDist(gen)), rad(gen), true, new Material(sf::Color(0, 0, 0, 255), false), _sceneLight.get()));
    }
}

void App::SpawnMeshes()
{
    //Add a static sphere with no specific colour and one with a colour for backdrop
    _staticHittables->_hittableObjects.push_back(new Sphere(AA::Vec3(0, -static_cast<double>(_height) - 1, -1), _height, true, new Material(sf::Color(102, 0, 102, 255), true), _sceneLight.get()));

    ////12 Tri Cube
    //_staticHittables->_hittableObjects.push_back(new Mesh(
    //        "assets/cube.obj",
    //        "assets/cubeHori.tga",
    //        AA::Vec3(0.0, 0.5, 0.0),
    //        AA::Vec3(1.5, 1.5, 1.5),
    //        true,
    //        new Diffuse(sf::Color(255, 0, 0, 255), true),
    //        _useMeshBvh,
    //        _useMeshSAH,
    //        Mesh::ModelParams::DEFAULT,
    //        _sceneLight.get()
    //    )
    //);

    ////104 Tri boat
    //_staticHittables->_hittableObjects.push_back(new Mesh(
    //        "assets/KennyPirate/boat_small.obj",
    //        "NO_TEXTURE",
    //        AA::Vec3(0.0, 0.5, 0.0),
    //        AA::Vec3(0.2, 0.2, 0.2),
    //        true,
    //        new Material(sf::Color(255, 0, 187, 255), true),
    //        _useMeshBvh,
    //        _useMeshSAH,
    //        Mesh::ModelParams::DEFAULT,
    //        _sceneLight.get()
    //    )
    //);

    //194 Tri palm tree
    _staticHittables->_hittableObjects.push_back(new Mesh(
            "assets/KennyPirate/palm_long.obj",
            "NO_TEXTURE",
            AA::Vec3(1.5, 0.5, 0.0),
            AA::Vec3(0.2, 0.2, 0.2),
            true,
            new Diffuse(sf::Color(50, 200, 255, 255), true),
            _useMeshBvh,
            _useMeshSAH,
            Mesh::ModelParams::DEFAULT,
            _sceneLight.get()
        )
    );

    ////428 Tri Pirate Captain
    //_staticHittables->_hittableObjects.push_back(new Mesh(
    //        "assets/KennyPirate/pirate_captain.obj",
    //        "NO_TEXTURE",
    //        AA::Vec3(0.0, 0.5, 0.0),
    //        AA::Vec3(0.2, 0.2, 0.2),
    //        true,
    //        new Material(sf::Color(255, 0, 187, 255), true),
    //        _useMeshBvh,
    //        _useMeshSAH,
    //        Mesh::ModelParams::DEFAULT,
    //        _sceneLight.get()
    //    )
    //);

    ////672 Tri Shibe
    //_staticHittables->_hittableObjects.push_back(new Mesh(
    //        "assets/Shibe/Shibe.obj",
    //        "assets/Shibe/Shibe.png",
    //        AA::Vec3(0.0, 1.0, 0.0),
    //        AA::Vec3(0.4, 0.4, 0.4),
    //        true,
    //        new Material(sf::Color(255, 0, 187, 255), true),
    //        _useMeshBvh,
    //        _useMeshSAH,
    //        Mesh::ModelParams::FLIP_Z,
    //        _sceneLight.get()
    //    )
    //);

    ////716 Tri Plant pot
    //_staticHittables->_hittableObjects.push_back(new Mesh(
    //        "assets/Plantpot/Pot.obj",
    //        "assets/Plantpot/textures/PotCol.jpg",
    //        AA::Vec3(0.0, 0.5, 0.0),
    //        AA::Vec3(4.0, 4.0, 4.0),
    //        true,
    //        new Material(sf::Color(255, 0, 187, 255), true),
    //        _useMeshBvh,
    //        _useMeshSAH,
    //        Mesh::ModelParams::DEFAULT,
    //        _sceneLight.get()
    //    )
    //);
}

void App::SpawnMeshStress()
{
    //Add a static sphere with no specific colour and one with a colour for backdrop
    _staticHittables->_hittableObjects.push_back(new Sphere(AA::Vec3(0, -static_cast<double>(_height) - 1, -1), _height, true, new Material(sf::Color(102, 0, 102, 255), true), _sceneLight.get()));

    //172 Tri Rocks
    _staticHittables->_hittableObjects.push_back(new Mesh(
            "assets/KennyPirate/formation_rock.obj",
            "NO_TEXTURE",
            AA::Vec3(4.0, -1.0, 0.0),
            AA::Vec3(0.175, 0.175, 0.175),
            true,
            new Diffuse(sf::Color(47, 255, 0, 255), true),
            _useMeshBvh,
            _useMeshSAH,
            Mesh::ModelParams::DEFAULT,
            _sceneLight.get()
        )
    );

    //92 Tri bottle
    _staticHittables->_hittableObjects.push_back(new Mesh(
            "assets/KennyPirate/bottleLarge.obj",
            "NO_TEXTURE",
            AA::Vec3(-1.0, -1.0, 0.0),
            AA::Vec3(0.5, 0.5, 0.5),
            true,
            new Diffuse(sf::Color(0, 242, 255, 255), true),
            _useMeshBvh,
            _useMeshSAH,
            Mesh::ModelParams::DEFAULT,
            _sceneLight.get()
        )
    );

    //194 Tri palm tree
    _staticHittables->_hittableObjects.push_back(new Mesh(
            "assets/KennyPirate/palm_long.obj",
            "NO_TEXTURE",
            AA::Vec3(1.75, -1.0, 0.0),
            AA::Vec3(0.2, 0.2, 0.2),
            true,
            new Diffuse(sf::Color(255, 170, 0, 255), true),
            _useMeshBvh,
            _useMeshSAH,
            Mesh::ModelParams::DEFAULT,
            _sceneLight.get()
        )
    );
}

void App::SpawnMovable()
{
    //Add a static sphere with no specific colour and one with a colour for backdrop
    _staticHittables->_hittableObjects.push_back(new Sphere(AA::Vec3(0, -static_cast<double>(_height) - 1, -1), _height, true, new Material(sf::Color(102, 0, 102, 255), true), _sceneLight.get()));

    //Spawns a box that can be controlled with wasd
    _dynamicHittables->_hittableObjects.push_back(new Box(AA::Vec3(2, 0.5, -0.5), AA::Vec3(1, 1, 2), false, new Material(sf::Color(0, 0, 0, 255), false), _sceneLight.get()));
    _testBox = dynamic_cast<Box*>(_dynamicHittables->_hittableObjects.back());
}

void App::SpawnLightTest()
{
    //Room boxes
    Box* temp = nullptr;

    _staticHittables->_hittableObjects.push_back(new Box(AA::Vec3(-6, 5, 0), AA::Vec3(1, 15, 20), true, new Diffuse(sf::Color(240, 0, 0, 255), true), _sceneLight.get()));     //Left
    temp = dynamic_cast<Box*>(_staticHittables->_hittableObjects.back());
    temp->OverrideNormal(AA::Vec3(1, 0, 0));

    _staticHittables->_hittableObjects.push_back(new Box(AA::Vec3(6, 5, 0), AA::Vec3(1, 15, 20), true, new Diffuse(sf::Color(240, 136, 0, 255), true), _sceneLight.get()));    //Right
    temp = dynamic_cast<Box*>(_staticHittables->_hittableObjects.back());
    temp->OverrideNormal(AA::Vec3(-1, 0, 0));

    _staticHittables->_hittableObjects.push_back(new Box(AA::Vec3(0, 5, 6), AA::Vec3(20, 15, 1), true, new Diffuse(sf::Color(247, 227, 0, 255), true), _sceneLight.get()));    //Back
    temp = dynamic_cast<Box*>(_staticHittables->_hittableObjects.back());
    temp->OverrideNormal(AA::Vec3(0, 0, -1));

    _staticHittables->_hittableObjects.push_back(new Box(AA::Vec3(0, 5, -6), AA::Vec3(20, 15, 1), true, new Diffuse(sf::Color(0, 247, 4, 255), true), _sceneLight.get()));     //Front
    temp = dynamic_cast<Box*>(_staticHittables->_hittableObjects.back());
    temp->OverrideNormal(AA::Vec3(0, 0, 1));

    _staticHittables->_hittableObjects.push_back(new Box(AA::Vec3(0, 6, 0), AA::Vec3(20, 1, 20), true, new Diffuse(sf::Color(0, 4, 247, 255), true), _sceneLight.get()));       //Top
    temp = dynamic_cast<Box*>(_staticHittables->_hittableObjects.back());
    temp->OverrideNormal(AA::Vec3(0, -1, 0));

    _staticHittables->_hittableObjects.push_back(new Box(AA::Vec3(0, -2, 0), AA::Vec3(20, 1, 20), true, new Diffuse(sf::Color(217, 0, 237, 255), true), _sceneLight.get()));     //Bottom
    temp = dynamic_cast<Box*>(_staticHittables->_hittableObjects.back());
    temp->OverrideNormal(AA::Vec3(0, 1, 0));

    //_staticHittables->_hittableObjects.push_back(new Sphere(AA::Vec3(3, 0.5, 1), 1.0, true, new Mirror(sf::Color(42, 209, 212, 255), true, _staticHittables.get(), _dynamicHittables.get()), _sceneLight.get()));
    _staticHittables->_hittableObjects.push_back(new Sphere(AA::Vec3(2, 1.5, -1), 0.8, true, new Diffuse(sf::Color(42, 209, 212, 255), true), _sceneLight.get()));
    _staticHittables->_hittableObjects.push_back(new Sphere(AA::Vec3(-2, 1.5, -1), 1.5, true, new Diffuse(sf::Color(194, 10, 10, 255), true), _sceneLight.get()));
    _staticHittables->_hittableObjects.push_back(new Sphere(AA::Vec3(0, 0.5, -1), 0.8, true, new Mirror(sf::Color(27, 209, 10, 255), true, _staticHittables.get(), _dynamicHittables.get()), _sceneLight.get()));
    //_staticHittables->_hittableObjects.push_back(new Sphere(AA::Vec3(0.0, 0.5, -1), 0.8, true, new Diffuse(sf::Color(27, 209, 10, 255), true), _sceneLight.get()));

    //_staticHittables->_hittableObjects.push_back(new Sphere(AA::Vec3(0, 0.5, 7), 4.0, true, new Mirror(sf::Color(255,255,255, 255), true, _staticHittables.get(), _dynamicHittables.get()), _sceneLight.get()));
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
    //if (_testBox != nullptr)
    //{
    //    if (_pEventHander->IsKeyPressed(sf::Keyboard::S))
    //    {
    //        AA::Vec3 previous = _testBox->GetPosition();
    //        previous[2] -= 0.25;
    //        previous[2] = previous.Z() < -3.0 ? -3.0 : previous.Z();
    //        _testBox->Move(previous);
    //    }
    //    else if (_pEventHander->IsKeyPressed(sf::Keyboard::W))
    //    {
    //        AA::Vec3 previous = _testBox->GetPosition();
    //        previous[2] += 0.25;
    //        previous[2] = previous.Z() > 3 ? 3 : previous.Z();
    //        _testBox->Move(previous);
    //    }
    //    if (_pEventHander->IsKeyPressed(sf::Keyboard::D))
    //    {
    //        AA::Vec3 previous = _testBox->GetPosition();
    //        previous[0] -= 0.25;
    //        previous[0] = previous.X() < -4.0 ? -4.0 : previous.X();
    //        _testBox->Move(previous);
    //    }
    //    else if (_pEventHander->IsKeyPressed(sf::Keyboard::A))
    //    {
    //        AA::Vec3 previous = _testBox->GetPosition();
    //        previous[0] += 0.25;
    //        previous[0] = previous.X() > 4.0 ? 4.0 : previous.X();
    //        _testBox->Move(previous);
    //    }
    //}

    if (_sceneLight)
    {
        _sceneLight->ResetRecursionCount();

        float lightDt = dt * 0.001;
        if (_pEventHander->IsKeyPressed(sf::Keyboard::S))
        {
            AA::Vec3 previous = _sceneLight->GetPosition();
            previous[2] -= ((_pEventHander->IsKeyPressed(sf::Keyboard::LShift) ? _lightMaxSpeed : _lightMinSpeed) * lightDt);
            previous[2] = previous.Z() < -_lightBounds ? -_lightBounds : previous.Z();
            _sceneLight->Move(previous);
        }
        else if (_pEventHander->IsKeyPressed(sf::Keyboard::W))
        {
            AA::Vec3 previous = _sceneLight->GetPosition();
            previous[2] += ((_pEventHander->IsKeyPressed(sf::Keyboard::LShift) ? _lightMaxSpeed : _lightMinSpeed) * lightDt);
            previous[2] = previous.Z() > _lightBounds ? _lightBounds : previous.Z();
            _sceneLight->Move(previous);
        }
        if (_pEventHander->IsKeyPressed(sf::Keyboard::D))
        {
            AA::Vec3 previous = _sceneLight->GetPosition();
            previous[0] -= ((_pEventHander->IsKeyPressed(sf::Keyboard::LShift) ? _lightMaxSpeed : _lightMinSpeed) * lightDt);
            previous[0] = previous.X() < -_lightBounds ? -_lightBounds : previous.X();
            _sceneLight->Move(previous);
        }
        else if (_pEventHander->IsKeyPressed(sf::Keyboard::A))
        {
            AA::Vec3 previous = _sceneLight->GetPosition();
            previous[0] += ((_pEventHander->IsKeyPressed(sf::Keyboard::LShift) ? _lightMaxSpeed : _lightMinSpeed) * lightDt);
            previous[0] = previous.X() > _lightBounds ? _lightBounds : previous.X();
            _sceneLight->Move(previous);
        }
        if (_pEventHander->IsKeyPressed(sf::Keyboard::Q))
        {
            AA::Vec3 previous = _sceneLight->GetPosition();
            previous[1] -= ((_pEventHander->IsKeyPressed(sf::Keyboard::LShift) ? _lightMaxSpeed : _lightMinSpeed) * lightDt);
            previous[1] = previous.Y() < -_lightBounds ? -_lightBounds : previous.Y();
            _sceneLight->Move(previous);
        }
        else if (_pEventHander->IsKeyPressed(sf::Keyboard::E))
        {
            AA::Vec3 previous = _sceneLight->GetPosition();
            previous[1] += ((_pEventHander->IsKeyPressed(sf::Keyboard::LShift) ? _lightMaxSpeed : _lightMinSpeed) * lightDt);
            previous[1] = previous.Y() > _lightBounds ? _lightBounds : previous.Y();
            _sceneLight->Move(previous);
        }
    }

    if (_pEventHander->IsKeyPressed(sf::Keyboard::Up))
    {
        double previous = _cam->GetVFov();
        previous += 3;
        previous = previous > 150 ? 150 : previous;
        _cam->SetVFov(previous);
    }
    else if (_pEventHander->IsKeyPressed(sf::Keyboard::Down))
    {
        double previous = _cam->GetVFov();
        previous -= 3;
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


    if (_isThreaded)
    {
        _currentDivision = _totalThreads;
        std::function<void()> call = std::bind(&App::CreateImageSegment, this);

        for (int i = 0; i < _totalThreads; ++i)
        {
            _jobManager->AddJobToQueue(JobManager::Job(call));
        }

        _jobManager->ProcessJobs();
        UpdateRenderTexture();
    }
    else
    {
        CreateImage();
        UpdateRenderTexture();
    }
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

    //TODO rework antialisating to work with new static and dynamic system

    //if (_antiAliasing)
    //{
    //    GetColourAntiAliasing(u, v, pixelColour);
    //}
    //else
    //{
        GetColour(u, v, pixelColour);
    //}

    return pixelColour;
}

void App::UpdateRenderTexture()
{
    //Create an image that uses the raytraced pixel data
    sf::Image renderData;
    renderData.create(_width, _height, reinterpret_cast<sf::Uint8*>(_pixelColourBuffer->GetDataBasePointer()));

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
}

void App::CreateImageSegment()
{
    //Use the current division to work of a section to iterate through based from width * height and the total thread count
    //Translate the total value back into an X and Y
    int startInd, endInd, currentDiv;

    //Scope to lock the mutex whilst grabbing current div
    {
        std::lock_guard<std::mutex> lock(_divisionMutex);
        currentDiv = _currentDivision;
        --_currentDivision;
    }
    
    startInd = (currentDiv - 1) * _calcsPerDivision;
    endInd = currentDiv * _calcsPerDivision;

    for (int i = startInd; i < endInd; ++i)
    {
        //Take the current i, translate it into X and Y
        int x, y;

        if (i == 0)
        {
            x = y = 0;
        }
        else
        {
            x = i % _width;
            y = floor(i / _width);
        }

        //Do the normal Calc from before
        double u = double(x / double(_width));
        double v = double(y / double(_height));
        //std::cout << "Pixel Positions: " << x << ", " << y << std::endl;
        _pixelColourBuffer->ColourPixelAtIndex(i, CalculatePixel(u, v));
    }
}

void App::GetColour(const double& u, const double& v, sf::Color& colOut)
{
    Hittable::HitResult staticRes, dynamicRes, lightRes;
    AA::Ray ray = _cam->GetRay(u, v);
    bool staticHit, dynamicHit, lightHit;
    staticRes.t = INFINITY;
    dynamicRes.t = INFINITY;
    lightRes.t = INFINITY;

    staticHit = _staticHittables->IntersectedRay(ray, 0.0, INFINITY, staticRes);
    dynamicHit = _dynamicHittables->IntersectedRay(ray, 0.0, INFINITY, dynamicRes);
    if (_sceneLight && _sceneLight->IsDebugRendering())
    {
        lightHit = _sceneLight->IntersectedRay(ray, 0.0, INFINITY, lightRes);
        if (staticHit && dynamicHit && lightHit)
        {
            if (staticRes.t < dynamicRes.t)
            {
                colOut = staticRes.t < lightRes.t ? staticRes.col : lightRes.col;
            }
            else
            {
                colOut = dynamicRes.t < lightRes.t ? dynamicRes.col : lightRes.col;
            }
        }
        else if (staticHit && lightHit)
        {
            colOut = staticRes.t < lightRes.t ? staticRes.col : lightRes.col;
        }
        else if (dynamicHit && lightHit)
        {
            colOut = dynamicRes.t < lightRes.t ? dynamicRes.col : lightRes.col;
        }
        else if (staticHit && dynamicHit)
        {
            colOut = staticRes.t < dynamicRes.t ? staticRes.col : dynamicRes.col;
        }
        else if (staticHit)
        {
            colOut = staticRes.col;
        }
        else if (dynamicHit)
        {
            colOut = dynamicRes.col;
        }
        else if (lightHit)
        {
            colOut = lightRes.col;
        }
        else
        {
            colOut = AA::BackgroundGradientCol(ray).Vec3ToCol();
        }
    }
    else
    {
        if (staticHit && dynamicHit)
        {
            colOut = staticRes.t < dynamicRes.t ? staticRes.col : dynamicRes.col;
        }
        else if (staticHit)
        {
            colOut = staticRes.col;
        }
        else if (dynamicHit)
        {
            colOut = dynamicRes.col;
        }
        else
        {
            colOut = AA::BackgroundGradientCol(ray).Vec3ToCol();
        }
    }
}

void App::GetColourAntiAliasing(const double& u, const double& v, sf::Color& colOut)
{
    //AA::Vec3 tempColValues = AA::Vec3(0,0,0);
    //Hittable::HitResult staticRes, dynamicRes;
    //bool staticHit, dynamicHit;

    ////Iterate with slight variance around the set X and Y position of the ray, get the colour data and add it to the temp
    //for (size_t i = 0; i < _perPixelAA; i++)
    //{
    //    double rayU = ((u * _width) + AA::RanDouble()) / _width;
    //    double rayV = ((v * _height) + AA::RanDouble()) / _height;
    //    AA::Ray ray = _cam->GetRay(rayU, rayV);

    //    if (_world->IntersectedRay(ray, 0.0, 20000.0, res))
    //    {
    //        tempColValues += AA::Vec3(res.col.r / 255.0, res.col.g / 255.0, res.col.b / 255.0);
    //    }
    //    else
    //    {
    //        tempColValues += BackgroundGradientCol(ray);
    //    }
    //}

    //tempColValues /= static_cast<double>(_perPixelAA);
    //colOut = tempColValues.Vec3ToCol();
}
