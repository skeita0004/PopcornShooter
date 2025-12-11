#pragma once

namespace Screen
{
    const int WIDTH{1280};
    const int HEIGHT{720};

    inline float GetAspectRatio()
    {
        return WIDTH / HEIGHT;
    }
}