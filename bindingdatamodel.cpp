#include <QMap>
#include <apiextractor.h>
#include "bindingdatamodel.h"
#include "bindingdataitem.h"

BindingDataModel::BindingDataModel(ApiExtractor* dataSource, QObject* parent)
    : QAbstractItemModel(parent), m_apiExtractor(dataSource)
{
    QStringList headerData;
    headerData << "Name" << "Type" << "Modifications";
    m_rootItem = new HeaderDataItem(headerData);
    setupModelData();
}

BindingDataModel::~BindingDataModel()
{
    if (m_apiExtractor) {
        delete m_rootItem;
        delete m_apiExtractor;
    }
}

QVariant
BindingDataModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    BindingDataItem* item = static_cast<BindingDataItem*>(index.internalPointer());

    if (role == Qt::DisplayRole) {
        return item->data(index.column());
    } else if (role == Qt::DecorationRole && index.column() == 0) {
        return item->decoration();
    }
    return QVariant();
}

Qt::ItemFlags
BindingDataModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return 0;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant
BindingDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_rootItem->data(section);
    return QVariant();
}

QModelIndex
BindingDataModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    BindingDataItem* parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<BindingDataItem*>(parent.internalPointer());

    BindingDataItem* childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex
BindingDataModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    BindingDataItem* childItem = static_cast<BindingDataItem*>(index.internalPointer());
    BindingDataItem* parentItem = childItem->parent();

    if (parentItem == m_rootItem)
        return QModelIndex();
    return createIndex(parentItem->row(), 0, parentItem);
}

int
BindingDataModel::rowCount(const QModelIndex& parent) const
{
    BindingDataItem* parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<BindingDataItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int
BindingDataModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return static_cast<BindingDataItem*>(parent.internalPointer())->columnCount();
    return m_rootItem->columnCount();
}

void
BindingDataModel::setupModelData()
{
    foreach (const PrimitiveTypeEntry* primitiveTypeEntry, m_apiExtractor->primitiveTypes())
        setupModelData(primitiveTypeEntry, m_rootItem);
    foreach (const ContainerTypeEntry* containerTypeEntry, m_apiExtractor->containerTypes())
        setupModelData(containerTypeEntry, m_rootItem);
    foreach (AbstractMetaClass* metaClass, m_apiExtractor->classes()) {
        if (!metaClass->typeEntry()->generateCode())
            continue;
        if (!metaClass->enclosingClass())
            setupModelData(metaClass, m_rootItem);
        if (m_packageName.isEmpty() && !metaClass->package().isEmpty())
            m_packageName = metaClass->package();
    }
    foreach (AbstractMetaEnum* metaEnum, m_apiExtractor->globalEnums())
        setupModelData(metaEnum, m_rootItem);
    foreach (const AbstractMetaFunctionList& funcs, getOverloads())
        setupModelData(funcs, m_rootItem);
}

void
BindingDataModel::setupModelData(const PrimitiveTypeEntry* primitiveTypeEntry, BindingDataItem* parent)
{
    BindingDataItem* item = new PrimitiveDataItem(const_cast<PrimitiveTypeEntry*>(primitiveTypeEntry), parent);
    parent->appendChild(item);
}

void
BindingDataModel::setupModelData(const ContainerTypeEntry* containerTypeEntry, BindingDataItem* parent)
{
    BindingDataItem* item = new ContainerDataItem(const_cast<ContainerTypeEntry*>(containerTypeEntry), parent);
    parent->appendChild(item);
}

void
BindingDataModel::setupModelData(AbstractMetaClass* metaClass, BindingDataItem* parent)
{
    BindingDataItem* item = new ClassDataItem(metaClass, parent);
    parent->appendChild(item);
    foreach (const AbstractMetaFunctionList& funcs, getOverloads(metaClass))
        setupModelData(funcs, item);
    foreach (AbstractMetaEnum* metaEnum, metaClass->enums())
        setupModelData(metaEnum, item);
    foreach (AbstractMetaField* metaField, metaClass->fields())
        setupModelData(metaField, item);
    foreach (AbstractMetaClass* innerClass, metaClass->innerClasses())
        setupModelData(innerClass, item);
}

void
BindingDataModel::setupModelData(const AbstractMetaFunctionList& funcs, BindingDataItem* parent)
{
    BindingDataItem* item = new FunctionOverloadDataItem(funcs, parent);
    foreach (AbstractMetaFunction* metaFunc, funcs)
        setupModelData(metaFunc, item);
    parent->appendChild(item);
}

void
BindingDataModel::setupModelData(AbstractMetaFunction* metaFunc, BindingDataItem* parent)
{
    BindingDataItem* item = new FunctionDataItem(metaFunc, parent);
    foreach (AbstractMetaArgument* arg, metaFunc->arguments())
        setupModelData(arg, item);
    parent->appendChild(item);
}

void
BindingDataModel::setupModelData(AbstractMetaArgument* metaArg, BindingDataItem* parent)
{
    BindingDataItem* item = new ArgumentDataItem(metaArg, parent);
    parent->appendChild(item);
}


void
BindingDataModel::setupModelData(AbstractMetaField* metaField, BindingDataItem* parent)
{
    BindingDataItem* item = new FieldDataItem(metaField, parent);
    parent->appendChild(item);
}

void
BindingDataModel::setupModelData(AbstractMetaEnum* metaEnum, BindingDataItem* parent)
{
    BindingDataItem* item = new EnumDataItem(metaEnum, parent);
    parent->appendChild(item);
    foreach (AbstractMetaEnumValue* enumValue, metaEnum->values())
        setupModelData(enumValue, item);
}

void
BindingDataModel::setupModelData(AbstractMetaEnumValue* metaEnumValue, BindingDataItem* parent)
{
    BindingDataItem* item = new EnumValueDataItem(metaEnumValue, parent);
    parent->appendChild(item);
}

QList<AbstractMetaFunctionList>
BindingDataModel::getOverloads(const AbstractMetaClass* scope)
{
    QMap<QString, AbstractMetaFunctionList> overloads;
    AbstractMetaFunctionList lst = scope ? scope->functions() : m_apiExtractor->globalFunctions();
    foreach (AbstractMetaFunction* func, lst)
        overloads[func->name()].append(func);
    return overloads.values();
}

