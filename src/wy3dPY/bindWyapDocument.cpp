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
#include <wyapDocument.h>
#include <wyapDocumentId.h>
#include <wydbDatabase.h>

namespace py = pybind11;

void bindWyapDocument(py::module_& m)
{
    // ========== DocumentId ==========
    py::class_<wyap::DocumentId>(m, "DocumentId")
        .def(py::init<>())
        .def(py::init<std::uint32_t>(), py::arg("value"))
        .def_readonly_static("kNull", &wyap::DocumentId::kNull)
        .def("value", &wyap::DocumentId::value)
        .def("isNull", &wyap::DocumentId::isNull)
        .def("__eq__", [](const wyap::DocumentId& a, const wyap::DocumentId& b) { return a == b; })
        .def("__ne__", [](const wyap::DocumentId& a, const wyap::DocumentId& b) { return a != b; })
        .def("__lt__", [](const wyap::DocumentId& a, const wyap::DocumentId& b) { return a < b; })
        .def("__hash__", [](const wyap::DocumentId& id) {
            return std::hash<std::uint32_t>()(id.value());
        })
        .def("__repr__", [](const wyap::DocumentId& id) {
            return "DocumentId(" + std::to_string(id.value()) + ")";
        });

    // ========== DocumentStatus ==========
    py::enum_<wyap::DocumentStatus>(m, "DocumentStatus")
        .value("None_", wyap::DocumentStatus::None)
        .value("NewlyCreated", wyap::DocumentStatus::NewlyCreated)
        .value("Modified", wyap::DocumentStatus::Modified)
        .def("__repr__", [](wyap::DocumentStatus s) {
            switch (s) {
            case wyap::DocumentStatus::None:         return "wy3d.DocumentStatus.None";
            case wyap::DocumentStatus::NewlyCreated: return "wy3d.DocumentStatus.NewlyCreated";
            case wyap::DocumentStatus::Modified:     return "wy3d.DocumentStatus.Modified";
            default: return "wy3d.DocumentStatus.Unknown";
            }});

    // ========== Document ==========
    py::class_<wyap::Document, std::unique_ptr<wyap::Document, py::nodelete>>(m, "Document")
        .def("getDatabase", &wyap::Document::getDatabase,
            py::return_value_policy::reference_internal)
        .def("getFileName", &wyap::Document::getFileName)
        .def("getId", &wyap::Document::getId)
        .def("hasStatus", &wyap::Document::hasStatus,
            py::arg("status"))
        .def("getStatus", &wyap::Document::getStatus);
}
