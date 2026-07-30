///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY3D_FEATURE_H
#define WY3D_FEATURE_H

#include <wydbElement.h>
#include <wy3dDefs.h>

NS_WY3D_BEG

class WY3D_EXPORT Feature : public wydb::Element
{
    WYDB_DECLARE_ABSTRACT_MEMBERS(Feature, wy3d::Feature, wydb::Element)


public:
    // 参数

protected:
    virtual void onChainUpdate(
        const wydb::ElementDataPiece& dirtyDataPiece,
        wydb::ChainUpdateFeedbackCollector& feedbackCollector,
        wydb::ChainUpdateCallbackManager& callbackManager) override;

    virtual void onChainUpdater_Completion(
        const wydb::ElementDataPiece& dirtyDataPiece,
        wydb::ChainUpdateFeedbackCollector& feedbackCollector);
};

NS_WY3D_END

#endif // WY3D_FEATURE_H