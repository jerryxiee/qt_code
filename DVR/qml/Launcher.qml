import QtQuick 2.0
import QtQuick.Controls 1.2

Rectangle {
    id:mainItem

    signal launcher(string titlename,string page)
    ListModel{
        id:model
        ListElement{
            name:"video"
            icon:"qrc:/images/video.jpg"
            page:"qrc:/qml/video.qml"
        }
        ListElement{
            name:"widgetvideo1"
            icon:"qrc:/images/video.jpg"
            page:""
        }
//        ListElement{name:"video1";icon:"qrc:images/1.png"}
    }

    GridView{
        id:grid
        x:100;y:100
        width: parent.width -200
        height: parent.height - 200
        anchors.margins: 20
        anchors.bottomMargin: 80
        anchors.rightMargin: 20
        anchors.leftMargin: 20

//        anchors.fill: parent

        cellHeight: 150;cellWidth: 150
        model: model
        delegate: mydelegate

        highlight: Rectangle{color: "lightsteelblue";radius: 5}
        focus: true
    }

    Component{
        id:mydelegate

        Rectangle{
            width: 100
            height: 100

            NaviButton {
                id:mavibut
                text: name
                enabled: true
                imageSource: icon
                anchors.fill: parent
                imageheight: 80
                imagewidth: 80

                onClicked: {
                    if(grid.currentItem){
                        console.log(Qt.resolvedUrl(page));
                        launcher(text,Qt.resolvedUrl(page))
                    }
                }
            }

            Text{
                id:winname
                text: name
                font.pixelSize: 16
                anchors.horizontalCenter: mavibut.horizontalCenter
                anchors.bottom: mavibut.bottom
            }
        }
    }

}
