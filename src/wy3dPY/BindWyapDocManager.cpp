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
#include <wyapDocManager.h>
#include <wyapDocument.h>
#include <wyapEnums.h>
#include <wydbDatabase.h>

namespace py = pybind11;

// ========== DocManagerReactor 的 pybind11 跳板类 ==========
class PyDocManagerReactor : public wyap::DocManagerReactor
{
public:
    using wyap::DocManagerReactor::DocManagerReactor;

    void onDocumentCreated(wyap::Document* pNewDoc) override
    {
        PYBIND11_OVERRIDE(void, wyap::DocManagerReactor, onDocumentCreated, pNewDoc);
    }
    void onDocumentToBeDestroyed(wyap::Document* pDocToDestroy) override
    {
        PYBIND11_OVERRIDE(void, wyap::DocManagerReactor, onDocumentToBeDestroyed, pDocToDestroy);
    }
    void onDocumentDestroyed(const std::string& fileName) override
    {
        PYBIND11_OVERRIDE(void, wyap::DocManagerReactor, onDocumentDestroyed, fileName);
    }
    void onDocumentToBeDeactivated(wyap::Document* pDocToDeactivate) override
    {
        PYBIND11_OVERRIDE(void, wyap::DocManagerReactor, onDocumentToBeDeactivated, pDocToDeactivate);
    }
    void onDocumentDeactivated(wyap::Document* pDeactivatedDoc) override
    {
        PYBIND11_OVERRIDE(void, wyap::DocManagerReactor, onDocumentDeactivated, pDeactivatedDoc);
    }
    void onDocumentToBeActivated(wyap::Document* pDocToActivate) override
    {
        PYBIND11_OVERRIDE(void, wyap::DocManagerReactor, onDocumentToBeActivated, pDocToActivate);
    }
    void onDocumentActivated(wyap::Document* pActivatedDoc) override
    {
        PYBIND11_OVERRIDE(void, wyap::DocManagerReactor, onDocumentActivated, pActivatedDoc);
    }
    void onDocumentStatusChanged(wyap::Document* pDoc, unsigned int oldStatus) override
    {
        PYBIND11_OVERRIDE(void, wyap::DocManagerReactor, onDocumentStatusChanged, pDoc, oldStatus);
    }
    void onDocumentTitleUpdated(wyap::Document* pDoc) override
    {
        PYBIND11_OVERRIDE(void, wyap::DocManagerReactor, onDocumentTitleUpdated, pDoc);
    }
};

