#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

/* MPU6050 default I2C address is 0x68*/
MPU6050 mpu;

/*---MPU6050 Control/Status Variables---*/
uint8_t devStatus;      // Return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // Expected DMP packet size (default is 42 bytes)
uint8_t FIFOBuffer[64]; // FIFO storage buffer

/*---Orientation/Motion Variables---*/ 
Quaternion q;           // [w, x, y, z]         Quaternion container
VectorFloat gravity;    // [x, y, z]            Gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   Yaw/Pitch/Roll container and gravity vector

void setup() {

  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock. Comment on this line if having compilation difficulties

  Serial.begin(115200); //115200 is required for Teapot Demo output
  while (!Serial);

  /*Initialize device*/
  mpu.initialize();

  /* Initialize and configure the DMP*/
  devStatus = mpu.dmpInitialize();

  /* Supply your gyro offsets here, scaled for min sensitivity */
  mpu.setXGyroOffset(0);
  mpu.setYGyroOffset(0);
  mpu.setZGyroOffset(0);
  mpu.setXAccelOffset(0);
  mpu.setYAccelOffset(0);
  mpu.setZAccelOffset(0);

  /* Making sure it worked (returns 0 if so) */ 
  if (devStatus == 0) {
    mpu.CalibrateAccel(6);  // Calibration Time: generate offsets and calibrate our MPU6050
    mpu.CalibrateGyro(6);
    mpu.setDMPEnabled(true);

    packetSize = mpu.dmpGetFIFOPacketSize(); //Get expected DMP packet size for later comparison
  } 
  else {
    Serial.println(F("DMP Initialization failed (code ")); //Print the error code
  }
}

void loop() {    
  /* Read a packet from FIFO */
  if (mpu.dmpGetCurrentFIFOPacket(FIFOBuffer)) { // Get the Latest packet 
      /* Display Euler angles in degrees */
      mpu.dmpGetQuaternion(&q, FIFOBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

      Serial.print(ypr[0] * 180/M_PI); // Psi in degrees
      Serial.print(",");
      Serial.print(ypr[2] * 180/M_PI); // Theta in degrees
      Serial.print(",");
      Serial.println(ypr[1] * 180/M_PI); // Phi in degrees
  }
}
