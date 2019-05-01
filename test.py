import time
from Jetson import GPIO
from nano.robot import Robot


print('Create robot')
robot = Robot()

print('Moving forward')
steps = 10
for i in range(steps):
    robot.forward(i/(steps*1.0))
    time.sleep(1)
robot.stop()

print('Moving backward')
steps = 10
for i in range(steps):
    robot.backward(i/(steps*1.0))
    time.sleep(1)
robot.stop()

print('Turn left')
steps = 10
for i in range(steps):
    robot.left(i/(steps*1.0))
    time.sleep(1)
robot.stop()

print('Turn right')
steps = 10
for i in range(steps):
    robot.right(i/(steps*1.0))
    time.sleep(1)

robot.set_motor_speed(0.3, 1)
time.sleep(5)

print('Stop')
robot.stop()
