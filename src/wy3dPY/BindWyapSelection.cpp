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
#include <sstream>
#include <wyapApplication.h>
#include <wyapSelManager.h>
#include <wyapSelection.h>
#include <cstdint>

namespace py = pybind11;

class PySelectionIterator
{
private:
    wy::Iterator<const wyap::Selection&> _iter;

public:
    PySelectionIterator(wy::Iterator<const wyap::Selection&> iter)
        : _iter(std::move(iter)) {}

    PySelectionIterator& __iter__()
    {
        return *this;
    }

    const wyap::Selection& __next__()
    {
        if (!_iter.isDone())
        {
            const wyap::Selection& item = _iter.current();
            _iter.moveNext();
            return item;
        }
        else
        {
            throw py::stop_iteration();
        }
    }
};

void bindWyapSelection(py::module_& m) 
{
    py::class_<PySelectionIterator>(m, "PySelectionIterator")
        .def("__iter__", &PySelectionIterator::__iter__)
        .def("__next__", &PySelectionIterator::__next__);

    py::class_<wyap::Selection>(m, "Selection")
        .def(py::init<const wydb::ElementId&>(), py::arg("elemId"))
        .def(py::init<std::uint32_t, const wydb::ElementId&>(), py::arg("type"), py::arg("elemId"))
        .def(py::init<std::uint32_t, const wydb::ElementId&, const std::string&>(),
            py::arg("type"), py::arg("elemId"), py::arg("subPath"))

        .def("getSelectionType", &wyap::Selection::getSelectionType)
        .def("setSelectionType", &wyap::Selection::setSelectionType)
        .def("getElementId", &wyap::Selection::getElementId)
        .def("setElementId", &wyap::Selection::setElementId)
        .def("getSubPath", &wyap::Selection::getSubPath)
        .def("setSubPath", &wyap::Selection::setSubPath)
        .def("getPickPosition", &wyap::Selection::getPickPosition)
        .def("setPickPosition", &wyap::Selection::setPickPosition)
        .def("__lt__", &wyap::Selection::operator<, py::arg("other"))

        // 字符串表示
        .def("__repr__", [](const wyap::Selection& sel) {
            std::stringstream ss;
            ss << "wy3d.Selection(" << sel.getSelectionType() << ", " << sel.getElementId().value() << ", \"";
            ss << sel.getSubPath();
            ss << "\")";
            return ss.str();
        });

    py::class_<wyap::SelectionSet>(m, "SelectionSet")
        .def(py::init<>())
        .def("isEmpty", &wyap::SelectionSet::isEmpty)
        .def("getCount", &wyap::SelectionSet::getCount)
        .def("contains", py::overload_cast<const wyap::Selection&>(&wyap::SelectionSet::contains, py::const_))
        .def("containsId", py::overload_cast<const wydb::ElementId&>(&wyap::SelectionSet::contains, py::const_))
        .def("getSelections", [](const wyap::SelectionSet& self, const wydb::ElementId& id) -> std::list<wyap::Selection> {
            std::list<wyap::Selection> sels;
            self.getSelections(id, sels);
            return sels; })
        .def("add", &wyap::SelectionSet::add)
        .def("remove", py::overload_cast<const wyap::Selection&>(&wyap::SelectionSet::remove))
        .def("removeId", py::overload_cast<const wydb::ElementId&>(&wyap::SelectionSet::remove))
        .def("clear", &wyap::SelectionSet::clear)
        .def("swap", &wyap::SelectionSet::swap)
        .def("__iter__", [](const wyap::SelectionSet& set) {
            return PySelectionIterator(set.createIterator());
        });

    m.def("getSelectionSet", []() -> const wyap::SelectionSet& {
        static const wyap::SelectionSet emptySS;
        wyap::Application* pAppIF = wyap::getApplication();
        if (!pAppIF) return emptySS;
        wyap::SelManager* pSelMgr = pAppIF->getSelManager();
        if (!pSelMgr) return emptySS;
        return pSelMgr->getSelections();
    });
}