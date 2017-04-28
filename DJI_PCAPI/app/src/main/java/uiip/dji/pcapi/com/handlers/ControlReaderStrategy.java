package uiip.dji.pcapi.com.handlers;

import android.os.Handler;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.SocketException;
import java.util.Timer;
import java.util.TimerTask;

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
 * Command should be sended between 5 - 25 Hz
 * command format:
 * byte direction_id double velocity
 * directions:
 * PITCH 0
 * ROLL 1
 * YAW 2
 * THROTTLE 3
 */

class ControlReaderStrategy extends HandleStrategy{
    private final int SEND_FREQUNCY_HZ = 10;
    private final int SEND_PERIOD_MS = 1000 / SEND_FREQUNCY_HZ;
    private enum Direction{
        PITCH,
        ROLL,
        YAW,
        THROTTLE
    }
    Timer timer = new Timer();
    TimerTask timerTask = null;

    private DJIVirtualStickFlightControlData controlData = new DJIVirtualStickFlightControlData(0,0,0,0);
    DJIFlightController flightController = null;

    ControlReaderStrategy(Socket client) {
        super(client);

        flightController = PcApiApplication.getFlightControllerInstance();

        timerTask = new TimerTask() {

            @Override
            public void run() {

                flightController.sendVirtualStickFlightControlData(controlData, new DJICommonCallbacks.DJICompletionCallback() {
                    @Override
                    public void onResult(DJIError error) {
                        if (error != null){
                            Logger.log("Control data not sended: " + error.getDescription());
                        }
                    }
                });

            }
        };
    }

    @Override
    protected void readMessage(InputStream istream) throws IOException {
        DataInputStream dis = new DataInputStream(istream);

        byte dir_idx = dis.readByte();
        double velocity = dis.readDouble();
        Logger.log("Dir: " + dir_idx + " velocity: " + velocity);
        if (dir_idx >= 0 && dir_idx < Direction.values().length){
            Direction dir = Direction.values()[dir_idx];

            switch (dir){
                case PITCH:
                    Logger.log("Control: got PITCH " + velocity);
                    Logger.log("Control: got PITCH float" + (float)velocity);
                    controlData.setPitch((float)velocity);
                    break;
                case ROLL:
                    Logger.log("Control: got ROLL " + velocity);
                    controlData.setRoll((float)velocity);
                    break;
                case YAW:
                    Logger.log("Control: got YAW " + velocity);
                    controlData.setYaw((float)velocity);
                    break;
                case THROTTLE:
                    Logger.log("Control: got THROTTLE " + velocity);
                    controlData.setVerticalThrottle((float)velocity);
                    break;
            }
        }

    }

    @Override
    protected void writeMessage(OutputStream ostream) throws IOException {

    }

    @Override
    protected boolean isNeedRead() { return true; }

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

        timer.schedule(timerTask, 0, SEND_PERIOD_MS);
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

        timer.cancel();
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

            Thread.sleep(50);
            client.getOutputStream().flush();
        } catch (IOException e) {
            Logger.log("ControlReader: " + e.getMessage());
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
