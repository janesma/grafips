#include <assert.h>

#include <QGuiApplication>
#include <QtQuick/QQuickView>
#include <QQmlContext>

#include "gripsrenderer.h"
#include "graph_view.h"
#include "publisher.h"
#include "cpu_provider.h"

using namespace Grafips;

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<QMetric>("GrafipsRendering", 1, 0, "QMetric");
    qmlRegisterType<GraphView>("GrafipsRendering", 1, 0, "Renderer");

    PublisherImpl pub;
    std::vector<MetricDescription> desc;
    CpuProvider cpu;
    cpu.setPublisher(&pub);


    QQuickView view;

    qmlRegisterType<GraphSetSubscriber>("GrafipsRendering", 1, 0, "MetricModel");
    GraphSetSubscriber sub;
    view.rootContext()->setContextProperty("my_subscriber", &sub);

    pub.Subscribe(&sub);

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///qml/HelloWorld.qml"));

    view.show();

    pub.GetDescriptions(&desc);

    pub.Enable(desc[0].id());

    std::thread t(&CpuProvider::Run, &cpu);

    return app.exec();
}
