package uiip.dji.pcapi.com;

import android.net.wifi.WifiManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.TextureView;
import android.widget.Toast;
import android.widget.EditText;

import uiip.dji.pcapi.com.media.DJIVideoStreamDecoder;
import uiip.dji.pcapi.com.media.NativeHelper;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = MainActivity.class.getName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        NativeHelper.getInstance().init();

        setContentView(R.layout.activity_main);

        Logger.logs = (EditText)findViewById(R.id.logs_edit);
        Logger.mainActivity = this;

        TextureView textureView = (TextureView)findViewById(R.id.textureView);

        DJIVideoStreamDecoder.getInstance().init(getApplicationContext(), null);

        Server server =
                new Server((WifiManager)getSystemService(WIFI_SERVICE), textureView.getContext());
        server.start();
    }
}
