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

TEST(Wy3dSphere, Create)
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

TEST(Wy3dSphere, DefaultColor)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Sphere* pSphere(nullptr);
    wydb::Transaction* pTrans = pTransMgr->startTransaction();
    Sphere::create(pTrans, 10.0, pSphere);
    pTransMgr->endTransaction();
    ASSERT_NE(pSphere, nullptr);

    const wy3d::Color c = pSphere->getColor();
    EXPECT_LE(c.red, 255u);
    EXPECT_LE(c.green, 255u);
    EXPECT_LE(c.blue, 255u);
}

TEST(Wy3dSphere, SetColor)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Sphere* pSphere(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Sphere::create(pTrans, 10.0, pSphere);
        pTransMgr->endTransaction();
        ASSERT_NE(pSphere, nullptr);
    }

    wy3d::Color newColor(255, 128, 0);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Sphere* pWrite = Sphere::cast(pTrans->getElementForWrite(pSphere->getId()));
        ASSERT_NE(pWrite, nullptr);
        EXPECT_EQ(pWrite->setColor(newColor), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pSphere->getColor(), newColor);
}

TEST(Wy3dSphere, ModifyRadius)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Sphere* pSphere(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Sphere::create(pTrans, 30.0, pSphere);
        pTransMgr->endTransaction();
        ASSERT_NE(pSphere, nullptr);
    }

    double newRadius(60.0);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Sphere* pWrite = Sphere::cast(pTrans->getElementForWrite(pSphere->getId()));
        EXPECT_EQ(pWrite->setRadius(newRadius), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pSphere->getRadius(), newRadius);
}

TEST(Wy3dSphere, CutFlag)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Sphere* pSphere(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Sphere::create(pTrans, 10.0, pSphere);
        pTransMgr->endTransaction();
    }
    ASSERT_NE(pSphere, nullptr);
    EXPECT_FALSE(pSphere->isCut());
}

TEST(Wy3dSphere, Shape)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Sphere* pSphere(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Sphere::create(pTrans, 10.0, pSphere);
        pTransMgr->endTransaction();
        ASSERT_NE(pSphere, nullptr);
    }

    EXPECT_FALSE(pSphere->getShape().IsNull());
}

TEST(Wy3dSphere, IO)
{
    double radius(45.0);
    std::string filePath("./test_sphere_unit.wy3dt");
    wydb::ElementId sphereId;

    {
        std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
        wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

        Sphere* pSphere(nullptr);
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Sphere::create(pTrans, radius, pSphere);
        pTransMgr->endTransaction();
        sphereId = pSphere->getId();

        EXPECT_EQ(pDb->writeFile(filePath, {wydb::FileType::Text}), wy::ErrorStatus::Ok);
    }

    {
        std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
        EXPECT_EQ(pDb->readFile(filePath, {wydb::FileType::Text}), wy::ErrorStatus::Ok);

        const Sphere* pSphere = Sphere::cast(pDb->getElement(sphereId));
        ASSERT_NE(pSphere, nullptr);
        EXPECT_EQ(pSphere->getRadius(), radius);
    }
}

TEST(Wy3dSphere, Cast)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Sphere* pSphere(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Sphere::create(pTrans, 30.0, pSphere);
        pTransMgr->endTransaction();
        ASSERT_NE(pSphere, nullptr);
    }

    const wydb::Element* pElem = pDb->getElement(pSphere->getId());
    ASSERT_NE(pElem, nullptr);

    const Sphere* pSphere2 = Sphere::cast(pElem);
    ASSERT_NE(pSphere2, nullptr);

    const Solid* pSolid = Solid::cast(pElem);
    ASSERT_NE(pSolid, nullptr);
}

TEST(Wy3dSphere, IdNotNull)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Sphere* pSphere(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Sphere::create(pTrans, 10.0, pSphere);
        pTransMgr->endTransaction();
    }
    ASSERT_NE(pSphere, nullptr);
    EXPECT_FALSE(pSphere->getId().isNull());
}

TEST(Wy3dSphere, ModifyTwice)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Sphere* pSphere(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Sphere::create(pTrans, 30.0, pSphere);
        pTransMgr->endTransaction();
        ASSERT_NE(pSphere, nullptr);
    }

    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Sphere* pWrite = Sphere::cast(pTrans->getElementForWrite(pSphere->getId()));
        pWrite->setRadius(60.0);
        pTransMgr->endTransaction();
    }
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Sphere* pWrite = Sphere::cast(pTrans->getElementForWrite(pSphere->getId()));
        pWrite->setRadius(90.0);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pSphere->getRadius(), 90.0);
}
