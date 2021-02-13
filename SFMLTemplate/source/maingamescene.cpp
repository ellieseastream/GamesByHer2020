#include "maingamescene.h"
#include "followcameranode.h"
#include "sfml-engine/game.h"
#include "sfml-engine/mathutils.h"
#include "sfml-engine/shapenode.h"
#include "sfml-engine/spritenode.h"
#include "sfml-engine/textnode.h"
#include "sfml-engine/physics/physicscontact.h"
#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>

//const std::string level_01 = "../assets/levels/level_01.txt";
const std::string kFont = "../assets/fonts/roboto-regular.ttf";
const std::string kCheckpoint = "../assets/gfx/checkpoint.png";
//const std::string kMainSceneMusic = "../assets/music/victor_2.ogg";
const std::string kMainSceneMusic = "../assets/music/victor_7.ogg";
const std::string kStarfield = "../assets/gfx/starfield-01.png";
const std::string kPlayerShip = "../assets/gfx/player-ship.png";
const std::string kAsteroid_small_01 = "../assets/gfx/asteroid-small-01.png";
const std::string kAsteroid_small_02 = "../assets/gfx/asteroid-small-02.png";
const std::string kAsteroid_medium_01 = "../assets/gfx/asteroid-medium-01.png";
const std::string kAsteroid_large_01 = "../assets/gfx/asteroid-large-01.png";
static const float ACCELERATION = 2000.0f;
static const float DEGREES_PER_SECOND_SMALL_ASTEROID = 7.0f;
static const float DEGREES_PER_SECOND_MEDIUM_ASTEROID = 10.0f;
static const float DEGREES_PER_SECOND_LARGE_ASTEROID = 2.0f;
const sf::Color kInactiveCheckpointColor = sf::Color(255, 255, 255, 64);
const sf::Color kNextCheckpointColor = sf::Color(64, 64, 255, 192);
const sf::Color kDoneCheckpointColor = sf::Color(64, 255, 64, 128);

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
    asteroid_1 = std::make_shared<gbh::SpriteNode>(kAsteroid_small_01);
    asteroid_1->setOrigin(0.5f, 0.5f);
    asteroid_1->setPosition(200, 200);
    asteroid_1->setPhysicsBody(getPhysicsWorld()->createCircle(18));
    asteroid_1->getPhysicsBody()->setAngularDamping(0.0f);
    asteroid_1->getPhysicsBody()->setLinearDamping(0.2);
    addChild(asteroid_1);
    
    asteroid_2 = std::make_shared<gbh::SpriteNode>(kAsteroid_small_02);
    asteroid_2->setOrigin(0.5f, 0.5f);
    asteroid_2->setPosition(400, 400);
    asteroid_2->setPhysicsBody(getPhysicsWorld()->createCircle(13));
    asteroid_2->getPhysicsBody()->setAngularDamping(0.0f);
    asteroid_2->getPhysicsBody()->setLinearDamping(0.2);
    addChild(asteroid_2);
    
    asteroid_3 = std::make_shared<gbh::SpriteNode>(kAsteroid_medium_01);
    asteroid_3->setOrigin(0.5f, 0.5f);
    asteroid_3->setPosition(500, 500);
    asteroid_3->setPhysicsBody(getPhysicsWorld()->createCircle(30));
    asteroid_3->getPhysicsBody()->setAngularDamping(0.0f);
    asteroid_3->getPhysicsBody()->setLinearDamping(0.2);
    addChild(asteroid_3);
    
    asteroid_4 = std::make_shared<gbh::SpriteNode>(kAsteroid_large_01);
    asteroid_4->setOrigin(0.5f, 0.5f);
    asteroid_4->setPosition(720, 200);
    asteroid_4->setPhysicsBody(getPhysicsWorld()->createCircle(60));
    asteroid_4->getPhysicsBody()->setAngularDamping(0.0f);
    asteroid_4->getPhysicsBody()->setLinearDamping(0.2);
    addChild(asteroid_4);
    
    setDrawPhysicsDebug(true);
    
