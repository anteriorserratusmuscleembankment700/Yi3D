///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2026 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY3D_COLOR_H
#define WY3D_COLOR_H

#include <wy3dDefs.h>

NS_WY3D_BEG

class Color
{
public:
    Color(unsigned char r, unsigned char g, unsigned char b)
        : red(r), green(g), blue(b)
    {}

    bool operator==(const Color& rhs) const
    {
        return red == rhs.red && green == rhs.green && blue == rhs.blue;
    }

    bool operator!=(const Color& rhs) const
    {
        return !(*this == rhs);
    }

    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

NS_WY3D_END

#endif // WY3D_COLOR_H
