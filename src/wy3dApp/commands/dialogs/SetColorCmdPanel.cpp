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

#include "SetColorCmdPanel.h"

#include <QColorDialog>
#include <QEvent>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>
#include <QShowEvent>
#include <QTimer>
#include <QVBoxLayout>

SetColorCmdPanel::SetColorCmdPanel(QWidget* parent)
    : QWidget(parent)
    , _pTitleBar(nullptr)
    , _pTitleLabel(nullptr)
    , _pColorButton(nullptr)
    , _pCancelButton(nullptr)
    , _pOkButton(nullptr)
    , _presetColors({
        QColor(245, 245, 245), QColor(220, 220, 220), QColor(188, 188, 188), QColor(146, 146, 146), QColor(96, 96, 96),
        QColor(255, 230, 230), QColor(255, 189, 189), QColor(242, 128, 128), QColor(210, 64, 64), QColor(148, 32, 32),
        QColor(255, 241, 224), QColor(255, 219, 179), QColor(255, 183, 107), QColor(245, 146, 20), QColor(173, 95, 8),
        QColor(255, 250, 204), QColor(255, 236, 153), QColor(255, 217, 102), QColor(230, 179, 26), QColor(166, 122, 0),
        QColor(234, 255, 204), QColor(194, 242, 163), QColor(128, 219, 128), QColor(76, 175, 80), QColor(40, 122, 46),
        QColor(224, 239, 255), QColor(179, 214, 255), QColor(128, 179, 242), QColor(66, 133, 214), QColor(32, 82, 163),
        QColor(244, 232, 255), QColor(225, 200, 255), QColor(199, 155, 255), QColor(168, 107, 224), QColor(123, 63, 174)
      })
    , _presetColorButtons()
    , _color(168, 107, 224)
    , _dragging(false)
    , _userMoved(false)
    , _dragOffset()
    , _parentOffset(8, 8)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    this->setAttribute(Qt::WA_DeleteOnClose, false);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setObjectName("SetColorCmdPanel");
    this->setStyleSheet(
        "QWidget#SetColorCmdPanel{background:#e7e7e7;border:1px solid #b7b7b7;}"
        "QFrame#titleBar{background:#0f6d93;border:none;}"
        "QLabel#titleLabel{color:#ffffff;font-weight:600;padding-left:6px;}"
        "QPushButton#titleButton{min-width:18px;max-width:18px;min-height:18px;max-height:18px;padding:0;border:1px solid #2b2b2b;background:#d7d7d7;color:#1f1f1f;}"
        "QFrame#section{background:#f3f3f3;border:1px solid #c3c3c3;}"
        "QLabel#sectionLabel{color:#202020;background:transparent;font-weight:600;padding:0 2px;}"
        "QLabel#fieldLabel{color:#202020;background:transparent;}"
        "QPushButton#swatchButton{min-height:22px;border:1px solid #7a7a7a;text-align:left;padding:0 6px;background:#ffffff;}"
        "QPushButton#presetButton{min-width:22px;max-width:22px;min-height:22px;max-height:22px;padding:0;}"
        "QPushButton#presetButton:pressed{padding:0;margin:0;}"
        "QPushButton#okBtn{min-width:66px;min-height:24px;border:1px solid #0f6d93;background:#0f6d93;color:#ffffff;}"
        "QPushButton#okBtn:hover{background:#117aa3;}"
        "QPushButton#okBtn:pressed{background:#0b5b7a;}"
        "QPushButton#cancelBtn{min-width:66px;min-height:24px;border:1px solid #0f6d93;background:#ffffff;color:#0f6d93;}"
        "QPushButton#cancelBtn:hover{background:#f2f9ff;}"
        "QPushButton#cancelBtn:pressed{background:#eef6fb;}");

    QVBoxLayout* pRootLayout = new QVBoxLayout(this);
    pRootLayout->setContentsMargins(0, 0, 0, 0);
    pRootLayout->setSpacing(0);

    _pTitleBar = new QFrame(this);
    _pTitleBar->setObjectName("titleBar");
    _pTitleBar->setFixedHeight(24);
    _pTitleBar->installEventFilter(this);
    QHBoxLayout* pTitleLayout = new QHBoxLayout(_pTitleBar);
    pTitleLayout->setContentsMargins(4, 2, 4, 2);
    pTitleLayout->setSpacing(4);

    _pTitleLabel = new QLabel(tr("Set Color"), _pTitleBar);
    _pTitleLabel->setObjectName("titleLabel");
    _pTitleLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    pTitleLayout->addWidget(_pTitleLabel, 1);

    pRootLayout->addWidget(_pTitleBar);

    QWidget* pBody = new QWidget(this);
    QVBoxLayout* pBodyLayout = new QVBoxLayout(pBody);
    pBodyLayout->setContentsMargins(8, 8, 8, 8);
    pBodyLayout->setSpacing(8);

    QFrame* pColorSection = new QFrame(pBody);
    pColorSection->setObjectName("section");
    QHBoxLayout* pColorLayout = new QHBoxLayout(pColorSection);
    pColorLayout->setContentsMargins(8, 8, 8, 8);
    pColorLayout->setSpacing(8);

    _pColorButton = new QPushButton("", pColorSection);
    _pColorButton->setObjectName("swatchButton");
    _pColorButton->setFocusPolicy(Qt::NoFocus);
    pColorLayout->addWidget(_pColorButton, 1);
    pBodyLayout->addWidget(pColorSection);

    QFrame* pPresetSection = new QFrame(pBody);
    pPresetSection->setObjectName("section");
    QGridLayout* pPresetLayout = new QGridLayout(pPresetSection);
    pPresetLayout->setContentsMargins(8, 8, 8, 8);
    pPresetLayout->setHorizontalSpacing(6);
    pPresetLayout->setVerticalSpacing(6);

    const int numColumns = 5;
    _presetColorButtons.reserve(_presetColors.size());
    for (int i = 0; i < _presetColors.size(); ++i)
    {
        QPushButton* pPresetButton = new QPushButton(pPresetSection);
        pPresetButton->setObjectName("presetButton");
        pPresetButton->setFocusPolicy(Qt::NoFocus);
        pPresetButton->setProperty("presetIndex", i);
        pPresetLayout->addWidget(pPresetButton, i / numColumns, i % numColumns);
        _presetColorButtons.push_back(pPresetButton);

        QObject::connect(pPresetButton, &QPushButton::clicked, this, [this, i]()
        {
            if (i >= 0 && i < _presetColors.size())
            {
                this->setColor(_presetColors[i]);
            }
        });
    }
    pBodyLayout->addWidget(pPresetSection);

    pBodyLayout->addStretch(1);

    QHBoxLayout* pFooterLayout = new QHBoxLayout();
    pFooterLayout->setContentsMargins(0, 0, 0, 0);
    pFooterLayout->setSpacing(6);
    pFooterLayout->addStretch(1);

    _pOkButton = new QPushButton(tr("OK"), pBody);
    _pOkButton->setObjectName("okBtn");
    _pOkButton->setFocusPolicy(Qt::NoFocus);
    pFooterLayout->addWidget(_pOkButton, 0);

    _pCancelButton = new QPushButton(tr("Cancel"), pBody);
    _pCancelButton->setObjectName("cancelBtn");
    _pCancelButton->setFocusPolicy(Qt::NoFocus);
    pFooterLayout->addWidget(_pCancelButton, 0);
    pBodyLayout->addLayout(pFooterLayout);

    pRootLayout->addWidget(pBody, 1);

    QObject::connect(_pColorButton, &QPushButton::clicked, this, &SetColorCmdPanel::onPickColor);
    QObject::connect(_pOkButton, &QPushButton::clicked, this, &SetColorCmdPanel::onOkClicked);
    QObject::connect(_pCancelButton, &QPushButton::clicked, this, &SetColorCmdPanel::onCancelClicked);

    this->setPickedCount(0);
    this->updateColorButton();
    this->setMinimumWidth(232);
    this->adjustSize();

    if (parent)
    {
        parent->installEventFilter(this);
        if (QWidget* pWindow = parent->window())
        {
            if (pWindow != parent)
            {
                pWindow->installEventFilter(this);
            }
        }
    }
}

