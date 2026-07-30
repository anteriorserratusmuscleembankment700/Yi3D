# 基本的设计原则
* 所有元素的修改都必须通过事务来实现。

    新增元素是通过具体类的静态`Create`方法实现，没有经过事务。一般应在新增完元素之后，通过调用事务类的`addNewlyCreatedElement`方法将新增的元素添加到事务中，交由事务管理。

* 借鉴ObjectARX、Creo、ParaSolid、ACIS等成熟产品与库，摒弃C++异常，每个接口函数返回错误码来标明函数的执行状态。

* 借鉴Revit，每个元素通过`ElementId`来唯一标识。

    同时在ElementId中记录额外的数据（元素表的索引）来加快通过`ElementId`获取元素的速度。

* 不提供ObjectARX那样自定义实体的功能。

    自定义实体是ObjectARX提供的一个功能非常强大也很复杂的功能。没必要提供这么强大的功能，因为这会增加太多的复杂度以及不确定性，不可控的因素太多。

# 基本的使用原则
1. DataBase类在业务代码中一律使用DataBase*，不需要const DataBase*。

# DataBase
### 一、访问数据库中元素的方法
1. 通过ID获取元素

`wy::ErrorStatus getElement(const ElementId& id, const Element*& pElem) const;`

注：只能获取const指针，要修改元素需要通过事务来实现。

2. 遍历数据库中的元素

`std::shared_ptr<Iterator<ElementId>> newIterator(ElementType type) const;`

### 二、修改元素
不支持从DataBase层面直接修改元素，所有修改都必须通过事务来实现。（注：新增元素可以脱离事务，但一般也需要把新增的元素添加到事务中由事务来管理）

不需要区分`DataBase`的`const`与非`const`，在编码中一律用`DataBase*`。

# 事务
事务最主要的功能：
* undo & redo
* 构建元素间的依赖关系网（增量式更新）
* 传播删除

    比如：删除墙，会顺带把墙上的所有门窗也一并删除。
* 级联更新

继承关系：

```c++
Transaction
----TransactionImpl
--------TransactionLeaf
--------TransactionGroup
```
其中Transaction是暴露出来的，TransactionImpl & TransactionLeaf & TransactionGroup 是内部使用的。


事务分为两类：
* TransactionLeaf
* TransactionGroup

TransactionGroup用来将几个独立的事务打包成一个组。当一个TransactionGroup被提交时，所有这个组里的事务保持原来状态（提交或撤销）。当一个TransactionGroup被撤销时，所有这个组里的事务，都会被撤销。

TransactionGroup本身不具备修改元素的能力，它是通过下属的事务子项来实现功能的。(TransactionGroup::getElementForWrite直接返回nullptr)

事务的嵌套是通过事务组来实现的；事务组内既可以包含TransactionLeaf，也可以包含子的TransactionGroup。例如：
```
TransactionGroup
{
    TransactionLeaf1
    TransactionGroupA
    {
        TransactionLeaf2
        TransactionLeaf3
        TransactionGroupB
        {
            ......
        }
    }
    TransactionLeaf4
}
```


### 一、基本原则
<1> 所有元素的修改都必须通过事务来实现。

一般的流程如下：1、开启事务；2、获取元素用于写；3、调用set方法修改元素；4、提交事务。
```c++
Transaction* pTrans = pTransManager->startTransaction();
{
    Element* pElem = pTrans->getElementForWrite(elemId);
    // 调用set方法修改元素
    ......
}
pTransManager->endTransaction();
```

目前只有通过事务类的`getElementForWrite`才能获取元素的非`const`指针，从而实现修改元素属性；通过`DataBase`类获取的元素指针都是`const`的。

