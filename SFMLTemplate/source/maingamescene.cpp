#include "maingamescene.h"
#include "sfml-engine/game.h"
#include "sfml-engine/mathutils.h"
#include "sfml-engine/shapenode.h"
#include "sfml-engine/spritenode.h"
#include "sfml-engine/textnode.h"

#include <iostream>

//const std::string kMainSceneMusic = "../assets/music/victor_2.ogg";
const std::string kMainSceneMusic = "../assets/music/victor_7.ogg";
const std::string kStarfield = "../assets/gfx/starfield-01.png";
const std::string kPlayerShip = "../assets/gfx/player-ship.png";
const std::string kAsteroid_small_01 = "../assets/gfx/asteroid-small-01.png";
const std::string kAsteroid_small_02 = "../assets/gfx/asteroid-small-02.png";
const std::string kAsteroid_medium_01 = "../assets/gfx/asteroid-medium-01.png";
const std::string kAsteroid_large_01 = "../assets/gfx/asteroid-large-01.png";
static const float ACCELERATION = 2000.0f;

void MainGameScene::onInitializeScene() {
    std::cout << "Hello from onInitialize in MainGameScene! \n";
    
    m_mainSceneMusic.openFromFile(kMainSceneMusic);
    m_mainSceneMusic.setVolume(20);
    
    std::cout << "Adding physics world...\n";
    Scene::createPhysicsWorld(sf::Vector2f(0.0f, 0.0f));
    std::cout << "... done adding physics world.\n";
    
    //add space background
    std::shared_ptr<gbh::SpriteNode> background = std::make_shared<gbh::SpriteNode>(kStarfield);
    background->setName("background");
    addChild(background);
    std::cout << "Added background. \n";
    
    //add player ship
    std::cout << "Adding ship. \n";
    const sf::Vector2f shipSize = sf::Vector2f(40.0f, 220.0f);
    m_playerShip = std::make_shared<gbh::SpriteNode>(kPlayerShip);
    m_playerShip->setName("playership");
    m_playerShip->setOrigin(0.5f, 0.5f);
    m_playerShip->setPosition(640, 360);
    //add a physics body to the ship (box shape)
    std::cout << "...done, adding physics to ship. \n";
    m_playerShip->setPhysicsBody(getPhysicsWorld()->createBox(shipSize * 0.5f));
    m_playerShip->getPhysicsBody()->setLinearDamping(2.0f);
    m_playerShip->getPhysicsBody()->setFixedRotation(true);
    addChild(m_playerShip);
    std::cout << "Added ship. \n";
    
    std::cout << "...done. Adding boundary...\n";
    // build a wall around the game. Almost as big as the screen (1270, 710) and centered.
    std::shared_ptr<gbh::Node> boundary = std::make_shared<gbh::Node>();
    boundary->setPhysicsBody(getPhysicsWorld()->createEdgeBox(sf::Vector2f(1270, 719)));
    boundary->getPhysicsBody()->setType(gbh::PhysicsBodyType::Static);
    boundary->setPosition(1280.0f/2.0f, 720.0f/2.0f);
    addChild(boundary);
    
    // Add asteroids
    std::shared_ptr<gbh::SpriteNode> asteroid_1 = std::make_shared<gbh::SpriteNode>(kAsteroid_small_01);
    asteroid_1->setOrigin(0.5f, 0.5f);
    asteroid_1->setPosition(200, 200);
    asteroid_1->setPhysicsBody(getPhysicsWorld()->createCircle(18));
    asteroid_1->getPhysicsBody()->setAngularDamping(0.0f);
    asteroid_1->getPhysicsBody()->setLinearDamping(0.2);
    addChild(asteroid_1);
    
    std::shared_ptr<gbh::SpriteNode> asteroid_2 = std::make_shared<gbh::SpriteNode>(kAsteroid_small_02);
    asteroid_2->setOrigin(0.5f, 0.5f);
    asteroid_2->setPosition(400, 400);
    asteroid_2->setPhysicsBody(getPhysicsWorld()->createCircle(13));
    asteroid_2->getPhysicsBody()->setAngularDamping(0.0f);
    asteroid_2->getPhysicsBody()->setLinearDamping(0.2);
    addChild(asteroid_2);
    
    std::shared_ptr<gbh::SpriteNode> asteroid_3 = std::make_shared<gbh::SpriteNode>(kAsteroid_medium_01);
    asteroid_3->setOrigin(0.5f, 0.5f);
    asteroid_3->setPosition(500, 500);
    asteroid_3->setPhysicsBody(getPhysicsWorld()->createCircle(30));
    asteroid_3->getPhysicsBody()->setAngularDamping(0.0f);
    asteroid_3->getPhysicsBody()->setLinearDamping(0.2);
    addChild(asteroid_3);
    
    std::shared_ptr<gbh::SpriteNode> asteroid_4 = std::make_shared<gbh::SpriteNode>(kAsteroid_large_01);
    asteroid_4->setOrigin(0.5f, 0.5f);
    asteroid_4->setPosition(720, 200);
    asteroid_4->setPhysicsBody(getPhysicsWorld()->createCircle(60));
    asteroid_4->getPhysicsBody()->setAngularDamping(0.0f);
    asteroid_4->getPhysicsBody()->setLinearDamping(0.2);
    addChild(asteroid_4);
    
    setDrawPhysicsDebug(true);
    std::cout << "...done!\n";
}

void MainGameScene::onUpdate(double deltaTime) {
    sf::Vector2f moveDirection;
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        moveDirection.y -= 1.0f;
    }
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        moveDirection.y += 1.0f;
    }
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        moveDirection.x -= 1.0f;
    }
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        moveDirection.x += 1.0f;
    }
    
    moveDirection = gbh::math::normalize(moveDirection);
    
    // Multiply the move direction by the acceleration and apply the force to the player ship.
    m_playerShip->getPhysicsBody()->applyForceToCenter(moveDirection * ACCELERATION);
}

void MainGameScene::onShowScene() {
    // start music track
    m_mainSceneMusic.play();
    
}

void MainGameScene::onHideScene() {
    // stop music track
    m_mainSceneMusic.stop();
}
