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

#include <TopoDS.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedMapOfShape.hxx>

#include <wydbDatabase.h>
#include <wydbTransaction.h>
#include <wy3dChamfer.h>
#include <wy3dSolid.h>
#include <wy3dImpl.h>
#include <wydbFiler.h>
#include <wydbFieldRegistry.h>
#include <wy3dParamNames.h>
#include <wy3dErrorCode.h>
#include <wy3dDefaultChainUpdateFeedback.h>
#include "topo/TopoShapeComparer.h"
#include "topo/ChamferFilletTopoShapeComparer.h"
#include "topo/TopoNamingUtil.h"
#include "SolidModificationUtil.h"
#include "utils/FilerUtil.h"
#include "utils/Util.h"

NS_WY3D_BEG
WYDB_IMPLEMENT_MEMBERS(Chamfer)

BEGIN_FIELD_REGISTRATION()
    REGISTER_FIELD(Chamfer, _edgeNames)
    REGISTER_FIELD(Chamfer, _faceNames)
    REGISTER_FIELD(Chamfer, _distance)
END_FIELD_REGISTRATION()

Chamfer::Chamfer() : wy3d::SolidModification(), _distance(0.0)
{
}

Chamfer::~Chamfer()
{
}

wy::ErrorStatus Chamfer::create(
    wydb::Transaction* pTrans,
    wy3d::Solid* pSolid,
    const std::vector<std::uint32_t>& faceIndices,
    const std::vector<std::uint32_t>& edgeIndices,
    double distance,
    Chamfer*& pOutChamfer)
{
    if (!pTrans)
    {
        pOutChamfer = nullptr;
        return wy::ErrorStatus::NullDatabasePointer;
    }
    if (!pSolid)
    {
        pOutChamfer = nullptr;
        return wy::ErrorStatus::NullElementPointer;
    }
    if (faceIndices.empty() && edgeIndices.empty())
    {
        pOutChamfer = nullptr;
        return wy::ErrorStatus::InvalidInput;
    }
    if (distance < 0.0)
    {
        pOutChamfer = nullptr;
        return wy::ErrorStatus::InvalidInput;
    }

    TopoDS_Shape shape = pSolid->getShape();
    TopoNaming* pTopoNaming = pSolid->getTopoNaming();
    if (!pTopoNaming)
    {
        pOutChamfer = nullptr;
        return wy::ErrorStatus::InvalidInput;
    }

    TopoNameList faceNames;
    if (!faceIndices.empty())
    {
        if (!TopoNamingUtil::assemblyTopoNames(*pTopoNaming, shape,
            TopAbs_ShapeEnum::TopAbs_FACE, faceIndices, faceNames))
        {
            pOutChamfer = nullptr;
            return wy::ErrorStatus::InvalidInput;
        }
        assert(!faceNames.empty());
    }

    TopoNameList edgeNames;
    if (!edgeIndices.empty())
    {
        if (!TopoNamingUtil::assemblyTopoNames(*pTopoNaming, shape,
            TopAbs_ShapeEnum::TopAbs_EDGE, edgeIndices, edgeNames))
        {
            pOutChamfer = nullptr;
            return wy::ErrorStatus::InvalidInput;
        }
        assert(!edgeNames.empty());
    }

    Chamfer* pChamfer = new Chamfer();
    wy::ErrorStatus error = pTrans->addNewlyCreatedElement(pChamfer);
    if (wy::ErrorStatus::Ok != error)
    {
        wydb::deleteElement(pChamfer);
        pChamfer = nullptr;
        return error;
    }

    if (!edgeNames.empty())
    {
        error = pChamfer->setEdges(edgeNames);
        CHECK_ERROR_FOR_CREATE(error, pChamfer);
    }
    if (!faceNames.empty())
    {
        error = pChamfer->setFaces(faceNames);
        CHECK_ERROR_FOR_CREATE(error, pChamfer);
    }
    error = pChamfer->setDistance(distance);
    CHECK_ERROR_FOR_CREATE(error, pChamfer);

    error = pSolid->addModification(pChamfer);
    CHECK_ERROR_FOR_CREATE(error, pChamfer);

    pOutChamfer = pChamfer;
    return wy::ErrorStatus::Ok;
}

