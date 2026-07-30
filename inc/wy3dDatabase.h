///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY3D_DATABASE_H
#define WY3D_DATABASE_H

#include <wy3dDefs.h>
#include <wydbDatabase.h>
#include <wydbFileFormat.h>

NS_WY3D_BEG

class WY3D_EXPORT Database : public wydb::Database
{
public:
    Database();

    // 文件格式
    static const wydb::FileFormatConfig& fileFormatConfig();

    // 文件扩展名(默认返回文本格式扩展名)
    static const std::string& extension();

    // 根据文件类型返回对应的扩展名
    static const std::string& extension(wydb::FileType fileType);
};

NS_WY3D_END

#endif // WY3D_DATABASE_H