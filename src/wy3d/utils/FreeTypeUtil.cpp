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

#include "utils/FreeTypeUtil.h"
#include <cassert>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_SFNT_NAMES_H
#include FT_TRUETYPE_IDS_H

NS_WY3D_BEG

#define REPLACE_CODE 0xFFFD

std::vector<FT_ULong> FreeTypeUtil::wstringToCharCodes(const std::wstring& wstr)
{
    std::vector<FT_ULong> charCodes;
    if (sizeof(wchar_t) == 2) // Windows(UTF-16)
    {
        const size_t count = wstr.size();
        const size_t maxValidIndex = count - 1;
        charCodes.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            wchar_t high = wstr[i];
            if (high >= 0xD800 && high <= 0xDBFF) // 检查是否为高代理项(0xD800~0xDBFF)
            {
                if (i >= maxValidIndex) // 不完整代理对
                {
                    assert(false);
                    charCodes.push_back(REPLACE_CODE); // 替换字符
                    continue;
                }

                wchar_t low = wstr[++i];
                if (low >= 0xDC00 && low <= 0xDFFF) // 检查是否为低代理项(0xDC00~0xDFFF)
                {
                    // 正确转换代理对为Unicode码点
                    FT_ULong codePoint = 0x10000
                        + ((static_cast<FT_ULong>(high) - 0xD800) << 10)
                        + (static_cast<FT_ULong>(low) - 0xDC00);
                    charCodes.push_back(codePoint);
                }
                else // 低代理项无效
                {
                    assert(false);
                    charCodes.push_back(REPLACE_CODE); // 替换字符
                }
            }
            else if (high >= 0xDC00 && high <= 0xDFFF) // 单独的低代理项
            {
                assert(false);
                charCodes.push_back(REPLACE_CODE); // 替换字符
            }
            else // 非代理对字符(BMP平面)直接转换
            {
                charCodes.push_back(static_cast<FT_ULong>(high));
            }
        }
    }
    else // Linux(UTF-32)
    {
        charCodes.reserve(wstr.size());
        for (wchar_t wc : wstr)
        {
            charCodes.push_back(static_cast<FT_ULong>(wc));
        }
    }

    return charCodes;
}

std::vector<std::wstring> FreeTypeUtil::getFaceNames(FT_Face face)
{
    std::vector<std::wstring> names;
    if (!face || !FT_IS_SFNT(face))
    {
        return names;
    }

    FT_UInt nameCount = FT_Get_Sfnt_Name_Count(face);
    names.reserve(4);
    for (FT_UInt i = 0; i < nameCount; ++i)
    {
        FT_SfntName name;
        if (FT_Get_Sfnt_Name(face, i, &name) != 0)
        {
            continue;
        }
        if (!name.string)
        {
            assert(false);
            continue;
        }

        // 目前只处理Windows以及兼容Windows
        if (name.platform_id != TT_PLATFORM_MICROSOFT)
        {
            continue;
        }

        // 目前只处理字体全名称
        if (name.name_id != TT_NAME_ID_FULL_NAME)
        {
            continue;
        }
        
        if (name.encoding_id == TT_MS_ID_UNICODE_CS)
        {
            std::wstring wstrName;
            wstrName.reserve(name.string_len / 2);
            if (sizeof(wchar_t) == 2) // Windows平台
            {
                for (FT_UInt k = 0; k < name.string_len; k += 2)
                {
                    if (k + 1 >= name.string_len)
                    {
                        assert(false);
                        break;
                    }
                    wchar_t wch = (static_cast<wchar_t>(name.string[k]) << 8) 
                        | static_cast<wchar_t>(name.string[k + 1]);
                    wstrName.push_back(wch);
                }
            }
            else // Linux平台
            {
                for (FT_UInt k = 0; k < name.string_len; k += 2)
                {
                    if (k + 1 >= name.string_len)
                    {
                        assert(false);
                        break;
                    }
                    
                    char16_t high = (static_cast<char16_t>(name.string[k]) << 8) 
                        | static_cast<char16_t>(name.string[k + 1]);
                    if (high >= 0xD800 && high <= 0xDBFF)// 高代理项(0xD800~0xDBFF)
                    {
                        if (k + 3 >= name.string_len)
                        {
                            assert(false);
                            break;
                        }

                        char16_t low = (static_cast<char16_t>(name.string[k + 2]) << 8)
                            | static_cast<char16_t>(name.string[k + 3]);
                        if (low >= 0xDC00 && low <= 0xDFFF) // 低代理项(0xDC00~0xDFFF)
                        {
                            // 组合码点
                            char32_t codePoint = 0x10000
                                + ((static_cast<char32_t>(high) - 0xD800) << 10)
                                + (static_cast<char32_t>(low) - 0xDC00);
                            wstrName.push_back(codePoint);
                        }
                        else // 无效的低代理项
                        {
                            assert(false);
                        }
                        k += 2; // 额外跳过两个字节
                    }
                    else if (high >= 0xDC00 && high <= 0xDFFF) // 单独的低代理项
                    {
                        assert(false);
                        continue;
                    }
                    else
                    {
                        wstrName.push_back(static_cast<wchar_t>(high));
                    }
                }
            }

            if (!wstrName.empty())
            {
                names.emplace_back(std::move(wstrName));
            }
        }
        else if (name.encoding_id == TT_MS_ID_UCS_4)
        {
            std::wstring wstrName;
            wstrName.reserve(name.string_len / 4);

            if (sizeof(wchar_t) == 2) // Windows平台
            {
                for (FT_UInt k = 0; k < name.string_len; k += 4)
                {
                    if (k + 3 >= name.string_len)
                    {
                        //assert(false);
                        break;
                    }

                    char32_t codePoint = (static_cast<char32_t>(name.string[k]) << 24)
                        | (static_cast<char32_t>(name.string[k + 1]) << 16)
                        | (static_cast<char32_t>(name.string[k + 2]) << 8)
                        | static_cast<char32_t>(name.string[k + 3]);
                    if (codePoint <= 0xFFFF) // 基本多语言平面字符
                    {
                        wstrName.push_back(static_cast<wchar_t>(codePoint));
                    }
                    else if (codePoint <= 0x10FFFF) // 高低位代理
                    {
                        codePoint -= 0x10000;
                        char16_t high = static_cast<char16_t>((codePoint >> 10) + 0xD800);
                        char16_t low = static_cast<char16_t>((codePoint & 0x3FF) + 0xDC00);
                        wstrName.push_back(static_cast<wchar_t>(high));
                        wstrName.push_back(static_cast<wchar_t>(low));
                    }
                    else // 无效的Unicode码点
                    {
                        //assert(false);
                        continue;
                    }
                }
            }
            else // Linux平台
            {
                for (FT_UInt k = 0; k < name.string_len; k += 4)
                {
                    if (k + 3 >= name.string_len)
                    {
                        //assert(false);
                        break;
                    }
                    wchar_t codePoint = (static_cast<wchar_t>(name.string[k]) << 24)
                        | (static_cast<wchar_t>(name.string[k + 1]) << 16)
                        | (static_cast<wchar_t>(name.string[k + 2]) << 8) 
                        | static_cast<wchar_t>(name.string[k + 3]);
                    wstrName.push_back(codePoint);
                }
            }

            if (!wstrName.empty())
            {
                names.emplace_back(std::move(wstrName));
            }
        }
        else
        {
            continue;
        }
    }

    return names;
}

NS_WY3D_END