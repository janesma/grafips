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

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: enabRect
            color: "blue"
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
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
            id: currentList
            anchors.left: enabRect.right
            width: 200
            model: mySubscriber.metrics
            publisher: currentGroup.publisher
        }

        Renderer {
            visible: true
            Layout.preferredWidth: Number.POSITIVE_INFINITY
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: currentList.right
            subscriber: mySubscriber
        }
    }
}
