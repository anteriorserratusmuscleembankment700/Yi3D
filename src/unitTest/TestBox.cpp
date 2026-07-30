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

TEST(Wy3dBox, Create)
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

TEST(Wy3dBox, DefaultColor)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Box* pBox(nullptr);
    wydb::Transaction* pTrans = pTransMgr->startTransaction();
    Box::create(pTrans, 10.0, 10.0, 10.0, pBox);
    pTransMgr->endTransaction();
    ASSERT_NE(pBox, nullptr);

    // 新建实体应该有默认颜色
    const wy3d::Color c = pBox->getColor();
    EXPECT_LE(c.red, 255u);
    EXPECT_LE(c.green, 255u);
    EXPECT_LE(c.blue, 255u);
}

TEST(Wy3dBox, SetColor)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Box* pBox(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Box::create(pTrans, 10.0, 10.0, 10.0, pBox);
        pTransMgr->endTransaction();
        ASSERT_NE(pBox, nullptr);
    }

    wy3d::Color newColor(255, 0, 0);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Box* pBoxWrite = Box::cast(pTrans->getElementForWrite(pBox->getId()));
        ASSERT_NE(pBoxWrite, nullptr);
        EXPECT_EQ(pBoxWrite->setColor(newColor), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pBox->getColor(), newColor);
}

TEST(Wy3dBox, ModifyLength)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Box* pBox(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Box::create(pTrans, 100.0, 200.0, 300.0, pBox);
        pTransMgr->endTransaction();
        ASSERT_NE(pBox, nullptr);
    }

    double newLength(500.0);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Box* pBoxWrite = Box::cast(pTrans->getElementForWrite(pBox->getId()));
        EXPECT_EQ(pBoxWrite->setLength(newLength), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pBox->getLength(), newLength);
    EXPECT_EQ(pBox->getWidth(), 200.0);
    EXPECT_EQ(pBox->getHeight(), 300.0);
}

TEST(Wy3dBox, ModifyWidth)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Box* pBox(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Box::create(pTrans, 100.0, 200.0, 300.0, pBox);
        pTransMgr->endTransaction();
        ASSERT_NE(pBox, nullptr);
    }

    double newWidth(600.0);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Box* pBoxWrite = Box::cast(pTrans->getElementForWrite(pBox->getId()));
        EXPECT_EQ(pBoxWrite->setWidth(newWidth), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pBox->getWidth(), newWidth);
}

TEST(Wy3dBox, ModifyHeight)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Box* pBox(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Box::create(pTrans, 100.0, 200.0, 300.0, pBox);
        pTransMgr->endTransaction();
        ASSERT_NE(pBox, nullptr);
    }

    double newHeight(700.0);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Box* pBoxWrite = Box::cast(pTrans->getElementForWrite(pBox->getId()));
        EXPECT_EQ(pBoxWrite->setHeight(newHeight), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pBox->getHeight(), newHeight);
}

TEST(Wy3dBox, ModifyAll)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Box* pBox(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Box::create(pTrans, 100.0, 200.0, 300.0, pBox);
        pTransMgr->endTransaction();
        ASSERT_NE(pBox, nullptr);
    }

    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Box* pBoxWrite = Box::cast(pTrans->getElementForWrite(pBox->getId()));
        EXPECT_EQ(pBoxWrite->setLength(400.0), wy::ErrorStatus::Ok);
        EXPECT_EQ(pBoxWrite->setWidth(500.0), wy::ErrorStatus::Ok);
        EXPECT_EQ(pBoxWrite->setHeight(600.0), wy::ErrorStatus::Ok);
        pTransMgr->endTransaction();
    }
    EXPECT_EQ(pBox->getLength(), 400.0);
    EXPECT_EQ(pBox->getWidth(), 500.0);
    EXPECT_EQ(pBox->getHeight(), 600.0);
}

TEST(Wy3dBox, CutFlag)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Box* pBox(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Box::create(pTrans, 10.0, 10.0, 10.0, pBox);
        pTransMgr->endTransaction();
        ASSERT_NE(pBox, nullptr);
    }

    // 默认为 false
    EXPECT_FALSE(pBox->isCut());
}

TEST(Wy3dBox, Shape)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Box* pBox(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Box::create(pTrans, 10.0, 20.0, 30.0, pBox);
        pTransMgr->endTransaction();
        ASSERT_NE(pBox, nullptr);
    }

    EXPECT_FALSE(pBox->getShape().IsNull());
}

TEST(Wy3dBox, IO)
{
    double length(100.0), width(200.0), height(300.0);
    std::string filePath("./test_box_unit.wy3dt");
    wydb::ElementId boxId;

    {
        std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
        wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

        Box* pBox(nullptr);
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Box::create(pTrans, length, width, height, pBox);
        pTransMgr->endTransaction();
        boxId = pBox->getId();

        EXPECT_EQ(pDb->writeFile(filePath, {wydb::FileType::Text}), wy::ErrorStatus::Ok);
    }

    {
        std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
        EXPECT_EQ(pDb->readFile(filePath, {wydb::FileType::Text}), wy::ErrorStatus::Ok);

        const Box* pBox = Box::cast(pDb->getElement(boxId));
        ASSERT_NE(pBox, nullptr);
        EXPECT_EQ(pBox->getLength(), length);
        EXPECT_EQ(pBox->getWidth(), width);
        EXPECT_EQ(pBox->getHeight(), height);
    }
}

TEST(Wy3dBox, Cast)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Box* pBox(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Box::create(pTrans, 100.0, 200.0, 300.0, pBox);
        pTransMgr->endTransaction();
        ASSERT_NE(pBox, nullptr);
    }

    const wydb::Element* pElem = pDb->getElement(pBox->getId());
    ASSERT_NE(pElem, nullptr);

    // Box cast 成功
    const Box* pBox2 = Box::cast(pElem);
    ASSERT_NE(pBox2, nullptr);

    // Solid cast（基类）也应该成功
    const Solid* pSolid = Solid::cast(pElem);
    ASSERT_NE(pSolid, nullptr);

    // Feature cast（更上层基类）
    const Feature* pFeature = Feature::cast(pElem);
    ASSERT_NE(pFeature, nullptr);
}

TEST(Wy3dBox, IdNotNull)
{
    std::unique_ptr<wy3d::Database> pDb = std::make_unique<wy3d::Database>();
    wydb::TransactionManager* pTransMgr = pDb->getTransactionManager();

    Box* pBox(nullptr);
    {
        wydb::Transaction* pTrans = pTransMgr->startTransaction();
        Box::create(pTrans, 10.0, 10.0, 10.0, pBox);
        pTransMgr->endTransaction();
    }
    ASSERT_NE(pBox, nullptr);
    EXPECT_FALSE(pBox->getId().isNull());
}
