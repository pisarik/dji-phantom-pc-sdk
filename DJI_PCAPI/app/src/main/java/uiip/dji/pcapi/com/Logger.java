package uiip.dji.pcapi.com;

import android.os.Handler;
import android.os.Looper;
import android.widget.EditText;
import android.widget.Toast;

/**
 * Created by dji on 17.11.2016.
 */

public class Logger {
    public static EditText logs = null;
    public static MainActivity mainActivity = null;
    private static Handler uiHandler = new Handler();

    public static void log(final String message){
        synchronized (logs) {
            if (Looper.getMainLooper().getThread() == Thread.currentThread()) {
                System.out.println("UI thread: " + message);
                logs.append(message + "\n");
            } else {
                System.out.println("Another thread: " + message);
                boolean isAdded = uiHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        System.out.println("INRUNNABLE " + message);
                        logs.append(message + "\n");
                    }
                });
                System.out.println("isAdded: " + isAdded);
            }
        }
    }

    public static void showToast(final String msg) {
        uiHandler.post(new Runnable() {
            public void run() {
                Toast.makeText(mainActivity, msg, Toast.LENGTH_SHORT).show();
            }
        });
    }
}
