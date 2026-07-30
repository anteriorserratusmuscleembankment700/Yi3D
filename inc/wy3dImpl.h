///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY3D_IMPL_H
#define WY3D_IMPL_H

#include <wy3dDefs.h>
#include <wy3dDefaultChainUpdateFeedback.h>

NS_WY3D_BEG

WY3D_EXPORT extern const double kMinValue;
WY3D_EXPORT extern const double kMaxValue;
constexpr double TOL = 1e-7;

// 最大拔模角度
WY3D_EXPORT extern const double kMaxDraftAngle;

// 螺旋线最大螺距
WY3D_EXPORT extern const double kMaxHelixPitch;
// 螺旋线最多圈数
WY3D_EXPORT extern const double kMaxHelixTurns;

// 线性阵列最大数量
WY3D_EXPORT extern const unsigned int kMaxLinearPatternCount;
// 圆周阵列最大数量
WY3D_EXPORT extern const unsigned int kMaxCircularPatternCount;

NS_WY3D_END

#endif // WY3D_IMPL_H
