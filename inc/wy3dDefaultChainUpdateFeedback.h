///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2026 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY3D_DEFAULT_CHAIN_UPDATE_FEEDBACK_H
#define WY3D_DEFAULT_CHAIN_UPDATE_FEEDBACK_H

#include <string>

#include <wydbChainUpdateFeedback.h>
#include <wy3dDefs.h>

NS_WY3D_BEG

class WY3D_EXPORT DefaultChainUpdateFeedback : public wydb::ChainUpdateFeedback
{
public:
    DefaultChainUpdateFeedback(
        const wydb::ElementId& elementId,
        std::uint32_t code,
        wydb::Element::HealthState healthState,
        std::string message);
    virtual ~DefaultChainUpdateFeedback() = default;

    virtual wydb::ElementId getElementId() const override;
    virtual wydb::Element::HealthState getHealthState() const override;
    virtual std::uint32_t getCode() const override;
    virtual std::string getMessage() const override;

private:
    wydb::ElementId _elementId;
    std::uint32_t _code;
    wydb::Element::HealthState _healthState;
    std::string _message;
};

// Returns the default reported health state for the specified error code.
WY3D_EXPORT wydb::Element::HealthState getReportedHealthStateByErrorCode(std::uint32_t errorCode);

// Reports a healthy (no problem) chain-update feedback for the specified element.
// Call this before processing to ensure a healthy default — any error reported
// afterwards will override it.
WY3D_EXPORT wy::ErrorStatus reportChainUpdateSuccess(
    wydb::ChainUpdateFeedbackCollector& feedbackCollector,
    const wydb::ElementId& id);

// Reports a chain-update feedback converted from the specified error code.
// Appends the feedback to the collector.
WY3D_EXPORT wy::ErrorStatus reportChainUpdateError(
    wydb::ChainUpdateFeedbackCollector& feedbackCollector,
    const wydb::ElementId& id,
    std::uint32_t errorCode);

// Returns the code stored in the specified chain-update feedback.
// Returns 0 if pFeedback is nullptr.
WY3D_EXPORT std::uint32_t getErrorCodeFromChainUpdateFeedback(
    const wydb::ChainUpdateFeedback* pFeedback);

NS_WY3D_END

#endif // WY3D_DEFAULT_CHAIN_UPDATE_FEEDBACK_H
