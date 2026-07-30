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

#include "OsgSelectUtils.h"
#include <cassert>

void OsgSelectUtils::initPolytope(osg::Camera* pCamera,
    double winXMin, double winYMin, double winXMax, double winYMax,
    osg::Polytope& outPolytope)
{
    if (!pCamera)
    {
        assert(false);
        return;
    }

    osg::Matrix matrix;
    if (pCamera->getViewport())
    {
        matrix.preMult(pCamera->getViewport()->computeWindowMatrix());
    }
    matrix.preMult(pCamera->getProjectionMatrix());
    matrix.preMult(pCamera->getViewMatrix());

    if (winXMin > winXMax) std::swap(winXMin, winXMax);
    if (winYMin > winYMax) std::swap(winYMin, winYMax);
    osg::Polytope polytope;
    polytope.add(osg::Plane(1.0, 0.0, 0.0, -winXMin));
    polytope.add(osg::Plane(-1.0, 0.0, 0.0, winXMax));
    polytope.add(osg::Plane(0.0, 1.0, 0.0, -winYMin));
    polytope.add(osg::Plane(0.0, -1.0, 0.0, winYMax));
    polytope.add(osg::Plane(0.0, 0.0, 1.0, 0.0));
    outPolytope.setAndTransformProvidingInverse(polytope, matrix);

    return;
}

