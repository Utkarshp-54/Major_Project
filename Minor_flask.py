#Modified by smartbuilds.io
#Date: 27.09.20
#Desc: This web application serves a motion JPEG stream
# main.py
# import the necessary packages
from flask import Flask, render_template, Response, request, send_from_directory
from camera import VideoCamera
import os

pi_camera = VideoCamera(flip=False) # flip pi camera if upside down.

# App Globals (do not edit)
app = Flask(_name_)

@app.route('/')
def index():
    return render_template('index.html') #you can customze index.html here

def gen(camera):
    #get camera frame
    while True:
        frame = camera.get_frame()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')

@app.route('/video_feed')
def video_feed():
    return Response(gen(pi_camera),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

# Take a photo when pressing camera button
@app.route('/picture')
def take_picture():
    pi_camera.take_picture()
    return "None"

if _name_ == '_main_':

app.run(host='0.0.0.0', debug=False)

DHT-11 sensor code:

import RPi.GPIO as GPIO
import dht11
import time
GPIO.setmode(GPIO.BCM)
myDHT=dht11.DHT11(pin=17)
try:
    while True:
        result=myDHT.read()
        if result.is_valid():
            print('Temprature is:',result.temperature,'Humidity is:',result.humidity)
        time.sleep(.2)
except KeyboardInterrupt:
    GPIO.cleanup()
    print('GPIO good to go')

MQ-2 gas sensor:

import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

# Define GPIO pins for the sensor and LED
sensor_pin = 27
led_pin = 25

# Set up the GPIO pins
GPIO.setup(sensor_pin, GPIO.IN)
GPIO.setup(led_pin, GPIO.OUT)

while True:
    # Read the sensor value
    sensor_value = GPIO.input(sensor_pin)
    
    # Turn on the LED if gas is detected
    if sensor_value == 1:
        GPIO.output(led_pin, GPIO.HIGH)
        print("Gas detected!")
    else:
        GPIO.output(led_pin, GPIO.LOW)
        print("No gas detected.")
    
    # Wait for a short time before reading the sensor again
    time.sleep(0.1)