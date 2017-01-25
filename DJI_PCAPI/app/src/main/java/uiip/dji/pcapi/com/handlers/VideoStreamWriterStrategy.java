package uiip.dji.pcapi.com.handlers;

import android.widget.Toast;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Arrays;

import dji.common.camera.DJICameraSettingsDef;
import dji.common.error.DJIError;
import dji.common.util.DJICommonCallbacks;
import dji.sdk.camera.DJICamera;
import uiip.dji.pcapi.com.Logger;
import uiip.dji.pcapi.com.MainActivity;
import uiip.dji.pcapi.com.PcApiApplication;

/**
 * Created by dji on 17.11.2016.
 */

class VideoStreamWriterStrategy extends HandleStrategy {
    VideoStreamWriterStrategy(final Socket client) {
        super(client);

        DJICamera camera = PcApiApplication.getCameraInstance();
        if (camera != null) {
            camera.setDJICameraReceivedVideoDataCallback(
                    new DJICamera.CameraReceivedVideoDataCallback() {
                @Override
                public void onResult(byte[] bytes, int length) {
                    //Logger.log("Available bytes: " + bytes.length);
                    try {
                        //Logger.log("Size: " + length);
                        //Logger.log(Arrays.toString(bytes));
                        client.getOutputStream().write(bytes, 0, length);
                        client.getOutputStream().flush();
                    } catch (IOException e) {
                        interrupt();
                        e.printStackTrace();
                        Logger.log(e.getMessage());
                    }
                }
            });
        }
    }

    @Override
    protected void readMessage(BufferedReader reader) {
        throw new UnsupportedOperationException("Unexpected input");
    }

    @Override
    protected void writeMessage(OutputStream ostream) {
        /*PrintWriter writer = new PrintWriter(ostream);
        writer.write("Hello from Video Writer!\n");
        writer.flush();*/
    }

    @Override
    protected boolean isNeedWrite() {
        return false;
    }

    @Override
    protected void doJob() {
        startRecord();
    }

    @Override
    protected void interrupt() {
        stopRecord();
    }

    private void switchCameraMode(DJICameraSettingsDef.CameraMode cameraMode){

        DJICamera camera = PcApiApplication.getCameraInstance();
        if (camera != null) {
            camera.setCameraMode(cameraMode, new DJICommonCallbacks.DJICompletionCallback() {
                @Override
                public void onResult(DJIError error) {

                    if (error == null) {
                        //Logger.showToast("Switch Camera Mode Succeeded");
                        Logger.log("Switch Camera Mode Succeeded");
                    } else {
                        //Logger.showToast(error.getDescription());
                        Logger.log(error.getDescription());
                    }
                }
            });
        }

    }

    // Method for starting recording
    private void startRecord(){
        switchCameraMode(DJICameraSettingsDef.CameraMode.RecordVideo);

        final DJICamera camera = PcApiApplication.getCameraInstance();
        if (camera != null) {
            camera.startRecordVideo(new DJICommonCallbacks.DJICompletionCallback(){
                @Override
                public void onResult(DJIError error)
                {
                    if (error == null) {
                        //Logger.showToast("Record video: success");
                        Logger.log("Record video: success");
                    }else {
                        //Logger.showToast(error.getDescription());
                        Logger.log(error.getDescription());
                    }
                }
            }); // Execute the startRecordVideo API
        }
    }

    // Method for stopping recording
    private void stopRecord(){

        DJICamera camera = PcApiApplication.getCameraInstance();
        if (camera != null) {
            camera.stopRecordVideo(new DJICommonCallbacks.DJICompletionCallback(){

                @Override
                public void onResult(DJIError error)
                {
                    if(error == null) {
                        //Logger.showToast("Stop recording: success");
                        Logger.log("Stop recording: success");
                    }else {
                        //Logger.showToast(error.getDescription());
                        Logger.log(error.getDescription());
                    }
                }
            }); // Execute the stopRecordVideo API
        }

    }
}
