package gui

import javax.swing.*
import java.awt.*
import java.awt.event.ActionEvent
import socketClient.VictorClient

class DanmarkGui {
    val m_victorClient = VictorClient()


    fun createAndShowGUI() {
        val frame = JFrame("VICTOR")
        frame.defaultCloseOperation = JFrame.EXIT_ON_CLOSE
        frame.layout = BorderLayout()

        val panel = JPanel(GridLayout(4, 4)) // 3x3 grid layout for the buttons
        val emptyLabel = JLabel("") // Empty label for spacing

        // Create arrow buttons with labels
        val upButton = JButton("\u2191") // Up arrow
        val downButton = JButton("\u2193") // Down arrow
        val leftButton = JButton("\u2190") // Left arrow
        val rightButton = JButton("\u2192") // Right arrow
        val moveButton = JButton("move") // Right arrow

        // Add action listeners to the buttons

        

        upButton.addActionListener {
            m_victorClient.send("1")
        }
        downButton.addActionListener {
            m_victorClient.send("0")
        }
        leftButton.addActionListener {
            m_victorClient.send("3")
        }
        rightButton.addActionListener {
            m_victorClient.send("4")
        }
        moveButton.addActionListener {
            m_victorClient.send("6")
        }

        // Add buttons to the panel
        panel.add(emptyLabel)
        panel.add(upButton)
        panel.add(emptyLabel)
        panel.add(leftButton)
        panel.add(emptyLabel)
        panel.add(rightButton)
        panel.add(emptyLabel)
        panel.add(downButton)
        panel.add(emptyLabel)
        panel.add(moveButton)
        panel.add(emptyLabel)

        // Add panel to the frame
        frame.add(panel, BorderLayout.CENTER)

        // Set frame size
        frame.setSize(400, 400) // Set desired width and height

        // Set key bindings for arrow keys
        setKeyBindings(frame, "UP", "UP", upButton)
        setKeyBindings(frame, "DOWN", "DOWN", downButton)
        setKeyBindings(frame, "LEFT", "LEFT", leftButton)
        setKeyBindings(frame, "RIGHT", "RIGHT", rightButton)

        // Set frame properties
        frame.isVisible = true
    }

    private fun setKeyBindings(frame: JFrame, key: String, keyStroke: String, button: JButton) {
        val inputMap = frame.rootPane.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW)
        val actionMap = frame.rootPane.actionMap

        inputMap.put(KeyStroke.getKeyStroke("pressed $keyStroke"), key)
        actionMap.put(key, object : AbstractAction() {
            override fun actionPerformed(e: ActionEvent?) {
                println("Key binding for $keyStroke triggered")
                button.doClick()
            }
        })
    }

    fun init() {
        SwingUtilities.invokeLater(::createAndShowGUI)
    }
}
