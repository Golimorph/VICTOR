package com.example.victor

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.navigation.fragment.findNavController
import com.example.victor.databinding.FragmentSecondBinding
import socketClient.VictorClient
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.launch
import kotlinx.coroutines.Dispatchers
import androidx.lifecycle.ViewModelProvider
import com.example.victor.victorCommunication.*
import kotlin.math.sqrt

import kotlinx.serialization.Serializable
import kotlinx.serialization.encodeToString
import kotlinx.serialization.json.Json


/**
 * A simple [Fragment] subclass as the second destination in the navigation.
 */
class SecondFragment : Fragment(), JoystickView.JoystickListener {

    private var _binding: FragmentSecondBinding? = null
    private lateinit var _sharedViewModel: SharedViewModel
    private lateinit var _victorClient: VictorClient

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {

        initVictorClient()
        _binding = FragmentSecondBinding.inflate(inflater, container, false)
        return binding.root

    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        binding.buttonSecond.setOnClickListener {
            findNavController().navigate(R.id.action_SecondFragment_to_FirstFragment)
        }

        val joystickView: JoystickView = view.findViewById(R.id.joystickView)
        joystickView.setJoystickListener(this)
    }



    override fun onJoystickMoved(xPercent: Float, yPercent: Float) {
        // Handle joystick movements here
        // xPercent and yPercent will be between -100 and 100
        val x_rotated = (125*(xPercent+yPercent)/1.41421356237).toInt()
        val y_rotated = (125*(yPercent-xPercent)/1.41421356237).toInt()
        binding.editTextText.setText(" x= $x_rotated, y= $y_rotated")
        //The current position of the joystick is sent to victor as soon as the joystick changes position.
        //Depending on the joystick position a MoveTracksMessage is sent which will set the speed of victors
        //two tracks.
        val message = MoveTracksMessage(x_rotated, y_rotated)
        CoroutineScope(Dispatchers.Main).launch {
            _victorClient.send(message.toString())
        }
    }

    private fun initVictorClient()
    {
        _sharedViewModel = ViewModelProvider(requireActivity()).get(SharedViewModel::class.java)
        // Observe the shared object
        _sharedViewModel.sharedObject.observe(viewLifecycleOwner, { obj ->
            if (obj != null) {
                _victorClient = obj
            }
        })
    }


    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}