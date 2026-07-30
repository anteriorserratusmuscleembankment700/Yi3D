///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY3D_UNION_H
#define WY3D_UNION_H

#include <wy3dDefs.h>
#include <wy3dBoolean.h>

NS_WY3D_BEG

class Solid;

class WY3D_EXPORT Union : public wy3d::Boolean
{
    WYDB_DECLARE_MEMBERS(Union, wy3d::Union, wy3d::Boolean);
public:
    // 创建Box
    // 可能的返回值:Ok or NullDatabasePointer or NullElementPointer or InvalidInput.
    static wy::ErrorStatus create(
        wydb::Transaction* pTrans,
        wy3d::Solid* pTarget, wy3d::Solid* pTool,
        Union*& pOutUnion);
    static wy::ErrorStatus create(
        wydb::Transaction* pTrans,
        wy3d::Solid* pTarget, const std::vector<wy3d::Solid*>& tools,
        Union*& pOutUnion);

};

NS_WY3D_END

#endif // WY3D_UNION_H