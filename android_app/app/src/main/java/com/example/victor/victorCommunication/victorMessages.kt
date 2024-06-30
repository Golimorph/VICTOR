package com.example.victor.victorCommunication

/*This data class is a message that can be sent to victor. It tells victor to start moving his
tracks leftSpeed and rightSpeed. The toString of this dataClass is sent do victor and victor has
a parser and knows how to deconstruct and act on it.
 */
data class MoveTracksMessage(var leftTrackSpeed: Int, var rightTrackSpeed: Int) {}

/* Message to move the robot arm to specified coordinates [mm] */
data class MoveArmMessage(var xcm: Int, var xmm: Int, var ycm: Int, var ymm: Int, var zcm: Int, var zmm: Int) {}


/* Message to open or close the claw 1 closed, 0 open */
data class MoveClawMessage(var state: Int) {}