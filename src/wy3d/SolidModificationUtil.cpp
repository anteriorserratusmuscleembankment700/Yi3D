///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024-2026 Wang Yao <wangyao1052@163.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
///////////////////////////////////////////////////////////////////////////////

#include "SolidModificationUtil.h"
#include <cassert>
#include <TopoDS_Shape.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp.hxx>
#include <wy3dSolid.h>
#include <wy3dSolid.h>

NS_WY3D_BEG

std::vector<unsigned int> SolidModificationUtil::computeNewFaceIndices(
    wydb::Database* pDb,
    const TopoNameList& newFaces,
    const wydb::ElementId& ownerSolidId)
{
    assert(pDb);

    std::vector<unsigned int> retFaceIndices;
    if (!pDb || newFaces.empty())
    {
        return retFaceIndices;
    }

    // 获取实体的拓扑命名
    const wy3d::Solid* pSolid = wy3d::Solid::cast(pDb->getElement(ownerSolidId));
    if (!pSolid)
    {
        assert(false);
        return retFaceIndices;
    }
    const TopoNaming* pTopoNaming = pSolid->getTopoNaming();
    if (!pTopoNaming)
    {
        assert(false);
        return retFaceIndices;
    }

    // 匹配对应的TopoDS_Shape
    TopoShapeSet matchedShapes;
    for (const TopoName& topoName : newFaces)
    {
        TopoDS_Shape shapeFound = pTopoNaming->smartFind(TopAbs_ShapeEnum::TopAbs_FACE, topoName);
        if (!shapeFound.IsNull())
        {
            matchedShapes.insert(shapeFound);
        }
    }
    if (matchedShapes.empty()) return retFaceIndices;

    // 构建面索引的映射
    const TopoDS_Shape& shape = pSolid->getShape();
    TopTools_IndexedMapOfShape faceMap;
    TopExp::MapShapes(shape, TopAbs_FACE, faceMap);
    TopoShape2IdMap shape2Index;
    for (int i = 1; i <= faceMap.Extent(); ++i)
    {
        shape2Index[faceMap(i)] = static_cast<unsigned int>(i - 1);
    }

    // 获取面的索引
    retFaceIndices.reserve(matchedShapes.size());
    for (const TopoDS_Shape& matchedShape : matchedShapes)
    {
        auto iter = shape2Index.find(matchedShape);
        if (iter == shape2Index.cend())
        {
            assert(false);
            continue;
        }
        retFaceIndices.emplace_back(iter->second);
    }

    return retFaceIndices;
}

NS_WY3D_END
