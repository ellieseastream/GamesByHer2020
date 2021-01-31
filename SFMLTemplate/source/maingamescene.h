#pragma once

#include "sfml-engine/scene.h"
#include <SFML/Audio.hpp>

class MainGameScene : public gbh::Scene {
protected:
    void onInitializeScene() override;
    void onUpdate(double deltaTime) override;
    void onShowScene() override;
    void onHideScene() override;
private:
    std::shared_ptr<gbh::SpriteNode> m_playerShip;
    sf::Music m_mainSceneMusic;
};
