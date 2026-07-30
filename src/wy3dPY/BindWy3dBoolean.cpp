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
#include <wy3dSolid.h>
#include <wy3dBoolean.h>
#include <wy3dUnion.h>
#include <wy3dIntersection.h>
#include <wy3dDifference.h>

namespace py = pybind11;

void bindWy3dBoolean(py::module_& m)
{
    py::enum_<wy3d::BooleanType>(m, "BooleanType")
        .value("Undefined", wy3d::BooleanType::Undefined)
        .value("Union", wy3d::BooleanType::Union)
        .value("Difference", wy3d::BooleanType::Difference)
        .value("Intersection", wy3d::BooleanType::Intersection)
        .def("__repr__", [](wy3d::BooleanType booleanType) {
        switch (booleanType) {
        case wy3d::BooleanType::Undefined: return "wy3d.BooleanType.Undefined";
        case wy3d::BooleanType::Union: return "wy3d.BooleanType.Union";
        case wy3d::BooleanType::Difference: return "wy3d.BooleanType.Difference";
        case wy3d::BooleanType::Intersection: return "wy3d.BooleanType.Intersection";
        default: return "wy3d.BooleanType.Unknown";
        }});

    py::class_ <wy3d::Boolean, wy3d::Solid, std::unique_ptr<wy3d::Boolean, py::nodelete >>(m, "Boolean")
        .def("getBooleanType", &wy3d::Boolean::getBooleanType)
        .def("getTarget", &wy3d::Boolean::getTarget)
        .def("getTools", &wy3d::Boolean::getTools)
        .def("addTool", &wy3d::Boolean::addTool)
        .def("cancelBoolean", &wy3d::Boolean::cancelBoolean);

    py::class_<wy3d::Union, wy3d::Boolean, std::unique_ptr<wy3d::Union, py::nodelete>>(m, "Union")
        .def_static("create",
            [](wydb::Transaction* pTrans, wy3d::Solid* pTarget, const std::vector<wy3d::Solid*>& tools) -> wy3d::Union*
            {
                wy3d::Union* pUnion = nullptr;
                wy::ErrorStatus status = wy3d::Union::create(pTrans, pTarget, tools, pUnion);
                return pUnion;
            },
            py::arg("transaction"),
            py::arg("target"),
            py::arg("tools"),
            py::return_value_policy::reference);

    py::class_<wy3d::Intersection, wy3d::Boolean, std::unique_ptr<wy3d::Intersection, py::nodelete>>(m, "Intersection")
        .def_static("create",
            [](wydb::Transaction* pTrans, wy3d::Solid* pTarget, const std::vector<wy3d::Solid*>& tools) -> wy3d::Intersection*
            {
                wy3d::Intersection* pIntersection = nullptr;
                wy::ErrorStatus status = wy3d::Intersection::create(pTrans, pTarget, tools, pIntersection);
                return pIntersection;
            },
            py::arg("transaction"),
            py::arg("target"),
            py::arg("tools"),
            py::return_value_policy::reference);

    py::class_<wy3d::Difference, wy3d::Boolean, std::unique_ptr<wy3d::Difference, py::nodelete>>(m, "Difference")
        .def_static("create",
            [](wydb::Transaction* pTrans, wy3d::Solid* pTarget, const std::vector<wy3d::Solid*>& tools) -> wy3d::Difference*
            {
                wy3d::Difference* pDifference = nullptr;
                wy::ErrorStatus status = wy3d::Difference::create(pTrans, pTarget, tools, pDifference);
                return pDifference;
            },
            py::arg("transaction"),
            py::arg("target"),
            py::arg("tools"),
            py::return_value_policy::reference);

}