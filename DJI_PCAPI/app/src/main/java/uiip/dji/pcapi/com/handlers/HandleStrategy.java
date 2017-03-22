package uiip.dji.pcapi.com.handlers;

import android.widget.Toast;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;
import java.net.SocketException;

import uiip.dji.pcapi.com.Logger;
import uiip.dji.pcapi.com.MainActivity;

/**
 * Created by dji on 16.11.2016.
 */

abstract class HandleStrategy {
    private final String TERMINATE_STRING = "CLOSE_CONNECTION";
    protected Socket client = null;
    protected Thread asyncWriteThread = null;

    protected abstract void readMessage(BufferedReader reader);
    protected abstract void writeMessage(OutputStream ostream) throws IOException;
    protected abstract boolean isNeedWrite();
    protected abstract void doJob();
    protected abstract void interrupt();

    HandleStrategy(Socket client){
        this.client = client;
        try{
            this.client.setSoTimeout(0);
        } catch (SocketException e) {
            e.printStackTrace();
        }
    }

    public void start(){
        doJob();
        if (isNeedWrite()){
            asyncWriteThread = new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        while (client.isConnected()) {
                            writeMessage(client.getOutputStream());
                        }
                    } catch (IOException e) {
                        //no connection
                    }
                }
            });
            asyncWriteThread.start();
        }


        try {
            BufferedReader in =
                    new BufferedReader(
                            new InputStreamReader(client.getInputStream()));

            while (client.isConnected()) {
                in.mark(Integer.MAX_VALUE);
                String str = in.readLine();
                if (str != null) {
                    Logger.log("Readed String: " + str);
                }
                else {
                    Logger.log("Readed String: null");
                }
                if (str != null &&
                        !str.equals(TERMINATE_STRING)){
                    in.reset();
                    readMessage(in);
                }
                else{
                    client.close();
                    break;
                }
            }
        } catch (IOException e) {
            //no connection
        } finally {
            Logger.log("Closing connection");
            if (isNeedWrite()){
                asyncWriteThread.interrupt();
            }
            interrupt();
        }
    }
}
