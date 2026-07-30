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

#include <wydbDatabase.h>
#include <wydbTransaction.h>
#include <wy3dDifference.h>
#include <wy3dBoolean.h>
#include "utils/Util.h"

NS_WY3D_BEG
WYDB_IMPLEMENT_MEMBERS(Difference)

Difference::Difference() : Boolean(BooleanType::Difference)
{
}

Difference::~Difference()
{
}

wy::ErrorStatus Difference::create(
    wydb::Transaction* pTrans,
    wy3d::Solid* pTarget,
    wy3d::Solid* pTool,
    Difference*& pOut)
{
    return Difference::create(pTrans, pTarget, std::vector<wy3d::Solid*>{ pTool }, pOut);
}

wy::ErrorStatus Difference::create(wydb::Transaction* pTrans, wy3d::Solid* pTarget, const std::vector<wy3d::Solid*>& tools, Difference*& pOut)
{
    if (!pTrans)
    {
        pOut = nullptr;
        return wy::ErrorStatus::NullDatabasePointer;
    }
    if (!pTarget || tools.empty())
    {
        pOut = nullptr;
        return wy::ErrorStatus::NullElementPointer;
    }

    Difference* pDifference = new Difference();
    wy::ErrorStatus error = pTrans->addNewlyCreatedElement(pDifference);
    if (error != wy::ErrorStatus::Ok)
    {
        wydb::deleteElement(pDifference);
        pDifference = nullptr;
        return error;
    }

    error = pDifference->_setTarget(pTarget);
    CHECK_ERROR_FOR_CREATE(error, pDifference)
    for (wy3d::Solid* pTool : tools)
    {
        error = pDifference->addTool(pTool);
        CHECK_ERROR_FOR_CREATE(error, pDifference)
    }

    pOut = pDifference;
    return wy::ErrorStatus::Ok;
}

void Difference::registerParameters(wydb::ParameterSchemaExtension* pParamSchema)
{
}

NS_WY3D_END
