from flask import Flask, render_template,request, redirect, url_for
from pyduino import *
import time

app = Flask(__name__)

# initialize connection to Arduino
# if your arduino was running on a serial port other than '/dev/ttyACM0/'
# declare: a = Arduino(serial_port='/dev/ttyXXXX')
a = Arduino(serial_port='COM3') 
time.sleep(3)

# declare the pins we're using
LED_PIN3 = 3
LED_PIN4 = 4
ANALOG_PIN = 0

# initialize the digital pin as output
a.set_pin_mode(LED_PIN3,'O')
a.set_pin_mode(LED_PIN4, 'O')

print ('Arduino initialized')

# we are able to make 2 different requests on our webpage
# GET = we just type in the url
# POST = some sort of form submission like a button
@app.route('/', methods = ['POST','GET'])
def hello_world():

    # variables for template page (templates/index.html)
    author = "Lithium Ion Battery Selector"

    # if we make a post request on the webpage aka press button then do stuff
    if request.method == 'POST':

        # if we press the turn on button
        if request.form['submit'] == '2600 mAh': 
            print ('2600 mAh')
    
            # turn on LED on arduino
            a.digital_write(LED_PIN3,1)
            
        # if we press the turn off button
        elif request.form['submit'] == '2500 mAh': 
            print ('2500 mAh')

            # turn off LED on arduino
            a.digital_write(LED_PIN3,0)

        elif request.form['submit'] == '2400 mAh':
            print ('2400 mAh')
            a.digital_write(LED_PIN4,1)

        elif request.form['submit'] == '2200 mAh':
            print ('2200 mAh')
            a.digital_write(LED_PIN4,0)

        else:
            pass
    
    # read in analog value from photoresistor
    readval = a.analog_read(ANALOG_PIN)

    # the default page to display will be our template with our template variables
    return render_template('libselector.html', author=author, value=100*(readval/1023.))

if __name__ == "__main__":

    # lets launch our webpage!
    # do 0.0.0.0 so that we can log into this webpage
    # using another computer on the same network later
    app.run(host='0.0.0.0')