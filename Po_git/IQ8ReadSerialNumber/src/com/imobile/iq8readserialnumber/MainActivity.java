package com.imobile.iq8readserialnumber;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;

import com.imobile.iq8_readserialnumber.R;

public class MainActivity extends Activity {

	final String TAG = "Po_debug";
	private TextView TV1;
	static {
		System.loadLibrary("iMobileReadSerialNumber");
	}
	
	private String Read_SN() {
		
		String SN_number = "";
		
		SN_number = iMobileReadSerialNumber.ReadSN();

		Log.i(TAG, SN_number);
		// not data return "Not SN"
		if (SN_number == null || SN_number == "" || (SN_number.endsWith("en_US"))) {
			SN_number = "Not SN";
			return SN_number;
		}
		return SN_number;
	}
	
	private void find_id(){
		this.TV1 = (TextView) findViewById(R.id.Po_TV1);
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		find_id();
		
		// show SN to TV1
		String SN= Read_SN();
		this.TV1.setText(SN);
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
}
