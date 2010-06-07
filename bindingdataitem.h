#ifndef BINDINGDATAITEM_H
#define BINDINGDATAITEM_H

#include <QList>
#include <QVariant>

class AbstractMetaClass;
class AbstractMetaFunction;
class AbstractMetaField;
class AbstractMetaEnum;
class AbstractMetaEnumValue;

class BindingDataItem
{
public:
    BindingDataItem(BindingDataItem* parent);
    virtual ~BindingDataItem();

    void appendChild(BindingDataItem* child);
    BindingDataItem* child(int row);
    int row() const;

    const QList<BindingDataItem*>& children() const { return m_children; }
    int childCount() const { return m_children.count(); }
    BindingDataItem* parent() { return m_parent; }

    enum DataColumns {
        ItemName,
        ItemType,
        ItemModifications
    };
    int columnCount() const { return 3; }

    virtual QVariant data(int column) const = 0;
    virtual QVariant decoration() const { return QVariant(); }

private:
    BindingDataItem* m_parent;
    QList<BindingDataItem*> m_children;
};


class HeaderDataItem : public BindingDataItem
{
public:
    HeaderDataItem(const QStringList& data, BindingDataItem* parent = 0);
    virtual QVariant data(int column) const;
private:
    QStringList m_data;
};


class ClassDataItem : public BindingDataItem
{
public:
    ClassDataItem(AbstractMetaClass* data, BindingDataItem* parent = 0);
    virtual QVariant decoration() const;
    virtual QVariant data(int column) const;
private:
    AbstractMetaClass* m_data;
};


class FunctionOverloadDataItem : public BindingDataItem
{
public:
    FunctionOverloadDataItem(const AbstractMetaFunctionList& data, BindingDataItem* parent = 0);
    virtual QVariant decoration() const;
    virtual QVariant data(int column) const;
private:
    AbstractMetaFunctionList m_data;
};


class FunctionDataItem : public BindingDataItem
{
public:
    FunctionDataItem(AbstractMetaFunction* data, BindingDataItem* parent = 0);
    virtual QVariant decoration() const;
    virtual QVariant data(int column) const;
private:
    AbstractMetaFunction* m_data;
};


class FieldDataItem : public BindingDataItem
{
public:
    FieldDataItem(AbstractMetaField* data, BindingDataItem* parent = 0);
    virtual QVariant decoration() const;
    virtual QVariant data(int column) const;
private:
    AbstractMetaField* m_data;
};


class EnumDataItem : public BindingDataItem
{
public:
    EnumDataItem(AbstractMetaEnum* data, BindingDataItem* parent = 0);
    virtual QVariant decoration() const;
    virtual QVariant data(int column) const;
private:
    AbstractMetaEnum* m_data;
};


class EnumValueDataItem : public BindingDataItem
{
public:
    EnumValueDataItem(AbstractMetaEnumValue* data, BindingDataItem* parent = 0);
    virtual QVariant decoration() const;
    virtual QVariant data(int column) const;
private:
    AbstractMetaEnumValue* m_data;
};

#endif //BINDINGDATAITEM_H

