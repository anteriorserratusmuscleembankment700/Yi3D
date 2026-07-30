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

#ifndef WY_OSG_SELECT_UTILS_H
#define WY_OSG_SELECT_UTILS_H

#include <osg/Camera>
#include <osg/Polytope>

class OsgSelectUtils
{
public:
    // 依据Window坐标系下的范围构造世界坐标系下的多面体
    // Input: pCamera & winXMin & winYMin & winXMax & winYMax
    // Output: polytope
    static void initPolytope(osg::Camera* pCamera,
        double winXMin, double winYMin, double winXMax, double winYMax,
        osg::Polytope& polytope);
};

#endif // WY_OSG_SELECT_UTILS_H