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

#include <utility>

#include <wydbTransaction.h>
#include <wy3dDefaultChainUpdateFeedback.h>
#include <wy3dErrorCode.h>

NS_WY3D_BEG

DefaultChainUpdateFeedback::DefaultChainUpdateFeedback(
    const wydb::ElementId& elementId,
    std::uint32_t code,
    wydb::Element::HealthState healthState,
    std::string message)
    : _elementId(elementId)
    , _code(code)
    , _healthState(healthState)
    , _message(std::move(message))
{
}

wydb::ElementId DefaultChainUpdateFeedback::getElementId() const
{
    return _elementId;
}

wydb::Element::HealthState DefaultChainUpdateFeedback::getHealthState() const
{
    return _healthState;
}

std::uint32_t DefaultChainUpdateFeedback::getCode() const
{
    return _code;
}

std::string DefaultChainUpdateFeedback::getMessage() const
{
    return _message;
}

wydb::Element::HealthState getReportedHealthStateByErrorCode(std::uint32_t errorCode)
{
    if (wy3d::isWarning(errorCode))
    {
        return wydb::Element::HealthState::Warning;
    }

    return wydb::Element::HealthState::Error;
}

wy::ErrorStatus reportChainUpdateSuccess(
    wydb::ChainUpdateFeedbackCollector& feedbackCollector,
    const wydb::ElementId& id)
{
    feedbackCollector.append(std::make_shared<DefaultChainUpdateFeedback>(
        id,
        0,
        wydb::Element::HealthState::Healthy,
        ""));
    return wy::ErrorStatus::Ok;
}

wy::ErrorStatus reportChainUpdateError(
    wydb::ChainUpdateFeedbackCollector& feedbackCollector,
    const wydb::ElementId& id,
    std::uint32_t errorCode)
{
    feedbackCollector.append(std::make_shared<DefaultChainUpdateFeedback>(
        id,
        errorCode,
        getReportedHealthStateByErrorCode(errorCode),
        ""));
    return wy::ErrorStatus::Ok;
}

std::uint32_t getErrorCodeFromChainUpdateFeedback(const wydb::ChainUpdateFeedback* pFeedback)
{
    if (!pFeedback)
    {
        return 0;
    }

    return pFeedback->getCode();
}

NS_WY3D_END
