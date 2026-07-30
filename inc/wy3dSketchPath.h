///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2025 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY3D_SKETCH_PATH_H
#define WY3D_SKETCH_PATH_H

#include <vector>
#include <wy3dDefs.h>
#include <wy3dSketchCurve.h>
#include <wy3dSketchCurveGraph.h>

NS_WY3D_BEG

class WY3D_EXPORT SketchPath
{
public:
    SketchPath(const Sketch* pSketch, double tol = 1e-5);

    // 校验
    bool check();

    // 获取错误
    std::shared_ptr<SketchError> getError() const { return _pError; }

    // 获取路径
    const std::vector<BiCurve>& getPath() const { return _path; }

private:
    // 初始化
    bool init();

private:
    const Sketch* _pSketch;
    double _tol;
    bool _isValid;

    // 路径
    std::vector<BiCurve> _path;

    // 错误信息
    std::shared_ptr<SketchError> _pError;
    std::shared_ptr<SketchError> newErrorOfUndefined() const;
};

class SketchCurveGraph_Path : public SketchCurveGraph
{
public:
    // 构造函数
    explicit SketchCurveGraph_Path(const std::vector<const SketchCurve*>& curves, double tol = 1e-5);

    // 查找路径
    bool findPath();

    // 获取路径
    const std::vector<CurveEntry>& getPath() const
    {
        return _pathCurves;
    }

private:
    enum class FindRet
    {
        Finished = 0,
        Continue = 1,
        Error    = 2,
    };
    FindRet findPathImpl(
        std::vector<bool>& visited,
        const CurveEntry& startCurveEntry,
        std::vector<CurveEntry>& pathCurves);

private:
    // 路径曲线
    std::vector<CurveEntry> _pathCurves;
};

NS_WY3D_END

#endif // WY3D_SKETCH_PATH_H