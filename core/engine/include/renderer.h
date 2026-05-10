#pragma once

#include "base.h"
#include "transform.h"

struct SpriteFrame {
    float x, y;
    float width, height;

    void setRect(shared::Rect r) {
        x = r.x;
        y = r.y;
        width = r.width;
        height = r.height;
    }

    shared::Rect getRect() {
        return { x, y, width, height };
    }
};

struct AnimationNode {
    std::string name;
    std::vector<SpriteFrame> frames;
    float frameDuration = 0.1f;
    bool loop = true;

    AnimationNode() : name("") {}
    AnimationNode(std::string name) : name(name) {}

    SpriteFrame getFrame(int index) {
        if (frames.empty()) return { 0, 0, 0, 0 };
        return frames[index % frames.size()];
    }
};

class SpriteAnimation {
private:
    std::map<std::string, AnimationNode> nodes;
    AnimationNode* currentNode = nullptr;

    float timer = 0.0f;
    int currentFrameIndex = 0;

public:
    void addNode(AnimationNode node) {
        nodes[node.name] = node;
        if (currentNode == nullptr) play(node.name);
    }

    void play(const std::string& name) {
        if (currentNode != nullptr && currentNode->name == name) return;

        if (nodes.find(name) != nodes.end()) {
            currentNode = &nodes[name];
            currentFrameIndex = 0;
            timer = 0;
        }
    }

    void update(float dt) {
        if (!currentNode) return;

        timer += dt;
        if (timer >= currentNode->frameDuration) {
            timer = 0;
            currentFrameIndex++;

            if (currentFrameIndex >= currentNode->frames.size()) {
                if (currentNode->loop) currentFrameIndex = 0;
                else currentFrameIndex = currentNode->frames.size() - 1;
            }
        }
    }

    SpriteFrame getCurrentFrame() {
        if (!currentNode) return { 0, 0, 0, 0 };
        return currentNode->getFrame(currentFrameIndex);
    }
};

class AnimationBuilder {
public:
    static AnimationNode create(std::string name, float duration = 0.1f, bool loop = true) {
        AnimationNode node(name);
        node.frameDuration = duration;
        node.loop = loop;
        return node;
    }

    static void addStrip(AnimationNode& node, int count, float startX, float y, float w, float h) {
        for (int i = 0; i < count; i++) {
            node.frames.push_back({ startX + (i * w), y, w, h });
        }
    }
};

// RENDERER
struct RenderContext {
    const TextureHandle* texture = nullptr;
    bool flipX = false;
    bool flipY = false;
    shared::ColorRGBA color = COLOR_WHITE;
    SpriteFrame currentFrame = { 0, 0, 0, 0 };
};

struct RenderItem {
    Transform2D* transform;
    RenderContext* context;

    float y() const { return transform->m_Pos.y; }
};

class Renderer {
    std::vector<RenderItem> m_Items;
public:
    bool m_RenderDebug = false;

    void add(Transform2D* tr, RenderContext* c);
    void drawItem(Transform2D* tr, RenderContext* c);
    void draw();
};