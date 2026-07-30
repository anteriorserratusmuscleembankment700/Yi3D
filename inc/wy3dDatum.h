///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2025 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY3D_DATUM_H
#define WY3D_DATUM_H

#include <wy3dDefs.h>
#include <wy3dFeature.h>

NS_WY3D_BEG

class WY3D_EXPORT Datum : public wy3d::Feature
{
    WYDB_DECLARE_ABSTRACT_MEMBERS(Datum, wy3d::Datum, wy3d::Feature)

public:
};

NS_WY3D_END

#endif // WY3D_DATUM_H