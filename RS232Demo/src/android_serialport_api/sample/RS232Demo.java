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
import java.security.InvalidParameterException;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android_serialport_api.SerialPort;

public class RS232Demo extends Activity {
	protected Application mApplication;
	protected SerialPort mSerialPort; 
	private static String TAG = "serial_port";

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        final Button buttonSetup = (Button)findViewById(R.id.ButtonSetup);
        buttonSetup.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				startActivity(new Intent(RS232Demo.this, SerialPortPreferences.class));
			}
		});

        final Button buttonConsole = (Button)findViewById(R.id.ButtonConsole);
        buttonConsole.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				startActivity(new Intent(RS232Demo.this, ConsoleActivity.class));
			}
		});

        final Button buttonLoopback = (Button)findViewById(R.id.ButtonLoopback);
        buttonLoopback.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				startActivity(new Intent(RS232Demo.this, LoopbackActivity.class));
			}
		});

        final Button button01010101 = (Button)findViewById(R.id.Button01010101);
        button01010101.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				startActivity(new Intent(RS232Demo.this, Sending01010101Activity.class));
			}
		});

        final Button buttonSetXon = (Button)findViewById(R.id.ButtonSetXon);
		if (mSerialPort != null) {
			if (mSerialPort.XON == 1) buttonSetXon.setText(R.string.SetXon);
			else buttonSetXon.setText(R.string.SetXon2);
		}
        buttonSetXon.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				if (mApplication == null) {
					mApplication = (Application) getApplication();
					Log.d(TAG, "get mApplication()");
				}
				if (mSerialPort == null) {
					try {
						mSerialPort = mApplication.getSerialPort();
					} catch (InvalidParameterException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					} catch (SecurityException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
				if (mSerialPort == null) {
					Log.d(TAG, "mSerialPort is null");
					return;
				}
				else Log.d(TAG, "mSerialPort not null");

				if (mSerialPort.XON == 1) {
					Log.d(TAG, "Call setXon(0)");
					mSerialPort.setXon(0);
					mSerialPort.XON = 0;
					buttonSetXon.setText(R.string.SetXon2);
				}
				else {
					Log.d(TAG, "Call setXon(0)");
					mSerialPort.setXon(1);
					mSerialPort.XON = 1;
					buttonSetXon.setText(R.string.SetXon);
				}
			}
		});

        final Button buttonSetRtsCts = (Button)findViewById(R.id.ButtonSetRtsCts);
		if (mSerialPort != null) {
			if (mSerialPort.RTSCTS == 1) buttonSetRtsCts.setText(R.string.SetRtsCts);
			else buttonSetRtsCts.setText(R.string.SetRtsCts2);
		}
        buttonSetRtsCts.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				if (mSerialPort.RTSCTS == 1) {
					mSerialPort.setRtsCts(0);
					mSerialPort.RTSCTS = 0;
					buttonSetRtsCts.setText(R.string.SetRtsCts2);
				}
				else {
					mSerialPort.setRtsCts(1);
					mSerialPort.RTSCTS = 1;
					buttonSetRtsCts.setText(R.string.SetRtsCts);
				}
			}
		});

        final Button buttonSetDtr = (Button)findViewById(R.id.ButtonSetDtr);
		if (mSerialPort != null) {
			if (mSerialPort.DTR == 1) buttonSetDtr.setText(R.string.SetDtr);
			else buttonSetDtr.setText(R.string.SetDtr2);
		}
        buttonSetDtr.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				if (mSerialPort.DTR == 1) {
					mSerialPort.setDtr(0);
					mSerialPort.DTR = 0;
					buttonSetDtr.setText(R.string.SetDtr2);
				}
				else {
					mSerialPort.setDtr(1);
					mSerialPort.DTR = 1;
					buttonSetDtr.setText(R.string.SetDtr);
				}
			}
		});

        final Button buttonSetDsr = (Button)findViewById(R.id.ButtonSetDsr);
		if (mSerialPort != null) {
			if (mSerialPort.DSR == 1) buttonSetDsr.setText(R.string.SetDsr);
			else buttonSetDsr.setText(R.string.SetDsr2);
		}
        buttonSetDsr.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				if (mSerialPort.DSR == 1) {
					mSerialPort.setDsr(0);
					mSerialPort.DSR = 0;
					buttonSetDsr.setText(R.string.SetDsr2);
				}
				else {
					mSerialPort.setDsr(1);
					mSerialPort.DSR = 1;
					buttonSetDsr.setText(R.string.SetDsr);
				}
			}
		});

        final Button buttonAbout = (Button)findViewById(R.id.ButtonAbout);
        buttonAbout.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				AlertDialog.Builder builder = new AlertDialog.Builder(RS232Demo.this);
				builder.setTitle("About");
				builder.setMessage(R.string.about_msg);
				builder.show();
			}
		});

        final Button buttonQuit = (Button)findViewById(R.id.ButtonQuit);
        buttonQuit.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				RS232Demo.this.finish();
			}
		});
    }
}
