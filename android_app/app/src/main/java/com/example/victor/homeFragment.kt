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



/**
 * A simple [Fragment] subclass as the second destination in the navigation.
 */
class homeFragment : Fragment(), JoystickView.JoystickListener {

    private var _binding: FragmentHomeBinding? = null
    private lateinit var _sharedViewModel: SharedViewModel
    private lateinit var _victorClient: VictorClient

    //seekBar coordinate limits
    private val _minX = -400
    private val _midX = 0
    private val _maxX = 400

    private val _minY = -200
    private val _midY = 190
    private val _maxY = 400

    private val _minZ = -200
    private val _midZ = 150
    private val _maxZ = 400

    private var _armX = _midX
    private var _armY = _midY
    private var _armZ = _midZ

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

        binding.editTextText.setText("x = $_armX, y = $_armY, z = $_armZ,")

        binding.seekBarX.min = _minX
        binding.seekBarX.progress = _midX
        binding.seekBarX.max = _maxX
        binding.seekBarX.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                _armX = progress
                binding.editTextText.setText("x = $_armX, y = $_armY, z = $_armZ,")
                val message = MoveArmMessage(_armX, _armY, _armZ)
                CoroutineScope(Dispatchers.Main).launch {
                    _victorClient.send(message.toString())
                }
            }
            override fun onStartTrackingTouch(seekBar: SeekBar?) {}
            override fun onStopTrackingTouch(seekBar: SeekBar?) { }
        })

        binding.seekBarY.min = _minY
        binding.seekBarY.progress = _midY
        binding.seekBarY.max = _maxY
        binding.seekBarY.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                _armY = progress
                binding.editTextText.setText("x = $_armX, y = $_armY, z = $_armZ,")
                val message = MoveArmMessage(_armX, _armY, _armZ)
                CoroutineScope(Dispatchers.Main).launch {
                    _victorClient.send(message.toString())
                }
            }
            override fun onStartTrackingTouch(seekBar: SeekBar?) {}
            override fun onStopTrackingTouch(seekBar: SeekBar?) { }
        })

        binding.seekBarZ.min = _minZ
        binding.seekBarZ.progress = _midZ
        binding.seekBarZ.max = _maxZ
        binding.seekBarZ.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                _armZ = progress
                binding.editTextText.setText("x = $_armX, y = $_armY, z = $_armZ,")
                val message = MoveArmMessage(_armX, _armY, _armZ)
                CoroutineScope(Dispatchers.Main).launch {
                    _victorClient.send(message.toString())
                }
            }
            override fun onStartTrackingTouch(seekBar: SeekBar?) {}
            override fun onStopTrackingTouch(seekBar: SeekBar?) { }
        })




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