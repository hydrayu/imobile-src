package com.imobile.systemuinfconoff;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.nfc.NfcAdapter;
import android.os.Build;
import android.os.Bundle;
import android.provider.Settings;
import android.util.Log;

public class MainActivity extends Activity {

	public NfcAdapter mNfcAdapter;
	public String TAG = "Po_debug";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		// changeNfcEnabled(this, true);

		android.nfc.NfcAdapter mNfcAdapter = android.nfc.NfcAdapter
				.getDefaultAdapter(this);

		if (mNfcAdapter.isEnabled()) {

			AlertDialog.Builder alertbox = new AlertDialog.Builder(this);
			alertbox.setTitle("Info");
			alertbox.setMessage("test");
			alertbox.setPositiveButton("Turn On",
					new DialogInterface.OnClickListener() {
						@Override
						public void onClick(DialogInterface dialog, int which) {
							if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
								Intent intent = new Intent(
										Settings.ACTION_NFC_SETTINGS);
								startActivity(intent);
							} else {
								Intent intent = new Intent(
										Settings.ACTION_WIRELESS_SETTINGS);
								startActivity(intent);
							}
						}
					});
			alertbox.setNegativeButton("Close",
					new DialogInterface.OnClickListener() {

						@Override
						public void onClick(DialogInterface dialog, int which) {

						}
					});
			alertbox.show();

		}

	}

	public boolean changeNfcEnabled(Context context, boolean enabled) {
		// Turn NFC on/off
		final boolean desiredState = enabled;
		mNfcAdapter = NfcAdapter.getDefaultAdapter(context);

		if (mNfcAdapter == null) {
			// NFC is not supported
			Log.d(TAG, "NFC null");
			return false;
		}

		new Thread("toggleNFC") {
			public void run() {
				Log.d(TAG, "Setting NFC enabled state to: " + desiredState);
				boolean success = false;
				Class<?> NfcManagerClass;
				Method setNfcEnabled, setNfcDisabled;
				boolean Nfc;
				if (desiredState) {
					try {
						NfcManagerClass = Class.forName(mNfcAdapter.getClass()
								.getName());
						setNfcEnabled = NfcManagerClass
								.getDeclaredMethod("enable");
						setNfcEnabled.setAccessible(true);
						Nfc = (Boolean) setNfcEnabled.invoke(mNfcAdapter);
						success = Nfc;
					} catch (ClassNotFoundException e) {
					} catch (NoSuchMethodException e) {
					} catch (IllegalArgumentException e) {
					} catch (IllegalAccessException e) {
					} catch (InvocationTargetException e) {
					}
				} else {
					try {
						NfcManagerClass = Class.forName(mNfcAdapter.getClass()
								.getName());
						setNfcDisabled = NfcManagerClass
								.getDeclaredMethod("disable");
						setNfcDisabled.setAccessible(true);
						Nfc = (Boolean) setNfcDisabled.invoke(mNfcAdapter);
						success = Nfc;
					} catch (ClassNotFoundException e) {
					} catch (NoSuchMethodException e) {
					} catch (IllegalArgumentException e) {
					} catch (IllegalAccessException e) {
					} catch (InvocationTargetException e) {
					}
				}
				if (success) {
					Log.d(TAG, "Successfully changed NFC enabled state to "
							+ desiredState);
				} else {
					Log.w(TAG, "Error setting NFC enabled state to "
							+ desiredState);
				}
			}
		}.start();
		return false;
	}// end method

}
