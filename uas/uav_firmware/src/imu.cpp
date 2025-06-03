/**
 ******************************************************************************
 * \file       imu.cpp
 * \author     Abdulrhman Bahaa
 * \brief      This source file contains implementations for the imu functions
 * \date       2025-03-14
 ******************************************************************************
*/
#include "imu.hpp"

/* Functions Declarations ---------------------------------------------------*/
void mpu6050_with_dmp_init();
void mpu6050_without_dmp_init(MPU6050* mpu);
void mpu6050_without_dmp_get_angles(MPU6050* mpu, float* euler_angles);
void mpu6050_without_dmp_get_angles(float euler_angles[3]);

/* Functions Implementations -------------------------------------------------*/
#if (DMP == 1)
void
mpu6050_with_dmp_init(MPU6050* mpu) {
    uint8_t devStatus; // Return status after each device operation (0 = success, !0 = error)

    Serial.begin(115200); //115200 is required for Teapot Demo output
    while (!Serial)
        ;

    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment on this line if having compilation difficulties

    /*Initialize device*/
    mpu->initialize();

    /* Initialize and configure the DMP*/
    devStatus = mpu->dmpInitialize();

    /* Supply your gyro offsets here, scaled for min sensitivity */
    mpu->setXGyroOffset(0);
    mpu->setYGyroOffset(0);
    mpu->setZGyroOffset(0);
    mpu->setXAccelOffset(0);
    mpu->setYAccelOffset(0);
    mpu->setZAccelOffset(0);

    /* Making sure it worked (returns 0 if so) */
    if (devStatus == 0) {
        mpu->CalibrateAccel(6); // Calibration Time: generate offsets and calibrate our MPU6050
        mpu->CalibrateGyro(6);
        mpu->setDMPEnabled(true);
    } else {
        Serial.println("DMP Initialization failed"); //Print the error code
    }
}

void
mpu6050_with_dmp_get_angles(MPU6050* mpu, uint8_t* FIFOBuffer, float* euler_angles) {
    /*---Orientation/Motion Variables---*/
    Quaternion q;        // [w, x, y, z]         Quaternion container
    VectorFloat gravity; // [x, y, z]            Gravity vector

    mpu->dmpGetQuaternion(&q, FIFOBuffer);
    mpu->dmpGetGravity(&gravity, &q);
    mpu->dmpGetYawPitchRoll(euler_angles, &q, &gravity);
}

void
Imu::init() {
    mpu6050_with_dmp_init(this->mpu)
}

void
Imu::update(){mpu6050_with_dmp_get_angles(this -> euler_angles)}

#else

/**
 * \brief Constructor for the Imu class
 */
Imu::Imu() { mpu = new MPU6050(Wire); }

/**
 * \brief Initializes the MPU6050 without DMP
 * @details This function initializes the MPU6050 sensor without using the DMP (Digital Motion Processor).
 *          It sets up the I2C communication and calculates the gyro offsets.
 */
void
mpu6050_without_dmp_init(MPU6050* mpu) {
    Wire.begin();
    mpu->begin();
    mpu->calcGyroOffsets(true);
}

void
mpu6050_without_dmp_get_angles(MPU6050* mpu, float* euler_angles) {
    mpu->update();
    euler_angles[0] = mpu->getAngleZ();
    euler_angles[1] = mpu->getAngleX();
    euler_angles[2] = mpu->getAngleY();
}

void
Imu::init() {
    mpu6050_without_dmp_init(mpu);
}

void
Imu::update() {
    mpu6050_without_dmp_get_angles(mpu, euler_angles);
}

#endif
