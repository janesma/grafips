#include <QGuiApplication>
#include <QtQuick/QQuickView>

#include "gripsrenderer.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<GripsRenderer>("GripsRendering", 1, 0, "Renderer");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///qml/HelloWorld.qml"));
    view.show();

    return app.exec();
}
