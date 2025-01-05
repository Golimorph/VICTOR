package com.example.victor

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.SeekBar
import androidx.navigation.fragment.findNavController
import com.example.victor.databinding.FragmentClawAngleBinding
import androidx.lifecycle.ViewModelProvider
import com.example.victor.victorCommunication.*


/**
 * Fragment for setting the angle of victors claw
 */
class ClawAngleFragment : Fragment() {

    private lateinit var _sharedViewModel: SharedViewModel
    private lateinit var _victorState: VictorState

    private var _binding: FragmentClawAngleBinding? = null

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        connectVictorState()
        _binding = FragmentClawAngleBinding.inflate(inflater, container, false)
        return binding.root

    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        setButtonListeners()
        setSeekbarsListeners()
    }

    private fun setButtonListeners(){
        binding.viewButton.setOnClickListener {
            findNavController().navigate(R.id.action_FirstFragment_to_SecondFragment)
        }
    }

    private fun updateText(){
        binding.textView2.setText("norm = ${_victorState.getClawNorm()}")
        binding.textView.setText("x = ${_victorState.getClawX()}, y = ${_victorState.getClawY()}, z = ${_victorState.getClawZ()}")
        binding.textView3.setText("ux = ${_victorState.getCameraX()}, uy = ${_victorState.getCameraY()}")
    }

    private fun setSeekbarsListeners(){
        //Initialize and define the coordinate limits for the arm movement.
        updateText()
        binding.seekBarX.min = -90
        binding.seekBarX.progress = _victorState.getClawX()
        binding.seekBarX.max = 90
        binding.seekBarY.min = -90
        binding.seekBarY.progress = _victorState.getClawY()
        binding.seekBarY.max = 90
        binding.seekBarZ.min = -90
        binding.seekBarZ.progress = _victorState.getClawZ()
        binding.seekBarZ.max = 90
        binding.seekBarCameraX.min = -90
        binding.seekBarCameraX.progress = 0
        binding.seekBarCameraX.max = 90
        binding.seekBarCameraY.min = -90
        binding.seekBarCameraY.progress = 0
        binding.seekBarCameraY.max = 90

        binding.seekBarX.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                _victorState.setClawX(progress)
                binding.seekBarY.progress = _victorState.getClawY()
                binding.seekBarZ.progress = _victorState.getClawZ()
                updateText()
            }
            override fun onStartTrackingTouch(seekBar: SeekBar?) {}
            override fun onStopTrackingTouch(seekBar: SeekBar?) {}
        })

        binding.seekBarY.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                _victorState.setClawY(progress)
                binding.seekBarX.progress = _victorState.getClawX()
                binding.seekBarZ.progress = _victorState.getClawZ()
                updateText()
            }
            override fun onStartTrackingTouch(seekBar: SeekBar?) {}
            override fun onStopTrackingTouch(seekBar: SeekBar?) {}
        })

        binding.seekBarZ.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                _victorState.setClawZ(progress)
                binding.seekBarX.progress = _victorState.getClawX()
                binding.seekBarY.progress = _victorState.getClawY()
                updateText()
            }
            override fun onStartTrackingTouch(seekBar: SeekBar?) {}
            override fun onStopTrackingTouch(seekBar: SeekBar?) {}
        })

        binding.seekBarCameraX.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                _victorState.setCameraAngle(progress, _victorState.getCameraY())
                updateText()
            }
            override fun onStartTrackingTouch(seekBar: SeekBar?) {}
            override fun onStopTrackingTouch(seekBar: SeekBar?) {}
        })

        binding.seekBarCameraY.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                _victorState.setCameraAngle(_victorState.getCameraX(),progress)
                updateText()
            }
            override fun onStartTrackingTouch(seekBar: SeekBar?) {}
            override fun onStopTrackingTouch(seekBar: SeekBar?) {}
        })
    }





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