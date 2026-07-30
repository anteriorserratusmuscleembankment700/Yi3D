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

#ifndef WY_UNIT_TEST_HEADERS_H
#define WY_UNIT_TEST_HEADERS_H

#include <gtest/gtest.h>

// wydb
#include <wydbDatabase.h>
#include <wydbTransaction.h>
#include <wydbFileFormat.h>

// wy3d
#include <wy3dBox.h>
#include <wy3dCylinder.h>
#include <wy3dSphere.h>
#include <wy3dCone.h>
#include <wy3dTorus.h>
#include <wy3dTube.h>
#include <wy3dDatabase.h>

// wy3d boolean
#include <wy3dUnion.h>
#include <wy3dDifference.h>
#include <wy3dIntersection.h>

using namespace wy3d;

#endif // WY_UNIT_TEST_HEADERS_H
