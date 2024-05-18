import curses
import paramiko
import time

# Function to execute command over SSH
def establish_ssh_connection(hostname, username, password):
    try:
        # Create SSH client
        ssh_client = paramiko.SSHClient()
        ssh_client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        # Connect to SSH server
        ssh_client.connect(hostname, username=username, password=password) 
        return ssh_client

    except Exception as e:
        print("Error:", e)

def main(stdscr):
    # Initialize curses
    stdscr.clear()
    curses.cbreak()
    stdscr.keypad(True)
    stdscr.nodelay(True)

    ssh_client = establish_ssh_connection("raspberrypi.local", "victor", "rotciv")
    
    # Main loop
    lastPress_time = time.time()
    while True:
        # Get user input
        key = stdscr.getch()
        if key != curses.ERR:
            if key == ord('a') or key == ord('A'):
                 stdin, stdout, stderr = ssh_client.exec_command("/home/victor/Repository/main 0")
            elif key == curses.KEY_UP:
                stdin, stdout, stderr = ssh_client.exec_command("/home/victor/Repository/main 1")
            elif key == curses.KEY_DOWN:
                stdin, stdout, stderr = ssh_client.exec_command("/home/victor/Repository/main 2")
            elif key == curses.KEY_LEFT:
                stdin, stdout, stderr = ssh_client.exec_command("/home/victor/Repository/main 3")
            elif key == curses.KEY_RIGHT:
                stdin, stdout, stderr = ssh_client.exec_command("/home/victor/Repository/main 4")
        #if time.time() - lastPress_time > 0.4:
        #    stdin, stdout, stderr = ssh_client.exec_command("/home/victor/Repository/main 0")
        # Delay for a short period to reduce CPU usage
        time.sleep(0.01)

# Run the program
curses.wrapper(main)
