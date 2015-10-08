package com.imobile.mt8382hotkeydefine;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.app.AlertDialog;
import android.app.Fragment;
import android.app.FragmentTransaction;
import android.content.DialogInterface;
import android.content.pm.ActivityInfo;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.os.Environment;
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.LinearLayout;
import android.widget.Switch;
import android.widget.Toast;

public class HomeScreen extends FragmentActivity {

	private final String TAG = "Po_debug";
	private Switch PoSwitch;
	private LinearLayout keySetup;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.homescreen);
		// set 橫向
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		PoFindId();
		setLayout();
		addFragment();
	}

	// Po add 2015/08/04
	void addFragment() {

		// Instantiate a new fragment.
		Fragment newFragment = new MyFirstFragment();

		FragmentTransaction ft = getFragmentManager().beginTransaction();
		ft.add(R.id.MainActivityUI, newFragment, "first");
		ft.setTransition(FragmentTransaction.TRANSIT_FRAGMENT_OPEN);
		ft.commit();
	}

	// Po End

	private void SaveCustomKpdStatus(int switch_status) {
		String root = Environment.getExternalStorageDirectory().toString();
		File Po_Kpd = new File(root, "Po_kpd.sh");
		if (Po_Kpd.exists()) {
			Po_Kpd.delete();
		}
		try {
			FileOutputStream out = new FileOutputStream(Po_Kpd);
			String msg = "";
			if (switch_status == 1) {
				msg = "echo QUIET > /proc/kpd\n";
			} else {
				msg = "echo NOTQUIET > /proc/kpd\n";
			}

			out.write(msg.getBytes());
			out.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 
	 * @param status
	 *            status is 1 ---> custom hotkey on status is 0 ---> custom
	 *            hotkey off
	 */
	private void OnOff_CustomHotkey(int status) {
		if (status == 1) {

			// set lay visible
			for (int i = 0; i < keySetup.getChildCount(); i++) {
				View view = keySetup.getChildAt(i);
				view.setVisibility(View.VISIBLE); // Or whatever you want to do
													// with the view.
			}
		} else {

			// set layout gone
			for (int i = 0; i < keySetup.getChildCount(); i++) {
				View view = keySetup.getChildAt(i);
				view.setVisibility(View.GONE); // Or whatever you want to do
												// with the view.
			}
		}
	}

	/***
	 * check 有沒有 /mnt/sdcard/Po_kpd.sh if have Po_kpd.sh, slipt string, get
	 * status if not Po_kpd.sh, set layout switch default 0
	 */
	private void CheckKpdFileStatus() {
		String FilePath = "/mnt/sdcard/Po_kpd.sh";
		if (isFileExsist(FilePath)) {
			Log.d(TAG, "have" + FilePath);
			// set Po_kpd.sh --> kpd status

			/****
			 * read /mnt/sdcard/Po_kpd.sh
			 */
			// Find the directory for the SD Card using the API
			// *Don't* hardcode "/sdcard"
			File sdcard = Environment.getExternalStorageDirectory();

			// Get the text file
			File file = new File(sdcard, "Po_kpd.sh");

			// Read text from file
			StringBuilder text = new StringBuilder();

			try {
				BufferedReader br = new BufferedReader(new FileReader(file));
				String line;

				while ((line = br.readLine()) != null) {
					text.append(line);
					text.append('\n');
				}
				br.close();
			} catch (IOException e) {
				// You'll need to add proper error handling here
			}

			Log.d(TAG, "data = " + text.toString());

			/****
			 * split string
			 */
			int PoCustomStatus = 0;
			String str = text.toString();
			String[] tokens = str.split(" ");
			for (String token : tokens) {
				// Log.d(TAG, "split = " + token);
				if (token.equals("NOTQUIET")) {
					PoCustomStatus = 0;
				} else if (token.equals("QUIET")) {
					PoCustomStatus = 1;
				}
			}

			/****
			 * set switch
			 */
			if (PoCustomStatus == 1) {
				// set the switch to ON
				this.PoSwitch.setChecked(true);
				// set layout visible
				OnOff_CustomHotkey(1);
				Log.d(TAG, "PoSwitch = on");
			} else {
				// set the switch to OFF
				this.PoSwitch.setChecked(false);
				// set layout gone
				OnOff_CustomHotkey(0);
				Log.d(TAG, "PoSwitch = off");
			}

		} else {
			Log.d(TAG, "not found " + FilePath);
			// set the switch to OFF
			this.PoSwitch.setChecked(false);
			// set layout gone
			OnOff_CustomHotkey(0);
		}
	}

	/***
	 * caheck switch status
	 */
	private void setLayout() {
		// 1. init switch
		CheckKpdFileStatus();

		// 2. check switch status
		// attach a listener to check for changes in state
		this.PoSwitch.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(CompoundButton buttonView,
					boolean isChecked) {

				if (isChecked) {
					Log.d(TAG, "PoSwitch is currently ON");
					// set lay visible
					OnOff_CustomHotkey(1);
					// call JNI
					SaveCustomKpdStatus(1);
				} else {
					Log.d(TAG, "PoSwitch is currently OFF");
					// set layout gone
					OnOff_CustomHotkey(0);
					SaveCustomKpdStatus(0);
					// call restore default
					CopyAssets();
					String FilePath = "/mnt/sdcard/hotkey.ini";
					if (isFileExsist(FilePath)) {
						Log.d(TAG, "have file");
						show_dialog("Restore Default Config Success!",
								"Please reboot devices");
					} else {
						Log.d(TAG, "not file");
					}
				}

			}
		});

	}

	private void PoFindId() {
		this.PoSwitch = (Switch) findViewById(R.id.PoSwitch);
		this.keySetup = (LinearLayout) findViewById(R.id.keySetup);
	}

	public void restore_default(View view) {
		// Log.d(TAG, "click");
		CopyAssets();
		String FilePath = "/mnt/sdcard/hotkey.ini";
		if (isFileExsist(FilePath)) {
			Log.d(TAG, "have file");
			show_dialog("Restore Default Config Success!",
					"Please reboot devices");
		} else {
			Log.d(TAG, "not file");
		}

	}

	public void Po_save_ini(View view) {
		Log.d(TAG, "Po_save_ini");
		if (AppsGridFragment.mKey1.getText().length() == 0) {
			Toast.makeText(this, "Please select application for keys",
					Toast.LENGTH_SHORT).show();
			return;
		}
		String root = Environment.getExternalStorageDirectory().toString();
		File hotkey_ini = new File(root, "hotkey.ini");
		String msg = "";
		// if (mKey2.getText().length() > 0) msg =
		// "Key1: "+mKey1.getText()+"\nKey2: "+mKey2.getText()+"\n";
		// else msg = "Key1: "+mKey1.getText()+"\n";
		if ((AppsGridFragment.mKey1.getText().length() != 0)
				& (AppsGridFragment.mKey2.getText().length() != 0)
				& (AppsGridFragment.mKey3.getText().length() != 0)
				& (AppsGridFragment.mKey4.getText().length() != 0)) {
			msg = "#key press command\n"
					+ "#key T\n"
					+ "#0 1 am start -a android.settings.WIFI_SETTINGS\n"
					+ "#0 1 echo 'Key T' >> /data/kpd.log\n"
					+ "0 1 ls\n\n"
					+ "#key 1\n"
					+ "#1 1 echo 'Key 1' >> /data/kpd.log\n"
					+ "1 1 am start -n "+ AppsGridFragment.mKey1.getText()+"\n\n"
					+ "#key 2\n"
					+ "#2 1 echo 'Key 2' >> /data/kpd.log\n"
					+ "2 1 am start -n "+AppsGridFragment.mKey2.getText()+"\n\n"
					+ "#key 3\n"
					+ "#3 1 echo 'Key 3' >> /data/kpd.log\n"
					+ "3 1 am start -n "+AppsGridFragment.mKey3.getText()+"\n\n"
					+ "#key 4\n"
					+ "#4 1 echo 'Key 4' >> /data/kpd.log\n"
					+ "4 1 am start -n "+AppsGridFragment.mKey4.getText()+"\n";

		}

		if (hotkey_ini.exists())
			hotkey_ini.delete();
		try {
			FileOutputStream out = new FileOutputStream(hotkey_ini);
			out.write(msg.getBytes());
			Toast.makeText(
					this,
					"You set 1 key -> " + AppsGridFragment.mKey1.getText()
							+ "\nYou set 2 key ->"
							+ AppsGridFragment.mKey2.getText()
							+ "\nYou set 3 key ->"
							+ AppsGridFragment.mKey3.getText()
							+ "\nYou set 4 key ->"
							+ AppsGridFragment.mKey4.getText(),
					Toast.LENGTH_LONG).show();
			show_dialog("Save your hotkey config success",
					"Please reboot devices\n");

		} catch (Exception e) {
			e.printStackTrace();
			Toast.makeText(this.getApplicationContext(),
					"Cannot write file: " + root + "/hotkey.ini",
					Toast.LENGTH_SHORT).show();
		}
	}

	@Override
	protected void onDestroy() {
		int pid = android.os.Process.myPid();
		android.os.Process.killProcess(pid);
		super.onDestroy();
	}

	/***
	 * 顯示diglog
	 * 
	 * @param Title
	 * @param Msg
	 */
	public void show_dialog(String Title, String Msg) {
		AlertDialog.Builder dialog = new AlertDialog.Builder(this);
		dialog.setTitle(Title);
		dialog.setMessage(Msg);
		dialog.setIcon(android.R.drawable.ic_dialog_alert);
		dialog.setCancelable(false);
		dialog.setPositiveButton("OK", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {
				// 按下PositiveButton要做的事
				Toast.makeText(HomeScreen.this, "APP is exit",
						Toast.LENGTH_SHORT).show();
				onDestroy();
			}
		});

		dialog.show();

	}

	/**
	 * isFileExsist() : can check file
	 * 
	 * @param filepath
	 * @return bool
	 */
	static public Boolean isFileExsist(String filepath) {

		File file = new File(filepath);
		return file.exists();

	}

	private void CopyAssets() {
		AssetManager assetManager = getAssets();
		String[] files = null;
		try {
			files = assetManager.list("Files");
		} catch (IOException e) {
			Log.e(TAG, e.getMessage());
		}

		for (String filename : files) {
			Log.i(TAG, "File name => " + filename);
			InputStream in = null;
			OutputStream out = null;
			try {
				in = assetManager.open("Files/" + filename); // if files resides
																// inside the
																// "Files"
																// directory
																// itself
				out = new FileOutputStream(Environment
						.getExternalStorageDirectory().toString()
						+ "/"
						+ filename);
				copyFile(in, out);
				in.close();
				in = null;
				out.flush();
				out.close();
				out = null;
			} catch (Exception e) {
				Log.i(TAG, e.getMessage());
			}
		}
	}

	private void copyFile(InputStream in, OutputStream out) throws IOException {
		byte[] buffer = new byte[1024];
		int read;
		while ((read = in.read(buffer)) != -1) {
			out.write(buffer, 0, read);
		}
	}

	// @Override
	// public boolean onCreateOptionsMenu(Menu menu) {
	// // Inflate the menu; this adds items to the action bar if it is present.
	// getMenuInflater().inflate(R.menu.main, menu);
	// return true;
	// }
}
