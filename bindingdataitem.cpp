#include <QStringList>
#include <apiextractor.h>
#include "bindingdataitem.h"

#include <QDebug>

BindingDataItem::BindingDataItem(BindingDataItem* parent)
    : m_parent(parent)
{
}

BindingDataItem::~BindingDataItem()
{
    qDeleteAll(m_children);
}

void
BindingDataItem::appendChild(BindingDataItem* child)
{
    m_children.append(child);
}

BindingDataItem*
BindingDataItem::child(int row)
{
    if (row < childCount())
        return m_children.at(row);
    return 0;
}

int
BindingDataItem::row() const
{
    if (m_parent)
        return m_parent->m_children.indexOf(const_cast<BindingDataItem*>(this));
    return 0;
}


// ----------------------------------------------------------------------------
HeaderDataItem::HeaderDataItem(const QStringList& data, BindingDataItem* parent)
    : BindingDataItem(parent), m_data(data)
{
}

QVariant
HeaderDataItem::data(int column) const
{
    return QVariant(m_data.at(column));
}


// ----------------------------------------------------------------------------
PrimitiveDataItem::PrimitiveDataItem(PrimitiveTypeEntry* data, BindingDataItem* parent)
    : BindingDataItem(parent), m_data(data)
{
}

QVariant
PrimitiveDataItem::data(int column) const
{
    switch (column) {
        case BindingDataItem::ItemName:
            return QVariant(m_data->name());
        case BindingDataItem::ItemType:
            return QVariant("primitive");
        case BindingDataItem::ItemModifications:
            return QVariant("");
    }
    return QVariant();
}

QVariant
PrimitiveDataItem::decoration() const
{
    return QVariant(Qt::red);
}


// ----------------------------------------------------------------------------
ClassDataItem::ClassDataItem(AbstractMetaClass* data, BindingDataItem* parent)
    : BindingDataItem(parent), m_data(data)
{
}

QVariant
ClassDataItem::data(int column) const
{
    switch (column) {
        case BindingDataItem::ItemName:
            return QVariant(m_data->name());
        case BindingDataItem::ItemType:
            return QVariant(m_data->isNamespace() ? "namespace" : "class");
        case BindingDataItem::ItemModifications:
            return QVariant("");
    }
    return QVariant();
}

QVariant
ClassDataItem::decoration() const
{
    if (m_data->isNamespace())
        return QVariant(Qt::lightGray);
    return QVariant(Qt::darkCyan);
}

// ----------------------------------------------------------------------------
FunctionOverloadDataItem::FunctionOverloadDataItem(const AbstractMetaFunctionList& data, BindingDataItem* parent)
    : BindingDataItem(parent), m_data(data)
{
}

QVariant
FunctionOverloadDataItem::data(int column) const
{
    switch (column) {
        case BindingDataItem::ItemName:
            return QVariant(QString("%1 (%2)").arg(m_data.first()->name()).arg(m_data.count()));
        case BindingDataItem::ItemType:
            return QVariant("overloads");
    }
    return QVariant();
}

QVariant
FunctionOverloadDataItem::decoration() const
{
    return QVariant(Qt::blue);
}


// ----------------------------------------------------------------------------
FunctionDataItem::FunctionDataItem(AbstractMetaFunction* data, BindingDataItem* parent)
    : BindingDataItem(parent), m_data(data)
{
}

QVariant
FunctionDataItem::data(int column) const
{
    switch (column) {
        case BindingDataItem::ItemName:
            return QVariant(m_data->minimalSignature());
        case BindingDataItem::ItemType:
            return QVariant("function");
        case BindingDataItem::ItemModifications:
            return QVariant("");
    }
    return QVariant();
}

QVariant
FunctionDataItem::decoration() const
{
    return QVariant(Qt::darkBlue);
}


// ----------------------------------------------------------------------------
FieldDataItem::FieldDataItem(AbstractMetaField* data, BindingDataItem* parent)
    : BindingDataItem(parent), m_data(data)
{
}

QVariant
FieldDataItem::data(int column) const
{
    switch (column) {
        case BindingDataItem::ItemName:
            return QVariant(m_data->name());
        case BindingDataItem::ItemType:
            return QVariant(m_data->type()->cppSignature());
        case BindingDataItem::ItemModifications:
            return QVariant("");
    }
    return QVariant();
}

QVariant
FieldDataItem::decoration() const
{
    return QVariant(Qt::darkRed);
}


// ----------------------------------------------------------------------------
EnumDataItem::EnumDataItem(AbstractMetaEnum* data, BindingDataItem* parent)
    : BindingDataItem(parent), m_data(data)
{
}

QVariant
EnumDataItem::data(int column) const
{
    switch (column) {
        case BindingDataItem::ItemName:
            return QVariant(m_data->name());
        case BindingDataItem::ItemType:
            return QVariant("enum");
        case BindingDataItem::ItemModifications:
            return QVariant("");
    }
    return QVariant();
}

QVariant
EnumDataItem::decoration() const
{
    return QVariant(Qt::green);
}


// ----------------------------------------------------------------------------
EnumValueDataItem::EnumValueDataItem(AbstractMetaEnumValue* data, BindingDataItem* parent)
    : BindingDataItem(parent), m_data(data)
{
}

QVariant
EnumValueDataItem::data(int column) const
{
    switch (column) {
        case BindingDataItem::ItemName:
            return QVariant(m_data->name());
        case BindingDataItem::ItemType:
            return QVariant(QString("%1").arg(m_data->value()));
        case BindingDataItem::ItemModifications:
            return QVariant("");
    }
    return QVariant();
}

QVariant
EnumValueDataItem::decoration() const
{
    return QVariant(Qt::darkGreen);
}

