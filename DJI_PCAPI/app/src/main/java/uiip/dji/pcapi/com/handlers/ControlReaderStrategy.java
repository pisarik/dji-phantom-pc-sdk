package uiip.dji.pcapi.com.handlers;

import java.io.IOException;
import java.io.InputStream;
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
    protected void readMessage(InputStream istream) throws IOException {

    }

    @Override
    protected void writeMessage(OutputStream ostream) throws IOException {

    }

    @Override
    protected boolean isNeedWrite() {
        return false;
    }

    @Override
    protected void initialize() {

    }

    @Override
    protected void interrupt() {

    }
}
