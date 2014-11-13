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

    qmlRegisterType<Grafips::CpuProvider>("Grafips", 1, 0, "CpuProvider");

    qmlRegisterType<Grafips::Publisher>();
    qmlRegisterType<Grafips::PublisherImpl>("Grafips", 1, 0, "PublisherImpl");

    qmlRegisterInterface<Grafips::Subscriber>("Subscriber");
    qmlRegisterType<Grafips::GraphSetSubscriber>("Grafips", 1, 0, "GraphSetSubscriber");

    qmlRegisterType<Grafips::QMetric>("Grafips", 1, 0, "QMetric");

    qmlRegisterType<Grafips::GraphView>("Grafips", 1, 0, "Renderer");
    
    QQmlApplicationEngine engine(QUrl("qrc:///qml/mainwin.qml"));
    return app.exec();
}
