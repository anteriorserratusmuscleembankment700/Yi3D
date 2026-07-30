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

#include "ChamferDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QMouseEvent>
#include <QKeyEvent>
#include <wy3dImpl.h>

ChamferDistanceLineEdit::ChamferDistanceLineEdit(QWidget* parent) : QLineEdit(parent)
{
}

void ChamferDistanceLineEdit::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_Enter:  // 确定
    case Qt::Key_Return: // 确定
    case Qt::Key_Escape: // 取消
    {
        event->ignore(); // 由父对话框处理
    }
    break;

    default:
    {
        QLineEdit::keyPressEvent(event);
    }
    }
}

ChamferDialog::ChamferDialog(double distance, QWidget* parent)
    : QDialog(parent), _distance(std::fabs(distance))
{
    this->setWindowTitle(tr("Chamfer"));

    QVBoxLayout* layout = new QVBoxLayout(this);

    QGridLayout* gridLayout = new QGridLayout();
    // 第一行:倒角距离
    {
        //
        QLabel* label = new QLabel(this);
        label->setText(tr("Distance"));
        gridLayout->addWidget(label, 0, 0);
        //
        _distanceEdit = new ChamferDistanceLineEdit(this);
        _distanceEdit->setText(QString::number(_distance));
        _distanceEdit->selectAll();
        {
            // 可匹配的示例：0、0.5、5.0、100.01
            // 不可匹配的示例: - 5、.5、00.5
            QRegExp regExp("^(0|[1-9]\\d*)(\\.\\d+)?$");
            QRegExpValidator* validator = new QRegExpValidator(regExp, _distanceEdit);
            _distanceEdit->setValidator(validator);
        }
        gridLayout->addWidget(_distanceEdit, 0, 1);
    }
    layout->addLayout(gridLayout);

    // 创建OK和Cancel按钮
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* okBtn = new QPushButton(tr("Ok"), this);
    QPushButton* cancelBtn = new QPushButton(tr("Cancel"), this);
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);

    // 信号槽
    this->connect(okBtn, SIGNAL(clicked()), this, SLOT(onOkBtnClicked()));
    this->connect(cancelBtn, SIGNAL(clicked()), this, SLOT(onCancelBtnClicked()));

    // 固定大小
    this->adjustSize();
    this->setFixedSize(this->size());
}

void ChamferDialog::onOkBtnClicked()
{
    bool ok(false);
    double distance = _distanceEdit->text().toDouble(&ok);
    if (!ok)
    {
        return;
    }
    if (distance < wy3d::kMinValue || distance > wy3d::kMaxValue)
    {
        QMessageBox::warning(this, tr("Warning"), tr(
            "Please enter a number that is greater than or equal to 0.001 and less than or equal to 1000000."));
        return;
    }

    _distance = distance;
    this->accept();
}

void ChamferDialog::onCancelBtnClicked()
{
    this->reject();
}

void ChamferDialog::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
    {
        return this->onOkBtnClicked(); // Ok
    }
    break;

    case Qt::Key_Escape:
    {
        return this->onCancelBtnClicked(); // Cancel
    }
    break;
    }

    QWidget::keyPressEvent(event);
}