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

#include "headers.h"

TEST(Wy3d, Box)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    double length(100.0), width(200.0), height(300.0);
    Box* pBox(nullptr);
    wydb::Transaction* pTrans = pTransMgr->startTransaction();
    wy::ErrorStatus error = Box::create(pTrans, length, width, height, pBox);
    pTransMgr->endTransaction();

    EXPECT_EQ(error, wy::ErrorStatus::Ok);
    ASSERT_NE(pBox, nullptr);
    EXPECT_EQ(pBox->getLength(), length);
    EXPECT_EQ(pBox->getWidth(), width);
    EXPECT_EQ(pBox->getHeight(), height);
}

TEST(Wy3d, Cylinder)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    double radius(50.0), height(200.0);
    Cylinder* pCylinder(nullptr);
    wydb::Transaction* pTrans = pTransMgr->startTransaction();
    wy::ErrorStatus error = Cylinder::create(pTrans, radius, height, pCylinder);
    pTransMgr->endTransaction();

    EXPECT_EQ(error, wy::ErrorStatus::Ok);
    ASSERT_NE(pCylinder, nullptr);
    EXPECT_EQ(pCylinder->getRadius(), radius);
    EXPECT_EQ(pCylinder->getHeight(), height);
}

TEST(Wy3d, Sphere)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    double radius(30.0);
    Sphere* pSphere(nullptr);
    wydb::Transaction* pTrans = pTransMgr->startTransaction();
    wy::ErrorStatus error = Sphere::create(pTrans, radius, pSphere);
    pTransMgr->endTransaction();

    EXPECT_EQ(error, wy::ErrorStatus::Ok);
    ASSERT_NE(pSphere, nullptr);
    EXPECT_EQ(pSphere->getRadius(), radius);
}

TEST(Wy3d, Cone)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    double radius(25.0), height(100.0);
    Cone* pCone(nullptr);
    wydb::Transaction* pTrans = pTransMgr->startTransaction();
    wy::ErrorStatus error = Cone::create(pTrans, radius, height, pCone);
    pTransMgr->endTransaction();

    EXPECT_EQ(error, wy::ErrorStatus::Ok);
    ASSERT_NE(pCone, nullptr);
    EXPECT_EQ(pCone->getRadius(), radius);
    EXPECT_EQ(pCone->getHeight(), height);
}

TEST(Wy3d, Torus)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    double majorRadius(80.0), minorRadius(20.0);
    Torus* pTorus(nullptr);
    wydb::Transaction* pTrans = pTransMgr->startTransaction();
    wy::ErrorStatus error = Torus::create(pTrans, majorRadius, minorRadius, pTorus);
    pTransMgr->endTransaction();

    EXPECT_EQ(error, wy::ErrorStatus::Ok);
    ASSERT_NE(pTorus, nullptr);
    EXPECT_EQ(pTorus->getMajorRadius(), majorRadius);
    EXPECT_EQ(pTorus->getMinorRadius(), minorRadius);
}

TEST(Wy3d, Tube)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    double outerRadius(50.0), innerRadius(30.0), height(200.0);
    Tube* pTube(nullptr);
    wydb::Transaction* pTrans = pTransMgr->startTransaction();
    wy::ErrorStatus error = Tube::create(pTrans, outerRadius, innerRadius, height, pTube);
    pTransMgr->endTransaction();

    EXPECT_EQ(error, wy::ErrorStatus::Ok);
    ASSERT_NE(pTube, nullptr);
    EXPECT_EQ(pTube->getOuterRadius(), outerRadius);
    EXPECT_EQ(pTube->getInnerRadius(), innerRadius);
    EXPECT_EQ(pTube->getHeight(), height);
}

