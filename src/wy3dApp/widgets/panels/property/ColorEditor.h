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

#ifndef WY3DAPP_COLOR_EDITOR_H
#define WY3DAPP_COLOR_EDITOR_H

#include <QPushButton>
#include <string>
#include <wy3dColor.h>

class ColorEditor : public QPushButton
{
    Q_OBJECT
public:
    explicit ColorEditor(const std::string& paramName, const wy3d::Color& color,
        bool isTheSameValue, QWidget* parent = nullptr);

signals:
    void colorPicked(const std::string& paramName, const wy3d::Color& newColor);

private:
    void updateAppearance(const wy3d::Color& color, bool isTheSameValue);
    std::string _paramName;
};

#endif // WY3DAPP_COLOR_EDITOR_H
