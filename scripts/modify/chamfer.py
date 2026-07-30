import wy3d


def createChamfer():
    """创建倒角特征 — 两步事务模式"""

    db = wy3d.getActiveDatabase()

    # ========== 事务1: 创建基体 ==========
    trans1 = db.getTransactionManager().startTransaction("create-box")
    box = wy3d.Box.create(transaction=trans1, length=30.0, width=20.0, height=10.0)
    trans1.addNewlyCreatedElement(box)
    db.getTransactionManager().endTransaction()
    print(f"Box created: id={box.getId().value()}")

    # ========== 事务2: 创建倒角 (需要基体拓扑已生成) ==========
    trans2 = db.getTransactionManager().startTransaction("create-chamfer")
    boxForWrite = trans2.getElementForWrite(box.getId())

    # 对指定边倒角
    chamfer = wy3d.Chamfer.create(
        transaction=trans2,
        solid=boxForWrite,
        faceIndices=[],
        edgeIndices=[0, 1, 2, 3],
        distance=2.0
    )

    if chamfer is None:
        print("❌ Chamfer 创建失败 — 边索引可能无效")
        db.getTransactionManager().abortTransaction()
        return

    trans2.addNewlyCreatedElement(chamfer)
    db.getTransactionManager().endTransaction()

    print(f"✅ Chamfer created: id={chamfer.getId().value()}")
    print(f"  className    = {chamfer.getClassName()}")
    print(f"  distance     = {chamfer.getDistance()}")
    print(f"  edges        = {chamfer.getEdges()}")
    print(f"  faces        = {chamfer.getFaces()}")


def createChamferScanEdges():
    """扫描所有边索引，找到有效边并创建倒角"""

    db = wy3d.getActiveDatabase()

    # 事务1: 创建基体
    trans1 = db.getTransactionManager().startTransaction("create-box")
    box = wy3d.Box.create(transaction=trans1, length=40.0, width=30.0, height=20.0)
    trans1.addNewlyCreatedElement(box)
    db.getTransactionManager().endTransaction()
    print(f"Box created: id={box.getId().value()}")

    # 事务2: 扫描边索引
    trans2 = db.getTransactionManager().startTransaction("create-chamfer")
    boxForWrite = trans2.getElementForWrite(box.getId())

    valid_edges = []
    for edge_idx in range(12):
        chamfer = wy3d.Chamfer.create(
            transaction=trans2,
            solid=boxForWrite,
            faceIndices=[],
            edgeIndices=[edge_idx],
            distance=2.0
        )
        if chamfer is not None:
            valid_edges.append(edge_idx)
            trans2.addNewlyCreatedElement(chamfer)
            print(f"  ✅ 边索引 {edge_idx} — Chamfer 创建成功")
        else:
            print(f"  ⚠️  边索引 {edge_idx} — 无效")

    db.getTransactionManager().endTransaction()
    print(f"\n有效边索引: {valid_edges}")


# =============================================================================
if __name__ == "__main__":
    createChamfer()
