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

#ifndef WY3DAP_ENVIRONMENT_IDS_H
#define WY3DAP_ENVIRONMENT_IDS_H

#include <string>

class EnvironmentIds
{
public:
    // 基础
    static inline const std::string Gateway = "Gateway";
    // 建模
    static inline const std::string Modeling = "modeling";
    // 草绘
    static inline const std::string Sketching = "sketching";
};

#endif // WY3DAP_ENVIRONMENT_IDS_H