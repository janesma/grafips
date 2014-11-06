import QtQuick 2.3
import QtQuick.Layouts 1.1
import Grafips 1.0

Item {
    id: currentGroup
    Layout.fillWidth:  true
    Layout.fillHeight: true
    Layout.minimumHeight:20
    Layout.preferredHeight: 400
    property string color
    property PublisherImpl publisher

    function refresh() {
        visible = false
        visible = true
    }

    function toggleShown()
    {
        if (Layout.maximumHeight != 20) {
            Layout.maximumHeight = 20
        } else {
            Layout.maximumHeight = Number.POSITIVE_INFINITY
        }
        refresh()
    }

    function resize(wheel) {
        var amount = wheel.angleDelta.y / 4
        if (Layout.preferredHeight + wheel.angleDelta.y < 20)
            Layout.preferredHeight = 20
        else
            Layout.preferredHeight += amount

        refresh()
    }

    GraphSetSubscriber {
        id: mySubscriber
    }

    Component.onCompleted: {
        publisher.Subscribe(mySubscriber);
    }

    Row {
        anchors.fill: parent

        Rectangle {
            color: "blue"
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 20
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    currentGroup.toggleShown()
                }
                onWheel: {
                    currentGroup.resize(wheel)
                }
            }
        }

        MetricList {
            model: mySubscriber.metrics
            publisher: currentGroup.publisher
        }

        Renderer {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 200
            subscriber: mySubscriber
        }
    }
}
