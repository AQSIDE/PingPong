#pragma once

#include "shared.h"

struct CameraData {
    shared::Vec2 target = {0};
    shared::Vec2 offset = {0};
    float zoom = 1;
    float rotation = 0;
};

class ICamera {
public:
    virtual ~ICamera() {}

    virtual void setTarget(shared::Vec2 target) = 0;
    virtual void setOffset(shared::Vec2 offset) = 0;
    virtual void setZoom(float zoom) = 0;
    virtual void setRotation(float rot) = 0;

    virtual void update() = 0;
    virtual void onResize(shared::Vec2 screenSize) = 0;

    virtual const CameraData& getData() const = 0;
};

class CameraBase : public ICamera {
public:
    CameraData m_Data;

    CameraBase(const CameraData &data);

    void setTarget(shared::Vec2 target) override;
    void setOffset(shared::Vec2 offset) override;
    void setZoom(float zoom) override;
    void setRotation(float rot) override;

    void update() override;
    void onResize(shared::Vec2 screenSize) override;

    const CameraData& getData() const override;
};
