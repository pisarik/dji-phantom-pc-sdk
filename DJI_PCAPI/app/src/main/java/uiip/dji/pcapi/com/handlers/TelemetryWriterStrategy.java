package uiip.dji.pcapi.com.handlers;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.Calendar;

import dji.common.flightcontroller.DJIFlightControllerCurrentState;
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
    protected void readMessage(InputStream istream) throws IOException {

    }

    @Override
    protected void writeMessage(OutputStream ostream) throws IOException{
        int frameIndex = DJIVideoStreamDecoder.getInstance().frameIndex;

        DJIFlightControllerCurrentState curState = flightController.getCurrentState();
        long time = System.currentTimeMillis();

        //try {
            DataOutputStream dos = new DataOutputStream(ostream);
            dos.writeInt(frameIndex);
            dos.writeLong(time);
            dos.writeDouble(curState.getAircraftLocation().getLatitude());
            dos.writeDouble(curState.getAircraftLocation().getLongitude());
            dos.writeDouble(curState.getAircraftLocation().getAltitude());

            dos.writeDouble(curState.getVelocityX());
            dos.writeDouble(curState.getVelocityY());
            dos.writeDouble(curState.getVelocityZ());

            dos.writeDouble(curState.getAttitude().pitch);
            dos.writeDouble(curState.getAttitude().roll);
            dos.writeDouble(curState.getAttitude().yaw);
        /*} catch (IOException e) {
            Logger.log("Telemetry: " + e.getMessage());
            e.printStackTrace();
            throw e;
        }*/

        ostream.flush();

        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

    }

    @Override
    protected boolean isNeedRead() { return true; }

    @Override
    protected boolean isNeedWrite() {
        return true;
    }

    @Override
    protected void initialize() {

    }

    @Override
    protected void interrupt() {

    }
}
