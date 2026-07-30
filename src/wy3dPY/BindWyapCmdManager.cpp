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
#include <wyapApplication.h>
#include <wyapCmdManager.h>

namespace py = pybind11;

void bindWyapCmdManager(py::module_& m)
{
    // ========== AbortCause enum ==========
    py::enum_<wyap::CmdExecution::AbortCause>(m, "AbortCause")
        .value("UserCancel", wyap::CmdExecution::AbortCause::UserCancel)
        .value("ErrorTerminate", wyap::CmdExecution::AbortCause::ErrorTerminate)
        .value("ForceTerminate", wyap::CmdExecution::AbortCause::ForceTerminate)
        .value("UserDefinedBegin", wyap::CmdExecution::AbortCause::UserDefinedBegin);

    // ========== Command (non-owning reference) ==========
    py::class_<wyap::Command, std::unique_ptr<wyap::Command, py::nodelete>>(m, "Command")
        .def("getName", &wyap::Command::getName)
        .def("getFlags", &wyap::Command::getFlags)
        .def("isModal", &wyap::Command::isModal)
        .def("isTransparent", &wyap::Command::isTransparent);

    // ========== CmdExecution (non-owning reference) ==========
    py::class_<wyap::CmdExecution, std::unique_ptr<wyap::CmdExecution, py::nodelete>>(m, "CmdExecution")
        .def("getCommand", &wyap::CmdExecution::getCommand,
            py::return_value_policy::reference);

    // ========== CmdManager ==========
    py::class_<wyap::CmdManager, std::unique_ptr<wyap::CmdManager, py::nodelete>>(m, "CmdManager")
        .def("executeCommand", &wyap::CmdManager::executeCommand,
            py::arg("commandName"))
        .def("postCommand", &wyap::CmdManager::postCommand,
            py::arg("commandName"))
        .def("abortCurrentModalCommand", &wyap::CmdManager::abortCurrentModalCommand,
            py::arg("abortCause"))
        .def("getCurrentModalCommand", &wyap::CmdManager::getCurrentModalCommand,
            py::return_value_policy::reference)
        .def("getCurrentModalCmdExecution", &wyap::CmdManager::getCurrentModalCmdExecution,
            py::return_value_policy::reference)
        .def("getRecentCommands", &wyap::CmdManager::getRecentCommands);

    m.def("getCmdManager", []() -> wyap::CmdManager* {
        wyap::Application* pApp = wyap::getApplication();
        return pApp ? pApp->getCmdManager() : nullptr;
    }, py::return_value_policy::reference);
}
