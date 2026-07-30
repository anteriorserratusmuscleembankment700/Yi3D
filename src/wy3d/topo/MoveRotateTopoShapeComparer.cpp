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

#include "topo/MoveRotateTopoShapeComparer.h"
#include <cassert>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>

NS_WY3D_BEG

MoveRotateTopoShapeComparer::MoveRotateTopoShapeComparer(
    BRepBuilderAPI_Transform& mkTransform, const TopoDS_Shape& oldShape)
    : TopoShapeComparer(mkTransform, oldShape), _mkTransform(mkTransform)
{
}

MoveRotateTopoShapeComparer::~MoveRotateTopoShapeComparer()
{
}


void MoveRotateTopoShapeComparer::recordModified()
{
    // 修改的边
    for (const TopoShapeInfo& oldEdgeInfo : _oldEdgeInfoSet)
    {
        const TopTools_ListOfShape& modified = _mkShape.Modified(oldEdgeInfo.shape);
        if (modified.IsEmpty()) continue;

        for (TopTools_ListIteratorOfListOfShape iter(modified); iter.More(); iter.Next())
        {
            assert(modified.Extent() == 1);
            assert(!iter.Value().IsNull());
            assert(iter.Value().ShapeType() == TopAbs_ShapeEnum::TopAbs_EDGE);
            _edgeDelta.modified[oldEdgeInfo.shape] = iter.Value();
        }
    }

    // 修改的面
    for (const TopoShapeInfo& oldFaceInfo : _oldFaceInfoSet)
    {
        const TopTools_ListOfShape& modified = _mkShape.Modified(oldFaceInfo.shape);
        if (modified.IsEmpty()) continue;

        for (TopTools_ListIteratorOfListOfShape iter(modified); iter.More(); iter.Next())
        {
            assert(modified.Extent() == 1);
            assert(!iter.Value().IsNull());
            assert(iter.Value().ShapeType() == TopAbs_ShapeEnum::TopAbs_FACE);
            _faceDelta.modified[oldFaceInfo.shape] = iter.Value();
        }
    }
}

void MoveRotateTopoShapeComparer::recordAdded()
{
    return;
}

void MoveRotateTopoShapeComparer::init()
{
    // 基类初始化
    TopoShapeComparer::init();
}

NS_WY3D_END