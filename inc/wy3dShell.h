///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2025 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY3D_SHELL_H
#define WY3D_SHELL_H

#include <vector>
#include <wy3dDefs.h>
#include <wy3dSolid.h>
#include <wy3dSolidModification.h>
#include <wy3dTableIndex.h>

NS_WY3D_BEG

// 抽壳方向
enum class ShellDirection : std::int32_t
{
    Inward  = 0,  // 向内抽壳
    Outward = 1,  // 向外抽壳
};

class WY3D_EXPORT Shell : public wy3d::SolidModification
{
    WYDB_DECLARE_MEMBERS(Shell, wy3d::Shell, wy3d::SolidModification)

public:
    // 创建抽壳
    static wy::ErrorStatus create(
        wydb::Transaction* pTrans,
        wy3d::Solid* pSolid,
        const std::vector<std::uint32_t>& faceIndices,
        double thickness,
        ShellDirection direction,
        Shell*& pOutShell);

    // 获取抽壳厚度
    double getThickness() const { return _thickness; }
    // 设置抽壳厚度
    wy::ErrorStatus setThickness(double thickness);

    // 获取抽壳方向
    ShellDirection getDirection() const { return _direction; }
    // 设置抽壳方向
    wy::ErrorStatus setDirection(ShellDirection direction);

    // 获取面集合
    const TopoNameList& getFaces() const { return _faceNames; }
    // 设置面集合
    wy::ErrorStatus setFaces(const TopoNameList& faces);

public:
    virtual wydb::ParameterValueUPtr getParameterValue(const std::string& className, const std::string& paramName) const override;
    virtual wy::ErrorStatus setParameterValue(const std::string& className, const std::string& paramName, const wydb::ParameterValue& paramValue) override;

protected:
    virtual bool getFieldValue(wydb::FieldId fieldId, std::any& value) override;
    virtual bool setFieldValue(wydb::FieldId fieldId, const std::any& value) override;
    virtual wy::ErrorStatus writeToFiler(wydb::OutFiler& filer) const override;
    virtual wy::ErrorStatus readFromFiler(wydb::InFiler& filer) override;
    virtual std::pair<bool, TopoDS_Shape> modifyOwnerShape(const TopoDS_Shape& shape, TopoNaming* pTopoNaming, wydb::ChainUpdateFeedbackCollector& feedbackCollector) override;

private:
    TopoNameList _faceNames;
    double _thickness;
    ShellDirection _direction;
};

NS_WY3D_END

#endif // WY3D_SHELL_H
