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

TEST(Wy3dTorus, Create)
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

TEST(Wy3dTorus, DefaultColor)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Torus* pTorus(nullptr);
    wydb::Transaction* pTrans = pTransMgr->startTransaction();
    Torus::create(pTrans, 30.0, 10.0, pTorus);
    pTransMgr->endTransaction();
    ASSERT_NE(pTorus, nullptr);

    const wy3d::Color c = pTorus->getColor();
    EXPECT_LE(c.red, 255u);
    EXPECT_LE(c.green, 255u);
    EXPECT_LE(c.blue, 255u);
}

TEST(Wy3dTorus, SetColor)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Torus* pTorus(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Torus::create(pTrans, 30.0, 10.0, pTorus);
        pTransMgr->endTransaction();
        ASSERT_NE(pTorus, nullptr);
    }

    wy3d::Color newColor(100, 200, 50);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Torus* pWrite = Torus::cast(pTrans->getElementForWrite(pTorus->getId()));
        ASSERT_NE(pWrite, nullptr);
        EXPECT_EQ(pWrite->setColor(newColor), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pTorus->getColor(), newColor);
}

TEST(Wy3dTorus, ModifyMajorRadius)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Torus* pTorus(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Torus::create(pTrans, 80.0, 20.0, pTorus);
        pTransMgr->endTransaction();
        ASSERT_NE(pTorus, nullptr);
    }

    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Torus* pWrite = Torus::cast(pTrans->getElementForWrite(pTorus->getId()));
        EXPECT_EQ(pWrite->setMajorRadius(120.0), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pTorus->getMajorRadius(), 120.0);
    EXPECT_EQ(pTorus->getMinorRadius(), 20.0);
}

TEST(Wy3dTorus, ModifyMinorRadius)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Torus* pTorus(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Torus::create(pTrans, 80.0, 20.0, pTorus);
        pTransMgr->endTransaction();
        ASSERT_NE(pTorus, nullptr);
    }

    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Torus* pWrite = Torus::cast(pTrans->getElementForWrite(pTorus->getId()));
        EXPECT_EQ(pWrite->setMinorRadius(30.0), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pTorus->getMinorRadius(), 30.0);
}

TEST(Wy3dTorus, ModifyBoth)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Torus* pTorus(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Torus::create(pTrans, 80.0, 20.0, pTorus);
        pTransMgr->endTransaction();
        ASSERT_NE(pTorus, nullptr);
    }

    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Torus* pWrite = Torus::cast(pTrans->getElementForWrite(pTorus->getId()));
        EXPECT_EQ(pWrite->setMajorRadius(100.0), wy::ErrorStatus::Ok);
        EXPECT_EQ(pWrite->setMinorRadius(15.0), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pTorus->getMajorRadius(), 100.0);
    EXPECT_EQ(pTorus->getMinorRadius(), 15.0);
}

TEST(Wy3dTorus, CutFlag)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Torus* pTorus(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Torus::create(pTrans, 30.0, 10.0, pTorus);
        pTransMgr->endTransaction();
    }
    ASSERT_NE(pTorus, nullptr);
    EXPECT_FALSE(pTorus->isCut());
}

TEST(Wy3dTorus, Shape)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Torus* pTorus(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Torus::create(pTrans, 30.0, 10.0, pTorus);
        pTransMgr->endTransaction();
        ASSERT_NE(pTorus, nullptr);
    }

    EXPECT_FALSE(pTorus->getShape().IsNull());
}

TEST(Wy3dTorus, IO)
{
    double majorRadius(60.0), minorRadius(15.0);
    std::string filePath("./test_torus_unit.wy3dt");
    wydb::ElementId torusId;

    {
        std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
        wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

        Torus* pTorus(nullptr);
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Torus::create(pTrans, majorRadius, minorRadius, pTorus);
        pTransMgr->endTransaction();
        torusId = pTorus->getId();

        EXPECT_EQ(pDb->writeFile(filePath, {wydb::FileType::Text}), wy::ErrorStatus::Ok);
    }

    {
        std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
        EXPECT_EQ(pDb->readFile(filePath, {wydb::FileType::Text}), wy::ErrorStatus::Ok);

        const Torus* pTorus = Torus::cast(pDb->getElement(torusId));
        ASSERT_NE(pTorus, nullptr);
        EXPECT_EQ(pTorus->getMajorRadius(), majorRadius);
        EXPECT_EQ(pTorus->getMinorRadius(), minorRadius);
    }
}

TEST(Wy3dTorus, Cast)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Torus* pTorus(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Torus::create(pTrans, 80.0, 20.0, pTorus);
        pTransMgr->endTransaction();
        ASSERT_NE(pTorus, nullptr);
    }

    const wydb::Element* pElem = pDb->getElement(pTorus->getId());
    ASSERT_NE(pElem, nullptr);

    const Torus* pTorus2 = Torus::cast(pElem);
    ASSERT_NE(pTorus2, nullptr);

    const Solid* pSolid = Solid::cast(pElem);
    ASSERT_NE(pSolid, nullptr);
}

TEST(Wy3dTorus, IdNotNull)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Torus* pTorus(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Torus::create(pTrans, 10.0, 5.0, pTorus);
        pTransMgr->endTransaction();
    }
    ASSERT_NE(pTorus, nullptr);
    EXPECT_FALSE(pTorus->getId().isNull());
}
