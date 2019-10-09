import QtQuick 2.0
import QtQuick.Controls 1.2

Item {
    id:bubiaoregister
    signal pressed(int setindex)


    TabView{
        id: tabView
        focus: true

        anchors.fill: parent

        Tab{
            title: "注册配置"
            RegisterConfig{
                index:0

            }
        }

        Tab{
            title: "终端配置"
            DeviceConfig{
                index:1
            }
        }
        Tab{
            title: "运行管理"
            RunManage{
                index:2
            }
        }
    }

    Connections{
        target: platformroot
        onSuresignal:{
            pressed(tabView.currentIndex)
            console.log("sure press")
        }
        onCancelsignal:{

            console.log("cancel press")
        }
    }

//    onDestroyed: console.log("chnset destroyed")

}
