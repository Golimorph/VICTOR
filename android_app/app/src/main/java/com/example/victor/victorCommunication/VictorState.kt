package com.example.victor.victorCommunication
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import socketClient.VictorClient
import android.os.Handler
import android.os.Looper
import kotlin.math.sqrt


/*This class tracks the current state of victor and sends a message to victor wheneve the state is updated.*/
class VictorState {

    private val _victorClient = VictorClient()
    private var _armStateChanged = false
    private var _clawStateChanged = false
    private var _clawAngleStateChanged = false
    private var _clawState = false
    private var _armX: Int = 0
    private var _armY: Int = 190
    private var _armZ: Int = 150
    private var _clawX: Int = 0
    private var _clawY: Int = 125
    private var _clawZ: Int = 0
    private var _leftTrackSpeed: Int = 0
    private var _rightTrackSpeed: Int = 0

    /*This method starts a thread in the scope of this class, which will take the current state of arm and claw and send it to
    * victor every 10ms. For the tracks this is not needed as a moveTrack message can be proceesed quick enough by victor.*/
    fun initStateListener(){
        moveArmMessageHandler.post(moveArmMessageRunnable)
    }

    fun isConnected(): Boolean{
        return _victorClient.isConnected()
    }

    fun setClaw(state: Boolean){
        if( _clawState == state){
            return
        }
        _clawState = state
        _clawStateChanged = true
    }

    fun getClaw(): Boolean{
        return _clawState
    }

    fun setArmX(x: Int) {
        if( x == _armX ){
            return
        }
        _armX = x
        _armStateChanged = true
    }

    fun setArmY(y: Int) {
        if( y == _armY ){
            return
        }
        _armY = y
        _armStateChanged = true
    }

    fun setArmZ(z: Int) {
        if( z == _armZ ){
            return
        }
        _armZ = z
        _armStateChanged = true
    }

    fun getArmX(): Int{
        return _armX
    }

    fun getArmY(): Int{
        return _armY
    }

    fun getArmZ(): Int{
        return _armZ
    }

    fun getClawNorm(): Double {
        return sqrt((_clawX*_clawX+_clawY*_clawY+_clawZ*_clawZ).toDouble())
    }

    fun setClawX(x: Int) {
        if( x == _clawX ){
            return
        }
        _clawX = x
        _clawY = (125*_clawY.toDouble()/ getClawNorm() + 0.5).toInt()
        _clawZ = (125*_clawZ.toDouble()/ getClawNorm() + 0.5).toInt()

        _clawAngleStateChanged = true
    }

    fun setClawY(y: Int) {
        if( y == _clawY ){
            return
        }
        _clawY = y
        _clawX = (125*_clawX.toDouble()/ getClawNorm() + 0.5).toInt()
        _clawZ = (125*_clawZ.toDouble()/ getClawNorm() + 0.5).toInt()
        _clawAngleStateChanged = true
    }

    fun setClawZ(z: Int) {
        if( z == _clawZ ){
            return
        }
        _clawZ = z
        _clawX = (125*_clawX.toDouble()/ getClawNorm() + 0.5).toInt()
        _clawY = (125*_clawY.toDouble()/ getClawNorm() + 0.5).toInt()
        _clawAngleStateChanged = true
    }

    fun getClawX(): Int {
        return _clawX
    }

    fun getClawY(): Int {
        return _clawY
    }

    fun getClawZ(): Int {
        return _clawZ
    }

    fun setTrackSpeed(leftTrackSpeed: Int, rightTrackSpeed: Int){
        if(leftTrackSpeed == _leftTrackSpeed && rightTrackSpeed == _rightTrackSpeed) {
            return
        }
        _leftTrackSpeed = leftTrackSpeed
        _rightTrackSpeed = rightTrackSpeed

        val message = MoveTracksMessage(leftTrackSpeed, rightTrackSpeed)
        CoroutineScope(Dispatchers.Main).launch {
            _victorClient.send(message.toString())
        }
    }


    private val moveArmMessageHandler = Handler(Looper.getMainLooper())
    private val moveArmMessageRunnable = object : Runnable {

        override fun run() {
            if(_armStateChanged){
                CoroutineScope(Dispatchers.Main).launch {
                    val moveArmMessage = MoveArmMessage(_armX/10,_armX%10,_armY/10,_armY%10,_armZ/10,_armZ%10)
                    _victorClient.send(moveArmMessage.toString())
                }
                _armStateChanged = false
            }
            if(_clawStateChanged){
                CoroutineScope(Dispatchers.Main).launch {
                    if(_clawState){
                        _victorClient.send(MoveClawMessage(1).toString())
                    }else{
                        _victorClient.send(MoveClawMessage(0).toString())
                    }
                }
                _clawStateChanged = false
            }

            if(_clawAngleStateChanged){
                CoroutineScope(Dispatchers.Main).launch {
                    _victorClient.send(MoveClawAngleMessage(_clawX,_clawY,_clawZ).toString())
                }
                _clawAngleStateChanged = false
            }
            moveArmMessageHandler.postDelayed(this, 10)
        }
    }
}



