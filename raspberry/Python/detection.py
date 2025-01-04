import gi
gi.require_version('Gst', '1.0')
from gi.repository import Gst, GLib
import os
import numpy as np
import cv2
import hailo
import socket
from hailo_rpi_common import (
    get_caps_from_pad,
    get_numpy_from_buffer,
    app_callback_class,
)
from detection_pipeline import GStreamerDetectionApp


# -----------------------------------------------------------------------------------------------
# Method to send the detected objects to the main C++ program running on VICTOR.
# All processing and logic of higher level abstraction is done in the C++ program.
# -----------------------------------------------------------------------------------------------
host = 'localhost'
port = 8081
object_to_detect = "None" #The object to detect, will be sent from C++ program.
socketToCppProgram = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socketToCppProgram.connect((host, port))
def send_detection(detection):
    socketToCppProgram.sendall(detection.encode())

def get_object_to_detect():
    data = socketToCppProgram.recv(1024)
    object_to_detect = data.decode('utf-8')
    print("Got an object: {object_to_detect}\n")

# -----------------------------------------------------------------------------------------------
# User-defined class to be used in the callback function
# -----------------------------------------------------------------------------------------------
# Inheritance from the app_callback_class
class user_app_callback_class(app_callback_class):
    def __init__(self):
        super().__init__()
        self.new_variable = 42  # New variable example

    def new_function(self):  # New function example
        return "The meaning of life is: "

# -----------------------------------------------------------------------------------------------
# User-defined callback function
# -----------------------------------------------------------------------------------------------

# This is the callback function that will be called when data is available from the pipeline
def app_callback(pad, info, user_data):
    # Get the GstBuffer from the probe info
    buffer = info.get_buffer()
    # Check if the buffer is valid
    if buffer is None:
        return Gst.PadProbeReturn.OK

    # Get the detections from the buffer
    roi = hailo.get_roi_from_buffer(buffer)
    detections = roi.get_objects_typed(hailo.HAILO_DETECTION)

    # Parse the detections
    detection_count = 0
    for detection in detections:
        label = detection.get_label()
        bbox = detection.get_bbox()
        confidence = detection.get_confidence()
        if label == "QR code":
            string_to_send = f"{label} {confidence:.2f} , {bbox.xmin()}, {bbox.xmax()}, {bbox.ymin()}, {bbox.ymax()} \n"
            send_detection(string_to_send);
            detection_count += 1

    return Gst.PadProbeReturn.OK

if __name__ == "__main__":
    # Create an instance of the user app callback class
    #get_object_to_detect()
    user_data = user_app_callback_class()
    app = GStreamerDetectionApp(app_callback, user_data)
    app.run()
