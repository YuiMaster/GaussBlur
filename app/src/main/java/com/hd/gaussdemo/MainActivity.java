package com.hd.gaussdemo;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.widget.TextView;

import com.hd.gaussdemo.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("native-lib");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        binding.imageView1.setImageResource(R.mipmap.ic_test);
        Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.mipmap.ic_test);
        gaussBlur(bitmap);
        binding.imageView2.setImageBitmap(bitmap);
    }

    public native void gaussBlur(Bitmap bitmap);
}