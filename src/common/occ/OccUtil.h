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

#ifndef WYCOMMON_OCC_UTIL_H
#define WYCOMMON_OCC_UTIL_H

#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <wyVector3.h>
#include <wy3dVector3.h>

class OccUtil
{
public:
    inline static gp_Pnt toPnt(const wy::Vector3& pnt)
    {
        return gp_Pnt(pnt.x(), pnt.y(), pnt.z());
    }

    inline static gp_Dir toDir(const wy::Vector3& dir)
    {
        return gp_Dir(dir.x(), dir.y(), dir.z());
    }
};

#endif // WYCOMMON_OCC_UTIL_H