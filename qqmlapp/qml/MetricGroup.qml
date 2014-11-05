import QtQuick 2.3
import QtQuick.Layouts 1.1
import Grafips 1.0

Item {
    id: foo
    Layout.fillWidth:  true
    Layout.fillHeight: true
    Layout.minimumHeight:20
    Layout.preferredHeight: 100
    property string color
    property PublisherImpl publisher

    GraphSetSubscriber {
        id: mySubscriber
    }

    Component.onCompleted: {
        publisher.Subscribe(mySubscriber);
    }

    Row {
        anchors.fill: parent
        MetricList {
            model: mySubscriber.metrics
        }
        
        // Rectangle {
        //     id: bigRect
        //     anchors.fill: parent
        //     color: foo.color

        // }
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            if (foo.Layout.maximumHeight != 20) {
                foo.Layout.maximumHeight = 20
            } else {
                foo.Layout.maximumHeight = Number.POSITIVE_INFINITY
            }
            foo.visible = false
            foo.visible = true
        }
        onWheel: {
            var amount = wheel.angleDelta.y / 4
            //console.log("h: " + foo.Layout.preferredHeight + " y: " + amount)
            if (foo.Layout.preferredHeight + wheel.angleDelta.y < 20)
                foo.Layout.preferredHeight = 20
            else
                foo.Layout.preferredHeight += amount
            foo.visible = false
            foo.visible = true                    
        }
    }
}
