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

#include "wy3dSketchCurveGraph.h"

#include <cassert>
#include <wyVector2.h>
#include <wy3dSketchProfile.h>
#include <wydbDatabase.h>
#include <wy3dSketchEntity.h>
#include <wy3dSketchCurve.h>
#include <wy3dSketchLine.h>
#include <wy3dSketchCenterLine.h>
#include <wy3dSketchCircle.h>
#include <wy3dSketchArc.h>
#include <wy3dSketchEllipse.h>
#include <wy3dSketchEllipseArc.h>

NS_WY3D_BEG

SketchCurveGraph::SketchCurveGraph(const std::vector<const SketchCurve*>& curves, double tol)
    : _curves(curves), _tol(tol), _isValid(false)
{
    if (this->buildGraph())
    {
        _isValid = true;
    }
}

IntVector2 SketchCurveGraph::convertToIntVec2(const wy::Vector2& pnt, double tol)
{
    return IntVector2(
        static_cast<int64_t>(std::round(pnt.x() / tol)),
        static_cast<int64_t>(std::round(pnt.y() / tol)));
}

bool SketchCurveGraph::buildGraph()
{
    size_t n = _curves.size();

    // 构建起点映射
    std::map<IntVector2, std::list<size_t>> startPntMapping;
    for (size_t i = 0; i < n; ++i)
    {
        std::list<size_t>& indices = startPntMapping[convertToIntVec2(_curves[i]->getStartPoint(), _tol)];
        if (indices.size() > 1)
        {
            _pError = this->newErrorWithIndices(ErrorCode::SKETCH_MoreThanTwoCurvesAtOneEndPoint,
                _curves[i]->getId(), indices);
            assert(false);
            return false;
        }
        indices.push_back(i);
    }

    // 构建终点映射
    std::map<IntVector2, std::list<size_t>> endPntMapping;
    for (size_t i = 0; i < n; ++i)
    {
        std::list<size_t>& indices = endPntMapping[convertToIntVec2(_curves[i]->getEndPoint(), _tol)];
        if (indices.size() > 1)
        {
            _pError = this->newErrorWithIndices(ErrorCode::SKETCH_MoreThanTwoCurvesAtOneEndPoint,
                _curves[i]->getId(), indices);
            assert(false);
            return false;
        }
        indices.push_back(i);
    }

    _endPointAdjacency.resize(n);
    _startPointAdjacency.resize(n);

    // 对每条曲线查找正向和反向连接
    // 终点
    for (size_t i = 0; i < n; i++)
    {
        assert(_curves[i]);
        IntVector2 endPnt = convertToIntVec2(_curves[i]->getEndPoint(), _tol);

        // 正向连接：查找所有以该点为起点的曲线
        auto iterSP = startPntMapping.find(endPnt);
        if (iterSP != startPntMapping.cend())
        {
            for (size_t j : iterSP->second)
            {
                if (j != i)
                {
                    _endPointAdjacency[i].push_back({ j, Orientation::Normal });
                }
            }
        }

        // 反向连接：查找所有以该点为终点的曲线
        auto iterEP = endPntMapping.find(endPnt);
        if (iterEP != endPntMapping.cend())
        {
            for (size_t j : iterEP->second)
            {
                if (j != i)
                {
                    _endPointAdjacency[i].push_back({ j, Orientation::Reversed });
                }
            }
        }
    }
    // 起点
    for (size_t i = 0; i < n; i++)
    {
        assert(_curves[i]);
        IntVector2 startPnt = convertToIntVec2(_curves[i]->getStartPoint(), _tol);

        // 正向连接：查找所有以该点为起点的曲线
        auto iterSP = startPntMapping.find(startPnt);
        if (iterSP != startPntMapping.cend())
        {
            for (size_t j : iterSP->second)
            {
                if (j != i)
                {
                    _startPointAdjacency[i].push_back({ j, Orientation::Normal });
                }
            }
        }

        // 反向连接：查找所有以该点为终点的曲线
        auto iterEP = endPntMapping.find(startPnt);
        if (iterEP != endPntMapping.cend())
        {
            for (size_t j : iterEP->second)
            {
                if (j != i)
                {
                    _startPointAdjacency[i].push_back({ j, Orientation::Reversed });
                }
            }
        }
    }

    return true;
}

std::shared_ptr<SketchError> SketchCurveGraph::newError(
    ErrorCode errorType,
    wydb::ElementId id,
    const std::vector<CurveEntry>& curveEntries) const
{
    std::shared_ptr<SketchError> pError = std::make_shared<SketchError>();
    pError->type = errorType;
    pError->ids.emplace_back(id);
    for (const CurveEntry& entry : curveEntries)
    {
        pError->ids.emplace_back(_curves[entry.index]->getId());
    }
    return pError;
}

std::shared_ptr<SketchError> SketchCurveGraph::newErrorWithIndices(
    ErrorCode errorType,
    wydb::ElementId id,
    const std::list<size_t>& indices) const
{
    std::shared_ptr<SketchError> pError = std::make_shared<SketchError>();
    pError->type = errorType;
    pError->ids.emplace_back(id);
    for (size_t idx : indices)
    {
        pError->ids.emplace_back(_curves[idx]->getId());
    }
    return pError;
}

NS_WY3D_END