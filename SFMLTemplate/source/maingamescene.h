#pragma once

#include "followcameranode.h"
#include "sfml-engine/scene.h"
#include "sfml-engine/cameranode.h"
#include "sfml-engine/physics/physicscontact.h"
#include <SFML/Audio.hpp>

class MainGameScene : public gbh::Scene {
protected:
    void onInitializeScene() override;
    void onUpdate(double deltaTime) override;
    void onShowScene() override;
    void onHideScene() override;
    void onBeginPhysicsContact(const gbh::PhysicsContact& contact) override;
    void advanceCheckpoint();
    void loadLevel(const std::string& filename);
private:
    std::shared_ptr<gbh::SpriteNode> m_playerShip;
    std::shared_ptr<gbh::SpriteNode> asteroid_1;
    std::shared_ptr<gbh::SpriteNode> asteroid_2;
    std::shared_ptr<gbh::SpriteNode> asteroid_3;
    std::shared_ptr<gbh::SpriteNode> asteroid_4;
    sf::Music m_mainSceneMusic;
    sf::Font m_robotoFont;
    std::shared_ptr<FollowCameraNode> m_followCamera;
    std::vector<std::shared_ptr<gbh::SpriteNode>> m_checkpoints;
    int m_currentCheckpoint = -1;
    double m_playerTime;
    std::shared_ptr<gbh::TextNode> m_timerText;
    bool m_courseFinished = false;
};