wy::ErrorStatus Chamfer::setDistance(double distance)
{
    if (distance < wy3d::kMinValue || distance > wy3d::kMaxValue)
    {
        return wy::ErrorStatus::InvalidInput;
    }
    if (distance == _distance)
    {
        return wy::ErrorStatus::Ok;
    }
    wy::ErrorStatus error = this->prepareForFieldChange(kChamfer_distance);
    if (wy::ErrorStatus::Ok == error)
    {
        _distance = distance;
        return wy::ErrorStatus::Ok;
    }
    else
    {
        return error;
    }
}

wy::ErrorStatus Chamfer::setEdges(const TopoNameList& edges)
{
    if (edges == _edgeNames)
    {
        return wy::ErrorStatus::Ok;
    }
    wy::ErrorStatus error = this->prepareForFieldChange(kChamfer_edgeNames);
    if (wy::ErrorStatus::Ok == error)
    {
        _edgeNames = edges;
        return wy::ErrorStatus::Ok;
    }
    else
    {
        return error;
    }
}

wy::ErrorStatus Chamfer::setFaces(const TopoNameList& faces)
{
    if (faces == _faceNames)
    {
        return wy::ErrorStatus::Ok;
    }
    wy::ErrorStatus error = this->prepareForFieldChange(kChamfer_faceNames);
    if (wy::ErrorStatus::Ok == error)
    {
        _faceNames = faces;
        return wy::ErrorStatus::Ok;
    }
    else
    {
        return error;
    }
}


void Chamfer::registerParameters(wydb::ParameterSchemaExtension* pParamSchema)
{
    {
        wydb::ParameterDefinitionData def;
        def.name = ParamNames::CHAMFER_DISTANCE1;
        pParamSchema->addParameterDefinition(def);
    }
}
wydb::ParameterValueUPtr Chamfer::getParameterValue(const std::string& className, const std::string& paramName) const
{
    if (className == Chamfer::classInfo()->className())
    {
        if (ParamNames::CHAMFER_DISTANCE1 == paramName)
        {
            return wydb::ParameterValue::createDouble(_distance);
        }
        return nullptr;
    }
    return __baseClass::getParameterValue(className, paramName);
}

wy::ErrorStatus Chamfer::setParameterValue(const std::string& className, const std::string& paramName, const wydb::ParameterValue& paramValue)
{
    if (className == Chamfer::classInfo()->className())
    {
        if (ParamNames::CHAMFER_DISTANCE1 == paramName)
        {
            if (!paramValue.isDouble()) return wy::ErrorStatus::InvalidInput;
            return this->setDistance(paramValue.asDouble());
        }
        return wy::ErrorStatus::ParameterNotFound;
    }
    return __baseClass::setParameterValue(className, paramName, paramValue);
}

bool Chamfer::getFieldValue(wydb::FieldId fieldId, std::any& value)
{
    switch (fieldId.value())
    {
    case kChamfer_edgeNames.value():
        value = _edgeNames;
        return true;
    case kChamfer_faceNames.value():
        value = _faceNames;
        return true;
    case kChamfer_distance.value():
        value = _distance;
        return true;
    default:
        bool baseRet = __baseClass::getFieldValue(fieldId, value);
        assert(baseRet);
        return baseRet;
    }
}

bool Chamfer::setFieldValue(wydb::FieldId fieldId, const std::any& value)
{
    switch (fieldId.value())
    {
    case kChamfer_edgeNames.value():
        _edgeNames = std::any_cast<const TopoNameList&>(value);
        return true;
    case kChamfer_faceNames.value():
        _faceNames = std::any_cast<const TopoNameList&>(value);
        return true;
    case kChamfer_distance.value():
        _distance = std::any_cast<double>(value);
        return true;
    default:
        bool baseRet = __baseClass::setFieldValue(fieldId, value);
        assert(baseRet);
        return baseRet;
    }
}

