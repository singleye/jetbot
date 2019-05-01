from Jetson import GPIO
from Adafruit_GPIO import I2C


LEFT_PIN1=15
LEFT_PIN2=16
LEFT_PWM=11

STB_PIN=12

RIGHT_PWM=19
RIGHT_PIN1=21
RIGHT_PIN2=22

I2C_BUS=1
I2C_ADDR=8

LEFT_MOTOR_REG = 9
RIGHT_MOTOR_REG = 11

MINIMUM_SPEED = 0.3



class Wheel(object):
    def __init__(self, pin_pwm, pin1, pin2, i2c_bus, i2c_addr, i2c_reg):
        self.pin_pwm = pin_pwm
        self.pin1 = pin1
        self.pin2 = pin2
        GPIO.setup(self.pin_pwm, GPIO.OUT)
        GPIO.setup(self.pin1, GPIO.OUT)
        GPIO.setup(self.pin2, GPIO.OUT)
        self.i2c = I2C.get_i2c_device(i2c_addr, i2c_bus)
        self.i2c_reg = i2c_reg
        self.speed = 0

    def set_speed(self, speed):
        if abs(speed) < MINIMUM_SPEED:
            speed = 0
        self.speed = int(speed*255)
        self.i2c.write8(self.i2c_reg, abs(self.speed))
        if speed > 0:
            GPIO.output(self.pin_pwm, GPIO.HIGH)
            GPIO.output(self.pin1, GPIO.LOW)
            GPIO.output(self.pin2, GPIO.HIGH)
        elif speed == 0:
            GPIO.output(self.pin_pwm, GPIO.LOW)
            GPIO.output(self.pin1, GPIO.LOW)
            GPIO.output(self.pin2, GPIO.LOW)
        elif speed < 0:
            GPIO.output(self.pin_pwm, GPIO.HIGH)
            GPIO.output(self.pin1, GPIO.HIGH)
            GPIO.output(self.pin2, GPIO.LOW)


class Robot(object):
    def __init__(self):
        self.left_wheel = Wheel(LEFT_PWM,
                           LEFT_PIN1,
                           LEFT_PIN2,
                           I2C_BUS,
                           I2C_ADDR,
                           LEFT_MOTOR_REG)
        self.right_wheel = Wheel(RIGHT_PWM,
                            RIGHT_PIN1,
                            RIGHT_PIN2,
                            I2C_BUS,
                            I2C_ADDR,
                            RIGHT_MOTOR_REG)
        GPIO.setup(STB_PIN, GPIO.OUT)
        # wheel distance is 125 mm
        self.wheel_distance = 125

    def init(self):
        GPIO.cleanup()
        GPIO.setmode(GPIO.BOARD)

    def uninit(self):
        GPIO.cleanup()

    def _normalize(self, speed):
        if speed > 1:
            speed = 1
        if speed < -1:
            speed = -1
        return speed

    def set_motor_speed(self, left_speed, right_speed):
        left_speed = self._normalize(left_speed)
        right_speed = self._normalize(right_speed)
        self.left_wheel.set_speed(left_speed)
        self.right_wheel.set_speed(right_speed)
        GPIO.output(STB_PIN, GPIO.HIGH)

    def forward(self, speed):
        self.set_motor_speed(speed, speed)

    def backward(self, speed):
        self.set_motor_speed(-speed, -speed)

    def stop(self):
        self.set_motor_speed(0, 0)

    def left(self, speed):
        self.set_motor_speed(-speed, speed)

    def right(self, speed):
        self.set_motor_speed(speed, -speed)
        speed = self._normalize(speed)

    def turn(self, speed, radius):
        '''
        * speed: motor speed
        * radius: turning radius
        '''
        raise NotImplementedError('Method not implemented')
