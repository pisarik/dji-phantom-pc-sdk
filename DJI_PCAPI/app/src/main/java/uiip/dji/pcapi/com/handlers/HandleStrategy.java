package uiip.dji.pcapi.com.handlers;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.SocketException;

import uiip.dji.pcapi.com.Logger;

/**
 * Created by dji on 16.11.2016.
 */

abstract class HandleStrategy {
    protected Socket client = null;
    protected Thread asyncWriteThread = null;

    protected abstract void readMessage(InputStream istream) throws IOException;
    protected abstract void writeMessage(OutputStream ostream) throws IOException;
    protected abstract boolean isNeedWrite();
    protected abstract void initialize();
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
        initialize();
        if (isNeedWrite()){
            asyncWriteThread = new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        while (client.isConnected()) {
                            writeMessage(client.getOutputStream());
                        }
                    } catch (IOException e) {
                        if (!client.isClosed()) {
                            try {
                                client.close();
                            } catch (IOException e1) {
                                Logger.log("WriteThread: " + e.getMessage());
                            }
                        }
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
        }

        if (isNeedWrite()){
            asyncWriteThread.interrupt();
        }
        interrupt();
    }
}
