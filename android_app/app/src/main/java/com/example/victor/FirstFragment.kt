package com.example.victor

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.navigation.fragment.findNavController
import com.example.victor.databinding.FragmentFirstBinding
import socketClient.VictorClient
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.launch
import kotlinx.coroutines.Dispatchers
import androidx.lifecycle.ViewModelProvider
import com.example.victor.victorCommunication.SharedViewModel


/**
 * A simple [Fragment] subclass as the default destination in the navigation.
 */
class FirstFragment : Fragment() {

    private lateinit var _sharedViewModel: SharedViewModel
    private lateinit var _victorClient: VictorClient

    private var _binding: FragmentFirstBinding? = null

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {

        connectVictorClient()
        _binding = FragmentFirstBinding.inflate(inflater, container, false)
        return binding.root

    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        /*
        binding.upButton.setOnClickListener {
            CoroutineScope(Dispatchers.Main).launch {
                _victorClient.send("1")
            }
        }
        binding.downButton.setOnClickListener {
            CoroutineScope(Dispatchers.Main).launch {
                _victorClient.send("0")
            }
        }
        binding.leftButton.setOnClickListener {
            CoroutineScope(Dispatchers.Main).launch {
                _victorClient.send("3")
            }
        }
        binding.rightButton.setOnClickListener {
            CoroutineScope(Dispatchers.Main).launch {
                _victorClient.send("4")
            }
        }
        */
        binding.viewButton.setOnClickListener {
            findNavController().navigate(R.id.action_FirstFragment_to_SecondFragment)
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