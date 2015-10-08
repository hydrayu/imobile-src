/*
 * Copyright 2009 Cedric Priscal
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */

package android_serialport_api.sample;

import java.io.IOException;

import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;

public class ConsoleActivity extends SerialPortActivity {
	private long lastPauseTime;
	EditText mReception;
	Handler handler = new Handler();
	int Po_delay = 0;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.console);

		setTitle("Test BT!!");
		mReception = (EditText) findViewById(R.id.EditTextReception);

		EditText Emission = (EditText) findViewById(R.id.EditTextEmission);
		Emission.setOnEditorActionListener(new OnEditorActionListener() {
			public boolean onEditorAction(TextView v, int actionId,
					KeyEvent event) {
				int i;
				CharSequence t = v.getText();
				char[] text = new char[t.length()];
				for (i = 0; i < t.length(); i++) {
					text[i] = t.charAt(i);
				}
				try {
					mOutputStream.write('\r');
					mOutputStream.write(new String(text).getBytes());
					mOutputStream.write('\n');
				} catch (IOException e) {
					e.printStackTrace();
				}
				return false;
			}
		});
	}

	@Override
	protected void onDataReceived(final byte[] buffer, final int size) {
		runOnUiThread(new Runnable() {
			public void run() {
				if (mReception != null) {

					mReception.setText(new String(buffer, 0, size));
//					Log.i("Po", new String(buffer, 0, size));

				}
			}
		});
	}

	/***
	 * Po Button 01 id=Po_BTN_01
	 */
	public void Po_BTN_01(View view) {
		handler.postDelayed(Po_BTN_01_runnable, 1000);

		Log.d("Po_BTN_01", "set msg");

	}

	/***
	 * Po_BTN_02 exit module
	 */
	public void Po_BTN_02(View view) {
		Log.d("Po_BTN_02", "+");
		String Po_msg01 = "+++";

		try {
			mOutputStream.write(Po_msg01.getBytes());
			// mOutputStream.write(Po_msg01.getBytes());
			// mOutputStream.write(Po_msg01.getBytes());
			handler.postDelayed(Po_BTN_02_runnable, 1000);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			Log.e("Po_error", "Error");
		}
	}

	/***
	 * Po_BTN_03 exit module
	 */
	public void Po_BTN_03(View view) {
		Log.d("Po_BTN_03", "+");
		String Po_msg01 = "+++";

		try {
			mOutputStream.write(Po_msg01.getBytes());
			handler.postDelayed(Po_BTN_03_runnable, 1000);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			Log.e("Po_error", "Error");
		}
	}
	
	/**
	 * 
	 */
	Runnable Po_BTN_01_runnable = new Runnable() {
		@Override
		public void run() {
			handler.postDelayed(this, 2000);
			// if delay 5 time off dell
			Po_delay++;

			String Po_msg01 = " SET PROFILE SPP ON\n";
			String Po_msg02 = " SET BT AUTH * 0000\n";
			String Po_msg03 = " RESET\n";
			String Po_msg04 = " INQUIRY 3 NAME\n";
			//String Po_msg05 = " CALL 00:07:80:0a:69:b2 1101 RFCOMM\n";
			String Po_msg05 = " CALL 00:07:80:0a:69:ae 1101 RFCOMM\n";//The foreign Customers
			//10-23 14:41:04.423: I/Po(2440): NAME 00:07:80:0a:69:b2 "imobile03"

			try {
				switch (Po_delay) {
				case 1:
					Log.d("runnable", "set msg 1");
					mOutputStream.write(Po_msg01.getBytes());
					break;
				case 2:
					Log.d("runnable", "set msg 2");
					mOutputStream.write(Po_msg02.getBytes());
					break;
				case 3:
					Log.d("runnable", "set msg 3");
					mOutputStream.write(Po_msg03.getBytes());
					break;
//				 case 4:
//				 Log.d("runnable", "set msg 4");
//				 mOutputStream.write(Po_msg04.getBytes());
//				 break;
				case 5:
					Log.d("runnable", "set msg 5");
					mOutputStream.write(Po_msg05.getBytes());
					break;
				default:
					break;
				}

			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				Log.e("Po_error", "Error");
			}

			// Log.d("runnable", "delay="+Po_delay);
			if (Po_delay >= 5) {
				handler.removeCallbacks(Po_BTN_01_runnable);
				Po_delay = 0;
			}
		}
	};

	Runnable Po_BTN_02_runnable = new Runnable() {
		@Override
		public void run() {
			// TODO Auto-generated method stub
			Log.d("Po_BTN_02_runnable", "Po_BTN_02_runnable()");
			String Po_msg = "\nclose 0\n";
			try {
				mOutputStream.write(Po_msg.getBytes());
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	};
	
	/**
	 * 
	 */
	Runnable Po_BTN_03_runnable = new Runnable() {
		@Override
		public void run() {
			handler.postDelayed(this, 2000);
			// if delay 5 time off dell
			Po_delay++;

			String Po_msg01 = " list\n";
			String Po_msg02 = " rssi 0\n";
			
			try {
				switch (Po_delay) {
				case 1:
					Log.d("runnable", "set msg 1");
					mOutputStream.write(Po_msg01.getBytes());
					break;
				case 2:
					Log.d("runnable", "set msg 2");
					mOutputStream.write(Po_msg02.getBytes());
					break;
				default:
					break;
				}

			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				Log.e("Po_error", "Error");
			}

			// Log.d("runnable", "delay="+Po_delay);
			if (Po_delay >= 2) {
				handler.removeCallbacks(Po_BTN_03_runnable);
				Po_delay = 0;
			}
		}
	};

}