请确保在事务开启后结束前调用set方法修改元素；当事务关闭后，由该事务打开的所有Element都不支持修改。
例如：以下代码在事务关闭后，调用set方法修改元素不起作用，直接返回`wy::ErrorStatus::NotOpenedForWrite`。
```c++
Transaction* pTrans = pTransManager->startTransaction();
Element* pElem = pTrans->getElementForWrite(elemId);
pTransManager->endTransaction();

// 事务结束后调用set方法不起作用,直接返回wy::ErrorStatus::NotOpenedForWrite
wy::ErrorStatus err = pElem->setA(); // err = wy::ErrorStatus::NotOpenedForWrite
```

<2> 新增元素

每一个`Element`的可实例化子类都提供了静态`Create`方法来创建实例。
在创建完实例后，应使用事务类的`addNewlyCreatedElement`方法将创建完的实例添加到事务中交由事务管理。

新增的元素若没有添加到事务中
* 会导致元素间的依赖关系网是不完整的，极大概率会引发一系列的错误。
* 不支持undo&redo。

```c++
// 调用具体元素类的Create方法新增元素
Extrusion* pExtrusion = Extrusion::Create(......);

// 将新创建的元素添加到事务中
Transaction* pTrans = pTransMgr->startTransaction();
pTrans->addNewlyCreatedElement(pExtrusion);
pTransMgr->endTransaction();
```

<3> 事务开启与提交 & 事务开启与终止 需要严格配对使用

请用户确保事务的开启与提交 & 事务的开启与终止 完全配对使用；否则会引发一系列的问题。
```c++
if (pTransMgr->startTransaction())
{
    pTransMgr->endTransaction();
}
```
```c++
if (pTransMgr->startTransaction())
{
    pTransMgr->abortTransaction();
}
```

<4> 在事务结束和事务终止后，不使用事务开启时获取的事务指针
```c++
Transaction* pTrans = pTransMgr->startTransaction();
pTransMgr->endTransaction();

// never use pTrans
```

### 二、事务流程
1. 传播删除
2. 增量式更新元素间依赖关系网
3. 级联更新
4. 提交undo&redo数据

### 三、事务嵌套

事务的嵌套是使用事务组来实现的。
```
TransactionGroup
{
    TransactionLeaf1
    TransactionGroupA
    {
        TransactionLeaf2
        TransactionLeaf3
        TransactionGroupB
        {
            ......
        }
    }
    TransactionLeaf4
}
```

### 四、事务的标准使用示例
* 基本事务

```c++ 
TransactionManager* pTransMgr = pDb->getTransactionManager();
Transaction* pTrans = pTransMgr->startTransaction();
if (pTrans)
{
    Element* pElem(nullptr);
    error = pTrans->getElementForWrite(id, pElem);
    if (wy::ErrorStatus::Ok == error && pElem)
    {
         ......
    }
    pTransMgr->endTransaction();
}
```
* 事务组
```c++
TransactionManager* pTransMgr = pDb->getTransactionManager();
Transaction* pTransGroup = pTransMgr->startTransactionGroup();
if (pTransGroup)
{
    Transaction* pTrans = pTransMgr->startTransaction();
    if (pTrans)
    {

        pTransMgr->endTransaction();
    }

    pTransMgr->endTransaction();
}
```

### 五、使用事务的特别注意事项
* 新增的元素没有通过Transaction::addNewlyCreatedElement交由事务管理，通过事务getElementForWrite之后修改，undo&redo是不起作用的。新增元素支持undo&redo的唯一方式是通过Transaction::addNewlyCreatedElement。
* 新增的元素通过Transaction::addNewlyCreatedElement交由事务管理，在同一个事务中再调用erase方法，事务提交后，该新增的元素为ghost鬼魅元素，脱离事务管理，不支持undo&redo，也无法修改。

# 级联更新
### 级联更新流程
1. 收集dirty data pieces
2. 收集dirty elements
3. 传播dirty elements从而获取所有dirty elements
4. 根据所有dirty elements全量构建data piece的依赖关系图（有向无环图）
5. 传播dirty pieces从而获取所有dirty pieces
6. DFS确定所有dirty pieces的拓扑顺序
7. 依序执行级联更新

# Element


