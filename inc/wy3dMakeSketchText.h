///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2025 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY3D_MAKE_SKETCH_TEXT_H
#define WY3D_MAKE_SKETCH_TEXT_H

#include <string>
#include <set>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include <wyVector2.h>
#include <wydbElementId.h>
#include <wydbDatabase.h>
#include <wydbTransaction.h>
#include <wy3dDefs.h>
#include <wy3dSketch.h>

NS_WY3D_BEG

class FreeTypeObject;

class WY3D_EXPORT MakeSketchText
{
public:
    struct Data
    {
        // 文本内容
        std::wstring text;

        // 字体文件路径
        std::string fontFilePath;
        // 字体面序号
        FT_Long fontFaceIndex;

        // 字体高度
        double height;
        // 字体水平间距
        double horizontalSpacing;

        Data() : fontFaceIndex(0), height(10.0), horizontalSpacing(1.0) {}
    };

    explicit MakeSketchText(const Data& data);
    ~MakeSketchText();

    wy::ErrorStatus perform(
        wydb::Database* pDb,
        wydb::Transaction* pTrans,
        wy3d::Sketch* pSketch);

    // 获取创建的曲线ID集合
    const std::set<wydb::ElementId>& getCreatedCurves() const
    {
        return _curveIds;
    }

private:
    bool createOpenCurve(
        wydb::Database* pDb,
        wydb::Transaction* pTrans,
        wy3d::Sketch* pSketch,
        const wy::Vector2& startPoint,
        const wy::Vector2& endPoint,
        const std::vector<wy::Vector2>& cubicControlPoints,
        const std::vector<wy::Vector2>& conicControlPoints,
        const wy::Vector2& position);

    bool createClosedSpline(
        wydb::Database* pDb,
        wydb::Transaction* pTrans,
        wy3d::Sketch* pSketch,
        const std::vector<wy::Vector2>& cubicControlPoints,
        const std::vector<wy::Vector2>& conicControlPoints,
        const wy::Vector2& position);

private:
    Data _data;
    FreeTypeObject* _pFreeTypeObj;
    std::set<wydb::ElementId> _curveIds;
};

NS_WY3D_END

#endif // WY3D_MAKE_SKETCH_TEXT_H