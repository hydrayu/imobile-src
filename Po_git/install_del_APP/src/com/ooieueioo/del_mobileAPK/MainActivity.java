package com.ooieueioo.del_mobileAPK;

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnMultiChoiceClickListener;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {

	private boolean province_bl[];
	// private CharSequence[] list;
	private String[] province;
	String Delete_File;

	private ListView lv;
	TextView Tv;
	ProgressBar myProgressBar;
	int myProgress = 20;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		Log.e("Debug", "onCreate()");
		findViewId();
		Tv.setText("Scanning i-mobile APKs");

		get_Dir();
		showMultiChoiceItems();

		Tv.setText("Create script..");
		// Create_File("/sdcard/r.sh");
		// Run_su("busybox chmod 777 /sdcard/r.sh");
		// Run_su("busybox msh /sdcard/r.sh");

		myProgress = 100;
		myProgressBar.setProgress(myProgress);

		// Tv.setText("OK, Please shutdown!");

	}

	public void findViewId() {
		Tv = (TextView) findViewById(R.id.Po_tv);
		myProgressBar = (ProgressBar) findViewById(R.id.progressbar);
		myProgressBar.setProgress(myProgress);
	}

	public boolean fileIsExists(String Path) {
		File f = new File(Path);
		if (!f.exists()) {
			return false;
		}
		return true;
	}

	public void show_dialog(String Title, String Msg) {
		AlertDialog.Builder dialog = new AlertDialog.Builder(this);
		dialog.setTitle(Title);
		dialog.setMessage(Msg);
		dialog.setIcon(android.R.drawable.ic_dialog_alert);
		dialog.setCancelable(false);
		dialog.setPositiveButton("OK", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {
				// ?��?PositiveButton要�??��?
				Toast.makeText(MainActivity.this, "APP is exit",
						Toast.LENGTH_SHORT).show();
				onDestroy();
			}
		});

		dialog.show();

	}

	/**
	 * ?��??�己
	 * 
	 * @see android.app.Activity#onDestroy()
	 */
	@Override
	protected void onDestroy() {
		int pid = android.os.Process.myPid();
		android.os.Process.killProcess(pid);
		super.onDestroy();
	}

	/***
	 * ?�建�??檔�?
	 ***/
	public void Create_File(String str) {
		Log.e("Debug", "Create_File()");
		try {
			String data01 = this.Delete_File;
			// String data01 = "rm /system/install_APKs/RootBrowser.apk \n";

			// 建�?FileOutputStream?�件，路徑為SD?�中?�output.txt
			FileOutputStream output = new FileOutputStream(str);

			output.write(data01.getBytes());

			output.close();
			Log.e("Debug", "Successfully Create_File()");
			Toast("Successfully write /sdcard/command");
		} catch (Exception e) {
			Log.e("Debug", "Fails Create_File()");
			Toast("write File Fails");
			e.printStackTrace();
		}
	}

	public void Run_su(String str) {
		Runtime ex = Runtime.getRuntime();
		String cmdBecomeSu = "su";
		String script = str;
		Log.e("Debug", "Run_su()");
		try {
			java.lang.Process runsum = ex.exec(cmdBecomeSu);
			int exitVal = 0;
			final OutputStreamWriter out = new OutputStreamWriter(
					runsum.getOutputStream());
			// Write the script to be executed
			out.write(script);
			// Ensure that the last character is an "enter"
			out.write("\n");
			out.flush();
			// Terminate the "su" process
			out.write("exit\n");
			out.flush();
			exitVal = runsum.waitFor();
			if (exitVal == 0) {
				Log.e("Debug", "Successfully to su");
				Toast("Successfully to su");
			}
		} catch (Exception e) {
			Log.e("Debug", "Fails to su");
			Toast("Fails to su");
		}
	}

	public void get_Dir() {
		Tv.setText("Create get_Dir..");
		// File sdDir2 = null;
		// sdDir2 =Environment.getRootDirectory();
		File file = new File("/system/install_APKs");
		Log.e("Debug", "getRootDirectory(): " + file.toString());

		File[] mediaInDir = file.listFiles();

		// 將目?�內容建立�???

		// list = new CharSequence[mediaInDir.length];
		province = new String[mediaInDir.length];
		province_bl = new boolean[mediaInDir.length];
		for (int i = 0; i < province.length; i++) {

			// list[i] = mediaInDir[i].getName();
			// Log.e("Debug", "list[" + i + "] = " + list[i]);
			province[i] = mediaInDir[i].getName().toString();
			Log.e("Debug", "province[" + i + "] = " + province[i]);
			province_bl[i] = false;
		}

	}

	/****
	 * ?�CheckBox ??Dialog
	 ****/
	private void showMultiChoiceItems() {
		AlertDialog builder = new AlertDialog.Builder(this)
				.setTitle("Please select the file you want to delete")
				.setMultiChoiceItems(province, province_bl,
						new OnMultiChoiceClickListener() {

							@Override
							public void onClick(DialogInterface dialog,
									int which, boolean isChecked) {
								// TODO Auto-generated method stub
							}
						})
				.setPositiveButton("Delete",
						new DialogInterface.OnClickListener() {

							@Override
							public void onClick(DialogInterface dialog,
									int which) {

								String s = "";
								// ?��?????��?表项，�??��??��?表项被�?中�?将�?表项?��??�追?�到s?��?中�?
								for (int i = 0; i < province.length; i++) {
									if (lv.getCheckedItemPositions().get(i)) {
										s += i + " ";
									}
								}

								// ?�户?��??�择了�?个�?表项
								if (lv.getCheckedItemPositions().size() > 0) {

									Check_Delete_File(s);
									Create_File("/sdcard/r.sh");
									Run_su("busybox chmod 777 /sdcard/r.sh ; busybox msh /sdcard/r.sh");
									Tv.setText("Delete File..");

									String Title = "Please remove APP and reboot!";
									String Msg = "Please to Setting -> APPS, Then remove APP";
									show_dialog(Title, Msg);
								}

								// ?�户?��??�任何�?表项
								else if (lv.getCheckedItemPositions().size() <= 0) {
									Tv.setText("Not Select Delete File..");

									String Title = "You did not remove any applications";
									String Msg = "Please close the application";
									show_dialog(Title, Msg);
								}
							}
						})
				.setNegativeButton("Cancel",
						new DialogInterface.OnClickListener() {

							@Override
							public void onClick(DialogInterface dialog,
									int which) {
								// TODO Auto-generated method stub
								onDestroy();
							}
						}).create();
		lv = builder.getListView();
		builder.show();
	}

	/****
	 * 確�?要Delete?��?�?
	 ****/
	private void Check_Delete_File(String str) {
		String[] token = str.split(" ");

		for (int i = 0; i < token.length; i++) {
			Log.e("Debug", " token[ " + i + " ] = " + token[i]);
			int intValue = Integer.valueOf(token[i]);
			this.province_bl[intValue] = true;
		}
		// ?�斷被選?��?
		this.Delete_File = "";
		String System_Path = "rm /system/install_APKs/";
		String Data_Path = "rm /data/app/";
		for (int i = 0; i < this.province_bl.length; i++) {
			if (this.province_bl[i] == true) {
				this.Delete_File += System_Path + province[i] + " \n"
						+ Data_Path + province[i] + " \n";
			}

		}
		Log.e("Debug", " Delete_File = ");
		Log.e("Debug", Delete_File);
	}

	public void Toast(String str) {
		Toast.makeText(this, str, Toast.LENGTH_SHORT).show();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
