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
#include <wydbElement.h>
#include <wydbDatabase.h>

namespace py = pybind11;

void bindWydbElement(py::module_& m)
{
    // wydb::Element
    py::class_<wydb::Element, wyrx::Object, std::unique_ptr<wydb::Element, py::nodelete>>(m, "Element")
        .def("getClassName", [](const wydb::Element& self) -> std::string {
            return self.getClassInfo()->className(); })
        .def("getId", &wydb::Element::getId)
        .def("getDatabase", &wydb::Element::getDatabase,
            py::return_value_policy::reference)
        .def("isErased", &wydb::Element::isErased)
        .def("erase", &wydb::Element::erase, py::arg("erasing") = true)
        .def("isHidden", &wydb::Element::isHidden)
        .def("hide", &wydb::Element::hide, py::arg("hiding") = true)
        .def("upgradeForWrite", &wydb::Element::upgradeForWrite)
        .def("getParent", &wydb::Element::getParent)
        .def("getChildren", &wydb::Element::getChildren)

        // 参数
        .def("listParameters", [](const wydb::Element& self)
            -> std::vector<std::pair<std::string, std::string>> {
            std::vector<std::pair<std::string, std::string>> result;
            for (const auto* p : self.listParameters()) {
                result.emplace_back(p->getClassName(), p->getName());
            }
            return result;
        })
        .def("getParameterValue", &wydb::Element::getParameterValue,
            py::arg("className"), py::arg("name"))
        .def("setParameterValue", &wydb::Element::setParameterValue,
            py::arg("className"), py::arg("name"), py::arg("value"));
}
