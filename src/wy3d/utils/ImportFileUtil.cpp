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

#include "ImportFileUtil.h"

#include <algorithm>
#include <list>
#include <filesystem>
#include <cassert>
#include <TopoDS.hxx>
#include <BRep_Builder.hxx>
#include <STEPControl_Reader.hxx>
#include <IGESControl_Reader.hxx>
#include <BRepTools.hxx>
#include <TopoDS_Iterator.hxx>

NS_WY3D_BEG

void _getCompoundAllSolidsAndFaces(const TopoDS_Compound& compound,
    std::list<TopoDS_Solid>& solidList, std::list<TopoDS_Face>& faceList)
{
    TopoDS_Iterator iter;
    for (iter.Initialize(compound); iter.More(); iter.Next())
    {
        TopoDS_Shape shape = iter.Value();
        switch (shape.ShapeType())
        {
        case TopAbs_ShapeEnum::TopAbs_SOLID:
        {
            TopoDS_Solid solid = TopoDS::Solid(shape);
            solidList.emplace_back(solid);
        }
        break;

        case TopAbs_ShapeEnum::TopAbs_FACE:
        {
            TopoDS_Face face = TopoDS::Face(shape);
            faceList.emplace_back(face);
        }
        break;

        case TopAbs_ShapeEnum::TopAbs_SHELL:
        {
            TopoDS_Shell shell = TopoDS::Shell(shape);
            TopoDS_Iterator shellIter;
            for (shellIter.Initialize(shell); shellIter.More(); shellIter.Next())
            {
                TopoDS_Shape subShape = shellIter.Value();
                if (subShape.ShapeType() == TopAbs_FACE)
                {
                    faceList.emplace_back(TopoDS::Face(subShape));
                }
            }
        }
        break;

        case TopAbs_ShapeEnum::TopAbs_COMPOUND:
        {
            TopoDS_Compound subCompound = TopoDS::Compound(shape);
            _getCompoundAllSolidsAndFaces(subCompound, solidList, faceList);
        }
        break;

        case TopAbs_ShapeEnum::TopAbs_COMPSOLID:
        {
            // 非流形
            assert(false);
        }
        break;

        default:
        {
            // do nothing
            // 目前只处理面和体
        }
        break;
        }
    }
}

void _addSolidAndFace(BRep_Builder& builder,
    TopoDS_Compound& solidCompounds, TopoDS_Compound& faceCompounds,
    const TopoDS_Shape& shape)
{
    switch (shape.ShapeType())
    {
    case TopAbs_ShapeEnum::TopAbs_SOLID:
    {
        TopoDS_Solid solid = TopoDS::Solid(shape);
        builder.Add(solidCompounds, solid);
    }
    break;

    case TopAbs_ShapeEnum::TopAbs_FACE:
    {
        TopoDS_Face face = TopoDS::Face(shape);
        builder.Add(faceCompounds, face);
    }
    break;

    case TopAbs_ShapeEnum::TopAbs_SHELL:
    {
        TopoDS_Shell shell = TopoDS::Shell(shape);
        TopoDS_Iterator shellIter;
        for (shellIter.Initialize(shell); shellIter.More(); shellIter.Next())
        {
            TopoDS_Shape subShape = shellIter.Value();
            if (subShape.ShapeType() == TopAbs_FACE)
            {
                builder.Add(faceCompounds, TopoDS::Face(subShape));
            }
        }
    }
    break;

    case TopAbs_ShapeEnum::TopAbs_COMPOUND:
    {
        TopoDS_Compound compound = TopoDS::Compound(shape);
        std::list<TopoDS_Solid> solidList;
        std::list<TopoDS_Face> faceList;
        _getCompoundAllSolidsAndFaces(compound, solidList, faceList);
        for (const TopoDS_Solid& solid : solidList)
        {
            builder.Add(solidCompounds, solid);
        }
        for (const TopoDS_Face& face : faceList)
        {
            builder.Add(faceCompounds, face);
        }
    }
    break;

    case TopAbs_ShapeEnum::TopAbs_COMPSOLID:
    {
        // 非流形
        assert(false);
    }
    break;

    default:
    {
        // do nothing
        // 目前只处理面和体
    }
    break;
    }
}

