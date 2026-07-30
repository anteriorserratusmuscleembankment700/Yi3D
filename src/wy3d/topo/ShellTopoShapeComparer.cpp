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

#include "topo/ShellTopoShapeComparer.h"
#include <cassert>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>

NS_WY3D_BEG

ShellTopoShapeComparer::ShellTopoShapeComparer(
    BRepOffsetAPI_MakeThickSolid& mkShell, const TopoDS_Shape& oldShape)
    : TopoShapeComparer(mkShell, oldShape), _mkShell(mkShell)
{
}

ShellTopoShapeComparer::~ShellTopoShapeComparer()
{
}


void ShellTopoShapeComparer::recordModified()
{
    // 修改的边
    for (const TopoShapeInfo& oldEdgeInfo : _oldEdgeInfoSet)
    {
        const TopTools_ListOfShape& modified = _mkShape.Modified(oldEdgeInfo.shape);
        int size = modified.Size();
        if (0 == size)
        {
            continue;
        }
        else if (1 == size)
        {
            const TopoDS_Shape& shape = modified.First();
            assert(!shape.IsNull());
            assert(shape.ShapeType() == TopAbs_ShapeEnum::TopAbs_EDGE);
            _edgeDelta.modified[oldEdgeInfo.shape] = shape;
        }
        else // > 1 这个在实践中是否存在待进一步观察
        {
            assert(false); // 如果后续存在请注释掉此代码
            unsigned int index(1); // 序号从1开始
            for (TopTools_ListIteratorOfListOfShape iter(modified); iter.More(); iter.Next(), ++index)
            {
                assert(!iter.Value().IsNull());
                assert(iter.Value().ShapeType() == TopAbs_ShapeEnum::TopAbs_EDGE);
                _edgeDelta.addedSingle[iter.Value()] = ShapeDelta::SingleSourceInfo::split(oldEdgeInfo.shape, index);
            }
        }
    }

    // 修改的面
    for (const TopoShapeInfo& oldFaceInfo : _oldFaceInfoSet)
    {
        const TopTools_ListOfShape& modified = _mkShape.Modified(oldFaceInfo.shape);
        int size = modified.Size();
        if (0 == size)
        {
            continue;
        }
        else if (1 == size)
        {
            const TopoDS_Shape& shape = modified.First();
            assert(!shape.IsNull());
            assert(shape.ShapeType() == TopAbs_ShapeEnum::TopAbs_FACE);
            _faceDelta.modified[oldFaceInfo.shape] = shape;
        }
        else // > 1 这个在实践中是存在的,具体请参看issue-002.y3dt
        {
            unsigned int index(1); // 序号从1开始
            for (TopTools_ListIteratorOfListOfShape iter(modified); iter.More(); iter.Next(), ++index)
            {
                assert(!iter.Value().IsNull());
                assert(iter.Value().ShapeType() == TopAbs_ShapeEnum::TopAbs_FACE);
                _faceDelta.addedSingle[iter.Value()] = ShapeDelta::SingleSourceInfo::split(oldFaceInfo.shape, index);
            }
        }
    }
}

