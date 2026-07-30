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

#include "CameraManipulator2d.h"
#include <cassert>
#include <osgViewer/View>

CameraManipulator2d::CameraManipulator2d()
	: osgGA::TrackballManipulator()
{
	this->setAllowThrow(false);

	// 默认的鼠标滚轮缩放操作和常用的CAD(SketchUp,AutoCAD)效果是相反的,
	// 通过对缩放参数取反来修正
	_wheelZoomFactor = -_wheelZoomFactor;
	_originalZoomFactor = _wheelZoomFactor;

	// added by wangyao 2022.02.16 {
	// 对于二维视图而言,设置摄像机离z=0的平面的最短距离
	_minimumDistance = 1e-3;
	_modelSize = 1.0;
	// }
}

CameraManipulator2d::~CameraManipulator2d()
{
}

void CameraManipulator2d::setZoomFactor(double factor)
{
	_wheelZoomFactor = _originalZoomFactor * factor;
}

bool CameraManipulator2d::performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
	// 屏蔽掉旋转功能,默认鼠标左键旋转
	//return osgGA::TrackballManipulator::performMovementLeftMouseButton(eventTimeDelta, dx, dy);
	return false;
}

bool CameraManipulator2d::performMovementMiddleMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
	// 鼠标中键移动视图
	return osgGA::TrackballManipulator::performMovementMiddleMouseButton(eventTimeDelta, dx, dy);
}

bool CameraManipulator2d::performMovementRightMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
	// 屏蔽掉右键按下移动鼠标缩放的功能
	//return osgGA::TrackballManipulator::performMovementRightMouseButton(eventTimeDelta, dx, dy);
	return false;
}

bool CameraManipulator2d::handleMouseDrag(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
	// 鼠标左右键不允许拖拽
	if (ea.getButtonMask() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON
		 || ea.getButtonMask() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
	{
		return false;
	}
	else
	{
		return osgGA::TrackballManipulator::handleMouseDrag(ea, us);
	}
}

bool CameraManipulator2d::handleMouseWheel(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
	osgViewer::View* view = dynamic_cast<osgViewer::View*>(us.asView());
	assert(view);
	osg::Camera* camera = view->getCamera();
	assert(camera);
	osg::Viewport* vp = camera->getViewport();
	assert(vp);
	// mouse screen position
	float x = ea.getX();
	float y = ea.getY();
	// view center screen position
	float cenX = vp->x() + vp->width() / 2;
	float cenY = vp->y() + vp->height() / 2;

	// move camera
	osg::Vec3d moveVec1;
	osg::Vec3d worldA;
	{
		// transform mouse position to world position
		// depth = 0.0
		osg::Matrix VPW = this->getInverseMatrix() * camera->getProjectionMatrix() *
			camera->getViewport()->computeWindowMatrix();
		osg::Matrix inverseVPW = osg::Matrix::inverse(VPW);
		worldA = osg::Vec3d(x, y, 0.0) * inverseVPW;

		// revised world position to z=0 plane
		osg::Vec3d eye, center, up;
		this->getTransformation(eye, center, up);
		osg::Vec3d lineOrigin = eye;
		osg::Vec3d lineDir = worldA - eye;
		lineDir.normalize();
		osg::Vec3d intPnt;
		bool retA = intersectionOfLineAndPlane(lineOrigin, lineDir,
			osg::Vec3d(0., 0., 0.), osg::Vec3d(0., 0., 1.), intPnt);
		if (!retA) return false;
		worldA = intPnt;
		worldA.z() = 0.0; // added by wangyao 2022.02.16 避免计算误差导致z!=0

		// compute depth value(range in [-1, 1])
		osg::Vec3d screenA = worldA * VPW;
		float depth = screenA.z();

		// screen center position --> world position in z=0 plane
		osg::Vec3d worldCenter = osg::Vec3d(cenX, cenY, depth) * inverseVPW;
		worldCenter.z() = 0.0; // added by wangyao 2022.02.16 避免计算误差导致z!=0

		// move camera
		moveVec1 = worldA - worldCenter;
		_center += moveVec1;
	}

	// zoom view
	bool zoomRet = osgGA::TrackballManipulator::handleMouseWheel(ea, us);
	if (!zoomRet)
	{
		_center -= moveVec1;
		return false;
	}

	// move camera
	osg::Vec3d moveVec2;
	osg::Vec3d worldB;
	{
		// transform mouse position to world position
		// depth = 0.0
		osg::Matrix VPW = this->getInverseMatrix() * camera->getProjectionMatrix() *
			camera->getViewport()->computeWindowMatrix();
		osg::Matrix inverseVPW = osg::Matrix::inverse(VPW);
		worldB = osg::Vec3d(x, y, 0.0) * inverseVPW;

		// revised world position to z=0 plane
		osg::Vec3d eye, center, up;
		this->getTransformation(eye, center, up);
		osg::Vec3d lineOrigin = eye;
		osg::Vec3d lineDir = worldB - eye;
		lineDir.normalize();
		osg::Vec3d intPnt;
		bool retA = intersectionOfLineAndPlane(lineOrigin, lineDir,
			osg::Vec3d(0., 0., 0.), osg::Vec3d(0., 0., 1.), intPnt);
		if (!retA) return false;
		worldB = intPnt;
		worldB.z() = 0.0; // added by wangyao 2022.02.16 避免计算误差导致z!=0

		// move camera
		moveVec2 = worldB - worldA;
		_center -= moveVec2;
	}

	assert(_center.z() == 0.0);
	return true;
}

void CameraManipulator2d::pan(double x, double y)
{
	this->panModel(x, y, 0.0f);
}

//void CameraManipulator2d::zoomModel(const float dy, bool pushForwardIfNeeded)
//{
//	// 强制设置pushForwardIfNeeded为false(OSG系统内部默认调用时传递的是true)
//	return osgGA::TrackballManipulator::zoomModel(dy, /*pushForwardIfNeeded*/false);
//}

void CameraManipulator2d::zoomModel(const float dy, bool pushForwardIfNeeded)
{
	pushForwardIfNeeded = false;

	// scale
	float scale = 1.0f + dy;

	// minimum distance
	float minDist = _minimumDistance;
	//if (getRelativeFlag(_minimumDistanceFlagIndex))
	//	minDist *= _modelSize;

	if (_distance * scale > minDist)
	{
		// regular zoom
		_distance *= scale;
	}
	else
	{
		if (pushForwardIfNeeded)
		{
			// push the camera forward
			float yscale = -_distance;
			osg::Matrixd rotation_matrix(_rotation);
			osg::Vec3d dv = (osg::Vec3d(0.0f, 0.0f, -1.0f) * rotation_matrix) * (dy * yscale);
			_center += dv;
		}
		else
		{
			// set distance on its minimum value
			_distance = minDist;
		}
	}
}

bool CameraManipulator2d::intersectionOfLineAndPlane(
    const osg::Vec3d& lineOrigin, const osg::Vec3d& lineDir,
    const osg::Vec3d& plnOrigin, const osg::Vec3d& plnNormal,
    osg::Vec3d& intPnt)
{
    // 直线的方向向量与平面的法向量垂直则不存在交点
    double s = lineDir * plnNormal;
    if (std::fabs(s) < 1e-5) return false;

    //
    double t = (plnOrigin - lineOrigin) * plnNormal / s;
    intPnt = lineOrigin + lineDir * t;
    return true;
}