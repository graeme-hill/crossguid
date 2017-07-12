package ca.graemehill.crossguid.testapp;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import java.util.UUID;

public class MainActivity extends Activity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        final TextView textView = (TextView)findViewById(R.id.mainTextView);
        textView.setText(test());
    }

    public native String test();

    static {
        System.loadLibrary("crossguidtest");
    }
}
