#pragma once
#include "main.hpp"
#include "version.hpp"
#include "stmepic.hpp"
#include "can.hpp"


namespace se = stmepic;

void config_usb_device();

void main_prog();

void can_callback_bmp280_get_status(se::CanBase &can, se::CanDataFrame &msg, void *args);
void can_callback_bmp280_get_data(se::CanBase &can, se::CanDataFrame &msg, void *args);

void can_callback_imu_status(se::CanBase &can, se::CanDataFrame &msg, void *args);
void can_callback_imu_orientation(se::CanBase &can, se::CanDataFrame &msg, void *args);
void can_callback_imu_lin_acceleration(se::CanBase &can, se::CanDataFrame &msg, void *args);
void can_callback_imu_magnetic_field(se::CanBase &can, se::CanDataFrame &msg, void *args);
void can_callback_imu_gyration(se::CanBase &can, se::CanDataFrame &msg, void *args);


void can_callback_gps_status(se::CanBase &can, se::CanDataFrame &msg, void *args);
void can_callback_gps_latitude(se::CanBase &can, se::CanDataFrame &msg, void *args);
void can_callback_gps_longitude(se::CanBase &can, se::CanDataFrame &msg, void *args);
void can_callback_gps_altitude(se::CanBase &can, se::CanDataFrame &msg, void *args);
void can_callback_gps_date(se::CanBase &can, se::CanDataFrame &msg, void *args);
void can_callback_gps_covariance(se::CanBase &can, se::CanDataFrame &msg, void *args);