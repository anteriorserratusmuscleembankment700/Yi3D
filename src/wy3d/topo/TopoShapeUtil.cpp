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

#include <wyVector3.h>
#include "topo/TopoShapeUtil.h"
#include <BRep_Builder.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Plane.hxx>

NS_WY3D_BEG

TopoDS_Compound TopoShapeUtil::makeCompound(const TopoDS_Shape& shape1, const TopoDS_Shape& shape2)
{
    BRep_Builder brepBuilder;
    TopoDS_Compound compound;
    brepBuilder.MakeCompound(compound);
    brepBuilder.Add(compound, shape1);
    brepBuilder.Add(compound, shape2);
    return compound;
}

static inline wy::Vector3 _toVector3(const gp_Pnt& pnt)
{
    return wy::Vector3(pnt.X(), pnt.Y(), pnt.Z());
}
static inline wy::Vector3 _toVector3(const gp_Dir& dir)
{
    return wy::Vector3(dir.X(), dir.Y(), dir.Z());
}

bool TopoShapeUtil::getFacePlane(const TopoDS_Face& face, wy3d::SketchPlane& plane)
{
    if (face.IsNull()) return false;
    Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
    if (surface.IsNull()) return false;
    Handle(Geom_Plane) planeSurf = Handle(Geom_Plane)::DownCast(surface);
    if (planeSurf.IsNull()) return false;

    const gp_Ax3& ax3 = planeSurf->Position();
    const gp_Pnt& origin = ax3.Location();
    const gp_Dir& xAxis = ax3.XDirection();
    const gp_Dir& zAxis = ax3.Direction();

    TopAbs_Orientation orient = face.Orientation();
    if (orient == TopAbs_REVERSED)
    {
        plane = wy3d::SketchPlane(
            _toVector3(origin),
            _toVector3(zAxis.Reversed()),
            _toVector3(xAxis.Reversed()));
    }
    else
    {
        plane = wy3d::SketchPlane(
            _toVector3(origin),
            _toVector3(zAxis),
            _toVector3(xAxis));
    }
    if (plane.isValid()) return true;
    else return false;
}

NS_WY3D_END
