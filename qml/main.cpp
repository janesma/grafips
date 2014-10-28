#include <assert.h>

#include <QGuiApplication>
#include <QtQuick/QQuickView>
#include <QQmlContext>

#include "gripsrenderer.h"
#include "graph_view.h"
#include "publisher.h"
#include "cpu_provider.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<GripsRenderer>("GrafipsRendering", 1, 0, "Renderer");
    qmlRegisterType<QMetric>("GrafipsRendering", 1, 0, "QMetric");
//qmlRegisterType<GraphView>("GrafipsRendering", 1, 0, "Renderer");
    GraphView gv;

    PublisherImpl pub;
    std::vector<MetricDescription> desc;
    CpuProvider cpu(&pub);


    QQuickView view;
    view.rootContext()->setContextProperty("renderer", &gv);

    qmlRegisterType<GraphSetSubscriber>("GrafipsRendering", 1, 0, "MetricModel");
    GraphSetSubscriber sub;
    view.rootContext()->setContextProperty("subscriber", &sub);

    pub.Subscribe(&sub);

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///qml/HelloWorld.qml"));

    view.show();

    // todo: get access to GraphViewRenderer

    // QObject *object = view.rootObject();
    // assert(object != NULL);
    // QObject *rowLayout = object->findChild<QObject*>("row");
    // assert(rowLayout != NULL);
    // QObject *graph = rowLayout->findChild<QObject*>("mygraph");
    // assert(graph != NULL);
    // QObject *renderer = graph->findChild<QObject*>("renderer");
    // assert(renderer != NULL);

    // gv.AddPublisher(&pub);

    //pub.GetDescriptions(&desc);

    //gv.AddMetric(desc[0].id());

    // pub.Enable(desc[0].id());

//std::thread t(&CpuProvider::Run, &cpu);

    return app.exec();
}
