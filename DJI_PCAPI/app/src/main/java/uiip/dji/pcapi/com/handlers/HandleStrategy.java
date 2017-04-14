package uiip.dji.pcapi.com.handlers;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.util.concurrent.TimeoutException;

import uiip.dji.pcapi.com.Logger;

/**
 * Created by dji on 16.11.2016.
 */

abstract class HandleStrategy {
    protected Socket client = null;
    protected Thread asyncWriteThread = null;

    protected abstract void readMessage(InputStream istream) throws IOException;
    protected abstract void writeMessage(OutputStream ostream) throws IOException;
    protected abstract boolean isNeedRead();
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
        if (isNeedReadWrite()){
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
                if (isNeedReadWrite() || isNeedRead()) {
                    //Main thread for reading
                    readMessage(client.getInputStream());
                }
                else if (isNeedWrite()) {
                    //if only write need main thread for writing
                    writeMessage(client.getOutputStream());
                }
                else { //job can be done via callbacks as video writer
                    //just wait in block
                    client.getInputStream().read();
                }
            }
        } catch (IOException e) {
            //no connection
            Logger.log("Strategy: " + e.getMessage());
        }

        if (isNeedReadWrite()){
            asyncWriteThread.interrupt();
        }
        interrupt();
    }

    boolean isNeedReadWrite(){
        return isNeedRead() && isNeedWrite();
    }
}
