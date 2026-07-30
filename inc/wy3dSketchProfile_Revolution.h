///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2025 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY3D_SKETCH_PROFILE_REVOLUTION_H
#define WY3D_SKETCH_PROFILE_REVOLUTION_H

#include <wy3dDefs.h>
#include <wy3dSketchProfile.h>

NS_WY3D_BEG

class WY3D_EXPORT SketchProfile_Revolution : public SketchProfile
{
public:
    SketchProfile_Revolution(const Sketch* pSketch, double tol = 1e-5);

protected:
    // 前置校验器
    virtual bool preValid(const wydb::Database* pDb) override;
};

NS_WY3D_END

#endif // WY3D_SKETCH_PROFILE_REVOLUTION_H