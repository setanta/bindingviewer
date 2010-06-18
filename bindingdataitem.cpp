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
    child->m_parent = this;
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
    if (column < m_data.count())
        return QVariant(m_data.at(column));
    return QVariant();
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
ContainerDataItem::ContainerDataItem(ContainerTypeEntry* data, BindingDataItem* parent)
    : BindingDataItem(parent), m_data(data)
{
}

QVariant
ContainerDataItem::data(int column) const
{
    switch (column) {
        case BindingDataItem::ItemName:
            return QVariant(m_data->name());
        case BindingDataItem::ItemType:
            switch (m_data->type()) {
                case ContainerTypeEntry::ListContainer:
                    return QVariant("list");
                    break;
                case ContainerTypeEntry::StringListContainer:
                    return QVariant("string list");
                    break;
                case ContainerTypeEntry::LinkedListContainer:
                    return QVariant("linked list");
                    break;
                case ContainerTypeEntry::VectorContainer:
                    return QVariant("vector");
                    break;
                case ContainerTypeEntry::StackContainer:
                    return QVariant("stack");
                    break;
                case ContainerTypeEntry::QueueContainer:
                    return QVariant("queue");
                    break;
                case ContainerTypeEntry::SetContainer:
                    return QVariant("set");
                    break;
                case ContainerTypeEntry::MapContainer:
                    return QVariant("map");
                    break;
                case ContainerTypeEntry::MultiMapContainer:
                    return QVariant("multimap");
                    break;
                case ContainerTypeEntry::HashContainer:
                    return QVariant("hash");
                    break;
                case ContainerTypeEntry::MultiHashContainer:
                    return QVariant("multihash");
                    break;
                case ContainerTypeEntry::PairContainer:
                    return QVariant("pair");
                    break;
            }
        case BindingDataItem::ItemModifications:
            return QVariant("");
    }
    return QVariant();
}

QVariant
ContainerDataItem::decoration() const
{
    return QVariant(Qt::darkMagenta);
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
            return QVariant(m_data->isNamespace() ? "namespace" : "");
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
            return QVariant(m_data.first()->name());
        case BindingDataItem::ItemType:
            return QVariant(QString("%1 overload%2").arg(m_data.count()).arg((m_data.count() == 1) ? "" : "s"));
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
            if (m_data->type())
                return QVariant(m_data->type()->name());
            else if (!m_data->isConstructor())
                return QVariant("void");
        case BindingDataItem::ItemModifications:
            QStringList mods;
            if (m_data->name() != m_data->originalName())
                mods << QString("renamed: " + m_data->name());
            if (m_data->isModifiedRemoved())
                mods << "removed";
            return QVariant(mods.join("; "));
    }
    return QVariant();
}

QVariant
FunctionDataItem::decoration() const
{
    return QVariant(Qt::darkBlue);
}


// ----------------------------------------------------------------------------
ArgumentDataItem::ArgumentDataItem(AbstractMetaArgument* data, BindingDataItem* parent)
    : BindingDataItem(parent), m_data(data)
{
}

QVariant
ArgumentDataItem::data(int column) const
{
    switch (column) {
        case BindingDataItem::ItemName:
            return QVariant(m_data->argumentName());
        case BindingDataItem::ItemType:
        {
            QString text = m_data->type()->name();
            if (!m_data->defaultValueExpression().isEmpty())
                text += " = " + m_data->defaultValueExpression();
            return QVariant(text);
        }
        case BindingDataItem::ItemModifications:
            QStringList mods;
            if (m_data->defaultValueExpression() != m_data->originalDefaultValueExpression()) {
                QString txt = QString("default value changed from '%1' to '%2'")
                        .arg(m_data->originalDefaultValueExpression())
                        .arg(m_data->defaultValueExpression());
                mods << txt;
            }
            return QVariant(mods.join("; "));
    }
    return QVariant();
}

QVariant
ArgumentDataItem::decoration() const
{
    return QVariant(Qt::lightGray);
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

