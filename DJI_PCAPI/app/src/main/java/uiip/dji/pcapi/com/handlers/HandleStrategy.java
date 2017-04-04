package uiip.dji.pcapi.com.handlers;

import android.widget.Toast;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
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
    protected Socket client = null;
    protected Thread asyncWriteThread = null;

    protected abstract void readMessage(InputStream istream);
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
            while (client.isConnected()) {
                readMessage(client.getInputStream());
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
