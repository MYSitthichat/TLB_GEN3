from machine import Pin,UART
import time

DV_ID = "1"
pc_command = ""

# box 1
solinoid_box1_1 = Pin(2,Pin.OUT)
solinoid_box1_2 = Pin(3,Pin.OUT)
# box 2
solinoid_box2_1 = Pin(6,Pin.OUT)
solinoid_box2_2 = Pin(7,Pin.OUT)
# box 3
solinoid_box3_1 = Pin(10,Pin.OUT)
solinoid_box3_2 = Pin(11,Pin.OUT)
# box 4
solinoid_box4_1 = Pin(15,Pin.OUT)
solinoid_box4_2 = Pin(14,Pin.OUT)
# box 5
solinoid_box5_1 = Pin(17,Pin.OUT)
solinoid_box5_2 = Pin(16,Pin.OUT)
# box 6
solinoid_box6_1 = Pin(19,Pin.OUT)
solinoid_box6_2 = Pin(18,Pin.OUT)

pc_link = UART(1, baudrate=115200, bits=8, parity=None, stop=1,tx=Pin(8), rx=Pin(9),timeout=1000)
pc_link.read() 

def stop_all():
    solinoid_box1_1.value(0)
    solinoid_box1_2.value(0)
    solinoid_box2_1.value(0)
    solinoid_box2_2.value(0)
    solinoid_box3_1.value(0)
    solinoid_box3_2.value(0)
    solinoid_box4_1.value(0)
    solinoid_box4_2.value(0)
    solinoid_box5_1.value(0)
    solinoid_box5_2.value(0)
    solinoid_box6_1.value(0)
    solinoid_box6_2.value(0)
    pc_link.write("All boxes stopped\n")

def init_tube():
    solinoid_box1_1.value(1)
    solinoid_box2_1.value(1)
    solinoid_box3_1.value(1)
    solinoid_box4_1.value(1)
    solinoid_box5_1.value(1)
    solinoid_box6_1.value(1)
    time.sleep(1.5)
    solinoid_box1_1.value(0)
    solinoid_box2_1.value(0)
    solinoid_box3_1.value(0)
    solinoid_box4_1.value(0)
    solinoid_box5_1.value(0)
    solinoid_box6_1.value(0)

init_tube()
time.sleep(1.5)

while True:
    try:
        char_cmd = pc_link.read(1)
        char_cmd = char_cmd.decode()
        if char_cmd == '\n':
            execute_flag = True
        else:
            pc_command = pc_command + char_cmd
    except:
        execute_flag = False
        pc_command = ""
    if execute_flag==True:
        if len(pc_command) > 0:
            if pc_command[0] == DV_ID:
                if pc_command[1] == '1':
                    solinoid_box1_2.value(1)
                    time.sleep(1.5)
                    solinoid_box1_2.value(0)
                    time.sleep(1.5)
                    solinoid_box1_1.value(1)
                    time.sleep(1.5)
                    solinoid_box1_1.value(0)
                    pc_link.write("Box 1 done\n")
                elif pc_command[1] == '2':
                    solinoid_box2_2.value(1)
                    time.sleep(1.5)
                    solinoid_box2_2.value(0)
                    time.sleep(1.5)
                    solinoid_box2_1.value(1)
                    time.sleep(1.5)
                    solinoid_box2_1.value(0)
                    pc_link.write("Box 2 done\n")
                elif pc_command[1] == '3':
                    solinoid_box3_2.value(1)
                    time.sleep(1.5)
                    solinoid_box3_2.value(0)
                    time.sleep(1.5)
                    solinoid_box3_1.value(1)
                    time.sleep(1.5)
                    solinoid_box3_1.value(0)
                    pc_link.write("Box 3 done\n")
                elif pc_command[1] == '4':
                    solinoid_box4_2.value(1)
                    time.sleep(1.5)
                    solinoid_box4_2.value(0)
                    time.sleep(1.5)
                    solinoid_box4_1.value(1)
                    time.sleep(1.5)
                    solinoid_box4_1.value(0)
                    pc_link.write("Box 4 done\n")
                elif pc_command[1] == '5':
                    solinoid_box5_2.value(1)
                    time.sleep(1.5)
                    solinoid_box5_2.value(0)
                    time.sleep(1.5)
                    solinoid_box5_1.value(1)
                    time.sleep(1.5)
                    solinoid_box5_1.value(0)
                    pc_link.write("Box 5 done\n")
                elif pc_command[1] == '6':
                    solinoid_box6_2.value(1)
                    time.sleep(1.5)
                    solinoid_box6_2.value(0)
                    time.sleep(1.5)
                    solinoid_box6_1.value(1)
                    time.sleep(1.5)
                    solinoid_box6_1.value(0)
                    pc_link.write("Box 6 done\n")
                else:
                    pc_link.write("Invalid box\n")
                    stop_all()
            pc_command = ""
            execute_flag = False