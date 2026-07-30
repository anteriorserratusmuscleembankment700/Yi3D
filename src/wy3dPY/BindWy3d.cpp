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
#include <wy3dMath.h>
#include <wyVector2.h>
#include <wyVector3.h>

namespace py = pybind11;

void bindWy3d(py::module_& m)
{
    m.attr("EPS") = wy3d::EPS;
    m.attr("EPS_SQUARED") = wy3d::EPS_SQUARED;
    m.attr("PI") = wy3d::PI;
    m.attr("PI_2") = wy3d::PI_2;
    m.attr("PI_4") = wy3d::PI_4;
    m.attr("TWO_PI") = wy3d::TWO_PI;
    m.attr("E") = wy3d::E;
    m.attr("LOG2E") = wy3d::LOG2E;
    m.attr("LOG10E") = wy3d::LOG10E;
    m.attr("LN2") = wy3d::LN2;
    m.attr("LN10") = wy3d::LN10;

    m.def("degreesToRadians", &wy3d::degreesToRadians, py::arg("angle"));
    m.def("radiansToDegrees", &wy3d::radiansToDegrees, py::arg("rad"));
    m.def("normalizeRadian", &wy3d::normalizeRadian, py::arg("rad"));
    m.def("ellipsePolarAngleToParametricAngle", &wy3d::ellipsePolarAngleToParametricAngle,
        py::arg("polarRad"), py::arg("a"), py::arg("b"));
    m.def("computeTotalAngle", &wy3d::computeTotalAngle, py::arg("startAngle"), py::arg("endAngle"));

    py::class_<wy::Vector2>(m, "Vector2")
        // constructor
        .def(py::init<>())
        .def(py::init<double, double>(), py::arg("x"), py::arg("y"))

        // static const
        .def_readonly_static("kZero", &wy::Vector2::kZero)
        .def_readonly_static("kXAxis", &wy::Vector2::kXAxis)
        .def_readonly_static("kYAxis", &wy::Vector2::kYAxis)

        // get
        .def("x", &wy::Vector2::x)
        .def("y", &wy::Vector2::y)

        // set
        .def("set", &wy::Vector2::set, py::arg("x"), py::arg("y"))
        .def("setX", &wy::Vector2::setX, py::arg("x"))
        .def("setY", &wy::Vector2::setY, py::arg("y"))

        // length
        .def("length", &wy::Vector2::length)
        .def("length2", &wy::Vector2::length2)

        // normalize
        .def("normalize", &wy::Vector2::normalize)
        .def("normalized", &wy::Vector2::normalized)

        // dot & cross
        .def("dot", &wy::Vector2::dot, py::arg("rhs"))
        .def("cross", &wy::Vector2::cross, py::arg("rhs"))

        // += & -= & *= & /=
        .def("__iadd__", &wy::Vector2::operator+=,
            py::arg("rhs"), py::return_value_policy::reference)
        .def("__isub__", &wy::Vector2::operator-=,
            py::arg("rhs"), py::return_value_policy::reference)
        .def("__imul__", &wy::Vector2::operator*=,
            py::arg("scalar"), py::return_value_policy::reference)
        .def("__itruediv__", &wy::Vector2::operator/=,
            py::arg("scalar"), py::return_value_policy::reference)

        // == & != & <
        .def("__eq__", [](const wy::Vector2& a, const wy::Vector2& b) { return a == b; }, py::arg("rhs"))
        .def("__ne__", [](const wy::Vector2& a, const wy::Vector2& b) { return a != b; }, py::arg("rhs"))
        .def("__lt__", [](const wy::Vector2& a, const wy::Vector2& b) { return a < b; }, py::arg("rhs"))

        // +-*/
        .def("__add__", [](const wy::Vector2& a, const wy::Vector2& b) { return a + b; }, py::arg("rhs"))
        .def("__sub__", [](const wy::Vector2& a, const wy::Vector2& b) { return a - b; }, py::arg("rhs"))
        .def("__neg__", [](const wy::Vector2& a) { return -a; })
        .def("__mul__", [](const wy::Vector2& a, double f) { return a * f; }, py::arg("factor"))
        .def("__rmul__", [](const wy::Vector2& a, double f) { return a * f; }, py::arg("factor"))
        .def("__truediv__", [](const wy::Vector2& a, double d) { return a / d; }, py::arg("divisor"))

        // angle
        .def_static("angle", &wy::Vector2::angle,
            py::arg("lhs"), py::arg("rhs"))
        .def_static("rotationAngle", &wy::Vector2::rotationAngle,
            py::arg("from"), py::arg("to"))

        // 字符串表示
        .def("__repr__", [](const wy::Vector2& v) {
            return "Vector2(" + std::to_string(v.x()) + ", " + std::to_string(v.y()) + ")";
    });

    py::class_<wy::Vector3>(m, "Vector3")
        // constructor
        .def(py::init<>())
        .def(py::init<double, double, double>(), py::arg("x"), py::arg("y"), py::arg("z"))

        // static const
        .def_readonly_static("kZero", &wy::Vector3::kZero)
        .def_readonly_static("kXAxis", &wy::Vector3::kXAxis)
        .def_readonly_static("kYAxis", &wy::Vector3::kYAxis)
        .def_readonly_static("kZAxis", &wy::Vector3::kZAxis)

        // get
        .def("x", &wy::Vector3::x)
        .def("y", &wy::Vector3::y)
        .def("z", &wy::Vector3::z)

        // set
        .def("set", &wy::Vector3::set, py::arg("x"), py::arg("y"), py::arg("z"))
        .def("setX", &wy::Vector3::setX, py::arg("x"))
        .def("setY", &wy::Vector3::setY, py::arg("y"))
        .def("setZ", &wy::Vector3::setZ, py::arg("z"))

        // length
        .def("length", &wy::Vector3::length)
        .def("length2", &wy::Vector3::length2)

        // normalize
        .def("normalize", &wy::Vector3::normalize)
        .def("normalized", &wy::Vector3::normalized)

        // dot & cross
        .def("dot", &wy::Vector3::dot, py::arg("rhs"))
        .def("cross", &wy::Vector3::cross, py::arg("rhs"))

        // += & -= & *= & /=
        .def("__iadd__", &wy::Vector3::operator+=,
            py::arg("rhs"), py::return_value_policy::reference)
        .def("__isub__", &wy::Vector3::operator-=,
            py::arg("rhs"), py::return_value_policy::reference)
        .def("__imul__", &wy::Vector3::operator*=,
            py::arg("scalar"), py::return_value_policy::reference)
        .def("__itruediv__", &wy::Vector3::operator/=,
            py::arg("scalar"), py::return_value_policy::reference)

        // == & != & <
        .def("__eq__", [](const wy::Vector3& a, const wy::Vector3& b) { return a == b; }, py::arg("rhs"))
        .def("__ne__", [](const wy::Vector3& a, const wy::Vector3& b) { return a != b; }, py::arg("rhs"))
        .def("__lt__", [](const wy::Vector3& a, const wy::Vector3& b) { return a < b; }, py::arg("rhs"))

        // +-*/
        .def("__add__", [](const wy::Vector3& a, const wy::Vector3& b) { return a + b; }, py::arg("rhs"))
        .def("__sub__", [](const wy::Vector3& a, const wy::Vector3& b) { return a - b; }, py::arg("rhs"))
        .def("__neg__", [](const wy::Vector3& a) { return -a; })
        .def("__mul__", [](const wy::Vector3& a, double f) { return a * f; }, py::arg("factor"))
        .def("__rmul__", [](const wy::Vector3& a, double f) { return a * f; }, py::arg("factor"))
        .def("__truediv__", [](const wy::Vector3& a, double d) { return a / d; }, py::arg("divisor"))

        // angle
        .def_static("angle", &wy::Vector3::angle,
            py::arg("lhs"), py::arg("rhs"))

        // 字符串表示
        .def("__repr__", [](const wy::Vector3& v) {
            return "Vector3(" + std::to_string(v.x()) + ", " + std::to_string(v.y()) + ", " + std::to_string(v.z()) + ")";
        });
}
