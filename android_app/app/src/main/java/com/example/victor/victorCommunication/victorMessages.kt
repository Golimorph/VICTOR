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

/* Change the angle of the claw. The data is a vector x,y,z of length 125 (normalize needed as Int is not so good) */
data class MoveClawAngleMessage(val x: Int, val y: Int, val z: Int) {}

/* Change the angle of the camera.*/
data class MoveCameraMessage(val xangle: Int, val yangle: Int) {}