package uiip.dji.pcapi.com;

import android.content.Context;
import android.net.wifi.WifiManager;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import android.net.wifi.*;
import android.text.format.Formatter;

import uiip.dji.pcapi.com.handlers.ClientHandler;

/**
 * Created by dji on 16.11.2016.
 */

public class Server extends Thread {

    private final int SERVER_PORT = 1212;
    private ServerSocket serverSocket = null;
    private WifiManager wifiManager = null;
    private final Context context;

    public Server(WifiManager manager, Context context){
        this.context = context;
        wifiManager = manager;
    }

    @Override
    public void run(){
        try {
            serverSocket = new ServerSocket(SERVER_PORT);

            Logger.log("Server started");
            Logger.log(getWiFiIP() + ":" + SERVER_PORT);
            Logger.log("Waiting for clients...");
            Socket client = null;
            while (true) {
                try {
                    client = serverSocket.accept();
                    Logger.log("New client accepted");
                    (new ClientHandler(client, context)).start();
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        } catch (IOException e) {
            Logger.log("Cannot start server!");
            e.printStackTrace();
        }

    }

    public String getWiFiIP(){
        WifiInfo wifiInfo = wifiManager.getConnectionInfo();

        return Formatter.formatIpAddress(wifiInfo.getIpAddress());
    }

}
