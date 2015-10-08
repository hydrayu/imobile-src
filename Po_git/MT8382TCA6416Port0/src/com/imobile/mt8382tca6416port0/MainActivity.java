package com.imobile.mt8382tca6416port0;

import java.util.HashMap;
import java.util.Iterator;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.PendingIntent;
import android.bluetooth.BluetoothAdapter;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.provider.Settings;
import android.util.Log;
import android.view.Gravity;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.lvrenyang.pos.Pos;
import com.lvrenyang.rw.PL2303Driver;
import com.lvrenyang.rw.TTYTermios;
import com.lvrenyang.rw.TTYTermios.FlowControl;
import com.lvrenyang.rw.TTYTermios.Parity;
import com.lvrenyang.rw.TTYTermios.StopBits;
import com.lvrenyang.rw.USBPort;
import com.lvrenyang.rw.USBSerialPort;

public class MainActivity extends Activity implements OnClickListener {

	private Button buttonTestText, buttonTestBarCode, buttonTestQrCode,
			buttonTestPic, buttonQueryStatus;
	private Button buttonDisconnect, buttonConnect;
	private TextView textView1;

	private static USBSerialPort serialPort;
	private static PL2303Driver mSerial;
	private static Pos mPos;
	private static Context mContext;

	private BroadcastReceiver broadcastReceiver;

	private boolean debug_main = true;

	private static final String str1 = "abcdefghijklmnopqrstuvwxyz";
	private static final String strBarCode = "123456789012";
	private static final String strQrCode = "Hello, the beautiful world!";
	// Po area Start
	private Context Po_context = null;
	private static boolean wifi_status = false;
	private static boolean BT_status = false;
	private static boolean GPS_status = false;
	private static int Thermal_status = 1; // thermal 0 is on , 1 is off
	private static int Finger_status = 3; // finger 2 is on , 3 is off
	//Po add 2015/06/10 (Setup2)
	private static int NFC_SCR_status = 5; // NFC/SCR 4 is on , 5 is off
	private static int Barcode_status = 7; // NFC/SCR 6 is on , 7 is off
	private static int USB_SGM_status = 9; // NFC/SCR 8 is on , 9 is off
	private static int Touch_status = 11; // NFC/SCR 10 is on , 11 is off
	private static int USB_GPS_status = 13; // NFC/SCR 12 is on , 13 is off
	//Po End
	private ImageView Po_IV1_top, Po_IV2_top, Po_IV3_top, Po_IV4_top,
			Po_IV5_top;
	private ImageView Po_IV1_below, Po_IV2_below, Po_IV3_below, Po_IV4_below,
			Po_IV5_below;
	// Po add 2015/06/10 function 2 (Setup1)
	private ImageView Po_IV6_top, Po_IV7_top, Po_IV8_top, Po_IV9_top,
			Po_IV10_top;
	private ImageView Po_IV6_below, Po_IV7_below, Po_IV8_below, Po_IV9_below,
			Po_IV10_below;
	// Po End
	private final String TAG = "Po_test";

	static {
		System.loadLibrary("imobileJNI");
	}

	// Po area END

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		textView1 = (TextView) findViewById(R.id.textView1);
		buttonDisconnect = (Button) findViewById(R.id.buttonDisconnect);
		buttonDisconnect.setOnClickListener(this);
		buttonDisconnect.setEnabled(false);
		buttonConnect = (Button) findViewById(R.id.buttonConnect);
		buttonConnect.setOnClickListener(this);
		buttonConnect.setEnabled(true);
		buttonTestText = (Button) findViewById(R.id.buttonTestText);
		buttonTestText.setOnClickListener(this);
		buttonTestBarCode = (Button) findViewById(R.id.buttonTestBarCode);
		buttonTestBarCode.setOnClickListener(this);
		buttonTestQrCode = (Button) findViewById(R.id.buttonTestQrCode);
		buttonTestQrCode.setOnClickListener(this);
		buttonTestPic = (Button) findViewById(R.id.buttonTestPic);
		buttonTestPic.setOnClickListener(this);
		buttonQueryStatus = (Button) findViewById(R.id.buttonQueryStatus);
		buttonQueryStatus.setOnClickListener(this);

		mContext = getApplicationContext();
		mSerial = new PL2303Driver();

		serialPort = new USBSerialPort(null, null);
		mPos = new Pos(serialPort, mSerial);

		initBroadcast();
		handleIntent();
		debug_toast("onCreate");

		// Po add Start

		this.Po_context = this;
		Po_find_id();
		Po_init_parameter(this);

