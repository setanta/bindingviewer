#include <QApplication>
#include "bindingviewer.h"

int
main(int argc, char* argv[])
{
    Q_INIT_RESOURCE(bindingviewer);

    QApplication app(argc, argv);

    BindingViewer bindingViewer;
    bindingViewer.show();

    return app.exec();
}

