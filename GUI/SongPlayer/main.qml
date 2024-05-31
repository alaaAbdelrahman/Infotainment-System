import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.company.DoorState
//import com.company.TempIndicator
import com.company.BatteryIndicator

Window {
    id:root
    width: 1920
    height: 1080
    visible: true
    title: qsTr("Eagle Theme")

    property bool doorOpen: true



    Item {
        id:main
        anchors.fill: parent
        Image{
            anchors.fill: parent
            source: "assets/images/background.png"
            Item {
                anchors.centerIn: parent
                Row{
                    anchors.centerIn: parent
                    spacing: 800
                    Image {
                        source:DoorState.door ? "assets/images/opened.png":"assets/images/car.png"
                        height: 300
                        width: 300
                    }
                    Image {
                        source: "assets/images/NEW.png"
                        height: 400
                        width: 400

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                Qt.createComponent("mediaPlayer.qml").createObject(main);
                            }
                        }
                    }
                }
            }
        }
}
    Item{
        id : top
        height : root.height * 0.09
        anchors{
            top: parent.top
            left: parent.left
            right: parent.right
        }
        Image{
            source: "assets/images/top_dark.png"
            anchors.fill: parent

            Row {
                anchors.centerIn: parent
                spacing : 1200

                Text {
                    id: currentDate
                    font.pixelSize: 20
                    color: "white"
                    text: {
                        var now = new Date();
                        return Qt.formatDate(now, "yyyy-MM-dd");
                    }
                }

                Text {
                    id: currentTime
                    font.pixelSize: 20
                    color: "white"
                    text: {
                        var now = new Date();
                        return Qt.formatTime(now, "hh:mm:ss");
                    }
                }
            }
        }
    }

    Item{
        id : bottom
        height : root.height * 0.09
        anchors{
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
            left: parent.left
            right: parent.right
        }
        Image{
            source: "assets/images/bottom_dark.png"
            anchors.fill: parent
        }
    }
}
