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
#include <wydbParameter.h>

namespace py = pybind11;

void bindWydbParameter(py::module_& m)
{
    py::enum_<wydb::ParameterValue::Type>(m, "ParameterValueType")
        .value("Integer", wydb::ParameterValue::Type::Integer)
        .value("Double", wydb::ParameterValue::Type::Double)
        .value("Boolean", wydb::ParameterValue::Type::Boolean)
        .value("String", wydb::ParameterValue::Type::String)
        .value("ElementId", wydb::ParameterValue::Type::ElementId)
        .def("__repr__", [](wydb::ParameterValue::Type type) {
            switch (type) {
            case wydb::ParameterValue::Type::Integer: return "wy3d.ParameterValueType.Integer";
            case wydb::ParameterValue::Type::Double: return "wy3d.ParameterValueType.Double";
            case wydb::ParameterValue::Type::Boolean: return "wy3d.ParameterValueType.Boolean";
            case wydb::ParameterValue::Type::String: return "wy3d.ParameterValueType.String";
            case wydb::ParameterValue::Type::ElementId: return "wy3d.ParameterValueType.ElementId";
            default: return "wy3d.ParameterValueType.Unknown";
        }});

    py::class_<wydb::ParameterValue>(m, "ParameterValue")
        .def("getType", &wydb::ParameterValue::getType)
        .def("clone", &wydb::ParameterValue::clone)

        // 判断是否是具体的值类型
        .def("isInteger", &wydb::ParameterValue::isInteger)
        .def("isDouble", &wydb::ParameterValue::isDouble)
        .def("isBoolean", &wydb::ParameterValue::isBoolean)
        .def("isString", &wydb::ParameterValue::isString)
        .def("isElementId", &wydb::ParameterValue::isElementId)

        // 获取值
        .def("asInteger", &wydb::ParameterValue::asInteger)
        .def("asDouble", &wydb::ParameterValue::asDouble)
        .def("asBoolean", &wydb::ParameterValue::asBoolean)
        .def("asString", &wydb::ParameterValue::asString)
        .def("asElementId", &wydb::ParameterValue::asElementId)

        // 设置值
        .def("setIntValue", py::overload_cast<int>(&wydb::ParameterValue::setValue))
        .def("setDoubleValue", py::overload_cast<double>(&wydb::ParameterValue::setValue))
        .def("setBooleanValue", py::overload_cast<bool>(&wydb::ParameterValue::setValue))
        .def("setStringValue", py::overload_cast<const std::string&>(&wydb::ParameterValue::setValue))
        .def("setElementIdValue", py::overload_cast<const wydb::ElementId&>(&wydb::ParameterValue::setValue))

        // 判断参数值是否相等
        //.def("isEqual", &wydb::ParameterValue::isEqual)

        // 静态创建函数
        .def_static("createInteger", &wydb::ParameterValue::createInteger)
        .def_static("createDouble", &wydb::ParameterValue::createDouble)
        .def_static("createBoolean", &wydb::ParameterValue::createBoolean)
        .def_static("createString", &wydb::ParameterValue::createString)
        .def_static("createElementId", &wydb::ParameterValue::createElementId)

        // 字符串表示
        .def("__repr__", [](const wydb::ParameterValue& self) {
                std::stringstream ss;
                ss << "wy3d.ParameterValue(";
                switch (self.getType())
                {
                case wydb::ParameterValue::Type::Integer:
                {
                    ss << "Integer, " << self.asInteger();
                }
                break;
                
                case wydb::ParameterValue::Type::Double:
                {
                    ss << "Double, " << self.asDouble();
                }
                break;

                case wydb::ParameterValue::Type::Boolean:
                {
                    ss << "Boolean, " << self.asBoolean();
                }
                break;

                case wydb::ParameterValue::Type::String:
                {
                    ss << "String, " << self.asString();
                }
                break;

                case wydb::ParameterValue::Type::ElementId:
                {
                    ss << "ElementId, " << self.asElementId().value();
                }
                break;

                default:
                {
                    ss << "Unknown, Unknown";
                }
                break;
                }
                ss << ")";
                return ss.str();
            });
}
