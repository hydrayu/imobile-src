package com.imobile.mt8382toggleadbandusb;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

public class MainActivity extends Activity {
	private TextView POTV1, POTV2;
	private ImageView POIV1;
	private ToggleButton tButton;
	private final int Default_config = 1;

	private final String TAG = "Po_dbg";

	static {
		System.loadLibrary("imobileJNI");
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		PO_findID();
		init();

		// toggle on/off event
		this.tButton.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(CompoundButton buttonView,
					boolean isChecked) {

				if (isChecked) {
					Log.d(TAG, "button on");
					show_dialog(
							"Warning",
							"USB host power will be disable!! (USB GPS, USB 3G, NFC e.t.c. function may not work)",
							Default_config);
					
				} else {
					
					// on USB power
					USBON();
					
					// show usb mode(USB/OTG)
					String SetUIText = ReadUSBStatus();
					POTV1.setText(SetUIText);
					
					// show picture
					POIV1.setImageResource(R.drawable.usb_hub);
					
					Log.d(TAG, "button off");
				}

			}
		});
	}
	
	/**
	 * this is a exit button, can close APP
	 * @param view
	 */
	public void Po_close(View view){
		Log.d(TAG, "exit this APP");
		android.os.Process.killProcess(android.os.Process.myPid());
        System.exit(1);
	}

	private void PO_findID() {
		this.POTV1 = (TextView) findViewById(R.id.POTV1);
		this.POTV2 = (TextView) findViewById(R.id.POTV2);
		this.tButton = (ToggleButton) findViewById(R.id.toggleButton1);
		this.POIV1 = (ImageView) findViewById(R.id.POIV1);
	}

	/**
	 * can read USB status(OTG/USB), through cat /proc/USBStatus
	 * 
	 * @return USB current status
	 */
	private String ReadUSBStatus() {
		String status = "";
		String ReturnUI = "";
		try {
			status = imobileJNI.ReadProc("/proc/USBStatus");
			Log.d(TAG, "status=" + status.toString());

			if (status.equals("USB") || status.equals("OTG")) {
				ReturnUI = status + " Model is currently";
			} else {
				ReturnUI = "Read USBStatus error!";
				return ReturnUI;
			}
		} catch (Exception e) {
			// TODO: handle exception
			ReturnUI = "Read USBStatus error!";
			return ReturnUI;
		}

		return ReturnUI;
	}
	
	/**
	 * can disable USB power(through echo USBOFF > "/proc/USBStatus")
	 * P.S if function fail will show "write USBOFF error" dialog
	 */
	private void USBOFF() {
		String status = "";
		try {
			status = imobileJNI.WriteProc("/proc/USBStatus", "USBOFF");
			Log.d(TAG, "status=" + status.toString());
			if (status.equals("xx")) {
				debug_toast("write USBOFF error");
			}
		} catch (Exception e) {
			// TODO: handle exception
			debug_toast("write USBOFF error");
		}
	}
	
	/**
	 * can enable USB power(through echo USBON > "/proc/USBStatus")
	 * if function fail will show "write USBOFF error" dialog
	 */
	private void USBON() {
		String status = "";
		try {
			status = imobileJNI.WriteProc("/proc/USBStatus", "USBON");
			Log.d(TAG, "status=" + status.toString());
			if (status.equals("xx")) {
				debug_toast("write USBON error");
			}
		} catch (Exception e) {
			// TODO: handle exception
			debug_toast("write USBON error");
		}
	}
	
	private void init() {
		// setting text size
		this.POTV1.setTextSize(36);
		this.POTV1.setTextColor(Color.BLACK);
		this.POTV2.setTextSize(30);
		this.POTV2.setTextColor(Color.BLACK);

		// show usb mode(USB/OTG)
		String SetUIText = ReadUSBStatus();
		this.POTV1.setText(SetUIText);
		this.POTV2.setText("OTG mode");

		// check status
		// Log.d(TAG, "status = "+SetUIText.substring(0, 3));
		if (SetUIText.substring(0, 3).equals("USB")) {
			
			this.tButton.setChecked(false);
			
			// show picture
			this.POIV1.setImageResource(R.drawable.usb_hub);
			
		} else if (SetUIText.substring(0, 3).equals("OTG")) {
			
			this.tButton.setChecked(true);
			// show picture
			this.POIV1.setImageResource(R.drawable.otg_600web);
			
		}

	}

	/***
	 * ≈„•‹diglog
	 * 
	 * @param Title
	 * @param Msg
	 * @param event
	 */
	public void show_dialog(String Title, String Msg, int Po_event) {
		AlertDialog.Builder dialog = new AlertDialog.Builder(this);
		dialog.setTitle(Title);
		dialog.setMessage(Msg);
		dialog.setIcon(android.R.drawable.ic_dialog_alert);
		dialog.setCancelable(false);

		if (Po_event == this.Default_config) {
			dialog.setIcon(R.drawable.warning_wevere);
			dialog.setPositiveButton("OK",
					new DialogInterface.OnClickListener() {

						public void onClick(DialogInterface dialog, int which) {
							// on USB power
							USBOFF();
							
							// show usb mode(USB/OTG)
							String SetUIText = ReadUSBStatus();
							POTV1.setText(SetUIText);
							
							// show picture
							POIV1.setImageResource(R.drawable.otg_600web);
							
							onDestroy();
						}
					});
			dialog.setNegativeButton("close",
					new DialogInterface.OnClickListener() {

						public void onClick(DialogInterface dialog, int which) {
							tButton.setChecked(false);
							
							// show usb mode(USB/OTG)
							String SetUIText = ReadUSBStatus();
							POTV1.setText(SetUIText);
							
							// show picture
							POIV1.setImageResource(R.drawable.usb_hub);
							
							onDestroy();
						}
					});

		}

		dialog.show();

	}

	private void debug_toast(String msg) {
		Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
	}
}
