package com.iMobile.RS232Beeper;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.security.InvalidParameterException;

import android.app.Service;
import android.content.Intent;
import android.content.SharedPreferences;
import android.media.AudioManager;
import android.media.ToneGenerator;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.os.Message;
import android.preference.PreferenceManager;
import android.util.Log;
import android_serialport_api.SerialPort;

public class BeepService extends Service {
	private static String TAG = "serial_port";
	public static Handler mMyServiceHandler = null;
	public static Boolean mIsServiceRunning = false;
	private static final int queryStartStop = 0;
	
	private long lastPauseTime;
	protected Application mApplication;
    protected SerialPort mSerialPort;
    protected OutputStream mOutputStream;
    private InputStream mInputStream;
    private MyThread myThread;
	
	@Override
	public IBinder onBind(Intent intent) {
		Log.d(TAG, "BeepService/onBind()");
		return null;
	}
	
	@Override
	public void onCreate() {
		Log.d(TAG, "BeepService/onCreate()");
	}
	
	@Override
	public void onStart(Intent intent, int startid) {
		Log.d(TAG, "BeepService/onStart()");
	}

	private SerialPort getSerialPort() throws SecurityException, IOException {
		/* Read serial port parameters */
		SharedPreferences sp = PreferenceManager.getDefaultSharedPreferences(this);
		String path = sp.getString("PORT", "");
		int baudrate = Integer.decode(sp.getString("BAUDRATE", "-1"));

		/* Check parameters */
		if ( (path.length() == 0) || (baudrate == -1)) {
			Log.d(TAG, "BeepService/getSerialPort(), no preference, using /dev/ttyUSB1 38400");
			mSerialPort = new SerialPort(new File("/dev/ttyUSB1"), 38400, 0);
		}
		else {
			Log.d(TAG, "BeepService/getSerialPort() : " + path + " / " + baudrate);
			mSerialPort = new SerialPort(new File(path), baudrate, 0);
		}
		return mSerialPort;
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		Log.d(TAG, "BeepService/onStartCommand(), call stop first..");

    	try {
    		Log.d(TAG, "BeepService/onStartCommand(), try to get serial port..");
    		mSerialPort = getSerialPort();
    		Log.d(TAG, "BeepService/onStartCommand() : " + mSerialPort.toString());
            mOutputStream = mSerialPort.getOutputStream();
            mInputStream = mSerialPort.getInputStream();
    	} catch (InvalidParameterException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

		mIsServiceRunning = true; // set service running status = true

		if (null == myThread) myThread = new MyThread();
		if (!myThread.isAlive()) myThread.start();

		try
		{
			Thread.sleep(200);
		}
		catch (InterruptedException e)
		{
			e.printStackTrace();
		}

		sendMsgToActivity(queryStartStop);

		return START_STICKY;
	}

	@Override
	public void onDestroy() {
		Log.d(TAG, "BeepService/onDestroy()");
		mIsServiceRunning = false;
		if (mSerialPort != null) {
			Log.d(TAG, "BeepService/onDestroy(), close serial port");
			mSerialPort.close();
			mSerialPort = null;
			mInputStream = null;
			mOutputStream = null;
		}
		if (myThread != null) {
			Log.d(TAG, "BeepService/onDestroy(), stop myThread");
			myThread.interrupt();
			myThread = null;
		}
		sendMsgToActivity(queryStartStop);
	}
	private void sendMsgToActivity(int m)
	{
        if (m == queryStartStop) {
        	if(null != MainMenu.mUiHandler)
        	{
        		Message msg = new Message();
        		msg.what = queryStartStop;
        		if(true == mIsServiceRunning)
        			msg.obj  = "running";
        		else
        			msg.obj  = "not running";

        		MainMenu.mUiHandler.sendMessage(msg);
        	}
        }
	}

	class MyThread extends Thread
	{
		private static final String INNER_TAG = "MyThread";
		public void run()
		{
			super.run(); // wade
			this.setName(INNER_TAG);
			Looper.prepare();
			mMyServiceHandler = new Handler()
			{
				public void handleMessage(Message msg)
				{
					Log.i(TAG,"handleMessage(Message msg)" );
					switch(msg.what)
					{
					case 0: 
						sendMsgToActivity(queryStartStop);

						break;

					default:
						break;
					}
				}
			};
			while (mIsServiceRunning && !isInterrupted()) {
				int size;
				try {
					byte[] buffer = new byte[64];
					if (null == mInputStream) return;
					if ((size = mInputStream.read(buffer)) > 0) {
						long t = System.currentTimeMillis();
						if (mIsServiceRunning && (t - lastPauseTime > 500)) {
							Log.d(TAG, "BeepService/Thread, beep!!! mIsServiceRunning " + mIsServiceRunning);
							// ToneGenerator tg = new   ToneGenerator(AudioManager.STREAM_DTMF,ToneGenerator.MAX_VOLUME);
							ToneGenerator tg = new   ToneGenerator(AudioManager.STREAM_ALARM, ToneGenerator.MAX_VOLUME);
                            tg.startTone(ToneGenerator.TONE_DTMF_1, 200);
						}
						lastPauseTime = t;
					}
				}
				catch (IOException e) {
					e.printStackTrace();
					return;
				}
			}
			Looper.loop();
		}
	}
}