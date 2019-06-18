import QtQuick 2.0
import QtQuick.Controls 1.2
import "../../Controls"

Item {
    property int fontpixelSize : 32
    property int boxwidth: 300
    signal pressed(int value)


    TabView{
        id: tabView
        focus: true

        anchors.fill: parent

        Tab{
            title: "Normal"
            Item{

                Text{
                    id:recordalarm
                    width: boxwidth-100
                    anchors.top: parent.top
                    anchors.topMargin: 50
                    anchors.left: parent.left
                    anchors.leftMargin: 100
                    text:qsTr("异常类型")
                    font.pixelSize: fontpixelSize
                }

                ComboBox{
                    id:recordalarmBox
                    width: boxwidth
                    anchors.left: recordalarm.right
                    anchors.leftMargin: 100
                    anchors.top: recordalarm.top
                    height:recordalarm.height
                    model: ["硬盘满","硬盘读写错误","网络异常","非法访问",
                        "视频信号异常"]
                }

                MyCheckBox{
                    id:alarmout1
                    width: boxwidth-100
                    anchors.top: recordalarm.bottom
                    anchors.topMargin: 20
                    anchors.left: recordalarm.left
                    pixSize:fontpixelSize
                    str:qsTr("声音报警")
                }

                MyCheckBox{
                    id:alarmout2
                    width: boxwidth-100
                    anchors.left: alarmout1.right
                    anchors.leftMargin: 150
                    anchors.verticalCenter: alarmout1.verticalCenter
                    pixSize:fontpixelSize
                    str:qsTr("报警输出")
                }



            }
        }
    }

    Connections{
        target: alarmsetroot
        onSuresignal:{
//            pressed(tabView.currentIndex)
            console.log("exception set")
        }
        onCancelsignal:{

            console.log("cancel press")
        }
    }

}
