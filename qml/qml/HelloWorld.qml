import QtQuick 2.3
import QtQuick.Layouts 1.1

Item {
    width: 800
    height: 400

    RowLayout {
        spacing: 5
        anchors.fill: parent
        id: row

        MetricList
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 100
            Layout.preferredWidth: 150
            Layout.maximumWidth: 300
            Layout.minimumHeight: 150
        }
        Graph {
            id: mygraph
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 200
            Layout.preferredWidth: 700
            Layout.minimumHeight: 150
        }

        // property int animatedValue: 0
        // SequentialAnimation on animatedValue {
        //     loops: Animation.Infinite
        //     PropertyAnimation { to: 150; duration: 1000 }
        //     PropertyAnimation { to: 0; duration: 1000 }
        // }

        // Text {
        //     anchors.centerIn: parent
        //     text: animatedValue
        // }
        // MouseArea {
        //     anchors.fill: parent
        //     onClicked: parent.color = "blue"
        // }
        // focus: true
        // Keys.onPressed: {
        //     if (event.key == Qt.Key_Return) {
        //         color = "blue";
        //         event.accepted = true;
        //     }
        //     else {
        //         color = "red";
        //         event.accepted = true;
        //     }
        // }

        // Rectangle {
        //     width: parent.width / 2
        //     height: parent.height/4
        //     color: "orange"
        //     property color nextColor: "blue"
        // }

        // Rectangle {
        //     width: parent.width / 2
        //     height: parent.height/4
        //     x: parent.width / 2
        //     color: "green"
        // }

        // Column {
        //     width: 200; height: 200; y:parent.height*3/4

        //     TextInput { id: myTextInput; text: "Hello World" }

        //     Text { text: myTextInput.text }
        // }

    }
}
