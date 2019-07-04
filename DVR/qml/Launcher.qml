import QtQuick 2.0
import QtQuick.Controls 1.2
import "Controls"


Rectangle {
    id:mainItem

    signal launcher(string flag ,string titlename,string page)
    ListModel{
        id:model
        ListElement{
            flag:"qml"
            name:"录像查询"
            icon:"qrc:/images/video.jpg"
            page:"qrc:/qml/videoplay.qml"
        }
        ListElement{
            flag:"qml"
            name:"设置"
            icon:"qrc:/qml/setting/images/set.jpg"
            page:"qrc:/qml/setting/Setting1.qml"
        }
        ListElement{
            flag:"qml"
            name:"报警设置"
            icon:"qrc:/qml/setting/images/set.jpg"
            page:"qrc:/qml/Alarm/AlarmSet.qml"
        }
        ListElement{
            flag:"qml"
            name:"状态管理"
            icon:"qrc:/qml/setting/images/settings.png"
            page:"qrc:/qml/State/StateManage.qml"
        }

        ListElement{
            flag:"qml"
            name:"test"
            icon:"qrc:/qml/setting/images/settings.png"
            page:"qrc:/qml/AlarmTest.qml"
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
                imageheight: 90
                imagewidth: 90

                onClicked: {
                    if(grid.currentItem){
                        launcher(flag,text,Qt.resolvedUrl(page))
                    }
                }
            }

            Text{
                id:winname
                text: name
                font.pixelSize: 16
//                color:"white"
                anchors.horizontalCenter: mavibut.horizontalCenter
                anchors.top: mavibut.bottom
            }
        }
    }

}
