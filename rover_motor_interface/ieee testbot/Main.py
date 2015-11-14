import time
import serial

class Wheels:


    def __init__(self, port):
        self.ser = \
            serial.Serial(  # configure the serial connections (the parameters differs on the device you are connecting to)
                port=port,  #'/dev/ttyUSB1'
                baudrate=9600,
                parity=serial.PARITY_ODD,
                stopbits=serial.STOPBITS_TWO,
                bytesize=serial.SEVENBITS
            )

        #self.ser.open()
        self.ser.isOpen()

        self.term_char = '\n'



    def write_cmd(self, message, encoding = 'utf-8'):

        data = str(message).encode(encoding)

        if self.term_char is not None:
            data += str(self.term_char).encode(encoding)[0:1]

            print(data)

            self.ser.write(data)

    def write(self, rwheel, lwheel):
        # input wheel speed is 0 for stop, -100 for full reverse, and +100 for full forward

        # mapping
        rwheel = int((rwheel*85)/100) + 95
        lwheel = int((lwheel*85)/100) + 95

        #print('left:' + lwheel + '    right:'+ rwheel)
        print('left:{l!s:s}    right:{r!s:s}'.format(r=rwheel, l=lwheel))

        if not ((0 <= rwheel <= 180 ) & (0 <= lwheel <= 180 )):
            raise Exception('wheel speed not whithin range')


        rwritestring = ('R{r!s:s}'.format(r=rwheel))
        lwritestring = ('L{l!s:s}'.format(l=lwheel))


        self.write_cmd(rwritestring)
        time.sleep(0.01)
        self.write_cmd(lwritestring)

class Joystick:
    def __init__(self, stickport):
        self.stick = None

    def connect(self):
        pass

    def get_axies(self):

        xaxies = None
        yaxies = None

        axies = xaxis, yaxis;
        return axies

    def map_tank_axies(self, x, y, ax_scale = 300000, out_scale=100 ):

        left = None
        right = None

        speed = left, right;
        return speed


wheels = Wheels('/dev/ttyACM0')
#jotstick = Joystick()

input=1
while 1 :
    # get keyboard input
    #input = raw_input(">> ")
        # Python 3 users
    #input = input(">> ")
    if input == 'exit':
        ser.close()
        print('exiting')
        exit()
    else:
        input = -25
        wheel = int(input)

        wheels.write(wheel, wheel)


        # # send the character to the device
        # # (note that I happend a \r\n carriage return and line feed to the characters - this is requested by my device)
        # ser.write(input + '\r\n')
        # out = ''
        # # let's wait one second before reading output (let's give device time to answer)
        # time.sleep(1)
        # while ser.inWaiting() > 0:
        #     out += ser.read(1)
        #
        # if out != '':
        #     print ">>" + out


        input='exit'
        #raise exception("die!")