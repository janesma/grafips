import QtQuick 2.3
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import GrafipsRendering 1.0

Item {
    width: 800
    height: 400

    SplitView {
        anchors.fill: parent

        MetricList
        {
            Layout.minimumWidth: 100
            width: 200
        }
        Renderer
        {
            subscriber: my_subscriber
            Layout.fillWidth: true
            Layout.minimumWidth: 200
            Layout.preferredWidth: 700
        }
    }
}
