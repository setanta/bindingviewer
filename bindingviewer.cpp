#include <QFile>
#include <QWidget>
#include <QTreeView>
#include <QUiLoader>

#include <apiextractor.h>

#include "bindingviewer.h"
#include "bindingdatamodel.h"

#include <QDebug>

BindingViewer::BindingViewer()
{
    QFile file(":/bindingviewer.ui");
    file.open(QFile::ReadOnly);

    QUiLoader loader;
    ui = loader.load(&file);
    file.close();

    bindingDataView = ui->findChild<QTreeView*>("bindingDataView");
    //connect(bindingDataModel, SIGNAL(activated()), this, SLOT(on_bindingDataView_activated(const QModelIndex&)));

    bindingDataView->setModel(createDataModel());
}

BindingViewer::~BindingViewer()
{
}

void
BindingViewer::show()
{
    ui->show();
}

BindingDataModel*
BindingViewer::createDataModel()
{
    ApiExtractor* extractor = new ApiExtractor;
    //extractor->setSilent(true);

    // Hardcoded trash.
    extractor->addIncludePath(QStringList("/home/setanta/garden/pythonruntime/shiboken/tests/libsample"));
    extractor->addTypesystemSearchPath(QStringList("/home/setanta/garden/pythonruntime/shiboken/tests/samplebinding"));
    extractor->setCppFileName("/home/setanta/garden/pythonruntime/shiboken/tests/samplebinding/global.h");
    extractor->setTypeSystem("/home/setanta/garden/pythonruntime/shiboken/tests/samplebinding/typesystem_sample.xml");

    bool ok = extractor->run();

    return new BindingDataModel(ok ? extractor : 0);
}

void
BindingViewer::on_actionImport_activated()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void
BindingViewer::on_actionQuit_activated()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void
BindingViewer::on_bindingDataView_activated(const QModelIndex& index)
{
    qDebug() << __PRETTY_FUNCTION__;
}

