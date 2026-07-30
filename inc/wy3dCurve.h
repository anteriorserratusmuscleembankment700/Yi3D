///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2025 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY3D_CURVE_H
#define WY3D_CURVE_H

#include <TopoDS_Edge.hxx>
#include <wy3dDefs.h>
#include <wy3dFeature.h>

NS_WY3D_BEG

class WY3D_EXPORT Curve : public wy3d::Feature
{
    WYDB_DECLARE_ABSTRACT_MEMBERS(Curve, wy3d::Curve, wy3d::Feature)

public:
    virtual wydb::ElementId getParent() const override { return _ownerId; }
    wy::ErrorStatus setOwner(const wydb::ElementId& ownerId);

    TopoDS_Edge getEdge() const { return _edge; }

public:

protected:
    virtual bool getFieldValue(wydb::FieldId fieldId, std::any& value) override;
    virtual bool setFieldValue(wydb::FieldId fieldId, const std::any& value) override;
    virtual wy::ErrorStatus writeToFiler(wydb::OutFiler& filer) const override;
    virtual wy::ErrorStatus readFromFiler(wydb::InFiler& filer) override;
    virtual void reportDependencies(std::set<wydb::ElementId>& dependencies) const override;
    virtual bool onDependenciesErased(const std::set<wydb::ElementId>& erasedDependencies) override;
    virtual void onChainUpdater_Completion(
        const wydb::ElementDataPiece& dirtyDataPiece,
        wydb::ChainUpdateFeedbackCollector& feedbackCollector);
    virtual TopoDS_Edge generateShape(
        wydb::ChainUpdateFeedbackCollector& feedbackCollector) const { return TopoDS_Edge(); };

private:
    wy::ErrorStatus _setEdge(const TopoDS_Edge& edge);

protected:
    wydb::ElementId _ownerId;
    TopoDS_Edge _edge;
};

NS_WY3D_END

#endif // WY3D_CURVE_H