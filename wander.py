import time
from nano.robot import Robot


print('Create robot')
robot = Robot()
robot.startup()

WANDER_SPEED = 0.5
# safe range 20cm
SAFE_RANGE = 20

last_detect_time = time.time()
r = robot.range_finder.detect()
last_move = ''
while True:
    r = robot.range_finder.detect()
    print('Range: %d' % r)
    if r <= SAFE_RANGE:
        if last_move != 'left':
            last_move = 'left'
            print('left')
            robot.left(0.5)
    else:
        if last_move != 'forward':
            last_move = 'forward'
            print('forward')
            robot.forward(WANDER_SPEED)
    time.sleep(0.05)
