package com.imobile.mt8382hotkeydefine;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Environment;
import android.support.v4.app.LoaderManager;
import android.support.v4.content.Loader;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

/**
 * Created by Arnab Chakraborty
 */
public class AppsGridFragment extends GridFragment implements
		LoaderManager.LoaderCallbacks<ArrayList<AppModel>> {

	AppListAdapter mAdapter;
	static EditText mKey1, mKey2, mKey3, mKey4;
	Context mContext;
	final String TAG = "Po_debug";
	private String Po_green = "#00ff00";
	private String Po_white = "#ffffff";
	private int Po_currnet = 0;

	@Override
	public void onActivityCreated(Bundle savedInstanceState) {
		super.onActivityCreated(savedInstanceState);

		setEmptyText("No Applications");

		mAdapter = new AppListAdapter(getActivity());
		setGridAdapter(mAdapter);

		// till the data is loaded display a spinner
		setGridShown(false);

		// create the loader to load the apps list in background
		getLoaderManager().initLoader(0, null, this);
	}

	@Override
	public Loader<ArrayList<AppModel>> onCreateLoader(int id, Bundle bundle) {
		return new AppsLoader(getActivity());
	}

	@Override
	public void onLoadFinished(Loader<ArrayList<AppModel>> loader,
			ArrayList<AppModel> apps) {
		mAdapter.setData(apps);

		if (isResumed()) {
			setGridShown(true);
		} else {
			setGridShownNoAnimation(true);
		}

		mKey1 = (EditText) getView().getRootView().findViewById(R.id.key1);
		mKey2 = (EditText) getView().getRootView().findViewById(R.id.key2);
		mKey3 = (EditText) getView().getRootView().findViewById(R.id.key3);
		mKey4 = (EditText) getView().getRootView().findViewById(R.id.key4);
		DisplayMetrics screenMetrics = new DisplayMetrics();
		getActivity().getWindowManager().getDefaultDisplay()
				.getMetrics(screenMetrics);
		int width = (screenMetrics.widthPixels - 150) >> 1;

		// Po add 2015/08/04
		if (Po_currnet == 0) {
			// set mKey2 off
			mKey2.setEnabled(false);
			mKey2.setText("");
			mKey2.setFocusable(false);
			mKey2.setWidth(width);

			// set mKey3 off
			mKey3.setEnabled(false);
			mKey3.setText("");
			mKey3.setFocusable(false);
			mKey3.setWidth(width);

			// set mKey4 off
			mKey4.setEnabled(false);
			mKey4.setText("");
			mKey4.setFocusable(false);
			mKey4.setWidth(width);

			// set mKey1 on
			mKey1.setEnabled(true);
			mKey1.setText("");
			mKey1.setFocusable(false);
			mKey1.setWidth(width);
		}
		// Po End

		// check have /mnt/sdcard/Hotkey.ini
		String FilePath = "/mnt/sdcard/hotkey.ini";
		if (HomeScreen.isFileExsist(FilePath)) {
			Log.d(TAG, "found " + FilePath);
			ReadHotkeyData();
		}
	}

	private void ReadHotkeyData() {

		/****
		 * read /mnt/sdcard/hotkey.ini
		 */
		// Find the directory for the SD Card using the API
		// *Don't* hardcode "/sdcard"
		File sdcard = Environment.getExternalStorageDirectory();

		// Get the text file
		File file = new File(sdcard, "hotkey.ini");

		// Read text from file
		StringBuilder text = new StringBuilder();

		try {
			BufferedReader br = new BufferedReader(new FileReader(file));
			String line;

			while ((line = br.readLine()) != null) {
				// copy need text
				if (line.indexOf("1 1 am") != -1) {
					text.append(line);
					text.append('\n');
				} else if (line.indexOf("2 1 am") != -1) {
					text.append(line);
					text.append('\n');
				} else if (line.indexOf("3 1 am") != -1) {
					text.append(line);
					text.append('\n');
				} else if (line.indexOf("4 1 am") != -1) {
					text.append(line);
					text.append('\n');
				}

			}
			br.close();
		} catch (IOException e) {
			// You'll need to add proper error handling here
		}

//		Log.d(TAG, "hotkey data = " + text.toString());

		/****
		 * split \n
		 */
		String str = text.toString();
		String[] tokens = str.split("\n");
		// for (String token : tokens) {
		// Log.d(TAG, "token = " + token.toString());
		// }
		if (tokens.length == 4) {
			// Log.d(TAG, "tokens length " + tokens.length);
			String Po_key1 = tokens[0];
			String Po_key2 = tokens[1];
			String Po_key3 = tokens[2];
			String Po_key4 = tokens[3];
			Log.d(TAG, "Po_key1 =" + Po_key1);

			// ㏃¾ data 돌 Po_key1
			// 1 1 am start -n com.anytag.android/com.anytag.android....
			tokens = Po_key1.split(" ");
			if (tokens.length == 6) {
				// Log.d(TAG, " Po_key1 tokens length " + tokens.length);
				Po_key1 = tokens[5];
				Log.d(TAG, "new Po_key1 = " + Po_key1);
			}

			// ㏃¾ data 돌 Po_key2
			// 2 1 am start -n com.android.browser/com.android.brows....
			tokens = Po_key2.split(" ");
			if (tokens.length == 6) {
				// Log.d(TAG, " Po_key2 tokens length " + tokens.length);
				Po_key2 = tokens[5];
				Log.d(TAG, "new Po_key2 = " + Po_key2);
			}
			
			// ㏃¾ data 돌 Po_key3
			//3 1 am start -n com.android.browser/com.android.brows....
			tokens = Po_key3.split(" ");
			if (tokens.length == 6) {
				// Log.d(TAG, " Po_key3 tokens length " + tokens.length);
				Po_key3 = tokens[5];
				Log.d(TAG, "new Po_key3 = " + Po_key3);
			}
			
			// ㏃¾ data 돌 Po_key4
			// 4 1 am start -n com.android.browser/com.android.brows....
			tokens = Po_key4.split(" ");
			if (tokens.length == 6) {
				// Log.d(TAG, " Po_key4 tokens length " + tokens.length);
				Po_key4 = tokens[5];
				Log.d(TAG, "new Po_key4 = " + Po_key4);
			}

			// set layout
			mKey1.setText(Po_key1);
			mKey2.setText(Po_key2);
			mKey3.setText(Po_key3);
			mKey4.setText(Po_key4);

		}

	}

	@Override
	public void onLoaderReset(Loader<ArrayList<AppModel>> loader) {
		mAdapter.setData(null);
	}

	@Override
	public void onGridItemClick(GridView g, View v, int position, long id) {
		AppModel app = (AppModel) getGridAdapter().getItem(position);
		// Launch App by:
		// Intent intent =
		// getActivity().getPackageManager().getLaunchIntentForPackage(app.getApplicationPackageName());
		if (mContext == null && app != null)
			mContext = getActivity().getApplicationContext();
		if (mContext != null) {
			String myActivityName = getActivity().getPackageManager()
					.getLaunchIntentForPackage(app.getApplicationPackageName())
					.resolveActivity(mContext.getPackageManager())
					.getClassName();
			String PoPackageName = getActivity().getPackageManager()
					.getLaunchIntentForPackage(app.getApplicationPackageName())
					.resolveActivity(mContext.getPackageManager())
					.getPackageName();
			String PoResult = PoPackageName + "/" + myActivityName;

			// mKey1.setTextColor(Color.parseColor(Po_green));
			// mKey2.setTextColor(Color.parseColor(Po_green));

			
			
			if (++this.Po_currnet > 4) {
				this.Po_currnet = 1;
				mKey1.setEnabled(true);
				mKey1.setText(PoResult);
				// mKey1.setTextColor(Color.parseColor(Po_green));
				mKey1.setEnabled(false);
				mKey2.setEnabled(true);
				mKey3.setEnabled(false);
				mKey4.setEnabled(false);
			} else {
				switch (this.Po_currnet) {
				case 1:
					mKey1.setEnabled(true);
					mKey1.setText(PoResult);
					// mKey1.setTextColor(Color.parseColor(Po_white));
					mKey1.setEnabled(false);
					mKey2.setEnabled(true);
					mKey3.setEnabled(false);
					mKey4.setEnabled(false);
					break;
				case 2:
					mKey2.setEnabled(true);
					mKey2.setText(PoResult);
					// mKey2.setTextColor(Color.parseColor(Po_white));
					mKey2.setEnabled(false);
					mKey3.setEnabled(true);
					mKey1.setEnabled(false);
					mKey4.setEnabled(false);
					break;
				case 3:
					mKey3.setEnabled(true);
					mKey3.setText(PoResult);
					// mKey3.setTextColor(Color.parseColor(Po_white));
					mKey3.setEnabled(false);
					mKey4.setEnabled(true);
					mKey1.setEnabled(false);
					mKey2.setEnabled(false);
					break;
				case 4:
					mKey4.setEnabled(true);
					mKey4.setText(PoResult);
					// mKey4.setTextColor(Color.parseColor(Po_white));
					mKey4.setEnabled(false);
					mKey1.setEnabled(true);
					mKey2.setEnabled(false);
					mKey3.setEnabled(false);
					break;

				default:
					break;
				}
			}
			Log.d(TAG, "Po current=" + this.Po_currnet);

			/*
			 * Toast.makeText(myContext, myActivityName,
			 * Toast.LENGTH_SHORT).show();
			 */
		}
	}
}
