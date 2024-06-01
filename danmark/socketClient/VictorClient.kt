package socketClient

import java.io.OutputStream
import java.net.Socket

/**
 * This is a documentation comment for the VictorClient class.
 *
 * More details about the class.
 */
class VictorClient(val m_serverAddress: String = "192.168.1.69", val m_port: Int = 8080){

    val m_socket = Socket(m_serverAddress, m_port)
    val m_outputStream: OutputStream = m_socket.getOutputStream()


    /**
     * This is a documentation comment for the myFunction method.
     *
     * @param data data to be sent to victor (raspberry) program.
     */
    fun send(data: String) {
        
        println("Sending to victor: " + data)
        m_outputStream.write(data.toByteArray())
        m_outputStream.flush()
    }
}









