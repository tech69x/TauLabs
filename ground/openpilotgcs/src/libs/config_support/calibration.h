/**
 ******************************************************************************
 *
 * @file       calibration.h
 * @author     The PhoenixPilo Team, http://www.openpilot.org Copyright (C) 2012.
 * @brief      Gui-less support class for calibration
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "ui_ccattitude.h"
#include "../uavobjectwidgetutils/configtaskwidget.h"
#include "extensionsystem/pluginmanager.h"
#include "uavobjectmanager.h"
#include "uavobject.h"
#include <QtGui/QWidget>
#include <QTimer>
#include <QString>

class Calibration
{
    Q_OBJECT

public:
    explicit Calibration();
    ~Calibration();

    void initialize(bool calibrateMags);

private:
    enum CALIBRATION_STATE {
        IDLE, LEVELING,
        SIX_POINT_WAIT1, SIX_POINT_COLLECT1,
        SIX_POINT_WAIT2, SIX_POINT_COLLECT2,
        SIX_POINT_WAIT3, SIX_POINT_COLLECT3,
        SIX_POINT_WAIT4, SIX_POINT_COLLECT4,
        SIX_POINT_WAIT5, SIX_POINT_COLLECT5,
        SIX_POINT_WAIT6, SIX_POINT_COLLECT6
    } calibration_state;

public slots:
    //! Start collecting data while aircraft is level
    void doStartLeveling();

    //! Start the six point calibration routine
    void doStartSixPoint();

    //! Indicates UAV is in a position to collect data during 6pt calibration
    doSaveSixPointPosition();

private slots:
    //! Indicates new data acquired
    void dataUpdated(UAVObject *);

signals:
    //! Indicate whether to enable or disable controls
    void toggleControls(bool enable);

    //! Change the UAV visualization
    void updatePlane(int position);

    //! Show an instruction to the user
    void showMessage(QString message);

private:
    QTimer timer;

    //! Whether to attempt to calibrate the mag (normally if it is present)
    bool calibrateMag;

    //! The expected magnetic amplitude
    double magLength;

    //! The expected gravity amplitude
    double accelLength;

    //! Store the initial accel meta data to restore it after calibration
    UAVObject::Metadata initialAccelsMdata;

    //! Store the initial gyro meta data to restore it after calibration
    UAVObject::Metadata initialGyrosMdata;

    // Track the number of updates
    int accelUpdates;
    int gyroUpdates;
    int magUpdates;

    QList<double> gyro_accum_x;
    QList<double> gyro_accum_y;
    QList<double> gyro_accum_z;
    QList<double> accel_accum_x;
    QList<double> accel_accum_y;
    QList<double> accel_accum_z;
    QList<double> mag_accum_x;
    QList<double> mag_accum_y;
    QList<double> accel_accum_z;

    double accel_data_x[6], accel_data_y[6], accel_data_z[6];
    double mag_data_x[6], mag_data_y[6], mag_data_z[6];

    static const int NUM_SENSOR_UPDATES = 300;

    double initialBoardRotation[3];

protected:
    //! Store a measurement at this position and indicate if it is the last one
    bool storeSixPointMeasured(UAVObject * obj, int position);

    void computeScaleBias();
    int SixPointInConstFieldCal( double ConstMag, double x[6], double y[6], double z[6], double S[3], double b[3] );
    int LinearEquationsSolving(int nDim, double* pfMatr, double* pfVect, double* pfSolution);

    //! Rotate a vector by the rotation matrix, optionally trasposing
    void rotate_vector(double R[3][3], const double vec[3], double vec_out[3], bool transpose = true);

    //! Compute a rotation matrix from a set of euler angles
    void Euler2R(double rpy[3], double Rbe[3][3]);

};

#endif // CALIBRATION_H
