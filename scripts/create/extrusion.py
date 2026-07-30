import wy3d

def createExtrusion():
    # 获取当前文档数据库
    db = wy3d.getActiveDatabase()
    # 开启事务
    trans = db.getTransactionManager().startTransaction()
    # 创建草图
    plane = wy3d.SketchPlane(
        origin = wy3d.Vector3(0.0, 0.0, 0.0),
        normal = wy3d.Vector3(0.0, 0.0, 1.0),
        xDir = wy3d.Vector3(1.0, 0.0, 0.0))
    sketch = wy3d.Sketch.create(trans, plane)
    trans.addNewlyCreatedElement(sketch)
    # 线1
    line1 = wy3d.SketchLine.create(trans, wy3d.Vector2(0.0, 0.0), wy3d.Vector2(100.0, 0.0))
    trans.addNewlyCreatedElement(line1)
    sketch.addEntity(line1)
    # 线2
    line2 = wy3d.SketchLine.create(trans, wy3d.Vector2(100.0, 0.0), wy3d.Vector2(100.0, 50.0))
    trans.addNewlyCreatedElement(line2)
    sketch.addEntity(line2)
    # 线3
    line3 = wy3d.SketchLine.create(trans, wy3d.Vector2(100.0, 50.0), wy3d.Vector2(0.0, 50.0))
    trans.addNewlyCreatedElement(line3)
    sketch.addEntity(line3)
    # 线4
    line4 = wy3d.SketchLine.create(trans, wy3d.Vector2(0.0, 50.0), wy3d.Vector2(0.0, 0.0))
    trans.addNewlyCreatedElement(line4)
    sketch.addEntity(line4)
    # 拉伸体
    extrusion1 = wy3d.Extrusion.create(trans, sketch, 20.0)
    trans.addNewlyCreatedElement(extrusion1)
    # 提交事务
    db.getTransactionManager().endTransaction()

# 创建拉伸体
createExtrusion()
