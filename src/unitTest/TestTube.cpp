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

TEST(Wy3dTube, Create)
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

TEST(Wy3dTube, DefaultColor)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Tube* pTube(nullptr);
    wydb::Transaction* pTrans = pTransMgr->startTransaction();
    Tube::create(pTrans, 20.0, 10.0, 50.0, pTube);
    pTransMgr->endTransaction();
    ASSERT_NE(pTube, nullptr);

    const wy3d::Color c = pTube->getColor();
    EXPECT_LE(c.red, 255u);
    EXPECT_LE(c.green, 255u);
    EXPECT_LE(c.blue, 255u);
}

TEST(Wy3dTube, SetColor)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Tube* pTube(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Tube::create(pTrans, 20.0, 10.0, 50.0, pTube);
        pTransMgr->endTransaction();
        ASSERT_NE(pTube, nullptr);
    }

    wy3d::Color newColor(50, 50, 50);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Tube* pWrite = Tube::cast(pTrans->getElementForWrite(pTube->getId()));
        ASSERT_NE(pWrite, nullptr);
        EXPECT_EQ(pWrite->setColor(newColor), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pTube->getColor(), newColor);
}

TEST(Wy3dTube, ModifyOuterRadius)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Tube* pTube(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Tube::create(pTrans, 50.0, 30.0, 200.0, pTube);
        pTransMgr->endTransaction();
        ASSERT_NE(pTube, nullptr);
    }

    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Tube* pWrite = Tube::cast(pTrans->getElementForWrite(pTube->getId()));
        EXPECT_EQ(pWrite->setOuterRadius(60.0), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pTube->getOuterRadius(), 60.0);
    EXPECT_EQ(pTube->getInnerRadius(), 30.0);
    EXPECT_EQ(pTube->getHeight(), 200.0);
}

TEST(Wy3dTube, ModifyInnerRadius)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Tube* pTube(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Tube::create(pTrans, 50.0, 30.0, 200.0, pTube);
        pTransMgr->endTransaction();
        ASSERT_NE(pTube, nullptr);
    }

    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Tube* pWrite = Tube::cast(pTrans->getElementForWrite(pTube->getId()));
        EXPECT_EQ(pWrite->setInnerRadius(20.0), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pTube->getInnerRadius(), 20.0);
}

TEST(Wy3dTube, ModifyHeight)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Tube* pTube(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Tube::create(pTrans, 50.0, 30.0, 200.0, pTube);
        pTransMgr->endTransaction();
        ASSERT_NE(pTube, nullptr);
    }

    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Tube* pWrite = Tube::cast(pTrans->getElementForWrite(pTube->getId()));
        EXPECT_EQ(pWrite->setHeight(300.0), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pTube->getHeight(), 300.0);
}

TEST(Wy3dTube, ModifyAll)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Tube* pTube(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Tube::create(pTrans, 50.0, 30.0, 200.0, pTube);
        pTransMgr->endTransaction();
        ASSERT_NE(pTube, nullptr);
    }

    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Tube* pWrite = Tube::cast(pTrans->getElementForWrite(pTube->getId()));
        EXPECT_EQ(pWrite->setOuterRadius(70.0), wy::ErrorStatus::Ok);
        EXPECT_EQ(pWrite->setInnerRadius(25.0), wy::ErrorStatus::Ok);
        EXPECT_EQ(pWrite->setHeight(250.0), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pTube->getOuterRadius(), 70.0);
    EXPECT_EQ(pTube->getInnerRadius(), 25.0);
    EXPECT_EQ(pTube->getHeight(), 250.0);
}

TEST(Wy3dTube, CutFlag)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Tube* pTube(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Tube::create(pTrans, 20.0, 10.0, 50.0, pTube);
        pTransMgr->endTransaction();
    }
    ASSERT_NE(pTube, nullptr);
    EXPECT_FALSE(pTube->isCut());
}

TEST(Wy3dTube, Shape)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Tube* pTube(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Tube::create(pTrans, 20.0, 10.0, 50.0, pTube);
        pTransMgr->endTransaction();
        ASSERT_NE(pTube, nullptr);
    }

    EXPECT_FALSE(pTube->getShape().IsNull());
}

TEST(Wy3dTube, IO)
{
    double outerRadius(50.0), innerRadius(30.0), height(200.0);
    std::string filePath("./test_tube_unit.wy3dt");
    wydb::ElementId tubeId;

    {
        std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
        wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

        Tube* pTube(nullptr);
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Tube::create(pTrans, outerRadius, innerRadius, height, pTube);
        pTransMgr->endTransaction();
        tubeId = pTube->getId();

        EXPECT_EQ(pDb->writeFile(filePath, {wydb::FileType::Text}), wy::ErrorStatus::Ok);
    }

    {
        std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
        EXPECT_EQ(pDb->readFile(filePath, {wydb::FileType::Text}), wy::ErrorStatus::Ok);

        const Tube* pTube = Tube::cast(pDb->getElement(tubeId));
        ASSERT_NE(pTube, nullptr);
        EXPECT_EQ(pTube->getOuterRadius(), outerRadius);
        EXPECT_EQ(pTube->getInnerRadius(), innerRadius);
        EXPECT_EQ(pTube->getHeight(), height);
    }
}

TEST(Wy3dTube, Cast)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Tube* pTube(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Tube::create(pTrans, 50.0, 30.0, 200.0, pTube);
        pTransMgr->endTransaction();
        ASSERT_NE(pTube, nullptr);
    }

    const wydb::Element* pElem = pDb->getElement(pTube->getId());
    ASSERT_NE(pElem, nullptr);

    const Tube* pTube2 = Tube::cast(pElem);
    ASSERT_NE(pTube2, nullptr);

    const Solid* pSolid = Solid::cast(pElem);
    ASSERT_NE(pSolid, nullptr);
}

TEST(Wy3dTube, IdNotNull)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Tube* pTube(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Tube::create(pTrans, 20.0, 10.0, 50.0, pTube);
        pTransMgr->endTransaction();
    }
    ASSERT_NE(pTube, nullptr);
    EXPECT_FALSE(pTube->getId().isNull());
}
