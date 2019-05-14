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

//        anchors.fill: parent

        cellHeight: 120;cellWidth: 120
        model: model
        delegate: mydelegate

//        highlight: Rectangle{color: "lightsteelblue";radius: 5}
//        focus: true
    }

    Component{
        id:mydelegate

        Rectangle{
            width: grid.cellWidth
            height: grid.cellHeight


//            Column{
//            AbstractButton{
//                anchors.fill: parent
                NaviButton {
                    id:mavibut
                    text: name
                    enabled: true
                    imageSource: icon
                    anchors.fill: parent
                    imageheight: grid.cellHeight - 20
                    imagewidth: grid.cellWidth - 20

                    onClicked: {
                        console.log("imageheight=%d",imageheight)
                        if(grid.currentItem){
                            console.log(Qt.resolvedUrl(page));
                            launcher(text,Qt.resolvedUrl(page))
                        }
                    }
                }

//                Image{
//                    source: icon
//                    anchors.horizontalCenter: parent.horizontalCenter
//                    width: grid.cellWidth
//                    height: grid.cellHeight
////                    sourceSize.width: parent.availableWidth
////                    sourceSize.height: parent.availableHeight

//                }
//                onClicked: console.log("click")
//            }
                Text{
                    id:winname
                    text: name
                    anchors.horizontalCenter: mavibut.horizontalCenter
                    anchors.top: mavibut.bottom
//                    anchors.horizontalCenter: parent.horizontalCenter
//                    anchors.bottom: parent.bottom
                }
            }
//        }
    }

}
