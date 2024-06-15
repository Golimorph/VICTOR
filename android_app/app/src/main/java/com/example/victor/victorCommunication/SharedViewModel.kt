package com.example.victor.victorCommunication

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import socketClient.VictorClient

/*The purpose of this class is to store variables that are commmonly used for all fragments. Such as the victorClient that provides
a socket to communicate with victor. It can only be initialized at one places and then the that same object has to be used in all other
Fragments. Typically it is initialized in the FirstFragment.*/
class SharedViewModel : ViewModel() {
    private val _sharedObject = MutableLiveData<VictorClient>()
    val sharedObject: LiveData<VictorClient> get() = _sharedObject

    fun setSharedObject(obj: VictorClient) {
        _sharedObject.value = obj
    }
}
