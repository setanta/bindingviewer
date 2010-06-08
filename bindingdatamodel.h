#ifndef BINDINGDATAMODEL_H
#define BINDINGDATAMODEL_H

#include <Qt>
#include <QString>
#include <QVariant>
#include <QModelIndex>
#include <QAbstractItemModel>

#include <abstractmetalang.h>

class ApiExtractor;
class PrimitiveTypeEntry;
class ContainerTypeEntry;
class BindingDataItem;

class BindingDataModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    BindingDataModel(ApiExtractor* dataSource = 0, QObject* parent = 0);
    ~BindingDataModel();

    QVariant data(const QModelIndex& index, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex& index) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;

private:
    void setupModelData();
    void setupModelData(const PrimitiveTypeEntry* metaClass, BindingDataItem* parent);
    void setupModelData(const ContainerTypeEntry* metaClass, BindingDataItem* parent);
    void setupModelData(AbstractMetaClass* metaClass, BindingDataItem* parent);
    void setupModelData(const AbstractMetaFunctionList& funcs, BindingDataItem* parent);
    void setupModelData(AbstractMetaFunction* metaFunc, BindingDataItem* parent);
    void setupModelData(AbstractMetaArgument* metaArg, BindingDataItem* parent);
    void setupModelData(AbstractMetaField* metaField, BindingDataItem* parent);
    void setupModelData(AbstractMetaEnum* metaEnum, BindingDataItem* parent);
    void setupModelData(AbstractMetaEnumValue* metaEnumValue, BindingDataItem* parent);

    QList<AbstractMetaFunctionList> getOverloads(const AbstractMetaClass* scope = 0);

    ApiExtractor* m_apiExtractor;
    BindingDataItem* m_rootItem;
    BindingDataItem* m_classesSeparator;
    BindingDataItem* m_containerTypesSeparator;
    BindingDataItem* m_primitiveTypesSeparator;
    BindingDataItem* m_globalEnumsSeparator;
    BindingDataItem* m_globalFunctionsSeparator;

    QString m_packageName;
};

#endif //BINDINGDATAMODEL_H

