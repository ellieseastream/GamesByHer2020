#pragma once

#include "followcameranode.h"
#include "sfml-engine/scene.h"
#include "sfml-engine/cameranode.h"
#include <SFML/Audio.hpp>

class MainGameScene : public gbh::Scene {
protected:
    void onInitializeScene() override;
    void onUpdate(double deltaTime) override;
    void onShowScene() override;
    void onHideScene() override;
private:
    std::shared_ptr<gbh::SpriteNode> m_playerShip;
    std::shared_ptr<gbh::SpriteNode> asteroid_1;
    std::shared_ptr<gbh::SpriteNode> asteroid_2;
    std::shared_ptr<gbh::SpriteNode> asteroid_3;
    std::shared_ptr<gbh::SpriteNode> asteroid_4;
    sf::Music m_mainSceneMusic;
    std::shared_ptr<FollowCameraNode> m_followCamera;
};
