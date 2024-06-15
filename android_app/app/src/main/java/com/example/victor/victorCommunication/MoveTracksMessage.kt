package com.example.victor.victorCommunication

/*This data class is a message that can be sent to victor. It tells victor to start moving his
tracks leftSpeed and rightSpeed. The toString of this dataClass is sent do victor and victor has
a parser and knows how to deconstruct and act on it.
 */
data class MoveTracksMessage(val leftTrackSpeed: Int, val rightTrackSpeed: Int) {

}
