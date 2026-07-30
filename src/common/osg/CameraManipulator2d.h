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

#pragma once

#include <osgGA/CameraManipulator>
#include <osgGA/TrackballManipulator>

class CameraManipulator2d : public osgGA::TrackballManipulator
{
public:
    CameraManipulator2d();
	~CameraManipulator2d();
	
	// 移动视图
	void pan(double x, double y);

	// 设置滚轮缩放速率
	void setZoomFactor(double factor);

protected:
	virtual bool performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy);
	virtual bool performMovementMiddleMouseButton(const double eventTimeDelta, const double dx, const double dy);
	virtual bool performMovementRightMouseButton(const double eventTimeDelta, const double dx, const double dy);
	virtual bool handleMouseDrag(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
	virtual bool handleMouseWheel(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

	// added by wangyao 2022.02.16 {
	// osgGA::OrbitManipulator在handleMouseWheel中调用zoomModel时传递pushForwardIfNeeded的值为true.
	// 故重载使始终传递pushForwardIfNeeded=false.
	void zoomModel(const float dy, bool pushForwardIfNeeded);
	// }

private:
    static bool intersectionOfLineAndPlane(
        const osg::Vec3d& lineOrigin, const osg::Vec3d& lineDir,
        const osg::Vec3d& plnOrigin, const osg::Vec3d& plnNormal,
        osg::Vec3d& intPnt);

private:
	double _originalZoomFactor;
};