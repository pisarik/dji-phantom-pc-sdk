package uiip.dji.pcapi.com.handlers;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.view.TextureView;

import java.io.BufferedReader;
import java.io.OutputStream;
import java.net.Socket;

import dji.common.camera.DJICameraSettingsDef;
import dji.common.error.DJIError;
import dji.common.util.DJICommonCallbacks;
import dji.sdk.camera.DJICamera;
import dji.sdk.codec.DJICodecManager;
import uiip.dji.pcapi.com.Logger;
import uiip.dji.pcapi.com.PcApiApplication;
import uiip.dji.pcapi.com.R;

/**
 * Created by dji on 17.01.2017.
 */

class VideoWriterViaSurfaceTextureStrategy extends HandleStrategy
                                               implements TextureView.SurfaceTextureListener{

    private DJICodecManager mCodecManager = null;
    private final Context context;
    private int frameNumber = 0;
    private TextureView textureView;

    VideoWriterViaSurfaceTextureStrategy(final Socket client, Context context) {
        super(client);
        this.context = context;
        textureView = new TextureView(context);
        textureView = (TextureView)textureView.findViewById(R.id.textureView);
        textureView.setSurfaceTextureListener(this);

        DJICamera camera = PcApiApplication.getCameraInstance();
        if (camera != null) {
            camera.setDJICameraReceivedVideoDataCallback(
                    new DJICamera.CameraReceivedVideoDataCallback() {
                        @Override
                        public void onResult(byte[] bytes, int length) {
                            if(mCodecManager != null){
                                // Send the raw H264 video data to codec manager for decoding
                                mCodecManager.sendDataToDecoder(bytes, length);
                            }else {
                                Logger.log("VideoWriterViaSurfaceTextureStrategy: mCodecManager is null");
                            }
                        }
                    });
        }

        Logger.log("VideoViaSurfaceTexture: created");
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

    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        if (mCodecManager == null) {
            mCodecManager = new DJICodecManager(textureView.getContext(), surface, width, height);
        }
    }

    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {

    }

    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        if (mCodecManager != null) {
            mCodecManager.cleanSurface();
            mCodecManager = null;
        }
        return false;
    }

    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {
        //here I should get frame from texture and send it.
        if (frameNumber % 50 == 0){
            Logger.showToast("Received frame with number " + frameNumber);

        }
        frameNumber++;
    }
}
