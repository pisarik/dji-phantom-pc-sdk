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
            try{
                PrintWriter writer = new PrintWriter(client.getOutputStream());
                writer.write("Not supported socket_type");
                writer.flush();
                client.close();
            } catch (IOException e) {
                //no connection
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
            try{
                Logger.log("Time for specify type expired");
                PrintWriter writer = new PrintWriter(client.getOutputStream());
                writer.write("Choosing time expired");
                writer.flush();
                client.close();
            } catch (IOException er) {
                //no connection
            }
        }
    }
}
