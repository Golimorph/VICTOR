package com.example.victor

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.navigation.fragment.findNavController
import com.example.victor.databinding.FragmentButtonsBinding
import socketClient.VictorClient
import androidx.lifecycle.ViewModelProvider
import com.example.victor.victorCommunication.SharedViewModel


/**
 * A simple [Fragment] subclass as the default destination in the navigation.
 */
class buttonsFragment : Fragment() {

    private lateinit var _sharedViewModel: SharedViewModel
    private lateinit var _victorClient: VictorClient

    private var _binding: FragmentButtonsBinding? = null

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {

        initVictorClient()
        _binding = FragmentButtonsBinding.inflate(inflater, container, false)
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