package uiip.dji.pcapi.com.handlers;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

/**
 * Created by dji on 17.11.2016.
 */

class ControlReaderStrategy extends HandleStrategy{
    ControlReaderStrategy(Socket client) {
        super(client);
    }

    @Override
    protected void readMessage(BufferedReader reader) {
        try {
            System.out.println("I got message: " + reader.readLine());
        } catch (IOException e) {
            throw new IllegalArgumentException("ControlReader: cannot readLine");
        }
    }

    @Override
    protected void writeMessage(OutputStream ostream) {

    }

    @Override
    protected boolean isNeedWrite() {
        return false;
    }

    @Override
    protected void doJob() {

    }

    @Override
    protected void interrupt() {

    }
}
