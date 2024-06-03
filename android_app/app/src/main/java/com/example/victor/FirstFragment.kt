package com.example.victor

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.navigation.fragment.findNavController
import com.example.victor.databinding.FragmentFirstBinding
import com.google.android.material.snackbar.Snackbar
import socketClient.VictorClient
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.MainScope
import kotlinx.coroutines.launch
import kotlinx.coroutines.Dispatchers



/**
 * A simple [Fragment] subclass as the default destination in the navigation.
 */
class FirstFragment : Fragment() {
    val m_victorClient = VictorClient()

    private var _binding: FragmentFirstBinding? = null

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {

        _binding = FragmentFirstBinding.inflate(inflater, container, false)
        return binding.root

    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)



        binding.upButton.setOnClickListener {
            CoroutineScope(Dispatchers.Main).launch {
                m_victorClient.send("1")
            }
        }

        binding.downButton.setOnClickListener {
            CoroutineScope(Dispatchers.Main).launch {
                m_victorClient.send("0")
            }
        }

        binding.leftButton.setOnClickListener {
            CoroutineScope(Dispatchers.Main).launch {
                m_victorClient.send("3")
            }
        }

        binding.rightButton.setOnClickListener {
            CoroutineScope(Dispatchers.Main).launch {
                m_victorClient.send("4")
            }
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}