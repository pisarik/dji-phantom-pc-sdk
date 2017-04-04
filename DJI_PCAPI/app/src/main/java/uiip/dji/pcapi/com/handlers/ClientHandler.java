package uiip.dji.pcapi.com.handlers;

import android.content.Context;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.util.concurrent.TimeoutException;

import uiip.dji.pcapi.com.Logger;

/**
 * Created by dji on 16.11.2016.
 */

public class ClientHandler extends Thread{
    private Socket client = null;
    private HandleStrategy handler = null;
    private final Context context;

    public ClientHandler(Socket client, Context context){
        this.client = client;
        this.context = context;
    }

    @Override
    public void run(){
        dispatch();

        if (handler != null){
            handler.start();
        }
        else{
            Logger.log("Not supported socket type");
            try{
                sendMessageToClient("Not supported socket_type");
                client.close();
            } catch (IOException e) {
                //no connection
            }
        }

        if (!client.isClosed()) {
            try {
                client.close();
            } catch (IOException e) {
                Logger.log("Client Handler: " + e.getMessage());
            }
        }
    }

    private void dispatch() {
        try {
            BufferedReader in =
                    new BufferedReader(
                            new InputStreamReader(client.getInputStream()));

            client.setSoTimeout(30000);

            String socket_type = in.readLine();
            HandleStrategyFactoryMethod factory =
                    new HandleStrategyFactoryMethod();
            handler = factory.getAlgo(socket_type, client, context);
        }
        catch (IOException e) {
            Logger.log("Time for specify type expired");
            try{
                sendMessageToClient("Choosing time expired");
                client.close();
            } catch (IOException er) {
                //no connection
            }
        }
    }

    private void sendMessageToClient(String msg) throws IOException {
        PrintWriter writer = new PrintWriter(client.getOutputStream());
        writer.write(msg);
        writer.flush();
    }
}
