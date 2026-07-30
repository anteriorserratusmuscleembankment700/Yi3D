///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2026 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY3D_TOPO_NAME_H
#define WY3D_TOPO_NAME_H

#include <cstdint>
#include <string>
#include <vector>

#include <wy3dDefs.h>

namespace wydb
{
class IdMapping;
}

NS_WY3D_BEG

using TopoName = std::string;
using TopoNameList = std::vector<TopoName>;

class TopoNameBuilder
{
public:
    TopoNameBuilder();
    explicit TopoNameBuilder(const TopoName& sourceName);

    TopoNameBuilder& id(std::uint32_t value);
    TopoNameBuilder& index(std::uint32_t value);
    TopoNameBuilder& generated(std::uint32_t updateId);
    TopoNameBuilder& split(std::uint32_t resultIndex);
    TopoNameBuilder& source(const TopoName& sourceName);

    TopoName build() const;

private:
    TopoName _name;
};

class WY3D_EXPORT TopoNameCodec
{
public:
    static bool isValid(const TopoName& name);
    static bool extractIds(const TopoName& name, std::vector<std::uint32_t>& ids);
    static bool remapIds(TopoName& name, const wydb::IdMapping& idMapping);
};

NS_WY3D_END

#endif // WY3D_TOPO_NAME_H
