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
#include <wyErrors.h>

namespace py = pybind11;

void bindWyErrorStatus(py::module_& m)
{
    py::enum_<wy::ErrorStatus>(m, "ErrorStatus")
        .value("Ok", wy::ErrorStatus::Ok)
        .value("Error", wy::ErrorStatus::Error)
        .value("NotImplementedYet", wy::ErrorStatus::NotImplementedYet)
        .value("NotSupported", wy::ErrorStatus::NotSupported)
        .value("NotCurrentlyAllowed", wy::ErrorStatus::NotCurrentlyAllowed)
        .value("NotAvailable", wy::ErrorStatus::NotAvailable)
        .value("OutOfRange", wy::ErrorStatus::OutOfRange)
        .value("SystemBusy", wy::ErrorStatus::SystemBusy)

        .value("NullInputPointer", wy::ErrorStatus::NullInputPointer)
        .value("NullElementPointer", wy::ErrorStatus::NullElementPointer)
        .value("NullElementId", wy::ErrorStatus::NullElementId)
        .value("NullDatabasePointer", wy::ErrorStatus::NullDatabasePointer)
        .value("NullTransactionPointer", wy::ErrorStatus::NullTransactionPointer)
        .value("NullDocumentPointer", wy::ErrorStatus::NullDocumentPointer)
        .value("NullEnvironmentPointer", wy::ErrorStatus::NullEnvironmentPointer)

        .value("InvalidInput", wy::ErrorStatus::InvalidInput)
        .value("InvalidElementId", wy::ErrorStatus::InvalidElementId)
        .value("InvalidParameterName", wy::ErrorStatus::InvalidParameterName)
        .value("InvalidElementTableIndex", wy::ErrorStatus::InvalidElementTableIndex)
        .value("InvalidFileName", wy::ErrorStatus::InvalidFileName)

        .value("WrongDatabase", wy::ErrorStatus::WrongDatabase)
        .value("ElementIdMismatch", wy::ErrorStatus::ElementIdMismatch)

        .value("KeyNotFound", wy::ErrorStatus::KeyNotFound)
        .value("DuplicateKey", wy::ErrorStatus::DuplicateKey)
        .value("InvalidIndex", wy::ErrorStatus::InvalidIndex)
        .value("DuplicateIndex", wy::ErrorStatus::DuplicateIndex)
        .value("AlreadyInDatabase", wy::ErrorStatus::AlreadyInDatabase)
        .value("DuplicateFileName", wy::ErrorStatus::DuplicateFileName)

        .value("ParameterNotFound", wy::ErrorStatus::ParameterNotFound)
        .value("ParameterReadonly", wy::ErrorStatus::ParameterReadonly)

        .value("ElementNotFound", wy::ErrorStatus::ElementNotFound)
        .value("ElementNotNewlyCreated", wy::ErrorStatus::ElementNotNewlyCreated)
        .value("ElementNotOpenedForWrite", wy::ErrorStatus::ElementNotOpenedForWrite)
        .value("ElementErased", wy::ErrorStatus::ElementErased)
        .value("ElementNotInTransaction", wy::ErrorStatus::ElementNotInTransaction)
        .value("ElementNotInDatabase", wy::ErrorStatus::ElementNotInDatabase)
        .value("NewElementNotAddedToTransaction", wy::ErrorStatus::NewElementNotAddedToTransaction)

        .value("NoActiveTransaction", wy::ErrorStatus::NoActiveTransaction)
        .value("NoFieldValue", wy::ErrorStatus::NoFieldValue)
        .value("FieldNotFound", wy::ErrorStatus::FieldNotFound)
        .value("TransactionManagerBusy", wy::ErrorStatus::TransactionManagerBusy)
        .value("TransactionGroupNotSupported", wy::ErrorStatus::TransactionGroupNotSupported)
        .value("TransactionLeafNotSupported", wy::ErrorStatus::TransactionLeafNotSupported)

        .value("FilerError", wy::ErrorStatus::FilerError)
        .value("CantOpenFile", wy::ErrorStatus::CantOpenFile)
        .value("DatabaseNotEmpty", wy::ErrorStatus::DatabaseNotEmpty)
        .value("BadFile", wy::ErrorStatus::BadFile)
        .value("FileVersionTooNew", wy::ErrorStatus::FileVersionTooNew)
        .value("FilePathIsDirectory", wy::ErrorStatus::FilePathIsDirectory)
        .value("DirectoryNotFound", wy::ErrorStatus::DirectoryNotFound)

        .value("DocumentNotManaged", wy::ErrorStatus::DocumentNotManaged)
        .value("NoActiveDocument", wy::ErrorStatus::NoActiveDocument)
        .value("ActiveDocumentChanged", wy::ErrorStatus::ActiveDocumentChanged)
        .value("DocumentActive", wy::ErrorStatus::DocumentActive)
        .value("DocumentNotFound", wy::ErrorStatus::DocumentNotFound)
        .value("DocumentManagerBusy", wy::ErrorStatus::DocumentManagerBusy)

        .value("CommandAlreadyExists", wy::ErrorStatus::CommandAlreadyExists)
        .value("CommandNotFound", wy::ErrorStatus::CommandNotFound)
        .value("CommandRejected", wy::ErrorStatus::CommandRejected)
        .value("CommandCanceled", wy::ErrorStatus::CommandCanceled)
        .value("CommandStartFailed", wy::ErrorStatus::CommandStartFailed)
        .value("CommandFailed", wy::ErrorStatus::CommandFailed)
        .value("ModalCommandInProgress", wy::ErrorStatus::ModalCommandInProgress)
        .value("NoCurrentModalCommand", wy::ErrorStatus::NoCurrentModalCommand)
        .value("TransparentCommandMustBeImmediate", wy::ErrorStatus::TransparentCommandMustBeImmediate)
        .value("InvalidCommandName", wy::ErrorStatus::InvalidCommandName)
        .value("NotCommandExecutionClass", wy::ErrorStatus::NotCommandExecutionClass)
        .value("CommandManagerBusy", wy::ErrorStatus::CommandManagerBusy)

        .value("NoActiveEnvironment", wy::ErrorStatus::NoActiveEnvironment)
        .value("ActiveEnvironmentChanged", wy::ErrorStatus::ActiveEnvironmentChanged)
        .value("InvalidEnvironmentTransition", wy::ErrorStatus::InvalidEnvironmentTransition)
        .value("NotAllowedInCurrentEnvironment", wy::ErrorStatus::NotAllowedInCurrentEnvironment)
        .value("EnvironmentManagerBusy", wy::ErrorStatus::EnvironmentManagerBusy)

        .value("SelectionManagerBusy", wy::ErrorStatus::SelectionManagerBusy)
        .value("GizmoManagerBusy", wy::ErrorStatus::GizmoManagerBusy)

        .value("InvalidFilePath", wy::ErrorStatus::InvalidFilePath)
        .value("FileSystemError", wy::ErrorStatus::FileSystemError)
        .value("FileNotFound", wy::ErrorStatus::FileNotFound)
        .value("NotRegularFile", wy::ErrorStatus::NotRegularFile)
        .value("UnsupportedFileFormat", wy::ErrorStatus::UnsupportedFileFormat)

        .value("InitFreeTypeError", wy::ErrorStatus::InitFreeTypeError)
        .value("FreeTypeLoadFontFaceError", wy::ErrorStatus::FreeTypeLoadFontFaceError)
        .value("FreeTypeDecodeError", wy::ErrorStatus::FreeTypeDecodeError);
}