void ShellTopoShapeComparer::recordAdded()
{
    // 旧边 >>>偏移>>> 新边
    for (const TopoShapeInfo& oldEdgeInfo : _oldEdgeInfoSet)
    {
        const TopTools_ListOfShape& generated = _mkShape.Generated(oldEdgeInfo.shape);
        if (generated.IsEmpty()) continue;

        unsigned int index(0);
        if (generated.Size() > 1)
        {
            assert(false); // 目前理论上应该是1>>>1的关系,如果后续实践中确认存在1>>>多的关系,请删除此代码
            index = 1;
        }
        for (TopTools_ListIteratorOfListOfShape iter(generated); iter.More(); iter.Next())
        {
            assert(!iter.Value().IsNull());
            assert(iter.Value().ShapeType() == TopAbs_ShapeEnum::TopAbs_EDGE);
            _edgeDelta.addedSingle[iter.Value()] = 0 == index
                ? ShapeDelta::SingleSourceInfo::generated(oldEdgeInfo.shape)
                : ShapeDelta::SingleSourceInfo::generatedMultiple(oldEdgeInfo.shape, index);
            ++index;
        }
    }

    // 旧面 >>>偏移>>> 新面
    for (const TopoShapeInfo& oldFaceInfo : _oldFaceInfoSet)
    {
        const TopTools_ListOfShape& generated = _mkShape.Generated(oldFaceInfo.shape);
        if (generated.IsEmpty()) continue;

        unsigned int index(0);
        if (generated.Size() > 1)
        {
            assert(false); // 目前理论上应该是1>>>1的关系,如果后续实践中确认存在1>>>多的关系,请删除此代码
            index = 1;
        }
        for (TopTools_ListIteratorOfListOfShape iter(generated); iter.More(); iter.Next())
        {
            assert(!iter.Value().IsNull());
            assert(iter.Value().ShapeType() == TopAbs_ShapeEnum::TopAbs_FACE);
            _faceDelta.addedSingle[iter.Value()] = 0 == index
                ? ShapeDelta::SingleSourceInfo::generated(oldFaceInfo.shape)
                : ShapeDelta::SingleSourceInfo::generatedMultiple(oldFaceInfo.shape, index);
            ++index;
        }
    }

    //// 已经记录的边
    //TopoShapeSet recordEdges;
    //recordEdges.insert(_edgeDelta.kept.cbegin(), _edgeDelta.kept.cend());
    //for (const auto& kvp : _edgeDelta.modified) // old <> new
    //{
    //    recordEdges.insert(kvp.second);
    //}
    //for (const auto& kvp : _edgeDelta.addedSingle)
    //{
    //    recordEdges.insert(kvp.first);
    //}
    //for (const auto& kvp : _edgeDelta.addedDouble)
    //{
    //    recordEdges.insert(kvp.first);
    //}
    //for (const auto& kvp : _edgeDelta.addedMulti)
    //{
    //    recordEdges.insert(kvp.first);
    //}

    //// 建立新生成形体中:边<>面的映射
    //TopTools_IndexedDataMapOfShapeListOfShape newEdgeToFacesMap;
    //TopExp::MapShapesAndAncestors(_newShape, TopAbs_EDGE, TopAbs_FACE, newEdgeToFacesMap);

    //// 遍历新添加的面
    //for (const auto& kvp : newFace2OldIndex)
    //{
    //    const TopoDS_Shape& newFace = kvp.first;
    //    TopTools_IndexedMapOfShape edgeMap;
    //    TopExp::MapShapes(newFace, TopAbs_ShapeEnum::TopAbs_EDGE, edgeMap);
    //    for (int i = 1; i <= edgeMap.Extent(); ++i)
    //    {
    //        const TopoDS_Shape& edgeShape = edgeMap(i);
    //        if (recordEdges.find(edgeShape) != recordEdges.cend()) // 已经记录了
    //        {
    //            continue;
    //        }

    //        const TopTools_ListOfShape& faceList = newEdgeToFacesMap.FindFromKey(edgeShape);
    //        int numFaces = faceList.Extent();
    //        switch (numFaces)
    //        {
    //        case 2:
    //        {
    //            TopTools_ListIteratorOfListOfShape faceIt(faceList);
    //            const TopoDS_Shape& face1 = faceIt.Value();
    //            faceIt.Next();
    //            const TopoDS_Shape& face2 = faceIt.Value();

    //            auto iter1 = newFace2OldIndex.find(face1);
    //            auto iter2 = newFace2OldIndex.find(face2);
    //            ShapeDelta::DoubleSourceInfo doubleSourceInfo;
    //            if (iter1 != newFace2OldIndex.cend() && iter2 != newFace2OldIndex.cend())
    //            {
    //                assert(iter1->second != iter2->second);
    //                // 索引小的在前,索引大的在后
    //                if (iter1->second <= iter2->second)
    //                {
    //                    doubleSourceInfo.source1 = face1;
    //                    doubleSourceInfo.source2 = face2;
    //                }
    //                else
    //                {
    //                    doubleSourceInfo.source1 = face2;
    //                    doubleSourceInfo.source2 = face1;
    //                }
    //            }
    //            else
    //            {
    //                assert(false);
    //                doubleSourceInfo.source1 = face1;
    //                doubleSourceInfo.source2 = face2;
    //            }
    //            _edgeDelta.addedDouble[edgeShape] = doubleSourceInfo;
    //            recordEdges.insert(edgeShape);
    //        }
    //        break;

    //        case 1:
    //        {
    //            assert(false); // 感觉应该不太可能
    //            TopTools_ListIteratorOfListOfShape faceIt(faceList);
    //            const TopoDS_Shape& face = faceIt.Value();
    //            _edgeDelta.addedSingle[edgeShape] = ShapeDelta::SingleSourceInfo::generated(face);
    //            recordEdges.insert(edgeShape);
    //        }
    //        break;

    //        default:
    //        {
    //            assert(false);
    //        }
    //        break;
    //        }
    //    }
    //}
}

void ShellTopoShapeComparer::init()
{
    // 基类初始化
    TopoShapeComparer::init();
}

NS_WY3D_END
