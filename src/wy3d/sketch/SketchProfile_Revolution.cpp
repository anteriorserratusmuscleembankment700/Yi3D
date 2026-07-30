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

#include <wy3dSketchProfile_Revolution.h>
#include <wydbDatabase.h>
#include <wy3dSketchCenterLine.h>

NS_WY3D_BEG

SketchProfile_Revolution::SketchProfile_Revolution(const Sketch* pSketch, double tol)
    : SketchProfile(pSketch, tol)
{
}

bool SketchProfile_Revolution::preValid(const wydb::Database* pDb)
{
    assert(pDb);
    assert(_pSketch);

    unsigned int numCenterLines(0);
    for (auto iter = _pSketch->createIterator(); !iter.isDone(); iter.moveNext())
    {
        const wy3d::SketchCenterLine* pCenterLine = wy3d::SketchCenterLine::cast(pDb->getElement(iter.current()));
        if (pCenterLine)
        {
            ++numCenterLines;
        }
    }
    
    if (1 == numCenterLines)
    {
        return true;
    }
    else
    {
        if (0 == numCenterLines)
        {
            _pError = std::make_shared<SketchError>();
            _pError->type = ErrorCode::REVOLUTION_NoRevolutionAxisLine;
        }
        else // numCenterLines > 1
        {
            _pError = std::make_shared<SketchError>();
            _pError->type = ErrorCode::REVOLUTION_MoreThanOneRevolutionAxisLine;
        }
        return false;
    }
}

NS_WY3D_END