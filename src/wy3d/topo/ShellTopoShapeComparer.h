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

#ifndef WY3D_SHELL_TOPO_SHAPE_COMPARER_H
#define WY3D_SHELL_TOPO_SHAPE_COMPARER_H

#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <wy3dDefs.h>
#include "topo/TopoShapeComparer.h"

NS_WY3D_BEG

// 抽壳拓扑元素比较器
class ShellTopoShapeComparer : public TopoShapeComparer
{
public:
    ShellTopoShapeComparer(BRepOffsetAPI_MakeThickSolid& mkShell, const TopoDS_Shape& oldShape);
    ~ShellTopoShapeComparer();

protected:
    // 初始化
    virtual void init() override;
    // 修改的
    virtual void recordModified() override;
    // 新增的
    virtual void recordAdded() override;

private:
    BRepOffsetAPI_MakeThickSolid& _mkShell;
};

NS_WY3D_END

#endif // WY3D_SHELL_TOPO_SHAPE_COMPARER_H