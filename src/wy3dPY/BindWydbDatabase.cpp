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
#include <wydbElement.h>
#include <wydbDatabase.h>
#include <wydbFileFormat.h>
#include <wydbTransaction.h>
#include <wyIterator.h>

namespace py = pybind11;

class PyDatabaseIterator
{
private:
    wy::Iterator<wydb::ElementId> _iter;

public:
    PyDatabaseIterator(wy::Iterator<wydb::ElementId> iter)
        : _iter(std::move(iter)) {}

    PyDatabaseIterator& __iter__()
    {
        return *this;
    }

    wydb::ElementId __next__()
    {
        if (!_iter.isDone())
        {
            wydb::ElementId id = _iter.current();
            _iter.moveNext();
            return id;
        }
        else
        {
            throw py::stop_iteration();
        }
    }
};

void bindWydbDatabase(py::module_& m)
{
    py::class_<PyDatabaseIterator>(m, "PyDatabaseIterator")
        .def("__iter__", &PyDatabaseIterator::__iter__)
        .def("__next__", &PyDatabaseIterator::__next__);

    // FileType
    py::enum_<wydb::FileType>(m, "FileType")
        .value("Text", wydb::FileType::Text)
        .value("Binary", wydb::FileType::Binary);

    // FileVersion
    py::class_<wydb::FileVersion>(m, "FileVersion")
        .def(py::init<>())
        .def(py::init<unsigned short, unsigned short>(),
            py::arg("major"), py::arg("minor"))
        .def_readwrite("major", &wydb::FileVersion::major)
        .def_readwrite("minor", &wydb::FileVersion::minor);

    // WriteFileOption
    py::class_<wydb::Database::WriteFileOption>(m, "WriteFileOption")
        .def(py::init<>())
        .def_readwrite("fileType", &wydb::Database::WriteFileOption::fileType)
        .def_readwrite("fileVersion", &wydb::Database::WriteFileOption::fileVersion)
        .def_readwrite("precision", &wydb::Database::WriteFileOption::precision);

    // ReadFileOption
    py::class_<wydb::Database::ReadFileOption>(m, "ReadFileOption")
        .def(py::init<>())
        .def_readwrite("fileType", &wydb::Database::ReadFileOption::fileType);

    py::class_<wydb::Database, std::unique_ptr<wydb::Database, py::nodelete>>(m, "Database")
        // getElement
        .def("getElement",
            py::overload_cast<const wydb::ElementId&>(&wydb::Database::getElement, py::const_),
            py::return_value_policy::reference,
            py::arg("id"))
        // getTransactionManager
        .def("getTransactionManager", &wydb::Database::getTransactionManager,
            py::return_value_policy::reference)
        // writeFile/readFile
        .def("writeFile", &wydb::Database::writeFile,
            py::arg("fileFullPath"), py::arg("option"))
        .def("readFile", &wydb::Database::readFile,
            py::arg("fileFullPath"), py::arg("option"))
        // iterator
        .def("__iter__", [](const wydb::Database& db) {
            return PyDatabaseIterator(db.createIterator());
        }, py::keep_alive<0, 1>());
}
