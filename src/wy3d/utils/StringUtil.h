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

#ifndef WY3D_STRING_UTIL_H
#define WY3D_STRING_UTIL_H

#include <string>
#include <wy3dDefs.h>
#include <TopoDS_Shape.hxx>

NS_WY3D_BEG

class StringUtil
{
public:
    // 宽字符转UTF-8
    static std::string wstringToUtf8(const std::wstring& wstr);

    // UTF-8转宽字符
    static std::wstring utf8ToWString(const std::string& u8Str);

    // 宽字符串转Base64(utf-8)
    // Windows下宽字符串为UTF-16编码,其它平台下为UTF-32编码
    static std::string wstringToBase64(const std::wstring& wstr);

    // Base64(utf-8)转宽字符串
    // Base64字符串为UTF-8编码的字符串编码而来
    static std::wstring base64ToWString(const std::string& b64Str);

    // 拓扑形体转Base64
    static std::string shapeToBase64(const TopoDS_Shape& shape);

    // Base64转拓扑形体
    static TopoDS_Shape base64ToShape(const std::string& b64ShapeData);
};

NS_WY3D_END

#endif // WY3D_STRING_UTIL_H