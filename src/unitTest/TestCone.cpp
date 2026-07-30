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

TEST(Wy3dCone, Create)
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

TEST(Wy3dCone, DefaultColor)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Cone* pCone(nullptr);
    wydb::Transaction* pTrans = pTransMgr->startTransaction();
    Cone::create(pTrans, 10.0, 20.0, pCone);
    pTransMgr->endTransaction();
    ASSERT_NE(pCone, nullptr);

    const wy3d::Color c = pCone->getColor();
    EXPECT_LE(c.red, 255u);
    EXPECT_LE(c.green, 255u);
    EXPECT_LE(c.blue, 255u);
}

TEST(Wy3dCone, SetColor)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Cone* pCone(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cone::create(pTrans, 10.0, 20.0, pCone);
        pTransMgr->endTransaction();
        ASSERT_NE(pCone, nullptr);
    }

    wy3d::Color newColor(0, 0, 255);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cone* pWrite = Cone::cast(pTrans->getElementForWrite(pCone->getId()));
        ASSERT_NE(pWrite, nullptr);
        EXPECT_EQ(pWrite->setColor(newColor), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pCone->getColor(), newColor);
}

TEST(Wy3dCone, ModifyRadius)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Cone* pCone(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cone::create(pTrans, 25.0, 100.0, pCone);
        pTransMgr->endTransaction();
        ASSERT_NE(pCone, nullptr);
    }

    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cone* pWrite = Cone::cast(pTrans->getElementForWrite(pCone->getId()));
        EXPECT_EQ(pWrite->setRadius(50.0), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pCone->getRadius(), 50.0);
    EXPECT_EQ(pCone->getHeight(), 100.0);
}

TEST(Wy3dCone, ModifyHeight)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Cone* pCone(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cone::create(pTrans, 25.0, 100.0, pCone);
        pTransMgr->endTransaction();
        ASSERT_NE(pCone, nullptr);
    }

    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cone* pWrite = Cone::cast(pTrans->getElementForWrite(pCone->getId()));
        EXPECT_EQ(pWrite->setHeight(200.0), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pCone->getHeight(), 200.0);
}

TEST(Wy3dCone, ModifyBoth)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Cone* pCone(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cone::create(pTrans, 25.0, 100.0, pCone);
        pTransMgr->endTransaction();
        ASSERT_NE(pCone, nullptr);
    }

    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cone* pWrite = Cone::cast(pTrans->getElementForWrite(pCone->getId()));
        EXPECT_EQ(pWrite->setRadius(40.0), wy::ErrorStatus::Ok);
        EXPECT_EQ(pWrite->setHeight(150.0), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pCone->getRadius(), 40.0);
    EXPECT_EQ(pCone->getHeight(), 150.0);
}

TEST(Wy3dCone, CutFlag)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Cone* pCone(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cone::create(pTrans, 10.0, 20.0, pCone);
        pTransMgr->endTransaction();
    }
    ASSERT_NE(pCone, nullptr);
    EXPECT_FALSE(pCone->isCut());
}

TEST(Wy3dCone, Shape)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Cone* pCone(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cone::create(pTrans, 10.0, 30.0, pCone);
        pTransMgr->endTransaction();
        ASSERT_NE(pCone, nullptr);
    }

    EXPECT_FALSE(pCone->getShape().IsNull());
}

TEST(Wy3dCone, IO)
{
    double radius(25.0), height(100.0);
    std::string filePath("./test_cone_unit.wy3dt");
    wydb::ElementId coneId;

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

    {
        std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
        EXPECT_EQ(pDb->readFile(filePath, {wydb::FileType::Text}), wy::ErrorStatus::Ok);

        const Cone* pCone = Cone::cast(pDb->getElement(coneId));
        ASSERT_NE(pCone, nullptr);
        EXPECT_EQ(pCone->getRadius(), radius);
        EXPECT_EQ(pCone->getHeight(), height);
    }
}

TEST(Wy3dCone, Cast)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Cone* pCone(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cone::create(pTrans, 25.0, 100.0, pCone);
        pTransMgr->endTransaction();
        ASSERT_NE(pCone, nullptr);
    }

    const wydb::Element* pElem = pDb->getElement(pCone->getId());
    ASSERT_NE(pElem, nullptr);

    const Cone* pCone2 = Cone::cast(pElem);
    ASSERT_NE(pCone2, nullptr);

    const Solid* pSolid = Solid::cast(pElem);
    ASSERT_NE(pSolid, nullptr);
}

TEST(Wy3dCone, IdNotNull)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Cone* pCone(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Cone::create(pTrans, 10.0, 10.0, pCone);
        pTransMgr->endTransaction();
    }
    ASSERT_NE(pCone, nullptr);
    EXPECT_FALSE(pCone->getId().isNull());
}