// Add camera
    std::cout << "... adding camera...\n";
    m_followCamera = std::make_shared<FollowCameraNode>();
    m_followCamera->setTarget(m_playerShip);
    m_followCamera->setPosition(640, 360);
    addChild(m_followCamera);
    setCamera(m_followCamera);

    std::cout << "...done!\n";
    
    /*
     std::shared_ptr<gbh::TextNode> textNode = std::make_shared<gbh::TextNode>("Space Race", m_robotoFont, 60);
     textNode->setOrigin(0.5f, 0.5f);
     textNode->setPosition(640, 100);
     textNode->setName("Title"); */
    
    m_robotoFont.loadFromFile(kFont);
    m_timerText = std::make_shared<gbh::TextNode>("0", m_robotoFont, 30);
    m_timerText->setOrigin(1.0f, 1.0f);
    m_timerText->setPosition(1270, 700);
    getOverlay().addChild(m_timerText);
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
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_courseFinished) {
        gbh::Game::getInstance().changeScene("title");
    }
    
    moveDirection = gbh::math::normalize(moveDirection);
    
    // Multiply the move direction by the acceleration and apply the force to the player ship.
    m_playerShip->getPhysicsBody()->applyForceToCenter(moveDirection * ACCELERATION);
    
    asteroid_1->rotate(DEGREES_PER_SECOND_SMALL_ASTEROID * deltaTime);
    asteroid_2->rotate(-DEGREES_PER_SECOND_SMALL_ASTEROID * deltaTime);
    asteroid_3->rotate(DEGREES_PER_SECOND_MEDIUM_ASTEROID * deltaTime);
    asteroid_4->rotate(-DEGREES_PER_SECOND_LARGE_ASTEROID * deltaTime);
    
    // move camera
    
    // update timer
    if(!m_courseFinished) {
        m_playerTime += deltaTime;
        m_timerText->setString(std::to_string(m_playerTime));
    }
}

void MainGameScene::onBeginPhysicsContact(const gbh::PhysicsContact& contact) {
    if(contact.containsNode(m_playerShip.get())) {
        gbh::Node* otherNode = contact.otherNode(m_playerShip.get());
        if(otherNode && otherNode->getName() == "checkpoint") {
            advanceCheckpoint();
        }
    }
}

void MainGameScene::advanceCheckpoint() {
    if(m_currentCheckpoint >= 0 && m_currentCheckpoint < m_checkpoints.size()) {
        // disable current checkpoint
        m_checkpoints[m_currentCheckpoint]->getPhysicsBody()->setEnabled(false);
        // set color of the current checkpoint to green
        m_checkpoints[m_currentCheckpoint]->setColor(kDoneCheckpointColor);
        // set currentCheckPoint = currentCheckpoint + 1
        m_currentCheckpoint++;
    } else {
        m_currentCheckpoint = 0;
    }
    
    if(m_currentCheckpoint < m_checkpoints.size()) {
        // set color of the next checkpoint (using the currentCheckpoint value) to blue
        m_checkpoints[m_currentCheckpoint]->setColor(kNextCheckpointColor);
        // enable the physics body of the next checkpoint to enable collosions
        m_checkpoints[m_currentCheckpoint]->getPhysicsBody()->setEnabled(true);
    } else {
        std::cout << "Completed all checkpoints.";
        m_courseFinished = true;
    }
}

void MainGameScene::onShowScene() {
    // start music track
    m_mainSceneMusic.play();
    
    // load level
    loadLevel("../assets/levels/level_01.txt");
    advanceCheckpoint();
}

void MainGameScene::onHideScene() {
    // stop music track
    m_mainSceneMusic.stop();
}

void MainGameScene::loadLevel(const std::string &filename) {
    
    std::ifstream file(filename);
    nlohmann::json jsonFile;
    
    try {
        jsonFile = nlohmann::json::parse(file);
    } catch(const std::exception& e) {
        std::cout << "Failed to load level from file " << filename << ": " << e.what() << "\n";
        return;
    }
    
    nlohmann::json checkpoints = jsonFile["checkpoints"];
    
    if(!checkpoints.is_array()) {
        std::cout << "Levels file either does not include a 'checkpoints' entry, or it is not an array.";
        return;
    }
    
    std::vector<sf::Vector2f> checkpointPositions;
    
    if(checkpoints.is_array()) {
        for(int i = 0; i < checkpoints.size(); i++) {
            float x = checkpoints[i]["x"].get<float>();
            float y = checkpoints[i]["y"].get<float>();
            
            std::cout << "Checkpoint: " << x << ", " << y << "\n";
            checkpointPositions.push_back(sf::Vector2f(x, y));
        }
    }
    
    for(int i= 0; i < checkpointPositions.size(); i++) {
        std::cout << "...adding checkpoint sprite...\n";
        std::shared_ptr<gbh::SpriteNode> node = std::make_shared<gbh::SpriteNode>(kCheckpoint);
        std::cout << "...adding checkpoint color...\n";
        node->setColor(kInactiveCheckpointColor);
        std::cout << "...adding checkpoint physicsBody...\n";
        node->setPhysicsBody(getPhysicsWorld()->createCircle(50));
        node->getPhysicsBody()->makeSensor();
        node->getPhysicsBody()->setEnabled(false);
        node->setPosition(checkpointPositions[i]);
        node->setName("checkpoint");
        
        m_checkpoints.push_back(node);
        addChild(node);
    }
    
    /*
    // Adding checkpoints
    std::vector<sf::Vector2f> checkpointPositions = {
        sf::Vector2f(640.0f, 720.0f),
        sf::Vector2f(1240.0f, 200.0f),
        sf::Vector2f(80.0f, 400.0f),
    };
     */
}