		// Po add END
	}

	// Po area Start

	/****
	 * toggle_Finger
	 * 
	 * @param context
	 *            on/of Finger Printer
	 */
	public void toggle_Finger(Context context) {

		String Finger_Read_Path = "/proc/tca6416";
		String Tmp_Finger_status = "";

		// 2. toggle Finger Printer status
		String oo = " success";
		String xx = " error!!";
		if (this.Finger_status == 2) {

			// call JNI, and echo "3" > /proc/tca6416
			Tmp_Finger_status = imobileJNI.WriteProc(Finger_Read_Path, "3");

			if (Tmp_Finger_status.equals("oo")) {

				Log.d(TAG, "toggle_Finger() via JNI write 3 is" + oo);
				this.Finger_status = 3;

				// set drawable
				this.Po_IV5_top.setImageDrawable(getResources().getDrawable(
						R.drawable.finger_printer__xx));
				this.Po_IV5_below.setImageDrawable(getResources().getDrawable(
						R.drawable.white));

			}

		} else {

			// call JNI, and echo "2" > /proc/tca6416
			Tmp_Finger_status = imobileJNI.WriteProc(Finger_Read_Path, "2");
			// Log.d(TAG, "toggle_Finger() Tmp_Finger_status =" +
			// Tmp_Finger_status);

			if (Tmp_Finger_status.equals("oo")) {

				Log.d(TAG, "toggle_Finger() via JNI write 2 is" + oo);
				this.Finger_status = 2;

				// set drawable
				this.Po_IV5_top.setImageDrawable(getResources().getDrawable(
						R.drawable.finger_printer));
				this.Po_IV5_below.setImageDrawable(getResources().getDrawable(
						R.drawable.blue));

			}

		}
		Log.d(TAG, "toggle_Finger() Finger_status (update) ="
				+ this.Finger_status);

	}

	/***
	 * Po_finger_area: click can toggle finger printer on/off
	 * 
	 * @param view
	 */
	public void Po_finger_area(View view) {
		toggle_Finger(this);
		// Log.d(TAG, "Po_finger_area() click");
	}

	@Override
	public void onBackPressed() {

		// check thermal status
		if (this.Thermal_status == 0) {
			AlertDialog.Builder builder = new AlertDialog.Builder(this);
			builder.setMessage(
					"Your Thermal Printer power is trun on\nPlease turn off Thermal Printer")
					.setCancelable(false)
					.setPositiveButton("OK",
							new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog,
										int id) {
									// not thing
								}
							});
			AlertDialog alert = builder.create();
			alert.show();
		} else {
			// finish
			MainActivity.this.finish();

		}

	}

	/****
	 * toggle_Thermal
	 * 
	 * @param context
	 *            on/of Thermal Printer
	 */
	public void toggle_Thermal(Context context) {
		/*
		 * // 1. Read Thermal status String Thermal_Read_Path = "/proc/tca6416";
		 * String Tmp_Thermal_status = imobileJNI.ReadProc(Thermal_Read_Path);
		 * //Log.d(TAG, "toggle_Thermal() Tmp_Thermal_status ="+
		 * Tmp_Thermal_status.toString());
		 * 
		 * if(Tmp_Thermal_status.equals("1")){ // set Thermal_status --> 1
		 * this.Thermal_status = 1; } else { // set Thermal_status --> 0
		 * this.Thermal_status = 0; } Log.d(TAG,
		 * "toggle_Thermal() Thermal_status (origin) ="+ this.Thermal_status);
		 */
		String Thermal_Read_Path = "/proc/tca6416";
		String Tmp_Thermal_status = "";

		// 2. toggle Thermal Printer status
		String oo = " success";
		String xx = " error!!";
		if (this.Thermal_status == 0) {

			// call JNI, and echo "1" > /proc/tca6416
			Tmp_Thermal_status = imobileJNI.WriteProc(Thermal_Read_Path, "1");
			// Log.d(TAG, "toggle_Thermal() Tmp_Thermal_status =" +
			// Tmp_Thermal_status);

			if (Tmp_Thermal_status.equals("oo")) {

				Log.d(TAG, "toggle_Thermal() via JNI write 1 is" + oo);
				this.Thermal_status = 1;

				// set drawable
				this.Po_IV4_top.setImageDrawable(getResources().getDrawable(
						R.drawable.thermal_printer_xx));
				this.Po_IV4_below.setImageDrawable(getResources().getDrawable(
						R.drawable.white));

			} else {
				Log.d(TAG, "toggle_Thermal() via JNI write 1 is" + xx);
			}

		} else {

			// call JNI, and echo "0" > /proc/tca6416
			Tmp_Thermal_status = imobileJNI.WriteProc(Thermal_Read_Path, "0");
			// Log.d(TAG, "toggle_Thermal() Tmp_Thermal_status =" +
			// Tmp_Thermal_status);

			if (Tmp_Thermal_status.equals("oo")) {

				Log.d(TAG, "toggle_Thermal() via JNI write 0 is" + oo);
				this.Thermal_status = 0;

				// set drawable
				this.Po_IV4_top.setImageDrawable(getResources().getDrawable(
						R.drawable.thermal_printer));
				this.Po_IV4_below.setImageDrawable(getResources().getDrawable(
						R.drawable.blue));

			} else {
				Log.d(TAG, "toggle_Thermal() via JNI write 0 is" + xx);
			}

		}
		Log.d(TAG, "toggle_Thermal() Thermal_status (update) ="
				+ this.Thermal_status);

	}

	/***
	 * Po_thermal_area: click can toggle thermal printer on/off
	 * 
	 * @param view
	 */
	public void Po_thermal_area(View view) {
		toggle_Thermal(this);
		// Log.d(TAG, "Po_thermal_area() click");
	}

	/****
	 * toggle_GPS
	 * 
	 * @param context
	 *            on/of GPS
	 */
	public void toggle_GPS(Context context) {
		// open GPS setting dialog
		Intent intent = new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS);
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		context.startActivity(intent);

	}
	
	//Po add 2015/06/10 (Setup4)
	/****
	 * toggle_NFC_SCR
	 * 
	 * @param context
	 *            on/of NFC_SCR
	 */
	public void toggle_NFC_SCR(Context context) {
		
		String NFC_SCR_Read_Path = "/proc/tca6416";
		String Tmp_NFC_SCR_status = "";

		// 2. toggle Thermal Printer status
		String oo = " success";
		String xx = " error!!";
		if (this.NFC_SCR_status == 4) {

			// call JNI, and echo "1" > /proc/tca6416
			Tmp_NFC_SCR_status = imobileJNI.WriteProc(NFC_SCR_Read_Path, "5");
			// Log.d(TAG, "toggle_NFC_SCR() Tmp_NFC_SCR_status =" +
			// Tmp_NFC_SCR_status);

			if (Tmp_NFC_SCR_status.equals("oo")) {

				Log.d(TAG, "toggle_NFC_SCR() via JNI write 5 is" + oo);
				this.NFC_SCR_status = 5;

				// set drawable
				this.Po_IV6_top.setImageDrawable(getResources().getDrawable(
						R.drawable.nfc_checkpoint2_x));
				this.Po_IV6_below.setImageDrawable(getResources().getDrawable(
						R.drawable.white));

			} else {
				Log.d(TAG, "toggle_NFC_SCR() via JNI write 5 is" + xx);
			}

		} else {

			// call JNI, and echo "4" > /proc/tca6416
			Tmp_NFC_SCR_status = imobileJNI.WriteProc(NFC_SCR_Read_Path, "4");
			// Log.d(TAG, "toggle_NFC_SCR() Tmp_NFC_SCR_status =" +
			// Tmp_NFC_SCR_status);

			if (Tmp_NFC_SCR_status.equals("oo")) {

				Log.d(TAG, "toggle_NFC_SCR() via JNI write 4 is" + oo);
				this.NFC_SCR_status = 4;

				// set drawable
				this.Po_IV6_top.setImageDrawable(getResources().getDrawable(
						R.drawable.nfc_checkpoint2_o));
				this.Po_IV6_below.setImageDrawable(getResources().getDrawable(
						R.drawable.blue));

			} else {
				Log.d(TAG, "toggle_NFC_SCR() via JNI write 4 is" + xx);
			}

		}
		Log.d(TAG, "toggle_NFC_SCR() NFC_SCR_status (update) ="
				+ this.NFC_SCR_status);

	}
	
	/****
	 * toggle_Barcode
	 * 
	 * @param context
	 *            on/of Barcode
	 */
	public void toggle_Barcode(Context context) {
		
		String Barcode_Read_Path = "/proc/tca6416";
		String Tmp_Barcode_status = "";

		String oo = " success";
		String xx = " error!!";
		if (this.Barcode_status == 6) {

			// call JNI, and echo "7" > /proc/tca6416
			Tmp_Barcode_status = imobileJNI.WriteProc(Barcode_Read_Path, "7");
			// Log.d(TAG, "toggle_Barcode() Tmp_Barcode_status =" +
			// Tmp_Barcode_status);

			if (Tmp_Barcode_status.equals("oo")) {

				Log.d(TAG, "toggle_NFC_SCR() via JNI write 7 is" + oo);
				this.Barcode_status = 7;

				// set drawable
				this.Po_IV7_top.setImageDrawable(getResources().getDrawable(
						R.drawable.barcode_canner_x));
				this.Po_IV7_below.setImageDrawable(getResources().getDrawable(
						R.drawable.white));

			} else {
				Log.d(TAG, "toggle_Barcode() via JNI write 7 is" + xx);
			}

		} else {

			// call JNI, and echo "6" > /proc/tca6416
			Tmp_Barcode_status = imobileJNI.WriteProc(Barcode_Read_Path, "6");
			// Log.d(TAG, "toggle_Barcode() Tmp_Barcode_status =" +
			// Tmp_Barcode_status);

			if (Tmp_Barcode_status.equals("oo")) {

				Log.d(TAG, "toggle_Barcode() via JNI write 6 is" + oo);
				this.Barcode_status = 6;

				// set drawable
				this.Po_IV7_top.setImageDrawable(getResources().getDrawable(
						R.drawable.barcode_canner));
				this.Po_IV7_below.setImageDrawable(getResources().getDrawable(
						R.drawable.blue));

			} else {
				Log.d(TAG, "toggle_Barcode() via JNI write 6 is" + xx);
			}

		}
		Log.d(TAG, "toggle_Barcode() Barcode_status (update) ="
				+ this.Barcode_status);

	}
	
	/****
	 * toggle_USBGSM
	 * 
	 * @param context
	 *            on/of USB GSM
	 */
	public void toggle_USBGSM(Context context) {
		
		String USBGSM_Read_Path = "/proc/tca6416";
		String Tmp_USBGSM_status = "";

		String oo = " success";
		String xx = " error!!";
		if (this.USB_SGM_status == 8) {

			// call JNI, and echo "9" > /proc/tca6416
			Tmp_USBGSM_status = imobileJNI.WriteProc(USBGSM_Read_Path, "9");
			// Log.d(TAG, "toggle_USBGSM() Tmp_USBGSM_status =" +
			// Tmp_USBGSM_status);

			if (Tmp_USBGSM_status.equals("oo")) {

				Log.d(TAG, "toggle_USBGSM() via JNI write 9 is" + oo);
				this.USB_SGM_status = 9;

				// set drawable
				this.Po_IV8_top.setImageDrawable(getResources().getDrawable(
						R.drawable.gsm_x));
				this.Po_IV8_below.setImageDrawable(getResources().getDrawable(
						R.drawable.white));

			} else {
				Log.d(TAG, "toggle_Barcode() via JNI write 9 is" + xx);
			}

		} else {

			// call JNI, and echo "8" > /proc/tca6416
			Tmp_USBGSM_status = imobileJNI.WriteProc(USBGSM_Read_Path, "8");
			// Log.d(TAG, "toggle_USBGSM() Tmp_USBGSM_status =" +
			// Tmp_USBGSM_status);

			if (Tmp_USBGSM_status.equals("oo")) {

				Log.d(TAG, "toggle_Barcode() via JNI write 8 is" + oo);
				this.USB_SGM_status = 8;

				// set drawable
				this.Po_IV8_top.setImageDrawable(getResources().getDrawable(
						R.drawable.gsm_o));
				this.Po_IV8_below.setImageDrawable(getResources().getDrawable(
						R.drawable.blue));

			} else {
				Log.d(TAG, "toggle_USBGSM() via JNI write 8 is" + xx);
			}

		}
		Log.d(TAG, "toggle_USBGSM() USB_SGM_status (update) ="
				+ this.USB_SGM_status);

	}
	
	/****
	 * toggle_Touch
	 * 
	 * @param context
	 *            on/of Touch
	 */
	public void toggle_Touch(Context context) {
		
		String Touch_Read_Path = "/proc/tca6416";
		String Tmp_Touch_status = "";

		String oo = " success";
		String xx = " error!!";
		if (this.Touch_status == 10) {

			// call JNI, and echo "11" > /proc/tca6416
			Tmp_Touch_status = imobileJNI.WriteProc(Touch_Read_Path, "B");
			// Log.d(TAG, "toggle_Touch() Tmp_Touch_status =" +
			// Tmp_Touch_status);

			if (Tmp_Touch_status.equals("oo")) {

				Log.d(TAG, "toggle_USBGSM() via JNI write B is" + oo);
				this.Touch_status = 11;

				// set drawable
				this.Po_IV9_top.setImageDrawable(getResources().getDrawable(
						R.drawable.touch_x));
				this.Po_IV9_below.setImageDrawable(getResources().getDrawable(
						R.drawable.white));

			} else {
				Log.d(TAG, "toggle_Barcode() via JNI write B is" + xx);
			}

		} else {

			// call JNI, and echo "10" > /proc/tca6416
			Tmp_Touch_status = imobileJNI.WriteProc(Touch_Read_Path, "A");
			// Log.d(TAG, "toggle_Touch() Tmp_Touch_status =" +
			// Tmp_Touch_status);

			if (Tmp_Touch_status.equals("oo")) {

				Log.d(TAG, "toggle_Touch() via JNI write A is" + oo);
				this.Touch_status = 10;

				// set drawable
				this.Po_IV9_top.setImageDrawable(getResources().getDrawable(
						R.drawable.touch));
				this.Po_IV9_below.setImageDrawable(getResources().getDrawable(
						R.drawable.blue));

			} else {
				Log.d(TAG, "toggle_Touch() via JNI write A is" + xx);
			}

		}
		Log.d(TAG, "toggle_Touch() Touch_status (update) ="
				+ this.Touch_status);

	}
	
	/****
	 * toggle_USBGPS
	 * 
	 * @param context
	 *            on/of USB GPS
	 */
	public void toggle_USBGPS(Context context) {
		
		String USBGPS_Read_Path = "/proc/tca6416";
		String Tmp_USBGPS_status = "";

		String oo = " success";
		String xx = " error!!";
		if (this.USB_GPS_status == 12) {

			// call JNI, and echo "13" > /proc/tca6416
			Tmp_USBGPS_status = imobileJNI.WriteProc(USBGPS_Read_Path, "D");
			// Log.d(TAG, "toggle_USBGPS() Tmp_USBGPS_status =" +
			// Tmp_USBGPS_status);

			if (Tmp_USBGPS_status.equals("oo")) {

				Log.d(TAG, "toggle_USBGSM() via JNI write D is" + oo);
				this.USB_GPS_status = 13;

				// set drawable
				this.Po_IV10_top.setImageDrawable(getResources().getDrawable(
						R.drawable.gps_receiving_x));
				this.Po_IV10_below.setImageDrawable(getResources().getDrawable(
						R.drawable.white));

			} else {
				Log.d(TAG, "toggle_USBGPS() via JNI write D is" + xx);
			}

		} else {

			// call JNI, and echo "12" > /proc/tca6416
			Tmp_USBGPS_status = imobileJNI.WriteProc(USBGPS_Read_Path, "C");
			// Log.d(TAG, "toggle_USBGPS() Tmp_Touch_status =" +
			// Tmp_Touch_status);

			if (Tmp_USBGPS_status.equals("oo")) {

				Log.d(TAG, "toggle_USBGPS() via JNI write C is" + oo);
				this.USB_GPS_status = 12;

				// set drawable
				this.Po_IV10_top.setImageDrawable(getResources().getDrawable(
						R.drawable.gps_receiving_o));
				this.Po_IV10_below.setImageDrawable(getResources().getDrawable(
						R.drawable.blue));

			} else {
				Log.d(TAG, "toggle_USBGPS() via JNI write C is" + xx);
			}

		}
		Log.d(TAG, "toggle_USBGPS() USB_GPS_status (update) ="
				+ this.USB_GPS_status);

	}
	
	/***
	 * Po_USB_GPS_area: click can toggle USB GPS on/off
	 * 
	 * @param view
	 */
	public void Po_USB_GPS_area(View view) {
		toggle_USBGPS(this);
		// Log.d(TAG, "click");
	}
	
	/***
	 * Po_MultiTouch_area: click can toggle Touch on/off
	 * 
	 * @param view
	 */
	public void Po_MultiTouch_area(View view) {
		toggle_Touch(this);
		// Log.d(TAG, "click");
	}
	
	/***
	 * Po_USBGSM_area: click can toggle USB GSM on/off
	 * 
	 * @param view
	 */
	public void Po_USBGSM_area(View view) {
		toggle_USBGSM(this);
		// Log.d(TAG, "click");
	}
	
	/***
	 * Po_Barcode_area: click can toggle Barcode on/off
	 * 
	 * @param view
	 */
	public void Po_Barcode_area(View view) {
		toggle_Barcode(this);
		// Log.d(TAG, "click");
	}

	/***
	 * Po_GPS_area: click can toggle GPS on/off
	 * 
	 * @param view
	 */
	public void Po_NFC_area(View view) {
		toggle_NFC_SCR(this);
		// Log.d(TAG, "click");
	}
	
	//Po End
	
	/***
	 * Po_GPS_area: click can toggle GPS on/off
	 * 
	 * @param view
	 */
	public void Po_GPS_area(View view) {
		toggle_GPS(this);
		// Log.d(TAG, "click");
	}

	/***
	 * toggle_BT
	 * 
	 * @param context
	 *            on/off BuleTooth
	 */
	public void toggle_BT(Context context) {
		BluetoothAdapter mBluetoothAdapter = BluetoothAdapter
				.getDefaultAdapter();
		if (!mBluetoothAdapter.isEnabled()) {
			mBluetoothAdapter.enable();
			this.BT_status = true;

			// set drawable
			this.Po_IV2_top.setImageDrawable(getResources().getDrawable(
					R.drawable.ic_appwidget_settings_bluetooth_on_holo));
			this.Po_IV2_below.setImageDrawable(getResources().getDrawable(
					R.drawable.blue));

		} else {
			mBluetoothAdapter.disable();
			this.BT_status = false;

			// set drawable
			this.Po_IV2_top.setImageDrawable(getResources().getDrawable(
					R.drawable.ic_appwidget_settings_bluetooth_off_holo));
			this.Po_IV2_below.setImageDrawable(getResources().getDrawable(
					R.drawable.white));

		}
		Log.d(TAG, "toggle_BT() BT_status=" + this.BT_status);
	}

	/***
	 * Po_BT_area: click can toggle BT on/off
	 * 
	 * @param view
	 */
	public void Po_BT_area(View view) {
		toggle_BT(this);
		// Log.d(TAG, "click");
	}

	/*****
	 * toggle_WiFi: on/off WiFi
	 * 
	 * @param context
	 */
	public void toggle_WiFi(Context context) {
		WifiManager wifiManager = (WifiManager) context
				.getSystemService(Context.WIFI_SERVICE);
		if (!wifiManager.isWifiEnabled()) {
			wifiManager.setWifiEnabled(true);
			this.wifi_status = true;

			// set drawable
			this.Po_IV1_top.setImageDrawable(getResources().getDrawable(
					R.drawable.ic_appwidget_settings_wifi_on_holo));
			this.Po_IV1_below.setImageDrawable(getResources().getDrawable(
					R.drawable.blue));

		} else {
			wifiManager.setWifiEnabled(false);
			this.wifi_status = false;

			// set drawable
			this.Po_IV1_top.setImageDrawable(getResources().getDrawable(
					R.drawable.ic_appwidget_settings_wifi_off_holo));
			this.Po_IV1_below.setImageDrawable(getResources().getDrawable(
					R.drawable.white));

		}
		Log.d(TAG, "toggleWiFi() wifi_status=" + this.wifi_status);
		// System.out.println("toggleWiFi() wifi_status=" + this.wifi_status);
	}

	/***
	 * Po_wifi_area: click can toggle wifi on/off
	 * 
	 * @param view
	 */
	public void Po_wifi_area(View view) {
		toggle_WiFi(this);
		// Log.d(TAG, "click");
	}

	/***
	 * Po_init_parameter: onCreate and onResume call can read wifi status, last
	 * chang wifi/BT icon
	 * 
	 * @param context
	 */
	private void Po_init_parameter(Context context) {

		// read wifi status
		WifiManager wifiManager = (WifiManager) context
				.getSystemService(Context.WIFI_SERVICE);
		if (!wifiManager.isWifiEnabled()) {
			this.wifi_status = false;

			// set drawable
			this.Po_IV1_top.setImageDrawable(getResources().getDrawable(
					R.drawable.ic_appwidget_settings_wifi_off_holo));
			this.Po_IV1_below.setImageDrawable(getResources().getDrawable(
					R.drawable.white));

		} else {
			this.wifi_status = true;

			// set drawable
			this.Po_IV1_top.setImageDrawable(getResources().getDrawable(
					R.drawable.ic_appwidget_settings_wifi_on_holo));
			this.Po_IV1_below.setImageDrawable(getResources().getDrawable(
					R.drawable.blue));

		}
		Log.d(TAG, "wifi_status=" + this.wifi_status);

		// read bluetooth status
		BluetoothAdapter mBluetoothAdapter = BluetoothAdapter
				.getDefaultAdapter();
		if (!mBluetoothAdapter.isEnabled()) {
			this.BT_status = false;

			// set drawable
			this.Po_IV2_top.setImageDrawable(getResources().getDrawable(
					R.drawable.ic_appwidget_settings_bluetooth_off_holo));
			this.Po_IV2_below.setImageDrawable(getResources().getDrawable(
					R.drawable.white));

		} else {
			this.BT_status = true;

			// set drawable
			this.Po_IV2_top.setImageDrawable(getResources().getDrawable(
					R.drawable.ic_appwidget_settings_bluetooth_on_holo));
			this.Po_IV2_below.setImageDrawable(getResources().getDrawable(
					R.drawable.blue));
		}

		Log.d(TAG, "BT_status=" + this.BT_status);

		// read GPS status
		String provider = Settings.Secure.getString(
				context.getContentResolver(),
				Settings.Secure.LOCATION_PROVIDERS_ALLOWED);
		this.GPS_status = provider.contains("gps");

		if (this.GPS_status) {

			// true
			// set drawable
			this.Po_IV3_top.setImageDrawable(getResources().getDrawable(
					R.drawable.ic_appwidget_settings_gps_on_holo));
			this.Po_IV3_below.setImageDrawable(getResources().getDrawable(
					R.drawable.blue));
		} else {

			// false
			// set drawable
			this.Po_IV3_top.setImageDrawable(getResources().getDrawable(
					R.drawable.ic_appwidget_settings_gps_off_holo));
			this.Po_IV3_below.setImageDrawable(getResources().getDrawable(
					R.drawable.white));

		}

		Log.d(TAG, "GPS_status=" + this.GPS_status);

		/*
		 * // read Thermal Printer status String Thermal_Read_Path =
		 * "/proc/tca6416"; String Tmp_Thermal_status =
		 * imobileJNI.ReadProc(Thermal_Read_Path); // Log.d(TAG,
		 * "toggle_Thermal() Tmp_Thermal_status ="+ //
		 * Tmp_Thermal_status.toString());
		 */

		if (this.Thermal_status == 1) {
			// set Thermal_status --> 1
			// this.Thermal_status = 1;

			// set drawable
			this.Po_IV4_top.setImageDrawable(getResources().getDrawable(
					R.drawable.thermal_printer_xx));
			this.Po_IV4_below.setImageDrawable(getResources().getDrawable(
					R.drawable.white));
			String Tmp_Thermal_status = imobileJNI.WriteProc("/proc/tca6416",
					"1");

		} else {
			// set Thermal_status --> 0
			// this.Thermal_status = 0;

			// set drawable
			this.Po_IV4_top.setImageDrawable(getResources().getDrawable(
					R.drawable.thermal_printer));
			this.Po_IV4_below.setImageDrawable(getResources().getDrawable(
					R.drawable.blue));

		}
		Log.d(TAG, "Thermal_status=" + this.Thermal_status);

		if (this.Finger_status == 3) {
			// set Finger_status --> 3

			// set drawable
			this.Po_IV5_top.setImageDrawable(getResources().getDrawable(
					R.drawable.finger_printer__xx));
			this.Po_IV5_below.setImageDrawable(getResources().getDrawable(
					R.drawable.white));
			String Tmp_Thermal_status = imobileJNI.WriteProc("/proc/tca6416",
					"3");

		} else {
			// set Thermal_status --> 2

			// set drawable
			this.Po_IV5_top.setImageDrawable(getResources().getDrawable(
					R.drawable.finger_printer));
			this.Po_IV5_below.setImageDrawable(getResources().getDrawable(
					R.drawable.blue));

		}
		Log.d(TAG, "Finger_status=" + this.Finger_status);
		
		//Po add 2015/06/10 (Setup3)
		if (this.NFC_SCR_status == 5) {
			// set NFC_SCR_status --> 5

			// set drawable
			this.Po_IV6_top.setImageDrawable(getResources().getDrawable(
					R.drawable.nfc_checkpoint2_x));
			this.Po_IV6_below.setImageDrawable(getResources().getDrawable(
					R.drawable.white));
			String Tmp_NFC_SCR_status = imobileJNI.WriteProc("/proc/tca6416",
					"4");

		} else {
			// set NFC_SCR_status --> 4

			// set drawable
			this.Po_IV6_top.setImageDrawable(getResources().getDrawable(
					R.drawable.nfc_checkpoint2_o));
			this.Po_IV6_below.setImageDrawable(getResources().getDrawable(
					R.drawable.blue));

		}
		Log.d(TAG, "NFC_SCR_status=" + this.NFC_SCR_status);
		
		if (this.Barcode_status == 7) {
			// set Barcode_status --> 7

			// set drawable
			this.Po_IV7_top.setImageDrawable(getResources().getDrawable(
					R.drawable.barcode_canner_x));
			this.Po_IV7_below.setImageDrawable(getResources().getDrawable(
					R.drawable.white));
			String Tmp_NFC_SCR_status = imobileJNI.WriteProc("/proc/tca6416",
					"7");

		} else {
			// set Barcode_status --> 6

			// set drawable
			this.Po_IV7_top.setImageDrawable(getResources().getDrawable(
					R.drawable.barcode_canner));
			this.Po_IV7_below.setImageDrawable(getResources().getDrawable(
					R.drawable.blue));

		}
		Log.d(TAG, "Barcode_status=" + this.Barcode_status);
		
		if (this.USB_SGM_status == 9) {
			// set USB_SGM_status --> 9

			// set drawable
			this.Po_IV8_top.setImageDrawable(getResources().getDrawable(
					R.drawable.gsm_x));
			this.Po_IV8_below.setImageDrawable(getResources().getDrawable(
					R.drawable.white));
			String Tmp_USB_SGM_status = imobileJNI.WriteProc("/proc/tca6416",
					"9");

		} else {
			// set USB_SGM_status --> 8

			// set drawable
			this.Po_IV8_top.setImageDrawable(getResources().getDrawable(
					R.drawable.gsm_o));
			this.Po_IV8_below.setImageDrawable(getResources().getDrawable(
					R.drawable.blue));

		}
		Log.d(TAG, "USB_SGM_status=" + this.USB_SGM_status);
		
		if (this.Touch_status == 11) {
			// set USB_SGM_status --> 11

			// set drawable
			this.Po_IV9_top.setImageDrawable(getResources().getDrawable(
					R.drawable.touch_x));
			this.Po_IV9_below.setImageDrawable(getResources().getDrawable(
					R.drawable.white));
			String Tmp_Touch_status = imobileJNI.WriteProc("/proc/tca6416",
					"B");

		} else {
			// set USB_SGM_status --> 10

			// set drawable
			this.Po_IV9_top.setImageDrawable(getResources().getDrawable(
					R.drawable.touch));
			this.Po_IV9_below.setImageDrawable(getResources().getDrawable(
					R.drawable.blue));

		}
		Log.d(TAG, "Touch_status=" + this.Touch_status);
		
		if (this.USB_GPS_status == 13) {
			// set USB_GPS_status --> 13

			// set drawable
			this.Po_IV10_top.setImageDrawable(getResources().getDrawable(
					R.drawable.gps_receiving_x));
			this.Po_IV10_below.setImageDrawable(getResources().getDrawable(
					R.drawable.white));
			String Tmp_USB_GPS_status = imobileJNI.WriteProc("/proc/tca6416",
					"D");

		} else {
			// set USB_GPS_status --> 12

			// set drawable
			this.Po_IV10_top.setImageDrawable(getResources().getDrawable(
					R.drawable.gps_receiving_o));
			this.Po_IV10_below.setImageDrawable(getResources().getDrawable(
					R.drawable.blue));

		}
		Log.d(TAG, "USB_GPS_status=" + this.USB_GPS_status);
		
	}

	/***
	 * Po_find_id: can run findViewById
	 */
	private void Po_find_id() {
		// find id wifi
		this.Po_IV1_top = (ImageView) findViewById(R.id.Po_IV1_top);
		this.Po_IV1_below = (ImageView) findViewById(R.id.Po_IV1_below);

		// find id BT
		this.Po_IV2_top = (ImageView) findViewById(R.id.Po_IV2_top);
		this.Po_IV2_below = (ImageView) findViewById(R.id.Po_IV2_below);

		// find id GPS
		this.Po_IV3_top = (ImageView) findViewById(R.id.Po_IV3_top);
		this.Po_IV3_below = (ImageView) findViewById(R.id.Po_IV3_below);

		// find id Thermal Printer
		this.Po_IV4_top = (ImageView) findViewById(R.id.Po_IV4_top);
		this.Po_IV4_below = (ImageView) findViewById(R.id.Po_IV4_below);

		// find id Finger Printer
		this.Po_IV5_top = (ImageView) findViewById(R.id.Po_IV5_top);
		this.Po_IV5_below = (ImageView) findViewById(R.id.Po_IV5_below);

		// find id NFC/SCR
		this.Po_IV6_top = (ImageView) findViewById(R.id.Po_IV6_top);
		this.Po_IV6_below = (ImageView) findViewById(R.id.Po_IV6_below);

		// find id Barcode
		this.Po_IV7_top = (ImageView) findViewById(R.id.Po_IV7_top);
		this.Po_IV7_below = (ImageView) findViewById(R.id.Po_IV7_below);

		// find id USB GSM
		this.Po_IV8_top = (ImageView) findViewById(R.id.Po_IV8_top);
		this.Po_IV8_below = (ImageView) findViewById(R.id.Po_IV8_below);

		// find id Touch
		this.Po_IV9_top = (ImageView) findViewById(R.id.Po_IV9_top);
		this.Po_IV9_below = (ImageView) findViewById(R.id.Po_IV9_below);

		// find id USB GPS
		this.Po_IV10_top = (ImageView) findViewById(R.id.Po_IV10_top);
		this.Po_IV10_below = (ImageView) findViewById(R.id.Po_IV10_below);
	}

	// Po area END

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	private void initBroadcast() {
		broadcastReceiver = new BroadcastReceiver() {

			@SuppressLint("NewApi")
			@Override
			public void onReceive(Context context, Intent intent) {
				// TODO Auto-generated method stub
				String action = intent.getAction();
				if (UsbManager.ACTION_USB_DEVICE_DETACHED.equals(action)) {
					textView1.setText("");
					buttonDisconnect.callOnClick();

				}
			}

		};
		IntentFilter intentFilter = new IntentFilter();
		intentFilter.addAction(UsbManager.ACTION_USB_DEVICE_ATTACHED);
		intentFilter.addAction(UsbManager.ACTION_USB_ACCESSORY_ATTACHED);
		intentFilter.addAction(UsbManager.ACTION_USB_DEVICE_DETACHED);
		mContext.registerReceiver(broadcastReceiver, intentFilter);
	}

	@SuppressLint("NewApi")
	private void uninitBroadcast() {
		mContext.unregisterReceiver(broadcastReceiver);
	}

	@SuppressLint("NewApi")
	private void handleIntent() {
		buttonConnect.callOnClick();
	}

	private void debug_toast(String msg) {
		if (debug_main)
			Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
	}

	@Override
	protected void onStart() {
		super.onStart();
	}

	@Override
	protected void onNewIntent(Intent intent) {
		setIntent(intent);
		handleIntent();
		debug_toast("onNewIntent()");
	}

	@Override
	protected void onResume() {
		super.onResume();
		Po_init_parameter(this);
		debug_toast("onResume");
	}

	@Override
	protected void onPause() {
		super.onPause();
	}

	@Override
	protected void onStop() {
		super.onStop();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		disconnect();
		uninitBroadcast();
		debug_toast("onDestroy");
		Log.d(TAG, "exit APP");
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
		case R.id.buttonConnect:
			try {
				connect();
			} catch (Exception e) {
				debug_toast(e.toString());
			}
			break;

		case R.id.buttonDisconnect:
			disconnect();
			break;

		case R.id.buttonTestText:
			/**
			 * usb??�印?��??��?�串?��波特??�只??�象征�?��?��?�波?��??�哪�?个都?��以�??
			 */
			open(115200, Parity.NONE);
			mPos.POS_S_TextOut(str1, 32, 0, 0, 0, 0x00 + 0x400);
			mPos.POS_FeedLine();
			mPos.POS_FeedLine();
			debug_toast("Testing TEXT");
			close();
			break;

		case R.id.buttonTestBarCode:
			open(115200, Parity.NONE);
			mPos.POS_S_SetBarcode(strBarCode, 32, 0x41, 3, 162, 0x00, 0x02);
			mPos.POS_FeedLine();
			mPos.POS_FeedLine();
			debug_toast("Testing BarCode");
			close();
			break;

		case R.id.buttonTestQrCode:
			open(115200, Parity.NONE);
			mPos.POS_S_SetQRcode(strQrCode, 6, 4);
			mPos.POS_FeedLine();
			mPos.POS_FeedLine();
			debug_toast("Testing QRCode");
			close();
			break;

		case R.id.buttonTestPic:
			Bitmap bitmap = BitmapFactory.decodeResource(getResources(),
					R.drawable.iu2);
			if (null == bitmap) {
				debug_toast("Image Decoding ERROR!");
				break;
			}
			open(115200, Parity.NONE);
			/**
			 * ?��??��?��?��?�宽度�?�须�?8??�整?��??
			 */
			mPos.POS_PrintPicture(bitmap, 360, 0);
			mPos.POS_FeedLine();
			mPos.POS_FeedLine();
			debug_toast("Testing Image");
			close();
			break;

		case R.id.buttonQueryStatus:

			open(115200, Parity.NONE);

			int retry = 0;
			int maxRetry = 3;
			byte[] data = { 0x10, 0x04, 0x01 };
			byte[] buf = new byte[1];

			/** ??�两次机�? */
			for (retry = 0; retry < maxRetry; retry++) {
				mPos.POS_Write(data, 0, data.length, 500);
				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				int rec = mPos.POS_Read(buf, 0, buf.length, 1000);
				if (rec == buf.length)
					break;
			}
			if (retry == maxRetry)
				debug_toast("Load Failed");
			else
				debug_toast("" + buf[1]);

			break;
		}
	}

	/**
	 * ??��???�关?��??�钮，就?��?��?��连接USB??��?��?��?就是probe，关?��就是disconnect
	 * probe会填??�port字段，open会根?��termios字段?��??��?串口
	 * 
	 */

	@SuppressLint("NewApi")
	private void connect() {
		final UsbManager mUsbManager = (UsbManager) getSystemService(Context.USB_SERVICE);
		HashMap<String, UsbDevice> deviceList = mUsbManager.getDeviceList();
		Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();
		if (deviceList.size() > 0) {
			// ??��?��?��?�择对�?��?��?��?，并添�?��?�钮??��?�件
			LinearLayout llSelectDevice = new LinearLayout(this);
			llSelectDevice.setOrientation(LinearLayout.VERTICAL);
			llSelectDevice.setLayoutParams(new LayoutParams(
					LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
			AlertDialog.Builder builder = new AlertDialog.Builder(this);
			builder.setTitle("Link Device").setView(llSelectDevice);
			final AlertDialog dialog = builder.create();

			while (deviceIterator.hasNext()) { // 这�?�是if不是while，说??��?�只?��?��??��?种device
				final UsbDevice device = deviceIterator.next();
				Button btDevice = new Button(llSelectDevice.getContext());
				btDevice.setLayoutParams(new LayoutParams(
						LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
				btDevice.setGravity(Gravity.LEFT);
				btDevice.setText("ID: " + device.getDeviceId());
				btDevice.setOnClickListener(new OnClickListener() {

					@Override
					public void onClick(View v) {
						// TODO Auto-generated method stub
						PendingIntent mPermissionIntent = PendingIntent
								.getBroadcast(
										MainActivity.this,
										0,
										new Intent(
												MainActivity.this
														.getApplicationInfo().packageName),
										0);
						serialPort.port = new USBPort(mUsbManager,
								MainActivity.this, device, mPermissionIntent);
						int ret = mSerial.pl2303_probe(serialPort);
						if (ret == 0) {
							textView1.setText(System.currentTimeMillis() + ": "
									+ " connection successful " + mSerial.type
									+ "\n");
							buttonConnect.setEnabled(false);
							buttonDisconnect.setEnabled(true);
						} else {
							textView1.setText(System.currentTimeMillis() + ": "
									+ " connection fail(" + ret + ")\n");
							buttonConnect.setEnabled(true);
							buttonDisconnect.setEnabled(false);
						}
					}
				});
				llSelectDevice.addView(btDevice);
			}
			if (llSelectDevice.getChildCount() == 1)
				llSelectDevice.getChildAt(0).callOnClick();
			else
				dialog.show();
		}
	}

	private void open(final int baudrate, final Parity parity) {

		TTYTermios termios = serialPort.termios;
		serialPort.termios = new TTYTermios(baudrate, FlowControl.NONE, parity,
				StopBits.ONE, 8);
		int ret = mSerial.pl2303_open(serialPort, termios);
		if (ret == 0)
			textView1.setText(System.currentTimeMillis() + ": "
					+ "open successful" + serialPort.termios.baudrate + " "
					+ serialPort.termios.parity + "\n");
		else
			textView1.setText(System.currentTimeMillis() + ": " + "open fail("
					+ ret + ")\n");

	}

	private void close() {
		mSerial.pl2303_close(serialPort);
	}

	private void disconnect() {
		close();
		mSerial.pl2303_disconnect(serialPort);
		textView1.setText("");
		buttonConnect.setEnabled(true);
		buttonDisconnect.setEnabled(false);

	}
}
