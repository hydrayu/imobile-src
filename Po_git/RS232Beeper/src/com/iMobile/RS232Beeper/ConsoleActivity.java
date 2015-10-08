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

package com.iMobile.RS232Beeper;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.security.InvalidParameterException;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.media.AudioManager;
import android.media.ToneGenerator;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;
import android_serialport_api.SerialPort;

public class ConsoleActivity extends Activity {
	private long lastPauseTime;
	EditText mReception;
	private String TAG="serial_port";
    protected Application mApplication;
    protected SerialPort mSerialPort;
    protected OutputStream mOutputStream;
    private InputStream mInputStream;
    private ReadThread mReadThread;

    private class ReadThread extends Thread {

    	@Override
    	public void run() {
    		super.run();
    		while(!isInterrupted()) {
    			int size;
    			try {
    				byte[] buffer = new byte[64];
    				if (mInputStream == null) return;
    				size = mInputStream.read(buffer);
    				if (size > 0) {
    					onDataReceived(buffer, size);
    				}
    			} catch (IOException e) {
    				e.printStackTrace();
    				return;
    			}
    		}
    	}
    }
    private void DisplayError(int resourceId) {
    	AlertDialog.Builder b = new AlertDialog.Builder(this);
    	b.setTitle("Error");
    	b.setMessage(resourceId);
    	b.setPositiveButton("OK", new OnClickListener() {
    		public void onClick(DialogInterface dialog, int which) {
    			ConsoleActivity.this.finish();
    		}
    	});
    	b.show();
    }
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
        mApplication = (Application) getApplication();
        try {
                mSerialPort = mApplication.getSerialPort();
                mOutputStream = mSerialPort.getOutputStream();
                mInputStream = mSerialPort.getInputStream();

                /* Create a receiving thread */
                mReadThread = new ReadThread();
                mReadThread.start();
        } catch (SecurityException e) {
                DisplayError(R.string.error_security);
        } catch (IOException e) {
                DisplayError(R.string.error_unknown);
        } catch (InvalidParameterException e) {
                DisplayError(R.string.error_configuration);
        }

		setContentView(R.layout.console);

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
					mOutputStream.write(new String(text).getBytes());
					mOutputStream.write('\n');
				} catch (IOException e) {
					e.printStackTrace();
				}
				return false;
			}
		});
	}

	protected void onDataReceived(final byte[] buffer, final int size) {
		runOnUiThread(new Runnable() {
			public void run() {
				if (mReception != null) {
					mReception.append(new String(buffer, 0, size));
					long time = System.currentTimeMillis();
					if (time - lastPauseTime > 500) {
						Log.d(TAG, "ConsoleActivity/onDataReceived() beep! beep! beep!");
						ToneGenerator tg = new   ToneGenerator(AudioManager.STREAM_DTMF,ToneGenerator.MAX_VOLUME);
				        tg.startTone(ToneGenerator.TONE_DTMF_1, 200);
					}
					lastPauseTime = time;

				}
			}
		});
	}
	protected void onDestroy() {
		if (mReadThread != null)
			mReadThread.interrupt();
		mApplication.closeSerialPort();
		mSerialPort = null;
		super.onDestroy();
	}
}
