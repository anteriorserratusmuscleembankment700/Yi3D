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

#ifndef WY3DAPP_MEASURE_DISTANCE_GUI_CMD_H
#define WY3DAPP_MEASURE_DISTANCE_GUI_CMD_H

#include <QLabel>
#include <QWidget>
#include <QLineEdit>

#include <wyVector2.h>
#include <wyVector3.h>
#include <wy3dVector3.h>
#include <wy3dSketchPlane.h>
#include "commands/OsgGuiCommand.h"
#include "commands/transient/BasicTransient.h"

class MeasureDistanceGuiCmdControls;

class MeasureDistanceGuiCmd : public OsgGuiCommand
{
    WYRX_DECLARE_MEMBERS(MeasureDistanceGuiCmd, MeasureDistanceGuiCmd, OsgGuiCommand)
public:
    MeasureDistanceGuiCmd();

protected:
    GuiCmdSketchInfo _sketchInfo;
    virtual wyap::CmdExecution::StartResult onStart() override;
    virtual void onEnd() override;
    virtual void onAbort(wyap::CmdExecution::AbortCause cause) override;

protected:
    enum class Step
    {
        Undefined = 0,
        SpecifyStartPnt = 1,
        SpecifyEndPnt = 2,
    };
    virtual void reset();
    virtual void onEscapeKey() override;
    bool finishStep(Step step);
    void gotoStep(Step step);

    virtual void onMouseMove(const MouseEvent& event) override;
    virtual void onLeftMouseDown(const MouseEvent& event) override;

    // 初始化控件
    virtual GuiCmdControlsSPtr initControls() override;

private:
    Step _step;
    wy3d::SketchPlane _workPln;
    wy::Vector3 _startPnt;
    wy::Vector2 _startPnt2d;
    wy::Vector3 _endPnt;
    wy::Vector2 _endPnt2d;
    SketchSnapContextSPtr _pSketchSnapContext;
    std::shared_ptr<LineTransient> _pLineTransient;

    MeasureDistanceGuiCmdControls* _pMeasureDistanceGuiCmdCtrls;
};

class MeasureDistanceGuiCmdControls : public GuiCmdControls
{
public:
    MeasureDistanceGuiCmdControls();
    ~MeasureDistanceGuiCmdControls();

    void showLength();
    void hideLength();
    void setLength(double length);

protected:
    virtual void timerEvent(QTimerEvent* event) override;

private:
    GuiCmdLabel* _pLengthLabel;
};

#endif // WY3DAPP_MEASURE_DISTANCE_GUI_CMD_H