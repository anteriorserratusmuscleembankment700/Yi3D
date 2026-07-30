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

#include <cstdint>
#include <pybind11/pybind11.h>
#include <wydbElementId.h>

namespace py = pybind11;

void bindWydbElementId(py::module_& m)
{
    py::class_<wydb::ElementId>(m, "ElementId")
        // constructor
        .def(py::init<>())
        .def(py::init<std::uint64_t>(), py::arg("id"))

        // 静态常量
        .def_readonly_static("kNull", &wydb::ElementId::kNull)

        // 成员方法
        .def("value", &wydb::ElementId::value)
        .def("isNull", &wydb::ElementId::isNull)

        // 比较运算符重载 (hidden friends，通过 lambda 包装)
        .def("__eq__", [](const wydb::ElementId& a, const wydb::ElementId& b) { return a == b; }, py::arg("other"))
        .def("__ne__", [](const wydb::ElementId& a, const wydb::ElementId& b) { return a != b; }, py::arg("other"))
        .def("__lt__", [](const wydb::ElementId& a, const wydb::ElementId& b) { return a < b; }, py::arg("other"))
        .def("__gt__", [](const wydb::ElementId& a, const wydb::ElementId& b) { return a > b; }, py::arg("other"))
        .def("__le__", [](const wydb::ElementId& a, const wydb::ElementId& b) { return a <= b; }, py::arg("other"))
        .def("__ge__", [](const wydb::ElementId& a, const wydb::ElementId& b) { return a >= b; }, py::arg("other"))

        // 哈希
        .def("__hash__", [](const wydb::ElementId& id) {
            return std::hash<std::uint64_t>()(id.value());
        })

        // 字符串表示
        .def("__repr__", [](const wydb::ElementId& id) {
            return "wy3d.ElementId(" + std::to_string(id.value()) + ")";
        });
}
