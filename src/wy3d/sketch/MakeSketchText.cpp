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

#include <wyVector2.h>
#include <wy3dMakeSketchText.h>
#include <cassert>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_SFNT_NAMES_H
#include FT_TRUETYPE_IDS_H
#include <wy3dVector2.h>
#include <wy3dSketchLine.h>
#include <wy3dSketchSpline.h>
#include <wydbTransaction.h>
#include "utils/FreeTypeUtil.h"

NS_WY3D_BEG

class FreeTypeObject
{
public:
    FreeTypeObject(const std::string& fontFilePath, FT_Long faceIndex)
        : _library(nullptr), _face(nullptr), _errorStatus(wy::ErrorStatus::Ok)
    {
        FT_Error error(0);
        
        error = FT_Init_FreeType(&_library);
        if (error || !_library)
        {
            _errorStatus = wy::ErrorStatus::InitFreeTypeError;
            return;
        }

        error = FT_New_Face(_library, fontFilePath.c_str(), faceIndex, &_face);
        if (error || !_face)
        {
            _errorStatus = wy::ErrorStatus::FreeTypeLoadFontFaceError;
            return;
        }
    }

    ~FreeTypeObject()
    {
        FT_Error error(0);

        if (_face)
        {
            error = FT_Done_Face(_face);
            assert(0 == error);
        }

        if (_library)
        {
            error = FT_Done_FreeType(_library);
            assert(0 == error);
        }
    }

    FT_Face getFace() const
    {
        return _face;
    }

    wy::ErrorStatus getErrorStatus() const
    {
        return _errorStatus;
    }

private:
    FT_Library _library;
    FT_Face _face;
    wy::ErrorStatus _errorStatus;
};

MakeSketchText::MakeSketchText(const Data& data) : _pFreeTypeObj(nullptr), _data(data)
{
    _pFreeTypeObj = new FreeTypeObject(_data.fontFilePath, _data.fontFaceIndex);
}

MakeSketchText::~MakeSketchText()
{
    if (_pFreeTypeObj)
    {
        delete _pFreeTypeObj;
        _pFreeTypeObj = nullptr;
    }
}

inline wy::Vector2 TO_PNT(const FT_Vector& ftPnt, double ratioX, double ratioY)
{
    return wy::Vector2(ratioX * ftPnt.x, ratioY * ftPnt.y);
}

