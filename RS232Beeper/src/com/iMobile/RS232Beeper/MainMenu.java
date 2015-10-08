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

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class MainMenu extends Activity {
	public static Handler mUiHandler = null;
	private String TAG = "serial_port";
	private static final int queryStartStop = 0;
	
	private void sendQueryToService(int q)
	{
		if (null != BeepService.mMyServiceHandler) {
			Message msg = new Message();
			msg.what = q;
			msg.obj = ""; // don't need here
			BeepService.mMyServiceHandler.sendMessage(msg);
		}
	}
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        Log.d(TAG, "MainMenu/onCreate()");
        
        mUiHandler = new Handler() // Receive messages from service class 
        {
        	public void handleMessage(Message msg)
        	{
        		switch(msg.what)
        		{
        		case queryStartStop:
        			// add the status which came from service and show on GUI
        			Log.d(TAG, msg.obj.toString());
        			if (msg.obj.toString() == "running") {
        				Button buttonStart = (Button)findViewById(R.id.ButtonStart);
        				buttonStart.setEnabled(false);
        				Button buttonStop = (Button)findViewById(R.id.ButtonStop);
        				buttonStop.setEnabled(true);
        			}
        			else {
        				Button buttonStart = (Button)findViewById(R.id.ButtonStart);
        				buttonStart.setEnabled(true);
        				Button buttonStop = (Button)findViewById(R.id.ButtonStop);
        				buttonStop.setEnabled(false);
        			}
        			break;

        		default:
        			break;
        		}
        	}
        };
		startService(new Intent(this, BeepService.class));
    }
    public void onClickConsole(View src) {
    	stopService(new Intent(this, BeepService.class));
    	startActivity(new Intent(MainMenu.this, ConsoleActivity.class));
    }
    public void onClickSetup(View src) {
    	startActivity(new Intent(MainMenu.this, SerialPortPreferences.class));
    }
    public void onClickStartService(View src) {
    	startService(new Intent(this, BeepService.class));
    }
    public void onClickStopService(View src) {
    	stopService(new Intent(this, BeepService.class));
    }
    public void onClickQuit(View src) {
    	MainMenu.this.finish();
    }
}