SetColorCmdPanel::~SetColorCmdPanel()
{
    if (QWidget* pParent = this->parentWidget())
    {
        pParent->removeEventFilter(this);
        if (QWidget* pWindow = pParent->window())
        {
            if (pWindow != pParent)
            {
                pWindow->removeEventFilter(this);
            }
        }
    }
}

void SetColorCmdPanel::setColor(const QColor& color)
{
    if (!color.isValid() || color == _color)
    {
        return;
    }
    _color = color;
    this->updateColorButton();
    emit colorChanged(_color);
}

void SetColorCmdPanel::setPickedCount(int count)
{
    Q_UNUSED(count);
}

bool SetColorCmdPanel::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == _pTitleBar && event)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* pMouseEvent = static_cast<QMouseEvent*>(event);
            if (pMouseEvent && pMouseEvent->button() == Qt::LeftButton)
            {
                _dragging = true;
                _dragOffset = pMouseEvent->globalPos() - this->frameGeometry().topLeft();
                return true;
            }
        }
        else if (event->type() == QEvent::MouseMove)
        {
            if (_dragging)
            {
                QMouseEvent* pMouseEvent = static_cast<QMouseEvent*>(event);
                if (pMouseEvent && (pMouseEvent->buttons() & Qt::LeftButton))
                {
                    QPoint topLeft = pMouseEvent->globalPos() - _dragOffset;
                    this->move(topLeft);
                    _userMoved = true;
                    if (QWidget* pParent = this->parentWidget())
                    {
                        _parentOffset = topLeft - pParent->mapToGlobal(QPoint(0, 0));
                        this->clampToParentBounds();
                        this->move(pParent->mapToGlobal(_parentOffset));
                    }
                    return true;
                }
            }
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent* pMouseEvent = static_cast<QMouseEvent*>(event);
            if (pMouseEvent && pMouseEvent->button() == Qt::LeftButton)
            {
                _dragging = false;
                return true;
            }
        }
        else if (event->type() == QEvent::Leave)
        {
            _dragging = false;
        }
    }

    QWidget* pParent = this->parentWidget();
    QWidget* pWindow = pParent ? pParent->window() : nullptr;
    if ((watched == pParent || watched == pWindow) && event)
    {
        if (event->type() == QEvent::Resize || event->type() == QEvent::Move || event->type() == QEvent::Show)
        {
            QTimer::singleShot(0, this, [this]()
            {
                QWidget* pParentWidget = this->parentWidget();
                if (!pParentWidget)
                {
                    return;
                }

                if (_userMoved)
                {
                    this->clampToParentBounds();
                    this->move(pParentWidget->mapToGlobal(_parentOffset));
                }
                else
                {
                    this->anchorToTopLeft();
                }
            });
        }
    }
    return QWidget::eventFilter(watched, event);
}