wy::ErrorStatus MakeSketchText::perform(
    wydb::Database* pDb,
    wydb::Transaction* pTrans,
    wy3d::Sketch* pSketch)
{
    // 字体
    if (!_pFreeTypeObj)
    {
        assert(false);
        return wy::ErrorStatus::Error;
    }
    if (_pFreeTypeObj->getErrorStatus() != wy::ErrorStatus::Ok)
    {
        return _pFreeTypeObj->getErrorStatus();
    }
    FT_Face face = _pFreeTypeObj->getFace();
    if (nullptr == face)
    {
        assert(false);
        return wy::ErrorStatus::Error;
    }

    // 转换文本为FreeType编码
    std::vector<FT_ULong> codes = FreeTypeUtil::wstringToCharCodes(_data.text);
    if (codes.empty())
    {
        assert(false);
        return wy::ErrorStatus::FreeTypeDecodeError;
    }

    // 文字缩放
    double ratio = _data.height / std::fabs(face->ascender - face->descender);

    // 遍历文字编码
    wy::Vector2 nextTextPos(0.0, 0.0);
    wy::Vector2 textPos(0.0, 0.0);
    for (FT_ULong code : codes)
    {
        textPos = nextTextPos;

        // 加载文字
        FT_UInt charIndex = FT_Get_Char_Index(face, code);
        FT_Error error = FT_Load_Glyph(face, charIndex, FT_LOAD_NO_SCALE);
        if (error)
        {
            assert(false);
            continue;
        }

        // 文字轮廓
        if (!face->glyph)
        {
            assert(false);
            continue;
        }
        const FT_Outline& outline = face->glyph->outline;

        // commented by wangyao 2025.08.27 {
        // FT_Outline_Get_CBox函数获取的是字形轮廓的控制盒(Control Box),它不完全等同于视觉上的"实际包围盒",
        // 但在大多数场景下可以近似作为包围盒使用.其核心特点是:包含轮廓的所有控制点(贝塞尔曲线的控制点),可能比视觉可见的字形范围略大.
        //    xMin  xMax  yMin  yMax
        // a  11    115   3     119
        // b  12    113   3     176
        // g  10    119  -32    120
        //FT_BBox bbox;
        //FT_Outline_Get_CBox(&outline, &bbox);
        // }

        // 下一个文字的位置
        nextTextPos.setX(textPos.x() + ratio * static_cast<double>(face->glyph->metrics.horiAdvance) + _data.horizontalSpacing);
        nextTextPos.setY(textPos.y());

        // 遍历轮廓
        short startPntIdx = 0;
        short endPntIdx = 0;
        for (short i = 0; i < outline.n_contours; ++i)
        {
            // 轮廓终止点索引
            endPntIdx = outline.contours[i];
            // 轮廓的第一个端点索引
            short indexOfFirstEndPnt = startPntIdx;

            // 起始控制点
            // 黑体的'武'字第三个轮廓的起点就是贝塞尔控制点
            std::vector<wy::Vector2> startCubicControlPoints;
            std::vector<wy::Vector2> startConicControlPoints;
            assert(startPntIdx < outline.n_points);
            if (!(outline.tags[startPntIdx] & FT_CURVE_TAG_ON)) // 轮廓起始点是贝塞尔控制点
            {
                for (short j = startPntIdx; j <= endPntIdx; ++j)
                {
                    const FT_Vector& point = outline.points[j];
                    char pointFlag = outline.tags[j];
                    if (pointFlag & FT_CURVE_TAG_ON) // 端点
                    {
                        indexOfFirstEndPnt = j;
                        break;
                    }

                    if (pointFlag & FT_CURVE_TAG_CUBIC) // 三次贝塞尔控制点
                    {
                        startCubicControlPoints.emplace_back(TO_PNT(point, ratio, ratio));
                    }
                    else // 二次贝塞尔控制点
                    {
                        startConicControlPoints.emplace_back(TO_PNT(point, ratio, ratio));
                    }
                }
                
                // 说明整个轮廓全部是贝塞尔控制点,也说明是闭合的贝塞尔曲线
                if (indexOfFirstEndPnt == startPntIdx)
                {
                    if (!startCubicControlPoints.empty())
                    {
                        startCubicControlPoints.push_back(startCubicControlPoints.front());
                    }
                    if (!startConicControlPoints.empty())
                    {
                        startConicControlPoints.push_back(startConicControlPoints.front());
                    }
                    this->createClosedSpline(pDb, pTrans, pSketch, startCubicControlPoints, startConicControlPoints, textPos);
                    startPntIdx = endPntIdx + 1;
                    continue;
                }
            }

            std::vector<wy::Vector2> endPoints;
            std::vector<wy::Vector2> cubicControlPoints;
            std::vector<wy::Vector2> conicControlPoints;
            for (short j = indexOfFirstEndPnt; j <= endPntIdx; ++j)
            {
                const FT_Vector& point = outline.points[j];
                char pointFlag = outline.tags[j];
                if (pointFlag & FT_CURVE_TAG_ON) // 端点
                {
                    if (endPoints.empty())
                    {
                        endPoints.emplace_back(TO_PNT(point, ratio, ratio));
                    }
                    else if (endPoints.size() == 1)
                    {
                        this->createOpenCurve(pDb, pTrans, pSketch,
                            endPoints.front(), TO_PNT(point, ratio, ratio),
                            cubicControlPoints, conicControlPoints, textPos);

                        cubicControlPoints.clear();
                        conicControlPoints.clear();
                        endPoints[0] = TO_PNT(point, ratio, ratio);
                    }
                    else
                    {
                        assert(false);
                    }
                }
                else if (pointFlag & FT_CURVE_TAG_CUBIC) // 三次贝塞尔控制点
                {
                    cubicControlPoints.emplace_back(TO_PNT(point, ratio, ratio));
                }
                else // 二次贝塞尔控制点(FT_CURVE_TAG_CONIC==0)
                {
                    conicControlPoints.emplace_back(TO_PNT(point, ratio, ratio));
                }
            }

            // 闭合轮廓
            if (startPntIdx == indexOfFirstEndPnt)
            {
                this->createOpenCurve(pDb, pTrans, pSketch,
                    endPoints.front(), TO_PNT(outline.points[indexOfFirstEndPnt], ratio, ratio),
                    cubicControlPoints, conicControlPoints, textPos);
            }
            else
            {
                conicControlPoints.insert(conicControlPoints.cend(), startConicControlPoints.cbegin(), startConicControlPoints.cend());
                this->createOpenCurve(pDb, pTrans, pSketch,
                    endPoints.front(), TO_PNT(outline.points[indexOfFirstEndPnt], ratio, ratio),
                    cubicControlPoints, conicControlPoints, textPos);
            }

            // 下一个轮廓的起始点索引
            startPntIdx = endPntIdx + 1;
        }
    }

    return wy::ErrorStatus::Ok;
}

