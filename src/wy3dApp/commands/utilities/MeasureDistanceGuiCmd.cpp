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

#include "commands/utilities/MeasureDistanceGuiCmd.h"

#include <wyVector2.h>
#include <wyVector3.h>
#include <wydbDatabase.h>
#include <wyapDocManager.h>
#include <wyapDocument.h>

#include "application/Application.h"
#include "snap/SketchSnapSystem.h"
#include "widgets/frame/MainWindow.h"


MeasureDistanceGuiCmd::MeasureDistanceGuiCmd() : OsgGuiCommand(), _step(Step::Undefined), _pMeasureDistanceGuiCmdCtrls(nullptr)
{
    _options.pointSelect = false;
    _options.boxSelect = false;
}

wyap::CmdExecution::StartResult MeasureDistanceGuiCmd::onStart()
{
    wyap::CmdExecution::StartResult ret = GuiCommand::onStart();
    assert(wyap::CmdExecution::StartResult::Succeeded == ret);

    _sketchInfo = GuiCommandUtil::initSketchInfo();
    if (_sketchInfo.pSketchSnapSys) _sketchInfo.pSketchSnapSys->clearSnapResult();

    // 命令控件
    _pMeasureDistanceGuiCmdCtrls = dynamic_cast<MeasureDistanceGuiCmdControls*>(_pControls.get());
    assert(_pMeasureDistanceGuiCmdCtrls);

    // 初始化
    this->gotoStep(Step::SpecifyStartPnt);

    // 鼠标样式
    Application::instance().setCursor(CursorType::Locate);

    return wyap::CmdExecution::StartResult::Succeeded;
}
void MeasureDistanceGuiCmd::onEnd()
{
    __baseClass::onEnd();

    _pLineTransient = nullptr;
}
void MeasureDistanceGuiCmd::onAbort(wyap::CmdExecution::AbortCause cause)
{
    __baseClass::onAbort(cause);

    _pLineTransient = nullptr;
}

void MeasureDistanceGuiCmd::reset()
{
    _step = Step::Undefined;
    _startPnt.set(0.0, 0.0, 0.0);
    _startPnt2d.set(0.0, 0.0);
    _endPnt.set(0.0, 0.0, 0.0);
    _endPnt2d.set(0.0, 0.0);
    _pSketchSnapContext = nullptr;
    _pLineTransient = nullptr;

    if (_pMeasureDistanceGuiCmdCtrls)
    {
        _pMeasureDistanceGuiCmdCtrls->hideLength();
        _pMeasureDistanceGuiCmdCtrls->setLength(0.0);
    }

    this->gotoStep(Step::SpecifyStartPnt);
}

void MeasureDistanceGuiCmd::onEscapeKey()
{
    if (_step == Step::SpecifyStartPnt || _step == Step::Undefined)
    {
        this->requestAbort(AbortCause::UserCancel);
    }
    else
    {
        this->reset();
    }
}

bool MeasureDistanceGuiCmd::finishStep(Step step)
{
    switch (step)
    {
    case Step::SpecifyStartPnt:
    {
        // 显示直线段
        _pLineTransient = std::make_shared<LineTransient>();
        _pLineTransient->update(_startPnt, _startPnt);

        // 下一步
        this->gotoStep(Step::SpecifyEndPnt);
        return true;
    }
    break;

    case Step::SpecifyEndPnt:
    {
        // 刷新直线段
        if (_pLineTransient)
        {
            _pLineTransient->update(_startPnt, _endPnt);
        }

        // 销毁直线段
        _pLineTransient = nullptr;

        // 循环执行第一步
        this->gotoStep(Step::SpecifyStartPnt);
        return true;
    }
    break;

    default:
    {
        assert(false);
    }
    break;
    }

    return false;
}

void MeasureDistanceGuiCmd::gotoStep(Step step)
{
    _step = step;

    switch (step)
    {
    case Step::SpecifyStartPnt:
    {
        // 隐藏长度标签控件
        if (_pMeasureDistanceGuiCmdCtrls)
        {
            _pMeasureDistanceGuiCmdCtrls->hideLength();
        }

        // 禁用文本输入
        // 提示信息
        Application::instance().getStatusBar()->setTips(QCoreApplication::translate("MeasureDistanceGuiCmd",
            "Specify the start point."));

        // 草图捕捉系统
        if (_sketchInfo.pSketchSnapSys)
        {
            if (_sketchInfo.pSketchSnapSys)
            {
                _sketchInfo.pSketchSnapSys->partiallyUpdate(Application::instance().getActiveDatabase());
            }
            _pSketchSnapContext = std::make_shared<SketchLocateContext>(wydb::ElementId::kNull);
        }
    }
    break;

    case Step::SpecifyEndPnt:
    {
        // 显示长度标签控件
        if (_pMeasureDistanceGuiCmdCtrls)
        {
            _pMeasureDistanceGuiCmdCtrls->setLength(0.0);
            _pMeasureDistanceGuiCmdCtrls->showLength();
        }

        // 禁用文本输入
        // 提示信息
        Application::instance().getStatusBar()->setTips(QCoreApplication::translate("MeasureDistanceGuiCmd",
            "Specify the end point."));

        // 草图捕捉系统
        if (_sketchInfo.pSketchSnapSys)
        {
            SketchDrawLineContextSPtr pDrawLineContext = std::make_shared<SketchDrawLineContext>(
                wydb::ElementId::kNull, _startPnt2d);
            _pSketchSnapContext = pDrawLineContext;
        }
    }
    break;

    default:
    {
        Application::instance().getStatusBar()->setTips("");
        assert(false);
    }
    break;
    }
}

