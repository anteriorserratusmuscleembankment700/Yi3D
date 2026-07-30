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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <wydbDatabase.h>
#include <wydbTransaction.h>
#include <wy3dHelix.h>
#include <wy3dSketch.h>

namespace py = pybind11;

void bindWy3dCurve(py::module_& m)
{
    py::class_<wy3d::Curve, wy3d::Feature, std::unique_ptr<wy3d::Curve, py::nodelete>> curve_class(m, "Curve");

    py::class_<wy3d::Helix, wy3d::Curve, std::unique_ptr<wy3d::Helix, py::nodelete>>(m, "Helix")
        .def("getSketch", &wy3d::Helix::getSketch)
        .def("getPitch", &wy3d::Helix::getPitch)
        .def("setPitch", &wy3d::Helix::setPitch)
        .def("getTurns", &wy3d::Helix::getTurns)
        .def("setTurns", &wy3d::Helix::setTurns)
        .def("getStartAngle", &wy3d::Helix::getStartAngle)
        .def("setStartAngle", &wy3d::Helix::setStartAngle)
        .def("isClockWise", &wy3d::Helix::isClockWise)
        .def("setClockWise", &wy3d::Helix::setClockWise)
        .def("isReversed", &wy3d::Helix::isReversed)
        .def("setReversed", &wy3d::Helix::setReversed)

        .def_static("create",
            [](wydb::Transaction* pTrans, wy3d::Sketch* pSketch, double pitch, double turns, double startAngle) -> wy3d::Helix*
            {
                wy3d::Helix* pHelix = nullptr;
                wy::ErrorStatus status = wy3d::Helix::create(pTrans, pSketch, pitch, turns, startAngle, pHelix);
                return pHelix;
            },
            py::arg("transaction"),
            py::arg("sketch"),
            py::arg("pitch"),
            py::arg("turns"),
            py::arg("startAngle"),
            py::return_value_policy::reference);
}