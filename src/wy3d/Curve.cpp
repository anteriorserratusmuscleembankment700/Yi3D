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

#include <wy3dCurve.h>
#include <wydbFiler.h>
#include <wydbElement.h>
#include <wydbFieldRegistry.h>
#include <Standard_Failure.hxx>

NS_WY3D_BEG
WYDB_IMPLEMENT_MEMBERS(Curve)

BEGIN_FIELD_REGISTRATION()
    REGISTER_FIELD(Curve, _ownerId)
    REGISTER_FIELD(Curve, _edge)
END_FIELD_REGISTRATION()

Curve::Curve() : wy3d::Feature(), _ownerId(wydb::ElementId::kNull), _edge()
{
}

Curve::~Curve()
{
}

wy::ErrorStatus Curve::setOwner(const wydb::ElementId& ownerId)
{
    if (ownerId == _ownerId)
    {
        return wy::ErrorStatus::Ok;
    }

    wy::ErrorStatus error = this->prepareForFieldChange(kCurve_ownerId, wydb::ElementDataPieceType::Appearance);
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

wy::ErrorStatus Curve::_setEdge(const TopoDS_Edge& edge)
{
    if (edge == _edge)
    {
        return wy::ErrorStatus::Ok;
    }

    wy::ErrorStatus error = this->prepareForFieldChange(kCurve_edge, wydb::ElementDataPieceType::None);
    if (wy::ErrorStatus::Ok == error)
    {
        _edge = edge;
        return wy::ErrorStatus::Ok;
    }
    else
    {
        return error;
    }
}

bool Curve::getFieldValue(wydb::FieldId fieldId, std::any& value)
{
    switch (fieldId.value())
    {
    case kCurve_ownerId.value():
        value = _ownerId;
        return true;
    case kCurve_edge.value():
        value = _edge;
        return true;
    default:
        bool baseRet = __baseClass::getFieldValue(fieldId, value);
        assert(baseRet);
        return baseRet;
    }
}

bool Curve::setFieldValue(wydb::FieldId fieldId, const std::any& value)
{
    switch (fieldId.value())
    {
    case kCurve_ownerId.value():
        _ownerId = std::any_cast<wydb::ElementId>(value);
        return true;
    case kCurve_edge.value():
        _edge = std::any_cast<TopoDS_Edge>(value);
        return true;
    default:
        bool baseRet = __baseClass::setFieldValue(fieldId, value);
        assert(baseRet);
        return baseRet;
    }
}

wy::ErrorStatus Curve::writeToFiler(wydb::OutFiler& filer) const
{
    __baseClass::writeToFiler(filer);
    filer << _ownerId;
    return wy::ErrorStatus::Ok;
}

wy::ErrorStatus Curve::readFromFiler(wydb::InFiler& filer)
{
    __baseClass::readFromFiler(filer);
    filer >> _ownerId;
    return wy::ErrorStatus::Ok;
}

void Curve::reportDependencies(std::set<wydb::ElementId>& dependencies) const
{
    __baseClass::reportDependencies(dependencies);
    if (!_ownerId.isNull())
    {
        dependencies.insert(_ownerId);
    }
}

bool Curve::onDependenciesErased(const std::set<wydb::ElementId>& erasedDependencies)
{
    bool responsed = __baseClass::onDependenciesErased(erasedDependencies);

    if (!_ownerId.isNull() && erasedDependencies.find(_ownerId) != erasedDependencies.cend())
    {
        this->erase(true);
        this->setOwner(wydb::ElementId::kNull);
        return true;
    }

    return responsed;
}

void Curve::onChainUpdater_Completion(
    const wydb::ElementDataPiece& dirtyDataPiece,
    wydb::ChainUpdateFeedbackCollector& feedbackCollector)
{
    try
    {
        TopoDS_Edge edge = this->generateShape(feedbackCollector);
        this->_setEdge(edge);
    }
    catch (const Standard_Failure&)
    {
        assert(false);
        this->_setEdge(TopoDS_Edge());
    }
    catch (...)
    {
        assert(false);
        this->_setEdge(TopoDS_Edge());
    }
}

void Curve::registerParameters(wydb::ParameterSchemaExtension* pParamSchema)
{
}

NS_WY3D_END
