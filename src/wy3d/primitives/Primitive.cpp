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

#include <wyVector3.h>
#include <wydbFiler.h>
#include <wydbFieldRegistry.h>
#include <wy3dPrimitive.h>
#include <gp_Quaternion.hxx>
#include "topo/TopoNamingUtil.h"
#include "utils/OccUtil.h"

NS_WY3D_BEG
WYDB_IMPLEMENT_MEMBERS(Primitive)

BEGIN_FIELD_REGISTRATION()
    REGISTER_FIELD(Primitive, _position)
    REGISTER_FIELD(Primitive, _rotation)
END_FIELD_REGISTRATION()

Primitive::Primitive() : wy3d::Solid()
{
}

Primitive::~Primitive()
{
}

wy::ErrorStatus Primitive::setPosition(const wy::Vector3& position)
{
    if (position == _position)
    {
        return wy::ErrorStatus::Ok;
    }

    wy::ErrorStatus error = this->prepareForFieldChange(kPrimitive_position, wydb::ElementDataPieceType::Transform);
    if (wy::ErrorStatus::Ok == error)
    {
        _position = position;
        return wy::ErrorStatus::Ok;
    }
    else
    {
        return error;
    }
}

wy::ErrorStatus Primitive::setRotation(const wy::Vector3& rotation)
{
    if (rotation == _rotation)
    {
        return wy::ErrorStatus::Ok;
    }

    wy::ErrorStatus error = this->prepareForFieldChange(kPrimitive_rotation, wydb::ElementDataPieceType::Transform);
    if (wy::ErrorStatus::Ok == error)
    {
        _rotation = rotation;
        return wy::ErrorStatus::Ok;
    }
    else
    {
        return error;
    }
}

bool Primitive::getFieldValue(wydb::FieldId fieldId, std::any& value)
{
    switch (fieldId.value())
    {
    case kPrimitive_position.value():
        value = _position;
        return true;
    case kPrimitive_rotation.value():
        value = _rotation;
        return true;
    default:
        bool baseRet = __baseClass::getFieldValue(fieldId, value);
        assert(baseRet);
        return baseRet;
    }
}

bool Primitive::setFieldValue(wydb::FieldId fieldId, const std::any& value)
{
    switch (fieldId.value())
    {
    case kPrimitive_position.value():
        _position = std::any_cast<const wy::Vector3&>(value);
        return true;
    case kPrimitive_rotation.value():
        _rotation = std::any_cast<const wy::Vector3&>(value);
        return true;
    default:
        bool baseRet = __baseClass::setFieldValue(fieldId, value);
        assert(baseRet);
        return baseRet;
    }
}

wy::ErrorStatus Primitive::writeToFiler(wydb::OutFiler& filer) const
{
    __baseClass::writeToFiler(filer);
    filer << _position.x() << _position.y() << _position.z();
    filer << _rotation.x() << _rotation.y() << _rotation.z();
    return wy::ErrorStatus::Ok;
}

wy::ErrorStatus Primitive::readFromFiler(wydb::InFiler& filer)
{
    __baseClass::readFromFiler(filer);

    double x(0.0), y(0.0), z(0.0);
    filer >> x >> y >> z;
    _position.set(x, y, z);

    filer >> x >> y >> z;
    _rotation.set(x, y, z);

    return wy::ErrorStatus::Ok;
}

TopoDS_Shape Primitive::generateShape(
    TopoNaming* pTopoNaming,
    wydb::ChainUpdateFeedbackCollector& feedbackCollector)
{
    TopoDS_Shape originalShape = this->generateOriginalShape();
    TopoDS_Shape shape = OccUtil::transformShape(originalShape, _position, _rotation);

    unsigned int idValue = this->getId().value();
    bool namingRet = TopoNamingUtil::primitiveNaming(shape, idValue, *pTopoNaming);
    assert(namingRet);

#ifdef _DEBUG
    char szFileName[100] = { 0 };
    sprintf_s(szFileName, 100, "D:/logs/%d.txt", this->getId().value());
    pTopoNaming->print(szFileName, shape);
#endif // _DEBUG

    return shape;
}

void Primitive::registerParameters(wydb::ParameterSchemaExtension* pParamSchema)
{
}

NS_WY3D_END
