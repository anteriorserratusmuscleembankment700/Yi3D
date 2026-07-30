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

#include <wy3dFeature.h>
#include <wy3dDefaultChainUpdateFeedback.h>

NS_WY3D_BEG
WYDB_IMPLEMENT_MEMBERS(Feature)


Feature::Feature()
{
}

Feature::~Feature()
{
}

void Feature::onChainUpdate(
    const wydb::ElementDataPiece& dirtyDataPiece,
wydb::ChainUpdateFeedbackCollector& feedbackCollector,
    wydb::ChainUpdateCallbackManager& callbackManager)
{
    switch (dirtyDataPiece.getType())
    {
    case wydb::ElementDataPieceType::Completion:
    case wydb::ElementDataPieceType::Shape:
    {
        wy3d::reportChainUpdateSuccess(feedbackCollector, this->getId());
        this->onChainUpdater_Completion(dirtyDataPiece, feedbackCollector);
    }
    break;
    }
}

void Feature::onChainUpdater_Completion(
    const wydb::ElementDataPiece& dirtyDataPiece,
    wydb::ChainUpdateFeedbackCollector& feedbackCollector)
{
}

void Feature::registerParameters(wydb::ParameterSchemaExtension* pParamSchema)
{
}

NS_WY3D_END
