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

#include "utils/StringUtil.h"
#include <sstream>
#include <cassert>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <utfcpp/utf8.h>
#include <base64/base64.h>

NS_WY3D_BEG

std::string StringUtil::wstringToUtf8(const std::wstring& wstr)
{
    std::string utf8_str;
#ifdef _WIN32
    // Windows平台下std::wstring是UTF-16编码
    utf8::utf16to8(wstr.cbegin(), wstr.cend(), std::back_inserter(utf8_str));
#else
    // 其他平台(Linux&Macos)下std::wstring是UTF-32编码
    utf8::utf32to8(wstr.cbegin(), wstr.cend(), std::back_inserter(utf8_str));
#endif
    return utf8_str;
}

std::wstring StringUtil::utf8ToWString(const std::string& u8Str)
{
    std::wstring wstr;
#ifdef _WIN32
    // Windows平台下std::wstring是UTF-16编码
    utf8::utf8to16(u8Str.cbegin(), u8Str.cend(), std::back_inserter(wstr));
#else
    // 其他平台(Linux&Macos)下std::wstring是UTF-32编码
    utf8::utf8to32(u8Str.cbegin(), u8Str.cend(), std::back_inserter(wstr));
#endif
    return wstr;
}

std::string StringUtil::wstringToBase64(const std::wstring& wstr)
{
    std::string utf8_str;
#ifdef _WIN32
    // Windows平台下std::wstring是UTF-16编码
    utf8::utf16to8(wstr.cbegin(), wstr.cend(), std::back_inserter(utf8_str));
#else
    // 其他平台(Linux&Macos)下std::wstring是UTF-32编码
    utf8::utf32to8(wstr.cbegin(), wstr.cend(), std::back_inserter(utf8_str));
#endif
    return base64_encode(utf8_str);
}

std::wstring StringUtil::base64ToWString(const std::string& b64Str)
{
    std::string utf8_str = base64_decode(b64Str);

    std::wstring wstr;
#ifdef _WIN32
    // Windows平台下std::wstring是UTF-16编码
    utf8::utf8to16(utf8_str.cbegin(), utf8_str.cend(), std::back_inserter(wstr));
#else
    // 其他平台(Linux&Macos)下std::wstring是UTF-32编码
    utf8::utf8to32(utf8_str.cbegin(), utf8_str.cend(), std::back_inserter(wstr));
#endif
    return wstr;
}

std::string StringUtil::shapeToBase64(const TopoDS_Shape& shape)
{
    std::ostringstream shapeStream(std::ios::binary);
    BRepTools::Write(shape, shapeStream, false, false, TopTools_FormatVersion_CURRENT);
    if (!shapeStream.good())
    {
        assert(false);
        return "";
    }
    return base64_encode(shapeStream.str()); // 现代编译器会优化吗?
}

TopoDS_Shape StringUtil::base64ToShape(const std::string& b64ShapeData)
{
    std::string shapeData = base64_decode(b64ShapeData);
    std::istringstream shapeStream(shapeData, std::ios::binary);
    if (!shapeStream.good())
    {
        assert(false);
        return TopoDS_Shape();
    }

    TopoDS_Shape shape;
    BRep_Builder brepBuilder;
    BRepTools::Read(shape, shapeStream, brepBuilder);
    return shape;
}

NS_WY3D_END