wy::ErrorStatus Chamfer::writeToFiler(wydb::OutFiler& filer) const
{
    __baseClass::writeToFiler(filer);
    FilerUtil::writeVector(filer, _edgeNames);
    FilerUtil::writeVector(filer, _faceNames);
    filer << _distance;
    return wy::ErrorStatus::Ok;
}

wy::ErrorStatus Chamfer::readFromFiler(wydb::InFiler& filer)
{
    __baseClass::readFromFiler(filer);

    FilerUtil::readTopoNameList(filer, _edgeNames);
    FilerUtil::readTopoNameList(filer, _faceNames);
    filer >> _distance;

    return wy::ErrorStatus::Ok;
}

std::pair<bool, TopoDS_Shape> Chamfer::modifyOwnerShape(
    const TopoDS_Shape& shape,
    TopoNaming* pTopoNaming,
    wydb::ChainUpdateFeedbackCollector& feedbackCollector)
{
    assert(pTopoNaming);

    wydb::Database* pDb = this->getDatabase();
    assert(pDb);
    wydb::Transaction* pTrans = pDb->getTransactionManager()->getActiveTransaction();
    assert(pTrans);
    assert(false == pTrans->isGroup());

    std::vector<TopoDS_Edge> topoEdges;
    ErrorCode errorCode = SolidModificationUtil::getTopoEdgesByTopoNamings<
        ErrorCode::CHAMFER_InvalidData,
        ErrorCode::CHAMFER_EdgeNotExists,
        ErrorCode::CHAMFER_FaceNotExists>(*pTopoNaming, _edgeNames, _faceNames, topoEdges);
    if (ErrorCode::NoError != errorCode)
    {
        wy3d::reportChainUpdateError(feedbackCollector, this->getId(), static_cast<unsigned int>(errorCode));
        return std::pair<bool, TopoDS_Shape>(false, shape);
    }
    if (topoEdges.empty())
    {
        assert(false);
        wy3d::reportChainUpdateError(feedbackCollector, this->getId(), static_cast<std::uint32_t>(ErrorCode::CHAMFER_InvalidData));
        return std::pair<bool, TopoDS_Shape>(false, shape);
    }

    try
    {
        BRepFilletAPI_MakeChamfer chamfer(shape);
        for (const TopoDS_Edge& topoEdge : topoEdges)
        {
            chamfer.Add(_distance, topoEdge);
        }
        chamfer.Build();
        if (chamfer.IsDone())
        {
            TopoDS_Shape retShape = chamfer.Shape();

            ChamferFilletTopoShapeComparer topoComparer(chamfer, shape);
            topoComparer.perform();
#ifdef _DEBUG
            {
                char szFileName[100] = { 0 };
                sprintf_s(szFileName, 100, "D:/logs/%d_topoComparer.txt", this->getId().value());
                topoComparer.print(szFileName);
            }
#endif
            pTopoNaming->update(&topoComparer, this->getId().value());

#ifdef _DEBUG
            char szFileName[100] = { 0 };
            sprintf_s(szFileName, 100, "D:/logs/%d.txt", this->getId().value());
            pTopoNaming->print(szFileName, retShape);
#endif

            this->recordNewFaces(topoComparer.getFaceDelta(), pTopoNaming);

            return std::pair<bool, TopoDS_Shape>(true, retShape);
        }
    }
    catch (const Standard_Failure&)
    {
    }

    wy3d::reportChainUpdateError(feedbackCollector, this->getId(),
        static_cast<std::uint32_t>(ErrorCode::CHAMFER_GenerateChamferError));
    return std::pair<bool, TopoDS_Shape>(false, shape);
}

NS_WY3D_END