void MeasureDistanceGuiCmd::onMouseMove(const MouseEvent& event)
{
    switch (_step)
    {
    case Step::SpecifyStartPnt:
    {
        if (_sketchInfo.pSketchSnapSys)
        {
            wy::Vector2 pnt2d = this->computePosition2d(event.x, event.y, _sketchInfo.sketchPlane, _snapExcludeIds, _pSketchSnapContext, _sketchInfo.pSketchSnapSys);
        }
        else
        {
            auto ret = this->computePosition3d(event.x, event.y, _workPln, _snapExcludeIds);
            wy::Vector3 pnt3d = ret.first;
            if (ret.second)
            {
                pnt3d = ret.second->getPosition();
            }
        }
        return;
    }
    break;

    case Step::SpecifyEndPnt:
    {
        if (_sketchInfo.pSketchSnapSys)
        {
            wy::Vector2 endPnt2d = this->computePosition2d(event.x, event.y, _sketchInfo.sketchPlane, _snapExcludeIds, _pSketchSnapContext, _sketchInfo.pSketchSnapSys);
            if (_pLineTransient) _pLineTransient->update(_sketchInfo.sketchPlane, _startPnt2d, endPnt2d);
            if (_pMeasureDistanceGuiCmdCtrls) _pMeasureDistanceGuiCmdCtrls->setLength((endPnt2d - _startPnt2d).length());
        }
        else
        {
            auto ret = this->computePosition3d(event.x, event.y, wy3d::SketchPlane(), _snapExcludeIds);
            wy::Vector3 endPnt = ret.first;
            if (ret.second)
            {
                endPnt = ret.second->getPosition();
            }
            if (_pLineTransient) _pLineTransient->update(_startPnt, endPnt);
            if (_pMeasureDistanceGuiCmdCtrls) _pMeasureDistanceGuiCmdCtrls->setLength((endPnt - _startPnt).length());
        }
        return;
    }
    break;

    default:
    {
        assert(false);
        return;
    }
    break;
    }

    return;
}

void MeasureDistanceGuiCmd::onLeftMouseDown(const MouseEvent& event)
{
    switch (_step)
    {
    case Step::SpecifyStartPnt:
    {
        if (_sketchInfo.pSketchSnapSys)
        {
            _startPnt2d = this->computePosition2d(event.x, event.y, _sketchInfo.sketchPlane, _snapExcludeIds, _pSketchSnapContext, _sketchInfo.pSketchSnapSys);
        }
        else
        {
            auto ret = this->computePosition3d(event.x, event.y, wy3d::SketchPlane(), _snapExcludeIds);
            _startPnt = ret.first;
            if (ret.second)
            {
                _startPnt = ret.second->getPosition();
            }
        }
        this->finishStep(_step);
        return;
    }
    break;

    case Step::SpecifyEndPnt:
    {
        if (_sketchInfo.pSketchSnapSys)
        {
            _endPnt2d = this->computePosition2d(event.x, event.y, _sketchInfo.sketchPlane, _snapExcludeIds, _pSketchSnapContext, _sketchInfo.pSketchSnapSys);
        }
        else
        {
            auto ret = this->computePosition3d(event.x, event.y, wy3d::SketchPlane(), _snapExcludeIds);
            _endPnt = ret.first;
            if (ret.second)
            {
                _endPnt = ret.second->getPosition();
            }
        }
        this->finishStep(_step);
        return;
    }
    break;

    default:
    {
        assert(false);
        return;
    }
    break;
    }

    return;
}

GuiCmdControlsSPtr MeasureDistanceGuiCmd::initControls()
{
    return std::make_shared<MeasureDistanceGuiCmdControls>();
}

MeasureDistanceGuiCmdControls::MeasureDistanceGuiCmdControls() : GuiCmdControls(), _pLengthLabel(nullptr)
{
    _pLengthLabel = this->newLabel();
    _pLengthLabel->setText("");
    _pLengthLabel->hide();
}

MeasureDistanceGuiCmdControls::~MeasureDistanceGuiCmdControls()
{
}

void MeasureDistanceGuiCmdControls::showLength()
{
    _pLengthLabel->show();
}

void MeasureDistanceGuiCmdControls::hideLength()
{
    _pLengthLabel->hide();
}

void MeasureDistanceGuiCmdControls::setLength(double length)
{
    _pLengthLabel->setText(QString::number(length, 'f', 4));
}

void MeasureDistanceGuiCmdControls::timerEvent(QTimerEvent* event)
{
    static QPoint delta(12, -12);
    QPoint newPosLocal = _pLengthLabel->parentWidget()->mapFromGlobal(
        QCursor::pos() + delta - QPoint(0, _pLengthLabel->size().height()));
    if (newPosLocal != _pLengthLabel->pos())
    {
        _pLengthLabel->move(newPosLocal);
    }
}