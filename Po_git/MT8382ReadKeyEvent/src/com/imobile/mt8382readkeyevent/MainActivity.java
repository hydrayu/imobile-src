package com.imobile.mt8382readkeyevent;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.widget.TextView;

public class MainActivity extends Activity {

	final String TAG = "Po_ReadKey";
	final int F13 = 183; // 0x00b7
	final int F14 = 184; // 0x00b8
	final int F15 = 185; // 0x00b9
	final int F16 = 186; // 0x00ba
	TextView Po_show_key;
	private Thread Updata_UI;
	int PoKeyCode;
	final String Po_Path = "/proc/kpd";

	static {
		System.loadLibrary("imobileJNI");
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		Po_findID();
		
	}

	private void Po_findID() {
		Po_show_key = (TextView) findViewById(R.id.Po_show_key);
		// setting TextView
		this.Po_show_key.setText("You not press button");
	}

	/*
	 * private Handler mHandler = new Handler() {
	 * 
	 * public void handleMessage(Message msg) {
	 * 
	 * super.handleMessage(msg);
	 * 
	 * Log.d(TAG, "msg=" + msg.arg1); if (msg != null) { if (msg.arg1 != 0) {
	 * Po_show_key.setText("You press button " + msg.arg1); } else {
	 * Po_show_key.setText("You not press button"); } }
	 * 
	 * }
	 * 
	 * };
	 * 
	 * class Updata_UI extends Thread {
	 * 
	 * @Override public void run() { // TODO Auto-generated method stub
	 * super.run(); try { while (true) { Log.d(TAG, "Updata_UI"); Message msg =
	 * new Message();
	 * 
	 * switch (PoKeyCode) { case F13: msg.arg1 = 1; case F14: msg.arg1 = 2; case
	 * F15: msg.arg1 = 3; case F16: msg.arg1 = 4; default: msg.arg1 = 0; }
	 * 
	 * mHandler.sendMessage(msg); Thread.sleep(1000); } } catch
	 * (InterruptedException e) { Log.d("TAG", "Thread was inturrupted"); }
	 * catch (Exception e) { e.printStackTrace(); Log.d("TAG",
	 * "Thread was Exception"); } } }
	 */

	public String Po_split_keycode(String keyevent) {
		Log.d(TAG, "Po_split_keycode start");
		// Log.d(TAG, keyevent);
		String last_code;
		// 1. splite ,
		String[] AfterSplit = keyevent.split(",");
		// for (int i = 0; i < AfterSplit.length; i++) {
		// Log.d(TAG, "AfterSplit[" + i + "]=" + AfterSplit[i]);
		// }
		if (AfterSplit.length == 10) {
			// Log.d(TAG, "AfterSplit[2]=" + AfterSplit[2]);
			// 2. split kernel keycode
			last_code = AfterSplit[2];
			AfterSplit = last_code.split("=");
			if (AfterSplit.length == 2) {
				// Log.d(TAG, "AfterSplit[1]=" + AfterSplit[1]);
				last_code = AfterSplit[1];
			} else {
				return "ERROR";
			}
		} else {
			return "ERROR";
		}

		return last_code;
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		Log.d(TAG, "start onKeyDown ok");
		// Po_split_keycode(event.toString());
		// Log.d(TAG, "Po_split_keycode = " +
		// Po_split_keycode(event.toString()));
		String Po_Keycode = Po_split_keycode(event.toString());
		if (Po_Keycode.equals("ERROR")) {
			Log.d(TAG, "not thing");
		} else {
			// show UI
			PoKeyCode = Integer.parseInt(Po_Keycode);
			Log.d(TAG, "PoKeyCode = " + PoKeyCode);
			switch (PoKeyCode) {
			case F13:
				Po_show_key.setText("You press button 1");
				return true;
			case F14:
				Po_show_key.setText("You press button 2");
				return true;
			case F15:
				Po_show_key.setText("You press button 3");
				return true;
			case F16:
				Po_show_key.setText("You press button 4");
				return true;
			default:
				Po_show_key.setText("You not press button");
			}
		}

		// *** call standard android API ***//
		// switch (keyCode) {
		// case KeyEvent.KEYCODE_F1:
		// // your Action code
		// Log.d(TAG, "F1");
		// return true;
		// default:
		// // Log.d(TAG, "keycode ->"+keyCode);
		// }
		return super.onKeyDown(keyCode, event);
	}

}
