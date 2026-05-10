#pragma once

#include <random>
#include <cmath>
#include <string>

namespace shared {
    struct Vec2 {
        float x, y;

        static Vec2 scale(const Vec2 &v, float factor) {
            return {v.x * factor, v.y * factor};
        }

        Vec2 operator*(float factor) const {
            return {x * factor, y * factor};
        }
    };

    struct Rect {
        float x = 0;
        float y = 0;
        float width = 0;
        float height = 0;
    };

    struct ColorRGBA {
        unsigned char r, g, b, a;

        static inline ColorRGBA fromHex(unsigned int hex) {
            return {
                (unsigned char) ((hex >> 24) & 0xFF),
                (unsigned char) ((hex >> 16) & 0xFF),
                (unsigned char) ((hex >> 8) & 0xFF),
                (unsigned char) (hex & 0xFF)
            };
        }

        static inline ColorRGBA alpha(ColorRGBA color, float alphaFactor) {
            return {
                color.r,
                color.g,
                color.b,
                (unsigned char) fminf(fmaxf(alphaFactor * 255.0f, 0.0f), 255.0f)
            };
        }

        static inline ColorRGBA brightness(ColorRGBA color, float factor) {
            if (factor < -1.0f) factor = -1.0f;
            if (factor > 1.0f) factor = 1.0f;

            if (factor <= 0.0f) {
                float f = 1.0f + factor;
                return {
                    (unsigned char) (color.r * f),
                    (unsigned char) (color.g * f),
                    (unsigned char) (color.b * f),
                    color.a
                };
            } else {
                return {
                    (unsigned char) ((255 - color.r) * factor + color.r),
                    (unsigned char) ((255 - color.g) * factor + color.g),
                    (unsigned char) ((255 - color.b) * factor + color.b),
                    color.a
                };
            }
        }
    };
}

struct TextureHandle {
    int width = 0;
    int height = 0;
    void *native = nullptr;
};

struct RenderTextureHandle {
    TextureHandle texture;
    void* handle = nullptr;
};

struct FontHandle {
    int baseSize = 0;
    TextureHandle texture;
    void *native = nullptr;
};

struct ShaderHandle {
    void *native = nullptr;
};

constexpr shared::ColorRGBA COLOR_BLACK = {0, 0, 0, 255};
constexpr shared::ColorRGBA COLOR_WHITE = {255, 255, 255, 255};
constexpr shared::ColorRGBA COLOR_EMPTY = {0, 0, 0, 0};

constexpr shared::ColorRGBA COLOR_RED = {255, 0, 0, 255};
constexpr shared::ColorRGBA COLOR_GREEN = {0, 255, 0, 255};
constexpr shared::ColorRGBA COLOR_BLUE = {0, 0, 255, 255};
constexpr shared::ColorRGBA COLOR_YELLOW = {255, 255, 0, 255};
constexpr shared::ColorRGBA COLOR_MAGENTA = {255, 0, 255, 255};
constexpr shared::ColorRGBA COLOR_CYAN = {0, 255, 255, 255};

constexpr shared::ColorRGBA COLOR_GRAY = {128, 128, 128, 255};
constexpr shared::ColorRGBA COLOR_DARK_GRAY = {45, 45, 45, 255};
constexpr shared::ColorRGBA COLOR_LIGHT_GRAY = {200, 200, 200, 255};
constexpr shared::ColorRGBA COLOR_ORANGE = {255, 165, 0, 255};
constexpr shared::ColorRGBA COLOR_LIME = {50, 205, 50, 255};
constexpr shared::ColorRGBA COLOR_SKYBLUE = {135, 206, 235, 255};
constexpr shared::ColorRGBA COLOR_PINK = {255, 192, 203, 255};
constexpr shared::ColorRGBA COLOR_BLANK = {0, 0, 0, 0};

static bool checkCollisionPointRect(shared::Vec2 point, shared::Rect rect) {
    return (point.x >= rect.x &&
            point.x <= rect.x + rect.width &&
            point.y >= rect.y &&
            point.y <= rect.y + rect.height);
}

static int getRandomValue(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}
