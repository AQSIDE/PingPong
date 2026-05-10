#include "camera.h"
#include "graphics.h"

CameraBase::CameraBase(const CameraData &data) : m_Data(data) {}

void CameraBase::setTarget(shared::Vec2 target) {
    m_Data.target = target;
}

void CameraBase::setOffset(shared::Vec2 offset) {
    m_Data.offset = offset;
}

void CameraBase::setRotation(float rot) {
    m_Data.rotation = rot;
}

void CameraBase::setZoom(float zoom) {
    m_Data.zoom = zoom;
}

void CameraBase::update() {
    //float softness = 2.0f * App()->getDeltaTime();
    //camera.target = Vector2Lerp(camera.target, target, softness);

    //m_Data.target = m_Target;
}

void CameraBase::onResize(shared::Vec2 screenSize) {
    m_Data.offset = {
        screenSize.x / 2.0f,
        screenSize.y / 2.0f
    };
}

const CameraData &CameraBase::getData() const {
    return m_Data;
}


