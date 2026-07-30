///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY3D_INTERSECTION_H
#define WY3D_INTERSECTION_H

#include <wy3dDefs.h>
#include <wy3dBoolean.h>

NS_WY3D_BEG

class Solid;

class WY3D_EXPORT Intersection : public wy3d::Boolean
{
    WYDB_DECLARE_MEMBERS(Intersection, wy3d::Intersection, wy3d::Boolean);
public:
    // 创建Box
    // 可能的返回值:Ok or NullDatabasePointer or NullElementPointer or InvalidInput.
    static wy::ErrorStatus create(
        wydb::Transaction* pTrans,
        wy3d::Solid* pTarget, wy3d::Solid* pTool,
        Intersection*& pOutIntersection);
    static wy::ErrorStatus create(
        wydb::Transaction* pTrans,
        wy3d::Solid* pTarget, const std::vector<wy3d::Solid*>& tools,
        Intersection*& pOutIntersection);

};

NS_WY3D_END

#endif // WY3D_INTERSECTION_H