TEST(Wy3d, ConeIO)
{
    double radius(25.0), height(100.0);
    std::string filePath("./test_cone.wy3dt");
    wydb::ElementId coneId = wydb::ElementId::kNull;

    // 写入
    {
        std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
        wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

        Cone* pCone(nullptr);
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cone::create(pTrans, radius, height, pCone);
        pTransMgr->endTransaction();
        coneId = pCone->getId();

        EXPECT_EQ(pDb->writeFile(filePath, {wydb::FileType::Text}), wy::ErrorStatus::Ok);
    }

    // 读取
    {
        std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
        EXPECT_EQ(pDb->readFile(filePath, {wydb::FileType::Text}), wy::ErrorStatus::Ok);

        const wydb::Element* pElem = pDb->getElement(coneId);
        ASSERT_NE(pElem, nullptr);
        const Cone* pCone = Cone::cast(pElem);
        ASSERT_NE(pCone, nullptr);
        EXPECT_EQ(pCone->getRadius(), radius);
        EXPECT_EQ(pCone->getHeight(), height);
    }
}

TEST(Wy3d, PrimitiveModify)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    // 创建一个 Box 然后修改尺寸
    double length(100.0), width(200.0), height(300.0);
    Box* pBox(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Box::create(pTrans, length, width, height, pBox);
        pTransMgr->endTransaction();
        ASSERT_NE(pBox, nullptr);
    }

    // 修改参数
    {
        double newLength(400.0);
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Box* pBoxWrite = Box::cast(pTrans->getElementForWrite(pBox->getId()));
        ASSERT_NE(pBoxWrite, nullptr);
        EXPECT_EQ(pBoxWrite->setLength(newLength), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
        EXPECT_EQ(pBox->getLength(), newLength);
        EXPECT_EQ(pBox->getWidth(), width);  // 宽、高不变
        EXPECT_EQ(pBox->getHeight(), height);
    }
}

TEST(Wy3d, MultiPrimitive)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();
    wydb::Transaction* pTrans = pTransMgr->startTransaction();

    Box* pBox(nullptr);
    Cylinder* pCylinder(nullptr);
    Cone* pCone(nullptr);
    Sphere* pSphere(nullptr);

    Box::create(pTrans, 100.0, 200.0, 300.0, pBox);
    Cylinder::create(pTrans, 25.0, 50.0, pCylinder);
    Cone::create(pTrans, 30.0, 80.0, pCone);
    Sphere::create(pTrans, 20.0, pSphere);

    pTransMgr->endTransaction();

    ASSERT_NE(pBox, nullptr);
    ASSERT_NE(pCylinder, nullptr);
    ASSERT_NE(pCone, nullptr);
    ASSERT_NE(pSphere, nullptr);

    // 验证各元素独立存在、id 不同
    EXPECT_FALSE(pBox->getId().isNull());
    EXPECT_FALSE(pCylinder->getId().isNull());
    EXPECT_FALSE(pCone->getId().isNull());
    EXPECT_FALSE(pSphere->getId().isNull());
    EXPECT_NE(pBox->getId(), pCylinder->getId());
    EXPECT_NE(pBox->getId(), pCone->getId());
    EXPECT_NE(pBox->getId(), pSphere->getId());
}

TEST(Wy3d, IO)
{
    double boxLength(100.0), boxWidth(200.0), boxHeight(300.0);
    std::string filePath("./test_box.wy3dt");
    wydb::ElementId boxId = wydb::ElementId::kNull;

    // 写入文件
    {
        std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
        wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

        Box* pBox(nullptr);
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Box::create(pTrans, boxLength, boxWidth, boxHeight, pBox);
        pTransMgr->endTransaction();
        boxId = pBox->getId();

        EXPECT_EQ(pDb->writeFile(filePath, {wydb::FileType::Text}), wy::ErrorStatus::Ok);
    }

    // 读取文件
    {
        std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
        EXPECT_EQ(pDb->readFile(filePath, {wydb::FileType::Text}), wy::ErrorStatus::Ok);

        const wydb::Element* pElem = pDb->getElement(boxId);
        ASSERT_NE(pElem, nullptr);
        const Box* pBox = Box::cast(pElem);
        ASSERT_NE(pBox, nullptr);
        EXPECT_EQ(pBox->getLength(), boxLength);
        EXPECT_EQ(pBox->getWidth(), boxWidth);
        EXPECT_EQ(pBox->getHeight(), boxHeight);
    }
}
