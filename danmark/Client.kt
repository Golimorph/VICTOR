import org.jline.keymap.KeyMap
import org.jline.terminal.TerminalBuilder
import org.jline.utils.InfoCmp
import java.io.OutputStream
import java.net.Socket
import java.io.InputStreamReader
import java.io.BufferedReader
import java.io.IOException
import java.io.Reader
import java.io.FileInputStream
import java.io.FileDescriptor

fun main() {
    // Disable line buffering on Unix-like systems using stty command
    //disableLineBuffering()

    // Create a key map for key bindings
    val keyMap = KeyMap<String>()
    keyMap.bind("LEFT", "\u001BOD") // Left arrow
    keyMap.bind("RIGHT", "\u001BOC") // Right arrow
    keyMap.bind("UP", "\u001BOA") // Up arrow
    keyMap.bind("DOWN", "\u001BOB") // Down arrow

    // Connect to the server
    val serverAddress = "192.168.1.69" // Replace with the IP address of your Raspberry Pi
    val port = 8080 // Replace with the port nu
    val socket = Socket(serverAddress, port)
    val outputStream: OutputStream = socket.getOutputStream()

    // Listen for key presses
    while (true) {
        // Read a single character from the standard input stream
        val char = readChar()
        val input = char.toString()

        // Check if the input matches a binding in the key map
        val binding = keyMap.getBound(input)
        if (binding != null) {
            // Send the binding to the server
            outputStream.write(binding.toByteArray())
            outputStream.flush()
        }
    }
}

fun readChar(): Char {
    return System.`in`.read().toChar()
}

fun disableLineBuffering() {
    try {
        val command = arrayOf("/bin/sh", "-c", "stty raw -echo </dev/tty")
        val process = Runtime.getRuntime().exec(command)
        process.waitFor()
    } catch (e: IOException) {
        e.printStackTrace()
    }
}
