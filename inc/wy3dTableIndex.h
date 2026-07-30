///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2025 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY3D_TABLE_INDEX_H
#define WY3D_TABLE_INDEX_H

#include <wy3dDefs.h>

NS_WY3D_BEG

constexpr int BOX_TABLE_INDEX = 2;
constexpr int CYLINDER_TABLE_INDEX = 3;
constexpr int SPHERE_TABLE_INDEX = 4;
constexpr int CONE_TABLE_INDEX = 5;
constexpr int TORUS_TABLE_INDEX = 6;
constexpr int TUBE_TABLE_INDEX = 7;
constexpr int BOOLEAN_TABLE_INDEX = 8;
constexpr int DATUM_PLANE_TABLE_INDEX = 9;
constexpr int SKETCH_TABLE_INDEX = 10;

// 草绘图元
constexpr int SKETCH_ENTITY_TABLE_INDEX_BEG = 11;
constexpr int SKETCH_POINT_TABLE_INDEX = 11;
constexpr int SKETCH_LINE_TABLE_INDEX = 12;
constexpr int SKETCH_CIRCLE_TABLE_INDEX = 13;
constexpr int SKETCH_ARC_TABLE_INDEX = 14;
constexpr int SKETCH_ELLIPSE_TABLE_INDEX = 15;
constexpr int SKETCH_ELLIPSE_ARC_TABLE_INDEX = 16;
constexpr int SKETCH_SPLINE_TABLE_INDEX = 17;
constexpr int SKETCH_CENTER_LINE_TABLE_INDEX = 18;
constexpr int SKETCH_ENTITY_TABLE_INDEX_END = 19;

// 拉伸体
constexpr int EXTRUSION_TABLE_INDEX = 20;
// 旋转体
constexpr int REVOLUTION_TABLE_INDEX = 21;
// 扫描体
constexpr int SWEEP_TABLE_INDEX = 22;
// 放样体
constexpr int LOFT_TABLE_INDEX = 23;

// 倒角
constexpr int CHAMFER_TABLE_INDEX = 28;
// 圆角
constexpr int FILLET_TABLE_INDEX  = 29;
// 抽壳
constexpr int SHELL_TABLE_INDEX   = 30;
// 拔模
constexpr int DRAFT_TABLE_INDEX   = 31;
// 阵列
constexpr int PATTERN_TABLE_INDEX = 32;

// 曲线
constexpr int CURVE_TABLE_INDEX = 34;

NS_WY3D_END

#endif // WY3D_TABLE_INDEX_H