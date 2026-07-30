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
#include <wy3dColor.h>

namespace py = pybind11;

void bindWy3dColor(py::module_& m)
{
    py::class_<wy3d::Color>(m, "Color")
        .def(py::init<unsigned char, unsigned char, unsigned char>(),
            py::arg("r"), py::arg("g"), py::arg("b"))
        .def_readwrite("red", &wy3d::Color::red)
        .def_readwrite("green", &wy3d::Color::green)
        .def_readwrite("blue", &wy3d::Color::blue)
        .def("__eq__", [](const wy3d::Color& a, const wy3d::Color& b) { return a == b; })
        .def("__ne__", [](const wy3d::Color& a, const wy3d::Color& b) { return a != b; })
        .def("__repr__", [](const wy3d::Color& c) {
            return "Color(" + std::to_string(c.red) + ", " + std::to_string(c.green)
                + ", " + std::to_string(c.blue) + ")";
        });
}
