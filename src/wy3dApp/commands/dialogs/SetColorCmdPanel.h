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

#ifndef WY3DAPP_SET_COLOR_CMD_PANEL_H
#define WY3DAPP_SET_COLOR_CMD_PANEL_H

#include <QColor>
#include <QPoint>
#include <QVector>
#include <QWidget>

class QLabel;
class QPushButton;
class QEvent;
class QShowEvent;
class QFrame;

class SetColorCmdPanel : public QWidget
{
    Q_OBJECT
public:
    explicit SetColorCmdPanel(QWidget* parent = nullptr);
    ~SetColorCmdPanel();

    QColor color() const { return _color; }
    void setColor(const QColor& color);
    void setPickedCount(int count);

signals:
    void colorChanged(const QColor& color);
    void accepted();
    void canceled();

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;

private:
    void updateColorButton();
    void refreshPresetColorButtons();
    void anchorToTopLeft();
    void clampToParentBounds();

private slots:
    void onPickColor();
    void onOkClicked();
    void onCancelClicked();

private:
    QFrame* _pTitleBar;
    QLabel* _pTitleLabel;
    QPushButton* _pColorButton;
    QPushButton* _pCancelButton;
    QPushButton* _pOkButton;
    QVector<QColor> _presetColors;
    QVector<QPushButton*> _presetColorButtons;
    QColor _color;
    bool _dragging;
    bool _userMoved;
    QPoint _dragOffset;
    QPoint _parentOffset;
};

#endif // WY3DAPP_SET_COLOR_CMD_PANEL_H

