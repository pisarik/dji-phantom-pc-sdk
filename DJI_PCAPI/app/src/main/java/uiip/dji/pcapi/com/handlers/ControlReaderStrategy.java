package uiip.dji.pcapi.com.handlers;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

import dji.common.error.DJIError;
import dji.common.flightcontroller.DJIFlightControllerDataType;
import dji.common.flightcontroller.DJIVirtualStickFlightControlData;
import dji.common.flightcontroller.DJIVirtualStickRollPitchControlMode;
import dji.common.flightcontroller.DJIVirtualStickVerticalControlMode;
import dji.common.flightcontroller.DJIVirtualStickYawControlMode;
import dji.common.util.DJICommonCallbacks;
import dji.sdk.flightcontroller.DJIFlightController;
import uiip.dji.pcapi.com.Logger;
import uiip.dji.pcapi.com.PcApiApplication;

/**
 * Created by dji on 17.11.2016.
 */

class ControlReaderStrategy extends HandleStrategy{

    private DJIVirtualStickFlightControlData controller = new DJIVirtualStickFlightControlData(0,0,0,0);
    DJIFlightController flightController = null;

    ControlReaderStrategy(Socket client) {
        super(client);

        flightController = PcApiApplication.getFlightControllerInstance();
    }

    @Override
    protected void readMessage(InputStream istream) throws IOException {

    }

    @Override
    protected void writeMessage(OutputStream ostream) throws IOException {

    }

    @Override
    protected boolean isNeedWrite() {
        return false;
    }

    @Override
    protected void initialize() {
        flightController.enableVirtualStickControlMode(new DJICommonCallbacks.DJICompletionCallback() {
            @Override
            public void onResult(DJIError error) {
                if (error == null) {
                    Logger.log("Enabled virtual stick control mode");
                } else {
                    Logger.log(error.getDescription());
                }
            }
        });

        flightController.setRollPitchControlMode(DJIVirtualStickRollPitchControlMode.Velocity);
        flightController.setYawControlMode(DJIVirtualStickYawControlMode.AngularVelocity);
        flightController.setVerticalControlMode(DJIVirtualStickVerticalControlMode.Velocity);

        sendVelocityRanges();
    }

    @Override
    protected void interrupt() {
        flightController.disableVirtualStickControlMode(new DJICommonCallbacks.DJICompletionCallback() {
            @Override
            public void onResult(DJIError error) {
                if (error == null) {
                    Logger.log("Disabled virtual stick control mode");
                } else {
                    Logger.log(error.getDescription());
                }
            }
        });
    }

    private void sendVelocityRanges(){
        double pitchMinVelocity = DJIFlightControllerDataType.DJIVirtualStickRollPitchControlMinVelocity;
        double pitchMaxVelocity = DJIFlightControllerDataType.DJIVirtualStickRollPitchControlMaxVelocity;
        double rollMinVelocity = DJIFlightControllerDataType.DJIVirtualStickRollPitchControlMinVelocity;
        double rollMaxVelocity = DJIFlightControllerDataType.DJIVirtualStickRollPitchControlMaxVelocity;
        double yawMinVelocity = DJIFlightControllerDataType.DJIVirtualStickYawControlMinAngularVelocity;
        double yawMaxVelocity = DJIFlightControllerDataType.DJIVirtualStickYawControlMaxAngularVelocity;
        double throttleMinVelocity = DJIFlightControllerDataType.DJIVirtualStickVerticalControlMinVelocity;
        double throttleMaxVelocity = DJIFlightControllerDataType.DJIVirtualStickVerticalControlMaxVelocity;

        try {
            DataOutputStream dos = new DataOutputStream(client.getOutputStream());

            dos.writeDouble(pitchMinVelocity);
            dos.writeDouble(pitchMaxVelocity);
            dos.writeDouble(rollMinVelocity);
            dos.writeDouble(rollMaxVelocity);
            dos.writeDouble(yawMinVelocity);
            dos.writeDouble(yawMaxVelocity);
            dos.writeDouble(throttleMinVelocity);
            dos.writeDouble(throttleMaxVelocity);

            dos.flush();
        } catch (IOException e) {
            Logger.log("ControlReader: " + e.getMessage());
        }
    }
}
