import QtQuick
import QtQuick.Controls


import com.company.PlayerController
import com.company.DoorState
//import com.company.TempIndicator
import com.company.BatteryIndicator
Window {
  id: root

  width: 640
  height: 480

  visible: true
  Rectangle {
    id: topbar

    anchors {
      top: parent.top
      left: parent.left
      right: parent.right
    }

    height: 70
    color: "#5F8575"
        Row {
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 10
            }
            spacing: 10

            Image {
                id: doorstate
                source: DoorState.door ? "assets/icons/pause_icon.png" : "assets/icons/play_icon.png"
                width: 60
                height: 60
            }

            Image {
                id: batterystate
                source: BatteryIndicator.batValue ? "assets/icons/pause_icon.png" : "assets/icons/play_icon.png"
                width: 60
                height: 60
            }

              Image {
               id: tempState
                  source: TempIndicator.tempValue ? "assets/icons/pause_icon.png" : "assets/icons/play_icon.png"
                  width: 60
                  height: 60
           }
        }


  }
  title: qsTr("Song Player")




  Rectangle {
    id: mainSection

    anchors {
      top: topbar.bottom
      bottom: bottombar.top
      left: parent.left
      right: parent.right
    }

    color: "#1e1e1e"

    AudioInfoBox {
      id: firstSong

      anchors {
        verticalCenter: parent.verticalCenter
        left: parent.left
        right: parent.right
        margins: 20
      }

      infoProvider {
             songIndex: 0
             title: "Eine Kleine Nachtmusik"
             authorName: "Wolfgang Amadeus Mozart"
             imageSource: "assets/images/song_1.jpg"
             audioSource: "qrc:/userInterface/assets/audio/eine_kleine_nachtmusik.mp3"
           } }

    AudioInfoBox {
      id: secondSong

      anchors {
        verticalCenter: parent.verticalCenter
        left: parent.left
        right: parent.right
        margins: 20
      }

      infoProvider {
             songIndex: 1
             title: "Symphony No. 5"
             authorName: "Ludwig Van Beethoven"
             imageSource: "assets/images/song_2.jpg"
             audioSource: "qrc:/userInterface/assets/audio/symphony_no_5.mp3"
           } }

    AudioInfoBox {
      id: thirdSong

      anchors {
        verticalCenter: parent.verticalCenter
        left: parent.left
        right: parent.right
        margins: 20
      }

      infoProvider {
              songIndex: 2
              title: "Air on the G String"
              authorName: "Johann Sebastian Bach"
              imageSource: "assets/images/song_3.jpg"
              videoSource: "qrc:/userInterface/assets/videos/video_1.avi"
              audioSource: "qrc:/userInterface/assets/audio/air_on_the_g_string.mp3"
            }  }
  }

  Rectangle {
    id: bottombar

    anchors {
      bottom: parent.bottom
      left: parent.left
      right: parent.right
    }

    height: 100
    color: "#333333"

    Row {
      anchors.centerIn: parent

      spacing: 20

      ImageButton {
        id: previousButton

        width: 64
        height: 64

        source: "assets/icons/previous_icon.png"

        onClicked: PlayerController.switchToPreviousSong()
      }

      ImageButton {
        id: playPauseButton

        width: 64
        height: 64

        source: PlayerController.playing ? "assets/icons/pause_icon.png" : "assets/icons/play_icon.png"

        onClicked: PlayerController.playPause()
      }

      ImageButton {
        id: nextButton

        width: 64
        height: 64

        source: "assets/icons/next_icon.png"

        onClicked: PlayerController.switchToNextSong()
      }
    }
  }
}
