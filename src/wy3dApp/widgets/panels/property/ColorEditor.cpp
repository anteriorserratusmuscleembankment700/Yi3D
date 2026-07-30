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

#include "ColorEditor.h"
#include <QColorDialog>
#include <QColor>

ColorEditor::ColorEditor(const std::string& paramName, const wy3d::Color& color,
    bool isTheSameValue, QWidget* parent)
    : QPushButton(parent), _paramName(paramName)
{
    this->updateAppearance(color, isTheSameValue);

    connect(this, &QPushButton::clicked, this, [this]() {
        QColor initialColor(
            static_cast<int>(static_cast<unsigned char>(0)),  // will be replaced
            static_cast<int>(static_cast<unsigned char>(0)),
            static_cast<int>(static_cast<unsigned char>(0)));
        // 不预设初始颜色 — 让用户自己选
        QColor chosen = QColorDialog::getColor(initialColor, this, tr("Select Color"));
        if (!chosen.isValid()) return;

        wy3d::Color newColor(
            static_cast<unsigned char>(chosen.red()),
            static_cast<unsigned char>(chosen.green()),
            static_cast<unsigned char>(chosen.blue()));
        this->updateAppearance(newColor, true);
        emit colorPicked(_paramName, newColor);
    });
}

void ColorEditor::updateAppearance(const wy3d::Color& color, bool isTheSameValue)
{
    if (!isTheSameValue)
    {
        this->setText("-");
        this->setStyleSheet("");
        return;
    }

    QString bgColor = QString("rgb(%1, %2, %3)")
        .arg(static_cast<int>(color.red))
        .arg(static_cast<int>(color.green))
        .arg(static_cast<int>(color.blue));

    // 深色背景用白字，浅色背景用黑字
    int luminance = 0.299 * color.red + 0.587 * color.green + 0.114 * color.blue;
    QString textColor = luminance > 128 ? "black" : "white";

    this->setText(QString("%1, %2, %3")
        .arg(static_cast<int>(color.red))
        .arg(static_cast<int>(color.green))
        .arg(static_cast<int>(color.blue)));
    this->setStyleSheet(QString(
        "QPushButton {"
        "  background-color: %1;"
        "  color: %2;"
        "  border: 1px solid #999;"
        "  padding: 3px 8px;"
        "  min-height: 22px;"
        "}"
        "QPushButton:hover {"
        "  border: 2px solid #3399ff;"
        "}").arg(bgColor, textColor));
}
