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

TEST(Wy3dCylinder, Create)
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

TEST(Wy3dCylinder, DefaultColor)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Cylinder* pCylinder(nullptr);
    wydb::Transaction* pTrans = pTransMgr->startTransaction();
    Cylinder::create(pTrans, 10.0, 20.0, pCylinder);
    pTransMgr->endTransaction();
    ASSERT_NE(pCylinder, nullptr);

    // 新建实体应该有默认颜色
    const wy3d::Color c = pCylinder->getColor();
    EXPECT_LE(c.red, 255u);
    EXPECT_LE(c.green, 255u);
    EXPECT_LE(c.blue, 255u);
}

TEST(Wy3dCylinder, SetColor)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Cylinder* pCylinder(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cylinder::create(pTrans, 10.0, 20.0, pCylinder);
        pTransMgr->endTransaction();
        ASSERT_NE(pCylinder, nullptr);
    }

    wy3d::Color newColor(0, 255, 0);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cylinder* pWrite = Cylinder::cast(pTrans->getElementForWrite(pCylinder->getId()));
        ASSERT_NE(pWrite, nullptr);
        EXPECT_EQ(pWrite->setColor(newColor), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pCylinder->getColor(), newColor);
}

TEST(Wy3dCylinder, ModifyRadius)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Cylinder* pCylinder(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cylinder::create(pTrans, 50.0, 200.0, pCylinder);
        pTransMgr->endTransaction();
        ASSERT_NE(pCylinder, nullptr);
    }

    double newRadius(80.0);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cylinder* pWrite = Cylinder::cast(pTrans->getElementForWrite(pCylinder->getId()));
        EXPECT_EQ(pWrite->setRadius(newRadius), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pCylinder->getRadius(), newRadius);
    EXPECT_EQ(pCylinder->getHeight(), 200.0);
}

TEST(Wy3dCylinder, ModifyHeight)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Cylinder* pCylinder(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cylinder::create(pTrans, 50.0, 200.0, pCylinder);
        pTransMgr->endTransaction();
        ASSERT_NE(pCylinder, nullptr);
    }

    double newHeight(350.0);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cylinder* pWrite = Cylinder::cast(pTrans->getElementForWrite(pCylinder->getId()));
        EXPECT_EQ(pWrite->setHeight(newHeight), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pCylinder->getHeight(), newHeight);
}

TEST(Wy3dCylinder, ModifyBoth)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Cylinder* pCylinder(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cylinder::create(pTrans, 50.0, 200.0, pCylinder);
        pTransMgr->endTransaction();
        ASSERT_NE(pCylinder, nullptr);
    }

    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cylinder* pWrite = Cylinder::cast(pTrans->getElementForWrite(pCylinder->getId()));
        EXPECT_EQ(pWrite->setRadius(75.0), wy::ErrorStatus::Ok);
        EXPECT_EQ(pWrite->setHeight(300.0), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pCylinder->getRadius(), 75.0);
    EXPECT_EQ(pCylinder->getHeight(), 300.0);
}

TEST(Wy3dCylinder, ColorRoundtrip)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Cylinder* pCylinder(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cylinder::create(pTrans, 10.0, 20.0, pCylinder);
        pTransMgr->endTransaction();
        ASSERT_NE(pCylinder, nullptr);
    }

    // 设置颜色为黑色
    wy3d::Color black(0, 0, 0);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cylinder* pWrite = Cylinder::cast(pTrans->getElementForWrite(pCylinder->getId()));
        pWrite->setColor(black);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pCylinder->getColor(), black);

    // 再改成白色
    wy3d::Color white(255, 255, 255);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cylinder* pWrite = Cylinder::cast(pTrans->getElementForWrite(pCylinder->getId()));
        pWrite->setColor(white);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pCylinder->getColor(), white);
    EXPECT_NE(pCylinder->getColor(), black);
}

TEST(Wy3dCylinder, CutFlag)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Cylinder* pCylinder(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cylinder::create(pTrans, 10.0, 20.0, pCylinder);
        pTransMgr->endTransaction();
        ASSERT_NE(pCylinder, nullptr);
    }

    EXPECT_FALSE(pCylinder->isCut());
}

TEST(Wy3dCylinder, Shape)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Cylinder* pCylinder(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cylinder::create(pTrans, 10.0, 30.0, pCylinder);
        pTransMgr->endTransaction();
        ASSERT_NE(pCylinder, nullptr);
    }

    EXPECT_FALSE(pCylinder->getShape().IsNull());
}

TEST(Wy3dCylinder, IO)
{
    double radius(25.0), height(100.0);
    std::string filePath("./test_cylinder_unit.wy3dt");
    wydb::ElementId cylinderId;

    {
        std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
        wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

        Cylinder* pCylinder(nullptr);
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cylinder::create(pTrans, radius, height, pCylinder);
        pTransMgr->endTransaction();
        cylinderId = pCylinder->getId();

        EXPECT_EQ(pDb->writeFile(filePath, {wydb::FileType::Text}), wy::ErrorStatus::Ok);
    }

    {
        std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
        EXPECT_EQ(pDb->readFile(filePath, {wydb::FileType::Text}), wy::ErrorStatus::Ok);

        const Cylinder* pCylinder = Cylinder::cast(pDb->getElement(cylinderId));
        ASSERT_NE(pCylinder, nullptr);
        EXPECT_EQ(pCylinder->getRadius(), radius);
        EXPECT_EQ(pCylinder->getHeight(), height);
    }
}

TEST(Wy3dCylinder, Cast)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Cylinder* pCylinder(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cylinder::create(pTrans, 50.0, 200.0, pCylinder);
        pTransMgr->endTransaction();
        ASSERT_NE(pCylinder, nullptr);
    }

    const wydb::Element* pElem = pDb->getElement(pCylinder->getId());
    ASSERT_NE(pElem, nullptr);

    const Cylinder* pCylinder2 = Cylinder::cast(pElem);
    ASSERT_NE(pCylinder2, nullptr);

    const Solid* pSolid = Solid::cast(pElem);
    ASSERT_NE(pSolid, nullptr);
}

TEST(Wy3dCylinder, IdNotNull)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Cylinder* pCylinder(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cylinder::create(pTrans, 10.0, 10.0, pCylinder);
        pTransMgr->endTransaction();
    }
    ASSERT_NE(pCylinder, nullptr);
    EXPECT_FALSE(pCylinder->getId().isNull());
}
