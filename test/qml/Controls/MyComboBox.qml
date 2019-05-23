import QtQuick 2.0

FocusScope {
    id: container
    width:200
    height:40
    signal clicked
    property alias model:listView.model
    property variant listModel
//    property alias currentIndex: delegate.index
//    property alias source: image.source
    property alias text: label.text
//    property alias width:buttonRectangle.width
//    property alias hegiht:buttonRectangle.hegiht
    property color textColor: "black"
    property color borderColor: "limegreen"
    property color backgroundColor: "transparent"
    property int borderWidth: 2
    property int textSize: 24
    property int radius: 5
    property int margin: 0
    property int currentIndex:0
    Rectangle {
        id: buttonRectangle
        anchors.fill: parent
        color: container.backgroundColor
        radius: container.radius
        border{width: container.borderWidth; color: container.borderColor;}
        Image {
            id: image
            smooth: true
            source:"images/bullet_dn.png"
            fillMode: Image.PreserveAspectFit
            anchors{top: parent.top; right: parent.right; margins: container.margin;}
            width: parent.height/3; height: parent.height-2*container.margin }
        Item {
            anchors{top: parent.top; bottom: parent.bottom; left: parent.left; right: image.left; leftMargin: container.margin * 2;}
            Text {
                id: label
                text: model.get(currentIndex).name //typeof(listModel) == "undefined"? "":typeof( listModel.get(currentIndex).attributes)=="undefined"?listModel.get(comboBox.currentIndex).name:listModel.get(currentIndex).attributes.get(currentIndex2).description
                color: container.textColor
                anchors.verticalCenter: parent.verticalCenter
                font{pixelSize: container.textSize-5; bold: true;}
            }
        }
        MouseArea {
            id: mouseArea; anchors.fill: parent
            onClicked: {
                listBackground.visible = !listBackground.visible
                console.log("currentIndex"+currentIndex)
//                buttonRectangle.state = "pressed"
//                container.clicked()
            }
        }

//        states: State {
//            name: "pressed"
//            PropertyChanges { target: image; scale: 0.7 }
//        }
//        transitions: Transition {
//            NumberAnimation { properties: "scale"; duration: 200; easing.type: Easing.InOutQuad }
//        }
    }

    Component {
        id: delegate
        Item {
            width: parent.width; height: comboBoxButton.height;
            Text {
                id:comtext
                color:textColor
                anchors.verticalCenter: parent.verticalCenter
                font{pixelSize: container.textSize-5; bold: true;}
                text:name;
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled:true

                onClicked: {
                    console.log(listView.model.get(0).name)
                    console.log(index)
                    currentIndex = index
//                    label.text = comtext.text
                    listBackground.visible = false
                }
//                onExited: listBackground.visible = false
            }
//            onFocusChanged: {
//                delegatelistview.visible=true;
//            }
//            Keys.onPressed: {
//                if(event.key===Qt.Key_Enter||event.key===Qt.Key_Return) {
//                    comboBox.comboxClick();
//                }
//            }
        }
    }

    Rectangle {
        id: listBackground
        visible:false
        anchors{top: buttonRectangle.bottom; left: buttonRectangle.left;}
        width: parent.width
        height:listView.count > 3 ?parent.height*3:parent.height*listView.count
//        color: "#FF000000"
        radius: buttonRectangle.radius
        border{width: container.borderWidth; color: container.borderColor}
        ListView {
            id: listView
            anchors.fill: parent
            clip: true
            focus:true
            model:listModel
            delegate: delegate
//            highlight: Rectangle{color:"#FF00688A";
//                radius: 5;
//            }
            onCurrentIndexChanged: {
                console.log(currentIndex)

//                delegatelistview.model=listModel.get(currentIndex).attributes;
            }
        }

//        MouseArea {
//            anchors.fill: parent
////            hoverEnabled:true

////            onExited: listBackground.visible = false
//            onClicked:{
//                console.log("count "+listView.count)
//                console.log("currentIndex "+listView.currentIndex)
//            }

//        }
        Behavior on height { NumberAnimation {property: "height"; duration: 200 }
        }
    }
}

