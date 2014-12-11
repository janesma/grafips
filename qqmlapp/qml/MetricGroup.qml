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
    property MetricRouter publisher
    property variant filters: [ "foo" ]

    function formatFloat(num) {
        if (num < 10) {
            return num.toPrecision(2);
        }
        return num.toFixed(0);
    }

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

    function start() {
        publisher.AddGraph(mySubscriber, filters);
    }

    GraphSetSubscriber {
        id: mySubscriber
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


        Rectangle {
            id: scale
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            width: axisText.width
            Text {
                id: axisText
                visible: false
                lineHeightMode: Text.FixedHeight
                lineHeight: scale.height / 5.0
                text: formatFloat(graphView.graphMax) + "\n"
                    + formatFloat(graphView.graphMax * 0.8) + "\n"
                    + formatFloat(graphView.graphMax * 0.6) + "\n"
                    + formatFloat(graphView.graphMax * 0.4) + "\n"
                    + formatFloat(graphView.graphMax * 0.2)
            }
        }

        Renderer {
            id: graphView
            visible: true
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: scale.left
            anchors.left: currentList.right
            subscriber: mySubscriber
            publisher: currentGroup.publisher
            onOnGraphMax : { axisText.visible = true; }
        }
    }
}
