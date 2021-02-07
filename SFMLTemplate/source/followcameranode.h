#pragma once

#include "sfml-engine/scene.h"
#include "sfml-engine/cameranode.h"
#include <SFML/Audio.hpp>

class FollowCameraNode : public gbh::CameraNode {
public:
    void setTarget(std::shared_ptr<gbh::Node> target); // sets a member variable on the class which represents the target
protected:
    void onUpdate(double deltaTime) override;
private:
    std::shared_ptr<gbh::Node> m_target;
};
