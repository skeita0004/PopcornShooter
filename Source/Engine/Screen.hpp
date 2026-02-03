#pragma once

namespace Screen
{
    const int WIDTH{1920};
    const int HEIGHT{1080};

    inline float GetAspectRatio()
    {
        return WIDTH / HEIGHT;
    }
}