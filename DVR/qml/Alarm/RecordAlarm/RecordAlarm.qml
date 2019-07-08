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
                    text:qsTr("通道选择")
                    font.pixelSize: fontpixelSize
                }

                ComboBox{
                    id:recordalarmBox
                    width: boxwidth
                    anchors.left: recordalarm.right
                    anchors.leftMargin: 100
                    anchors.top: recordalarm.top
                    height:recordalarm.height
                    model: ["模拟通道0","模拟通道1","模拟通道2","模拟通道3",
                        "模拟通道4","模拟通道5","模拟通道6","模拟通道7"]
                }

                Text{
                    id:alarmtype
                    width: boxwidth-100
                    anchors.top: recordalarm.bottom
                    anchors.topMargin: 20
                    anchors.left: recordalarm.left
                    text:qsTr("报警类型")
                    font.pixelSize: fontpixelSize
                }

                ComboBox{
                    id:alarmtypeBox
                    width: boxwidth
                    anchors.left: alarmtype.right
                    anchors.leftMargin: 100
                    anchors.top: alarmtype.top
                    height:alarmtype.height
                    model: ["区域遮挡","移动侦测"]
                }

                Text{
                    id:rectposition
                    width: boxwidth-100
                    anchors.top: alarmtype.bottom
                    anchors.topMargin: 20
                    anchors.left: alarmtype.left
            //        anchors.leftMargin: 100
                    text:qsTr("区域位置")
                    font.pixelSize: fontpixelSize
                }

                Button{
                    id:rectpositionBox
                    width: boxwidth
                    anchors.left: rectposition.right
                    anchors.leftMargin: 100
                    anchors.top: rectposition.top
                    height:rectposition.height
                    text:qsTr("位置设置")
                    onClicked: regionSetSignal(recordalarmBox.currentIndex,"region")

                }

                MyCheckBox{
                    id:alarmout1
                    width: boxwidth-100
                    anchors.top: rectposition.bottom
                    anchors.topMargin: 20
                    anchors.left: rectposition.left
                    pixSize:fontpixelSize
                    str:qsTr("开启报警")
                    checked: AlarmConfig.getOpenRecord(recordalarmBox.currentIndex)
                }

                MyCheckBox{
                    id:alarmout2
                    width: boxwidth-100
                    anchors.left: alarmout1.right
                    anchors.leftMargin: 150
                    anchors.verticalCenter: alarmout1.verticalCenter
                    pixSize:fontpixelSize
                    str:qsTr("声音报警")
                    checked: AlarmConfig.getRecordSoundEnable(recordalarmBox.currentIndex)
                }

                MyCheckBox{
                    id:alarmout3
                    width: boxwidth-100
                    anchors.top: alarmout1.bottom
                    anchors.topMargin: 20
                    anchors.left: alarmout1.left
                    pixSize:fontpixelSize
                    str:qsTr("显示报警画面")
                    checked: AlarmConfig.getRecordDispEnable(recordalarmBox.currentIndex)
                }

                MyCheckBox{
                    id:alarmout4
                    width: boxwidth-100
                    anchors.left: alarmout3.right
                    anchors.leftMargin: 150
                    anchors.verticalCenter: alarmout3.verticalCenter
                    pixSize:fontpixelSize
                    str:qsTr("上报报警信息")
                    checked: AlarmConfig.getRecordReportEnable(recordalarmBox.currentIndex)
                }

                Connections{
                    target: alarmsetroot
                    onSuresignal:{
            //            pressed(tabView.currentIndex)
                        console.log("recordalarm set")
                        AlarmConfig.setRecordAlarm(recordalarmBox.currentIndex,alarmout1.checked,alarmout3.checked,
                                                   alarmout2.checked,alarmout4.checked)
                    }
                    onCancelsignal:{

                        console.log("cancel press")
                    }
                }

            }
        }
    }


}