void SetColorCmdPanel::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    if (_userMoved)
    {
        if (QWidget* pParent = this->parentWidget())
        {
            this->clampToParentBounds();
            this->move(pParent->mapToGlobal(_parentOffset));
        }
    }
    else
    {
        this->anchorToTopLeft();
    }
}

void SetColorCmdPanel::updateColorButton()
{
    const QString style = QString(
        "QPushButton#swatchButton {"
        "background-color: rgb(%1, %2, %3);"
        "border: 1px solid #6a6a6a;"
        "color:#1f1f1f;"
        "text-align:left;"
        "padding-left:6px;"
        "}")
        .arg(_color.red())
        .arg(_color.green())
        .arg(_color.blue());
    _pColorButton->setStyleSheet(style);
    this->refreshPresetColorButtons();
}

void SetColorCmdPanel::refreshPresetColorButtons()
{
    for (int i = 0; i < _presetColorButtons.size() && i < _presetColors.size(); ++i)
    {
        QPushButton* pButton = _presetColorButtons[i];
        if (!pButton)
        {
            continue;
        }

        const QColor& preset = _presetColors[i];
        const bool selected = (preset == _color);
        const QString presetStyle = QString(
            "QPushButton#presetButton{"
            "background-color: rgb(%1, %2, %3);"
            "border:%4;"
            "}")
            .arg(preset.red())
            .arg(preset.green())
            .arg(preset.blue())
            .arg(selected ? "2px solid #111111" : "2px solid #7a7a7a");
        pButton->setStyleSheet(presetStyle);
    }
}

void SetColorCmdPanel::anchorToTopLeft()
{
    QWidget* pParent = this->parentWidget();
    if (!pParent)
    {
        return;
    }

    const int margin = 8;
    _parentOffset = QPoint(margin, margin);
    this->clampToParentBounds();
    this->move(pParent->mapToGlobal(_parentOffset));
}

void SetColorCmdPanel::clampToParentBounds()
{
    QWidget* pParent = this->parentWidget();
    if (!pParent)
    {
        return;
    }

    const int minOffsetX = 0;
    const int minOffsetY = 0;
    const int maxOffsetX = qMax(minOffsetX, pParent->width() - this->width());
    const int maxOffsetY = qMax(minOffsetY, pParent->height() - this->height());

    _parentOffset.setX(qBound(minOffsetX, _parentOffset.x(), maxOffsetX));
    _parentOffset.setY(qBound(minOffsetY, _parentOffset.y(), maxOffsetY));
}

void SetColorCmdPanel::onPickColor()
{
    QColor color = QColorDialog::getColor(_color, this, tr("Select Color"));
    if (!color.isValid())
    {
        return;
    }
    this->setColor(color);
}

void SetColorCmdPanel::onOkClicked()
{
    this->hide();
    emit accepted();
}

void SetColorCmdPanel::onCancelClicked()
{
    this->hide();
    emit canceled();
}

