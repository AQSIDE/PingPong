#pragma once
#include "shared.h"

namespace COLORS {
    struct BoardTheme {
        shared::ColorRGBA boardColor;
        shared::ColorRGBA mainColor;
    };

    inline const BoardTheme ALL_THEMES[] = {
        { shared::ColorRGBA::fromHex(0x050510FF), COLOR_MAGENTA },   // Cyberpunk
        { shared::ColorRGBA::fromHex(0x000D00FF), COLOR_LIME },      // Matrix
        { shared::ColorRGBA::fromHex(0x241734FF), COLOR_ORANGE },    // Synthwave
        { shared::ColorRGBA::fromHex(0x001B2BFF), COLOR_SKYBLUE },   // Deep Sea
        { shared::ColorRGBA::fromHex(0x1A0000FF), COLOR_RED },       // Aggressive
        { shared::ColorRGBA::fromHex(0x00030BFF), COLOR_PINK }       // Default
    };

    inline BoardTheme getBoardTheme() {
        int count = sizeof(ALL_THEMES) / sizeof(ALL_THEMES[0]);
        int randomIndex = getRandomValue(0, count - 1);
        return ALL_THEMES[randomIndex];
    }
}