void bindWyapDocManager(py::module_& m)
{
    // ========== ExecutionMode ==========
    py::enum_<wyap::ExecutionMode>(m, "ExecutionMode")
        .value("Sync", wyap::ExecutionMode::Sync)
        .value("Async", wyap::ExecutionMode::Async)
        .def("__repr__", [](wyap::ExecutionMode mode) {
            switch (mode) {
            case wyap::ExecutionMode::Sync:  return "wy3d.ExecutionMode.Sync";
            case wyap::ExecutionMode::Async: return "wy3d.ExecutionMode.Async";
            default: return "wy3d.ExecutionMode.Unknown";
            }});

    // ========== DocManagerReactor (支持 Python 继承) ==========
    py::class_<wyap::DocManagerReactor, PyDocManagerReactor>(m, "DocManagerReactor")
        .def(py::init<>())
        .def("onDocumentCreated", &wyap::DocManagerReactor::onDocumentCreated)
        .def("onDocumentToBeDestroyed", &wyap::DocManagerReactor::onDocumentToBeDestroyed)
        .def("onDocumentDestroyed", &wyap::DocManagerReactor::onDocumentDestroyed)
        .def("onDocumentToBeDeactivated", &wyap::DocManagerReactor::onDocumentToBeDeactivated)
        .def("onDocumentDeactivated", &wyap::DocManagerReactor::onDocumentDeactivated)
        .def("onDocumentToBeActivated", &wyap::DocManagerReactor::onDocumentToBeActivated)
        .def("onDocumentActivated", &wyap::DocManagerReactor::onDocumentActivated)
        .def("onDocumentStatusChanged", &wyap::DocManagerReactor::onDocumentStatusChanged)
        .def("onDocumentTitleUpdated", &wyap::DocManagerReactor::onDocumentTitleUpdated);

    // ========== DocManager ==========
    py::class_<wyap::DocManager, std::unique_ptr<wyap::DocManager, py::nodelete>>(m, "DocManager")
        // 获取所有文档
        .def("getDocuments", &wyap::DocManager::getDocuments,
            py::return_value_policy::reference)

        // 获取活动文档
        .def("getActiveDocument", &wyap::DocManager::getActiveDocument,
            py::return_value_policy::reference)

        // 激活文档
        .def("activateDocument", &wyap::DocManager::activateDocument,
            py::arg("pDoc"), py::arg("mode"))

        // 打开文档 (返回指针版本)
        .def("openDocument", [](wyap::DocManager& self,
                                const std::string& filePath,
                                const wydb::Database::ReadFileOption& option) -> wyap::Document* {
            return self.openDocument(filePath, option);
        }, py::arg("filePath"), py::arg("option") = wydb::Database::ReadFileOption(),
            py::return_value_policy::reference)

        // 打开文档 (输出参数版本)
        .def("openDocumentEx", [](wyap::DocManager& self,
                                  const std::string& filePath,
                                  const wydb::Database::ReadFileOption& option) -> py::tuple {
            wyap::Document* pDoc = nullptr;
            wy::ErrorStatus err = self.openDocument(filePath, option, pDoc);
            return py::make_tuple(err, pDoc);
        }, py::arg("filePath"), py::arg("option") = wydb::Database::ReadFileOption())

        // 保存文档
        .def("saveDocument", &wyap::DocManager::saveDocument,
            py::arg("pDoc"), py::arg("filePath"), py::arg("option"))

        // 关闭文档
        .def("closeDocument", &wyap::DocManager::closeDocument,
            py::arg("pDoc"))

        // 关闭活动文档 (带 ExecutionMode)
        .def("closeActiveDocument", &wyap::DocManager::closeActiveDocument,
            py::arg("mode"))

        // 新建文档
        .def("newDocument", [](wyap::DocManager& self,
                               const std::string& fileName) -> wyap::Document* {
            return self.newDocument(fileName);
        }, py::arg("fileName"), py::return_value_policy::reference)

        // 添加/移除反应器
        .def("addReactor", &wyap::DocManager::addReactor, py::arg("pReactor"))
        .def("removeReactor", &wyap::DocManager::removeReactor, py::arg("pReactor"));

    // ========== 全局函数 ==========

    // 获取 DocManager 实例
    m.def("getDocManager", &wyap::getDocManager,
        py::return_value_policy::reference);

    // 便利函数: 获取活动文档
    m.def("getActiveDocument", []() -> wyap::Document* {
        wyap::DocManager* pDocMgr = wyap::getDocManager();
        return pDocMgr ? pDocMgr->getActiveDocument() : nullptr;
        }, py::return_value_policy::reference);

    // 便利函数: 获取活动数据库
    m.def("getActiveDatabase", []() -> wydb::Database* {
        wyap::DocManager* pDocMgr = wyap::getDocManager();
        if (!pDocMgr) return nullptr;
        wyap::Document* pActiveDoc = pDocMgr->getActiveDocument();
        return pActiveDoc ? pActiveDoc->getDatabase() : nullptr;
        }, py::return_value_policy::reference);

    // 便利函数: 关闭活动文档
    m.def("closeActiveDocument", []() -> int {
        wyap::DocManager* pDocMgr = wyap::getDocManager();
        if (!pDocMgr) return -1;
        return static_cast<int>(pDocMgr->closeActiveDocument(wyap::ExecutionMode::Sync));
    });
}
