///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2025 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY3D_PRIMITIVE_H
#define WY3D_PRIMITIVE_H

#include <wyVector3.h>
#include <wy3dDefs.h>
#include <wy3dSolid.h>

NS_WY3D_BEG

class WY3D_EXPORT Primitive : public wy3d::Solid
{
    WYDB_DECLARE_ABSTRACT_MEMBERS(Primitive, wy3d::Primitive, wy3d::Solid)

public:
    virtual wy::Vector3 getPosition() const { return _position; }
    virtual wy::ErrorStatus setPosition(const wy::Vector3& position);
    virtual wy::Vector3 getRotation() const { return _rotation; }
    virtual wy::ErrorStatus setRotation(const wy::Vector3& rotation);

public:

protected:
    virtual bool getFieldValue(wydb::FieldId fieldId, std::any& value) override;
    virtual bool setFieldValue(wydb::FieldId fieldId, const std::any& value) override;
    virtual wy::ErrorStatus writeToFiler(wydb::OutFiler& filer) const override;
    virtual wy::ErrorStatus readFromFiler(wydb::InFiler& filer) override;
    virtual TopoDS_Shape generateShape(TopoNaming* pTopoNaming, wydb::ChainUpdateFeedbackCollector& feedbackCollector) override;
    virtual TopoDS_Shape generateOriginalShape() const { return TopoDS_Shape(); }

protected:
    wy::Vector3 _position;
    wy::Vector3 _rotation;
};

NS_WY3D_END

#endif // WY3D_PRIMITIVE_H