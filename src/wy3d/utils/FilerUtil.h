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

#ifndef WY3D_FILER_UTIL_H
#define WY3D_FILER_UTIL_H

#include <vector>
#include <cstdint>

#include <wydbFiler.h>
#include <wy3dDefs.h>
#include <wy3dTopoName.h>

NS_WY3D_BEG

class FilerUtil
{
public:
    template<typename T>
    static inline void writeVector(wydb::OutFiler& filer, const std::vector<T>& vec)
    {
        filer << static_cast<std::uint32_t>(vec.size());
        for (const auto& item : vec)
        {
            filer << item;
        }
    }

    template<typename T>
    static inline void readVector(wydb::InFiler& filer, std::vector<T>& vec)
    {
        std::uint32_t size = 0;
        filer >> size;
        vec.resize(size);
        for (auto& item : vec)
        {
            filer >> item;
        }
    }

    static inline void readTopoNameList(wydb::InFiler& filer, TopoNameList& names)
    {
        readVector(filer, names);
        wydb::MemoryFiler* pMemoryFiler = dynamic_cast<wydb::MemoryFiler*>(&filer);
        if (!pMemoryFiler)
        {
            return;
        }

        const wydb::IdMapping& idMapping = pMemoryFiler->getIdMapping();
        for (TopoName& name : names)
        {
            const bool remapped = TopoNameCodec::remapIds(name, idMapping);
            assert(remapped);
        }
    }
};

NS_WY3D_END

#endif // WY3D_FILER_UTIL_H
