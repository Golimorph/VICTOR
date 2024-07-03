package com.example.victor

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.navigation.fragment.findNavController
import android.widget.SeekBar
import com.example.victor.databinding.FragmentHomeBinding
import socketClient.VictorClient
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.launch
import kotlinx.coroutines.Dispatchers
import androidx.lifecycle.ViewModelProvider
import com.example.victor.victorCommunication.*
import android.os.Handler
import android.os.Looper



/**
 * A simple [Fragment] subclass as the second destination in the navigation.
 */
class homeFragment : Fragment(), JoystickView.JoystickListener {

    private var _binding: FragmentHomeBinding? = null
    private lateinit var _sharedViewModel: SharedViewModel
    private lateinit var _victorClient: VictorClient

    //The initial state of victor
    var _moveArmMessage = MoveArmMessage(0,0,19,0,15,0)
    var _armStateUpdated = false
    var _clawState = false
    var _clawStateUpdated = false

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {

        connectVictorClient()
        _binding = FragmentHomeBinding.inflate(inflater, container, false)
        return binding.root

    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        binding.buttonSecond.setOnClickListener {
            findNavController().navigate(R.id.action_SecondFragment_to_FirstFragment)
        }

        binding.grabButton.setOnClickListener {
            CoroutineScope(Dispatchers.Main).launch {
                _clawState = !_clawState
                _clawStateUpdated = false
            }
        }

        //Initialize and define the coordinate limits for the arm movement.
        binding.editTextText.setText("x = ${_moveArmMessage.xcm*10+_moveArmMessage.xmm}, y = ${_moveArmMessage.ycm*10+_moveArmMessage.ymm}, z = ${_moveArmMessage.zcm*10+_moveArmMessage.zmm}")
        binding.seekBarX.min = -400
        binding.seekBarX.progress = _moveArmMessage.xcm*10+_moveArmMessage.xmm
        binding.seekBarX.max = 400
        binding.seekBarY.min = -200
        binding.seekBarY.progress = _moveArmMessage.ycm*10+_moveArmMessage.ymm
        binding.seekBarY.max = 400
        binding.seekBarZ.min = -200
        binding.seekBarZ.progress = _moveArmMessage.zcm*10+_moveArmMessage.zmm
        binding.seekBarZ.max = 400

        binding.seekBarX.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                _moveArmMessage.xcm = progress/10
                _moveArmMessage.xmm = progress%10
                _armStateUpdated = false
                binding.editTextText.setText("x = ${_moveArmMessage.xcm*10+_moveArmMessage.xmm}, y = ${_moveArmMessage.ycm*10+_moveArmMessage.ymm}, z = ${_moveArmMessage.zcm*10+_moveArmMessage.zmm}")
            }
            override fun onStartTrackingTouch(seekBar: SeekBar?) {}
            override fun onStopTrackingTouch(seekBar: SeekBar?) { }
        })

        binding.seekBarY.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                _moveArmMessage.ycm = progress/10
                _moveArmMessage.ymm = progress%10
                _armStateUpdated = false
                binding.editTextText.setText("x = ${_moveArmMessage.xcm*10+_moveArmMessage.xmm}, y = ${_moveArmMessage.ycm*10+_moveArmMessage.ymm}, z = ${_moveArmMessage.zcm*10+_moveArmMessage.zmm}")

            }
            override fun onStartTrackingTouch(seekBar: SeekBar?) {}
            override fun onStopTrackingTouch(seekBar: SeekBar?) { }
        })

        binding.seekBarZ.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                _moveArmMessage.zcm = progress/10
                _moveArmMessage.zmm = progress%10
                _armStateUpdated = false
                binding.editTextText.setText("x = ${_moveArmMessage.xcm*10+_moveArmMessage.xmm}, y = ${_moveArmMessage.ycm*10+_moveArmMessage.ymm}, z = ${_moveArmMessage.zcm*10+_moveArmMessage.zmm}")

            }
            override fun onStartTrackingTouch(seekBar: SeekBar?) {}
            override fun onStopTrackingTouch(seekBar: SeekBar?) { }
        })

        moveArmMessageHandler.post(moveArmMessageRunnable)

        val joystickView: JoystickView = view.findViewById(R.id.joystickView)
        joystickView.setJoystickListener(this)
    }

    override fun onJoystickMoved(xPercent: Float, yPercent: Float) {
        // Handle joystick movements here
        // xPercent and yPercent will be between -100 and 100
        val x_rotated = (125*(xPercent+yPercent)/1.41421356237).toInt()
        val y_rotated = (125*(yPercent-xPercent)/1.41421356237).toInt()
        //binding.editTextText.setText(" x= $x_rotated, y= $y_rotated")
        //The current position of the joystick is sent to victor as soon as the joystick changes position.
        //Depending on the joystick position a MoveTracksMessage is sent which will set the speed of victors
        //two tracks.
        val message = MoveTracksMessage(x_rotated, y_rotated)
        CoroutineScope(Dispatchers.Main).launch {
            _victorClient.send(message.toString())
        }
    }


    private val moveArmMessageHandler = Handler(Looper.getMainLooper())
    private val moveArmMessageRunnable = object : Runnable {

        override fun run() {
            if(!_armStateUpdated){
                _armStateUpdated = true
                CoroutineScope(Dispatchers.Main).launch {
                    _victorClient.send(_moveArmMessage.toString())
                }
            }
            if(!_clawStateUpdated){
                _clawStateUpdated = true
                CoroutineScope(Dispatchers.Main).launch {
                    if(_clawState){
                        _victorClient.send(MoveClawMessage(1).toString())
                    }else{
                        _victorClient.send(MoveClawMessage(0).toString())
                    }
                }
            }
            moveArmMessageHandler.postDelayed(this, 10)
        }
    }


    /* The connectVictorClient method initialises the _sharedViewModel with the VictorClient instance,
      which is used to communicate with victor. This is done so that the VictorClient instance can
      be reached from other fragments or any other part of the code. Because VictorClient opens the
      constructor socket to victor and can only be done once on startup. */
    private fun connectVictorClient()
    {
        _sharedViewModel = ViewModelProvider(requireActivity()).get(SharedViewModel::class.java)
        _victorClient = VictorClient() // Initialize with your logic
        _sharedViewModel.setSharedObject(_victorClient)
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}