ImportFileUtil::ReadResult _readStepFile(const char* szFilePath)
{
    ImportFileUtil::ReadResult ret;
    ret.flag = false;

    STEPControl_Reader reader;
#pragma warning(disable : 26812) 
    IFSelect_ReturnStatus status = reader.ReadFile(szFilePath);
#pragma warning(default : 26812)
    if (status != IFSelect_ReturnStatus::IFSelect_RetDone)
    {
        return ret;
    }

    BRep_Builder builder;
    TopoDS_Compound solidCompounds;
    builder.MakeCompound(solidCompounds);
    TopoDS_Compound faceCompounds;
    builder.MakeCompound(faceCompounds);

    reader.TransferRoots();
    int nbShapes = reader.NbShapes();
    for (int i = 1; i <= nbShapes; ++i)
    {
        TopoDS_Shape shape = reader.Shape(i);
        _addSolidAndFace(builder, solidCompounds, faceCompounds, shape);
    }

    ret.flag = true;
    ret.solids = solidCompounds;
    ret.faces = faceCompounds;
    return ret;
}

ImportFileUtil::ReadResult _readIgesFile(const char* szFilePath)
{
    ImportFileUtil::ReadResult ret;
    ret.flag = false;

    IGESControl_Reader reader;
#pragma warning(disable : 26812) 
    IFSelect_ReturnStatus status = reader.ReadFile(szFilePath);
#pragma warning(default : 26812)
    if (status != IFSelect_ReturnStatus::IFSelect_RetDone)
    {
        return ret;
    }

    BRep_Builder builder;
    TopoDS_Compound solidCompounds;
    builder.MakeCompound(solidCompounds);
    TopoDS_Compound faceCompounds;
    builder.MakeCompound(faceCompounds);

    reader.TransferRoots();
    int nbShapes = reader.NbShapes();
    for (int i = 1; i <= nbShapes; ++i)
    {
        TopoDS_Shape shape = reader.Shape(i);
        _addSolidAndFace(builder, solidCompounds, faceCompounds, shape);
    }

    ret.flag = true;
    ret.solids = solidCompounds;
    ret.faces = faceCompounds;
    return ret;
}

ImportFileUtil::ReadResult _readBrepFile(const char* szFilePath)
{
    ImportFileUtil::ReadResult ret;
    ret.flag = false;

    TopoDS_Shape shape;
    {
        BRep_Builder builder;
        if (!BRepTools::Read(shape, szFilePath, builder))
        {
            return ret;
        }
    }

    BRep_Builder builder;
    TopoDS_Compound solidCompounds;
    builder.MakeCompound(solidCompounds);
    TopoDS_Compound faceCompounds;
    builder.MakeCompound(faceCompounds);
    _addSolidAndFace(builder, solidCompounds, faceCompounds, shape);

    ret.flag = true;
    ret.solids = solidCompounds;
    ret.faces = faceCompounds;
    return ret;
}

ImportFileUtil::ReadResult ImportFileUtil::readFile(const std::wstring& filePath)
{
    ImportFileUtil::ReadResult result;
    result.flag = false;

    try
    {
        std::filesystem::path fsPath(filePath);
        std::error_code errorCode;

        // 检查文件是否存在
        if (!std::filesystem::exists(fsPath, errorCode))
        {
            return result;
        }

        // 检查是否是常规文件
        if (!std::filesystem::is_regular_file(fsPath, errorCode))
        {
            return result;
        }

        // 获取文件后缀名(转为小写便于比较)
        std::string extension = fsPath.extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(),
            [](unsigned char c) { return std::tolower(c); });

        // 通过不同的后缀名读取文件
        std::string utf8Path = fsPath.u8string();
        if (extension == ".step" || extension == ".stp")
        {
            return _readStepFile(utf8Path.c_str());
        }
        else if (extension == ".iges" || extension == ".igs")
        {
            return _readIgesFile(utf8Path.c_str());
        }
        else if (extension == ".brep")
        {
            return _readBrepFile(utf8Path.c_str());
        }
        else
        {
            return result;
        }
    }
    catch (const Standard_Failure&)
    {
        assert(false);
        result.solids = TopoDS_Compound();
        result.faces = TopoDS_Compound();
        result.flag = false;
        return result;
    }
    catch (...)
    {
        assert(false);
        result.solids = TopoDS_Compound();
        result.faces = TopoDS_Compound();
        result.flag = false;
        return result;
    }

    return result;
}

NS_WY3D_END