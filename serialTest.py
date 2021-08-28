import serial
ser = serial.Serial('/dev/ttyUSB0',baudrate=1200)  # open serial port
print(ser.name)         # check which port was really used
counter = 0
inc = 0.05
while (1):
    buffer = bytes([int(counter)])
    ser.write(buffer)     # write a string
    counter = (counter + inc)
    if(counter>199):
        counter=199
        inc=-0.5
    if(counter<1):
        counter=1
        inc=0.5
    print (counter)

