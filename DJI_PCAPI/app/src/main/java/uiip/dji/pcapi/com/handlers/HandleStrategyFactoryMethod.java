package uiip.dji.pcapi.com.handlers;

import android.content.Context;

import java.io.IOException;
import java.net.Socket;

import uiip.dji.pcapi.com.Logger;

/**
 * Created by dji on 16.11.2016.
 */

class HandleStrategyFactoryMethod {
    public HandleStrategy getAlgo(String algo_type, Socket client, Context context) throws IOException {
        HandleStrategy result = null;

        if (algo_type == null){
            throw new IOException("Algo type is null");
        }

        switch (algo_type){
            case "CONTROL_TYPE":
                Logger.log("CONTROL_TYPE obtained");
                result = new ControlReaderStrategy(client);
                break;
            case "TELEMETRY_TYPE":
                Logger.log("TELEMETRY_TYPE obtained");
                //result = new TelemetryWriterAlgo();
                break;
            case "VIDEO_STREAM_TYPE":
                Logger.log("VIDEO_STREAM_TYPE obtained");
                result = new VideoStreamWriterStrategy(client);
                break;
            case "VIDEO_YUV_TYPE":
                Logger.log("VIDEO_YUV_TYPE obtained");
                result = new VideoYuvWriterStrategy(client);
                break;
            case "VIDEO_VIA_SURFACE_TYPE":
                Logger.log("VIDEO_VIA_SURFACE_TYPE obtained");
                result = new VideoWriterViaSurfaceTextureStrategy(client, context);
                break;
            default:
                Logger.log("Unexpected type");
                result = null;
        }

        return result;
    }
}
