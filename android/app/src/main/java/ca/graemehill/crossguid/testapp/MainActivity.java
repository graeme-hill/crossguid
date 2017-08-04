package ca.graemehill.crossguid.testapp;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

import java.util.concurrent.CountDownLatch;

public class MainActivity extends Activity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        final TextView textView = (TextView)findViewById(R.id.mainTextView);
        textView.setText(test());

        final TextView javaThreadView = (TextView)findViewById(R.id.javaThreadView);
        javaThreadView.setText(createGuidFromJavaThread());

        final TextView nativeThreadView = (TextView)findViewById(R.id.nativeThreadView);
        nativeThreadView.setText(createGuidFromNativeThread());
    }

    public native String test();

    private static class StringCapture {
        private String value;

        public String getValue() {
            return value;
        }

        public void setValue(String value) {
            this.value = value;
        }
    }

    public String createGuidFromJavaThread() {
        final CountDownLatch created = new CountDownLatch(1);
        final StringCapture result = new StringCapture();
        new Thread(new Runnable() {
            @Override
            public void run() {
                result.setValue(newGuid());
                created.countDown();
            }
        }).start();
        try {
            created.await();
        } catch (InterruptedException e) {
            return "Could not get value: " + e.getMessage();
        }
        return result.getValue();
    }

    public native String newGuid();

    public native String createGuidFromNativeThread();
    static {
        System.loadLibrary("crossguidtest");
    }
}
