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

#include <cassert>
#include <wydbElement.h>
#include <wydbDatabase.h>
#include <wydbFiler.h>
#include <wydbFieldRegistry.h>
#include <wy3dSketchEntity.h>

NS_WY3D_BEG
WYDB_IMPLEMENT_MEMBERS(SketchEntity)

BEGIN_FIELD_REGISTRATION()
    REGISTER_FIELD(SketchEntity, _ownerId)
END_FIELD_REGISTRATION()

SketchEntity::SketchEntity() : wydb::Element(), _ownerId(wydb::ElementId::kNull)
{
}

SketchEntity::~SketchEntity()
{
}

wy::ErrorStatus SketchEntity::setOwner(const wydb::ElementId& ownerId)
{
    if (ownerId == _ownerId)
    {
        return wy::ErrorStatus::Ok;
    }

    wy::ErrorStatus error = this->prepareForFieldChange(kSketchEntity_ownerId, wydb::ElementDataPieceType::Appearance);
    if (wy::ErrorStatus::Ok == error)
    {
        _ownerId = ownerId;
        this->markDataPieceDirty(wydb::ElementDataPiece::hierarchy(this->getId()));
        return wy::ErrorStatus::Ok;
    }
    else
    {
        return error;
    }
}

bool SketchEntity::getFieldValue(wydb::FieldId fieldId, std::any& value)
{
    switch (fieldId.value())
    {
    case kSketchEntity_ownerId.value():
        value = _ownerId;
        return true;
    default:
        bool baseRet = __baseClass::getFieldValue(fieldId, value);
        assert(baseRet);
        return baseRet;
    }
}

bool SketchEntity::setFieldValue(wydb::FieldId fieldId, const std::any& value)
{
    switch (fieldId.value())
    {
    case kSketchEntity_ownerId.value():
        _ownerId = std::any_cast<wydb::ElementId>(value);
        return true;
    default:
        bool baseRet = __baseClass::setFieldValue(fieldId, value);
        assert(baseRet);
        return baseRet;
    }
}

wy::ErrorStatus SketchEntity::writeToFiler(wydb::OutFiler& filer) const
{
    __baseClass::writeToFiler(filer);
    filer << _ownerId;
    return wy::ErrorStatus::Ok;
}

wy::ErrorStatus SketchEntity::readFromFiler(wydb::InFiler& filer)
{
    __baseClass::readFromFiler(filer);
    filer >> _ownerId;
    return wy::ErrorStatus::Ok;
}

void SketchEntity::reportDependencies(std::set<wydb::ElementId>& dependencies) const
{
    __baseClass::reportDependencies(dependencies);
    if (!_ownerId.isNull())
    {
        dependencies.insert(_ownerId);
    }
}

bool SketchEntity::onDependenciesErased(const std::set<wydb::ElementId>& erasedDependencies)
{
    if (!_ownerId.isNull() && erasedDependencies.find(_ownerId) != erasedDependencies.cend())
    {
        this->erase(true);
        this->setOwner(wydb::ElementId::kNull);
        return true;
    }

    return __baseClass::onDependenciesErased(erasedDependencies);
}

void SketchEntity::registerParameters(wydb::ParameterSchemaExtension* pParamSchema)
{
}

NS_WY3D_END
