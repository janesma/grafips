import QtQuick 2.0
import Grafips 1.0

Item {
    id: currentList
    property var model
    property MetricRouter publisher
    property ActiveMetrics activeMetrics
    anchors.top: parent.top
    anchors.bottom: parent.bottom

    ListView {
        anchors.fill: parent
        model: currentList.model
        id: metricList

        delegate: Rectangle {
            id: currentRect
            height: nameText.height * 1.5
            width: currentList.width
            property int theId: model.met_id
            
            color: model.enabled ? "green" : "red"
            Text {
                id: nameText
                text: name
            }
            Item {
                id: bogusDrag
                //color: "blue"
                height: 25
                width: 25
                
                property int theId: model.met_id
                property string name: model.name
                property bool dragActive: elementMouse.drag.active
                onDragActiveChanged: {
                    if (dragActive) {
                        print ("drag started");
                        Drag.start();
                        return;
                    }
                    print ("drag finished");
                    Drag.drop();
                    //anchors.centerIn = currentRect
                }
                //Drag.active: elementMouse.drag.active 
                //Drag.hotSpot.x: 32
                //Drag.hotSpot.y: 32
                //Drag.dragType: Drag.Automatic
            }
            MouseArea {
                id: elementMouse
                anchors.fill: parent
                drag.target: bogusDrag
                onReleased : {
                    console.log("drag release")
                    if (bogusDrag.Drag.target !== null) {
                        console.log("drag target known")
                        //var t = bogusDrag.Drag.target
                        //t.parent.publisher.Enable(met_id)
                    }
                }
            }            
        }
    }
}