bool MakeSketchText::createOpenCurve(
    wydb::Database* pDb,
    wydb::Transaction* pTrans,
    wy3d::Sketch* pSketch,
    const wy::Vector2& startPoint,
    const wy::Vector2& endPoint,
    const std::vector<wy::Vector2>& cubicControlPoints,
    const std::vector<wy::Vector2>& conicControlPoints,
    const wy::Vector2& position)
{
    assert(pDb);
    assert(pTrans);
    assert(pSketch);

    wy::ErrorStatus error(wy::ErrorStatus::Ok);
    wy3d::SketchSpline* pOutSpline(nullptr);
    if (!cubicControlPoints.empty()) // 三次B样条
    {
        std::vector<wy::Vector2> controlPoints;
        controlPoints.reserve(2 + cubicControlPoints.size());
        controlPoints.push_back(startPoint);
        controlPoints.insert(controlPoints.cend(), cubicControlPoints.cbegin(), cubicControlPoints.cend());
        controlPoints.push_back(endPoint);

        if (wy::ErrorStatus::Ok != wy3d::SketchSpline::create(pTrans, 3, controlPoints, pOutSpline) || !pOutSpline)
        {
            assert(false);
            return false;
        }
        assert(pOutSpline);
        error = pSketch->addEntity(pOutSpline);
        assert(wy::ErrorStatus::Ok == error);
        error = pOutSpline->translate(position);
        assert(wy::ErrorStatus::Ok == error);

        _curveIds.insert(pOutSpline->getId());
    }
    else if (!conicControlPoints.empty()) // 二次B样条
    {
        std::vector<wy::Vector2> controlPoints;
        controlPoints.reserve(2 + conicControlPoints.size());
        controlPoints.push_back(startPoint);
        controlPoints.insert(controlPoints.cend(), conicControlPoints.cbegin(), conicControlPoints.cend());
        controlPoints.push_back(endPoint);
        if (wy::ErrorStatus::Ok != wy3d::SketchSpline::create(pTrans, 2, controlPoints, pOutSpline) || !pOutSpline)
        {
            assert(false);
            return false;
        }
        assert(pOutSpline);
        error = pSketch->addEntity(pOutSpline);
        assert(wy::ErrorStatus::Ok == error);
        error = pOutSpline->translate(position);
        assert(wy::ErrorStatus::Ok == error);

        _curveIds.insert(pOutSpline->getId());
    }
    else // 直线段
    {
        wy3d::SketchLine* pOutLine(nullptr);
        if (wy::ErrorStatus::Ok != wy3d::SketchLine::create(pTrans, startPoint, endPoint, pOutLine) || !pOutLine)
        {
            return false;
        }
        assert(pOutLine);
        error = pSketch->addEntity(pOutLine);
        assert(wy::ErrorStatus::Ok == error);
        error = pOutLine->translate(position);
        assert(wy::ErrorStatus::Ok == error);

        _curveIds.insert(pOutLine->getId());
    }

    return true;
}

bool MakeSketchText::createClosedSpline(
    wydb::Database* pDb,
    wydb::Transaction* pTrans,
    wy3d::Sketch* pSketch,
    const std::vector<wy::Vector2>& cubicControlPoints,
    const std::vector<wy::Vector2>& conicControlPoints,
    const wy::Vector2& position)
{
    assert(pDb);
    assert(pTrans);
    assert(pSketch);

    wy::ErrorStatus error(wy::ErrorStatus::Ok);
    wy3d::SketchSpline* pOutSpline(nullptr);
    if (!cubicControlPoints.empty()) // 三次B样条
    {
        if (wy::ErrorStatus::Ok != wy3d::SketchSpline::create(pTrans, 3, cubicControlPoints, pOutSpline) || !pOutSpline)
        {
            return false;
        }
    }
    else if (!conicControlPoints.empty()) // 二次B样条
    {
        if (wy::ErrorStatus::Ok != wy3d::SketchSpline::create(pTrans, 2, conicControlPoints, pOutSpline) || !pOutSpline)
        {
            return false;
        }
    }
    else
    {
        assert(false);
        return false;
    }

    _curveIds.insert(pOutSpline->getId());

    assert(pOutSpline);
    error = pSketch->addEntity(pOutSpline);
    assert(wy::ErrorStatus::Ok == error);
    error = pOutSpline->translate(position);
    assert(wy::ErrorStatus::Ok == error);

    return true;
}

NS_WY3D_END