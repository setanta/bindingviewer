#ifndef BINDINGVIEWER_H
#define BINDINGVIEWER_H

#include <QObject>
#include <QModelIndex>

class QWidget;
class QTreeView;
class BindingDataModel;

class BindingViewer : public QObject
{
    Q_OBJECT
public:
    BindingViewer();
    ~BindingViewer();

    void show();

private slots:
    void on_actionImport_activated();
    void on_actionQuit_activated();
    void on_bindingDataView_activated(const QModelIndex&);

private:
    BindingDataModel* createDataModel();

    QWidget* ui;
    QTreeView* bindingDataView;
};

#endif //BINDINGVIEWER_H

