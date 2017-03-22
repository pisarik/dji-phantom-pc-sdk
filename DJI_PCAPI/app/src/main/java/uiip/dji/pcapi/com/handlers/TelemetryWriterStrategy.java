package uiip.dji.pcapi.com.handlers;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

import dji.common.flightcontroller.DJIFlightControllerCurrentState;
import dji.sdk.base.DJIBaseProduct;
import dji.sdk.flightcontroller.DJIFlightController;
import uiip.dji.pcapi.com.Logger;
import uiip.dji.pcapi.com.PcApiApplication;
import uiip.dji.pcapi.com.media.DJIVideoStreamDecoder;

/**
 * Created by dji on 21.03.2017.
 */

class TelemetryWriterStrategy extends HandleStrategy {

    DJIFlightController flightController;

    public TelemetryWriterStrategy(Socket client) {
        super(client);

        flightController = PcApiApplication.getFlightControllerInstance();

    }

    @Override
    protected void readMessage(BufferedReader reader) {

    }

    @Override
    protected void writeMessage(OutputStream ostream) throws IOException{
        int frameIndex = DJIVideoStreamDecoder.getInstance().frameIndex;

        DJIFlightControllerCurrentState curState = flightController.getCurrentState();
        double latitude = curState.getAircraftLocation().getLatitude();
        double longitude = curState.getAircraftLocation().getLongitude();

        double velocityX = curState.getVelocityX();

        try {
            DataOutputStream dos = new DataOutputStream(ostream);
            dos.writeInt(frameIndex);
            dos.writeDouble(latitude);
            dos.writeDouble(longitude);

            dos.writeDouble(curState.getVelocityX());
            dos.writeDouble(curState.getVelocityY());
            dos.writeDouble(curState.getVelocityZ());

            dos.writeDouble(curState.getAttitude().roll);
            dos.writeDouble(curState.getAttitude().pitch);
            dos.writeDouble(curState.getAttitude().yaw);
        } catch (IOException e) {
            Logger.log("Telemetry: " + e.getMessage());
            e.printStackTrace();
            throw e;
        }


    }

    @Override
    protected boolean isNeedWrite() {
        return true;
    }

    @Override
    protected void doJob() {

    }

    @Override
    protected void interrupt() {

    }
}
