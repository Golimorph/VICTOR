package socketClient

import java.io.OutputStream
import java.net.Socket
import kotlinx.coroutines.*

/**
 * This is a documentation comment for the VictorClient class.
 *
 * More details about the class.
 */
class VictorClient(
    val m_serverAddress: String = "192.168.1.69", 
    val m_port: Int = 8080
) {
    private var m_socket: Socket? = null
    private var m_outputStream: OutputStream? = null

    init {
        CoroutineScope(Dispatchers.IO).launch {
            try {
                m_socket = Socket(m_serverAddress, m_port)
                m_outputStream = m_socket!!.getOutputStream()
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
    }

    /**
     * This is a documentation comment for the send method.
     *
     * @param data data to be sent to victor (raspberry) program.
     */
    suspend fun send(data: String) {
        withContext(Dispatchers.IO) {
            try {
                println("Sending to victor: $data")
                m_outputStream?.write(data.toByteArray())
                m_outputStream?.flush()
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
    }
}
