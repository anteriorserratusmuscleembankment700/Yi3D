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
#include <wydbDatabase.h>
#include <wydbTransaction.h>
#include <wy3dBox.h>
#include <wy3dCylinder.h>
#include <wy3dSphere.h>
#include <wy3dCone.h>
#include <wy3dTorus.h>
#include <wy3dTube.h>

namespace py = pybind11;

void bindWy3dPrimitives(py::module_& m) 
{
    py::class_<wy3d::Primitive, wy3d::Solid, std::unique_ptr<wy3d::Primitive, py::nodelete>>(m, "Primitive")
        .def("getPosition", &wy3d::Primitive::getPosition)
        .def("setPosition", &wy3d::Primitive::setPosition)
        .def("getRotation", &wy3d::Primitive::getRotation)
        .def("setRotation", &wy3d::Primitive::setRotation);

    py::class_<wy3d::Box, wy3d::Primitive, std::unique_ptr<wy3d::Box, py::nodelete>>(m, "Box")
        .def("getLength", &wy3d::Box::getLength)
        .def("setLength", &wy3d::Box::setLength)
        .def("getWidth", &wy3d::Box::getWidth)
        .def("setWidth", &wy3d::Box::setWidth)
        .def("getHeight", &wy3d::Box::getHeight)
        .def("setHeight", &wy3d::Box::setHeight)

        .def_static("create",
            [](wydb::Transaction* pTrans, double length, double width, double height) -> wy3d::Box*
            {
                wy3d::Box* pOutBox = nullptr;
                wy::ErrorStatus status = wy3d::Box::create(pTrans, length, width, height, pOutBox);
                return pOutBox;
            },
            py::arg("transaction"),
            py::arg("length"),
            py::arg("width"),
            py::arg("height"),
            py::return_value_policy::reference);

    py::class_<wy3d::Cylinder, wy3d::Primitive, std::unique_ptr<wy3d::Cylinder, py::nodelete>>(m, "Cylinder")
        .def("getRadius", &wy3d::Cylinder::getRadius)
        .def("setRadius", &wy3d::Cylinder::setRadius)
        .def("getHeight", &wy3d::Cylinder::getHeight)
        .def("setHeight", &wy3d::Cylinder::setHeight)

        .def_static("create",
            [](wydb::Transaction* pTrans, double radius, double height) -> wy3d::Cylinder*
            {
                wy3d::Cylinder* pOutCylinder = nullptr;
                wy::ErrorStatus status = wy3d::Cylinder::create(pTrans, radius, height, pOutCylinder);
                return pOutCylinder;
            },
            py::arg("transaction"),
            py::arg("radius"),
            py::arg("height"),
            py::return_value_policy::reference);

    py::class_<wy3d::Sphere, wy3d::Primitive, std::unique_ptr<wy3d::Sphere, py::nodelete>>(m, "Sphere")
        .def("getRadius", &wy3d::Sphere::getRadius)
        .def("setRadius", &wy3d::Sphere::setRadius)

        .def_static("create",
            [](wydb::Transaction* pTrans, double radius) -> wy3d::Sphere*
            {
                wy3d::Sphere* pOutSphere = nullptr;
                wy::ErrorStatus status = wy3d::Sphere::create(pTrans, radius, pOutSphere);
                return pOutSphere;
            },
            py::arg("transaction"),
            py::arg("radius"),
            py::return_value_policy::reference);

    py::class_<wy3d::Cone, wy3d::Primitive, std::unique_ptr<wy3d::Cone, py::nodelete>>(m, "Cone")
        .def("getRadius", &wy3d::Cone::getRadius)
        .def("setRadius", &wy3d::Cone::setRadius)
        .def("getHeight", &wy3d::Cone::getHeight)
        .def("setHeight", &wy3d::Cone::setHeight)

        .def_static("create",
            [](wydb::Transaction* pTrans, double radius, double height) -> wy3d::Cone*
            {
                wy3d::Cone* pOutCone = nullptr;
                wy::ErrorStatus status = wy3d::Cone::create(pTrans, radius, height, pOutCone);
                return pOutCone;
            },
            py::arg("transaction"),
            py::arg("radius"),
            py::arg("height"),
            py::return_value_policy::reference);

    py::class_<wy3d::Torus, wy3d::Primitive, std::unique_ptr<wy3d::Torus, py::nodelete>>(m, "Torus")
        .def("getMajorRadius", &wy3d::Torus::getMajorRadius)
        .def("setMajorRadius", &wy3d::Torus::setMajorRadius)
        .def("getMinorRadius", &wy3d::Torus::getMinorRadius)
        .def("setMinorRadius", &wy3d::Torus::setMinorRadius)

        .def_static("create",
            [](wydb::Transaction* pTrans, double majorRadius, double minorRadius) -> wy3d::Torus*
            {
                wy3d::Torus* pOutTorus = nullptr;
                wy::ErrorStatus status = wy3d::Torus::create(pTrans, majorRadius, minorRadius, pOutTorus);
                return pOutTorus;
            },
            py::arg("transaction"),
            py::arg("majorRadius"),
            py::arg("minorRadius"),
            py::return_value_policy::reference);

    py::class_<wy3d::Tube, wy3d::Primitive, std::unique_ptr<wy3d::Tube, py::nodelete>>(m, "Tube")
        .def("getOuterRadius", &wy3d::Tube::getOuterRadius)
        .def("setOuterRadius", &wy3d::Tube::setOuterRadius)
        .def("getInnerRadius", &wy3d::Tube::getInnerRadius)
        .def("setInnerRadius", &wy3d::Tube::setInnerRadius)
        .def("getHeight", &wy3d::Tube::getHeight)
        .def("setHeight", &wy3d::Tube::setHeight)

        .def_static("create",
            [](wydb::Transaction* pTrans, double outerRadius, double innerRadius, double height) -> wy3d::Tube*
            {
                wy3d::Tube* pOutTube = nullptr;
                wy::ErrorStatus status = wy3d::Tube::create(pTrans, outerRadius, innerRadius, height, pOutTube);
                return pOutTube;
            },
            py::arg("transaction"),
            py::arg("outerRadius"),
            py::arg("innerRadius"),
            py::arg("height"),
            py::return_value_policy::reference);
}