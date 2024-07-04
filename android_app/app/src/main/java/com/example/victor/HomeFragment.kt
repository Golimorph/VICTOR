package com.example.victor

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.navigation.fragment.findNavController
import android.widget.SeekBar
import com.example.victor.databinding.FragmentHomeBinding
import androidx.lifecycle.ViewModelProvider
import com.example.victor.victorCommunication.*


/**
 * This is the home destination in the navigaion.
 */
class HomeFragment : Fragment(), JoystickView.JoystickListener {

    private var _binding: FragmentHomeBinding? = null
    private lateinit var _sharedViewModel: SharedViewModel //To share VictorState between fragments
    private lateinit var _victorState: VictorState

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        connectVictorState()
        _binding = FragmentHomeBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        setButtonListeners()
        setSeekbarsListeners()
        setJoystickListener(view)
    }

    private fun setButtonListeners(){
        binding.buttonSecond.setOnClickListener {
            findNavController().navigate(R.id.action_SecondFragment_to_FirstFragment)
        }

        binding.grabButton.setOnClickListener {
            _victorState.setClaw(!_victorState.getClaw())
        }
    }

    private fun updateText(){
        binding.editTextText.setText("x = ${_victorState.getArmX()}, y = ${_victorState.getArmY()}, z = ${_victorState.getArmZ()}")
    }

    private fun setSeekbarsListeners(){
        //Initialize and define the coordinate limits for the arm movement.
        updateText()
        binding.seekBarX.min = -400
        binding.seekBarX.progress = _victorState.getArmX()
        binding.seekBarX.max = 400
        binding.seekBarY.min = -200
        binding.seekBarY.progress = _victorState.getArmY()
        binding.seekBarY.max = 400
        binding.seekBarZ.min = -200
        binding.seekBarZ.progress = _victorState.getArmZ()
        binding.seekBarZ.max = 400

        binding.seekBarX.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                _victorState.setArmX(progress)
                updateText()
            }
            override fun onStartTrackingTouch(seekBar: SeekBar?) {}
            override fun onStopTrackingTouch(seekBar: SeekBar?) {}
        })

        binding.seekBarY.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                _victorState.setArmY(progress)
                updateText()
            }
            override fun onStartTrackingTouch(seekBar: SeekBar?) {}
            override fun onStopTrackingTouch(seekBar: SeekBar?) {}
        })

        binding.seekBarZ.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                _victorState.setArmZ(progress)
                updateText()
            }
            override fun onStartTrackingTouch(seekBar: SeekBar?) {}
            override fun onStopTrackingTouch(seekBar: SeekBar?) {}
        })
    }

    override fun onJoystickMoved(xPercent: Float, yPercent: Float) {
        // This is a callback called when the joystick is moved.
        // xPercent and yPercent will be between -100 and 100
        val x_rotated = (125*(xPercent+yPercent)/1.41421356237).toInt()
        val y_rotated = (125*(yPercent-xPercent)/1.41421356237).toInt()
        _victorState.setTrackSpeed(x_rotated,y_rotated)
    }

    private fun setJoystickListener(view: View){
        val joystickView: JoystickView = view.findViewById(R.id.joystickView)
        joystickView.setJoystickListener(this)
    }

    /* The connectVictorState method initialises the _sharedViewModel with the VictorState instance,
     which is used to communicate with victor. This is done so that the VictorState instance can
     be reached from other fragments or any other part of the code. Because VictorState opens the
     constructor socket to victor and can only be done once on startup. */
    private fun connectVictorState()
    {
        _sharedViewModel = ViewModelProvider(requireActivity()).get(SharedViewModel::class.java)

        // Check if the sharedObject already exists
        if (_sharedViewModel.sharedObject.value == null) {
            _victorState = VictorState()
            _victorState.initStateListener() //starts a thread inside victorState that sends the current state to victor every few ms.
            _sharedViewModel.setSharedObject(_victorState)
        } else {
            _victorState = _sharedViewModel.sharedObject.value!!
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}