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

#include <wy3dImpl.h>
#include <wy3dMath.h>

NS_WY3D_BEG

const double kMinValue = 0.001;  
const double kMaxValue = 1e6; // 100万

const double kMaxDraftAngle = wy3d::degreesToRadians(89.9); // 89.9度

const double kMaxHelixPitch = 100000; // 10万
const double kMaxHelixTurns = 1000;  // 1000

const unsigned int kMaxLinearPatternCount = 1000;
const unsigned int kMaxCircularPatternCount = 1000;

NS_WY3D_END