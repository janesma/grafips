#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "cpu_provider.h"
#include "publisher.h"
#include "subscriber.h"
#include "graph_view.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<CpuProvider>("Grafips", 1, 0, "CpuProvider");

    qmlRegisterType<Publisher>();
    qmlRegisterType<PublisherImpl>("Grafips", 1, 0, "PublisherImpl");

    qmlRegisterInterface<Subscriber>("Subscriber");
    qmlRegisterType<GraphSetSubscriber>("Grafips", 1, 0, "GraphSetSubscriber");

    qmlRegisterType<QMetric>("Grafips", 1, 0, "QMetric");

    qmlRegisterType<GraphView>("Grafips", 1, 0, "Renderer");
    
    QQmlApplicationEngine engine(QUrl("qrc:///qml/mainwin.qml"));
    return app.exec();
}
