package org.cf.nativeharness;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("str-crypt");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView tv = (TextView) findViewById(R.id.hello_textview);

        byte[] encryptedStringBytes = new byte[]{40, 65, 24, 16, 4, 27, 9, 65, 24, 31, 75, 16, 91, 69, 19, 31, 4, 31, 30, 71, 18, 22, 65, 93, 24, 69, 13, 15, 77, 11, 30, 4, 18, 29, 4, 4, 20, 81, 15, 91, 64, 24, 8, 77, 15, 30, 87, 83, 91, 119, 24, 30, 79, 93, 31, 77, 14, 24, 77, 13, 23, 77, 19, 30, 4, 28, 21, 64, 93, 29, 77, 19, 31, 4, 4, 20, 81, 15, 91, 72, 20, 25, 65, 15, 15, 93, 83};
        Cryptor c = Cryptor.getInstance();
        tv.setText(c.decryptString(new String(encryptedStringBytes)));
    }

}
