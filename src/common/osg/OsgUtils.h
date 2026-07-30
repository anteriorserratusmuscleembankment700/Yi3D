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

#include <osg/Node>
#include <osg/ShapeDrawable>
#include <osgText/Text>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>

// OSG工具类
class OsgUtils
{
public:
	static osg::Node* createBox();

	static osg::Node* BuildShapeMesh(
		const TopoDS_Shape& aShape,
		double deflection = 0.1);

    static osg::Node* BuildShapeEdges(
        const TopoDS_Shape& shape,
        double deflection = 0.1);

	static osg::Node* BuildEdgeLine(
		const TopoDS_Edge& edge,
		double deflection = 0.1);

	static bool setNodeColor(osg::Node* node, const osg::Vec4& color);

	// 创建Box
	static osg::Node* createShapeDrawable_Box(
        float length, float width, float height, const osg::Vec4& color);
    // 创建Cylinder
	static osg::Node* createShapeDrawable_Cylinder(
		float radius, float height, const osg::Vec4& color);
    // 创建Cone
	static osg::Node* createShapeDrawable_Cone(
		float radius, float height, const osg::Vec4& color);
    // 创建Sphere
	static osg::Node* createShapeDrawable_Sphere(
		float radius, const osg::Vec4& color);
	// 直线段
	static osg::Node* createGeometry_Line(
		const osg::Vec3d& startPoint,
		const osg::Vec3d& endPoint,
		const osg::Vec4& color = osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f),
		float lineWidth = 1.0);
	// 点
	static osg::Node* createGeometry_Point(
		const osg::Vec3d& position,
		const osg::Vec4& color = osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f),
		float pointSize = 1.0);

	// 单行文本
	static osgText::Text* create_Text(
		const osg::Vec3d& position);
	// 多行文本
	static osg::Geode* create_MultiLinesTexts(
		const osg::Vec3d& position);
	// 抬头显示文本
	static osg::Camera* create_HUDTexts();
	static osg::Camera* create_LeftTopHUDTexts(osg::View* view);
};