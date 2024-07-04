package com.example.victor.victorCommunication

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import socketClient.VictorClient

/*The purpose of this class is to store variables that are commmonly used for all fragments.
It can only be initialized at one places and then the that same object has to be used in all other
Fragments. In my case it is initialized in the HomeFragment.*/
class SharedViewModel : ViewModel() {
    private val _sharedObject = MutableLiveData<VictorState>()
    val sharedObject: LiveData<VictorState> get() = _sharedObject

    fun setSharedObject(obj: VictorState) {
        _sharedObject.value = obj
    }
}
