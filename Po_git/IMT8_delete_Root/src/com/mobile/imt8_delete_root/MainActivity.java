package com.mobile.imt8_delete_root;

import java.io.FileOutputStream;
import java.io.OutputStreamWriter;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {
	private TextView Po_TV1;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		// 1. find ID
		findViewId();

		// 2. set text
		this.Po_TV1.setText("Are you ready delete Root??");
		show_dialog("Are you sure want to delete Root??",
				"Warning!\nwill delete root!", 2);
	}

	public void Create_File(String file_path, String str) {
		Log.e("Debug", "Create_File()");
		try {
			String data01 = str;
			// String data02 = "busybox cp " + COPY_FILE_PATH + " /data/\n";
			// 建立FileOutputStream物件，路徑為SD卡中的output.txt
			FileOutputStream output = new FileOutputStream(file_path);

			output.write(data01.getBytes());
			// output.write(data02.getBytes());
			output.close();
			Log.d("Debug", "Successfully Create_File()");
			Toast("Successfully write" + file_path);
		} catch (Exception e) {
			Log.d("Debug", "Fails Create_File()");
			Toast("write File Fails");
			e.printStackTrace();
		}
	}

	public void Toast(String str) {
		Toast.makeText(this, str, Toast.LENGTH_SHORT).show();
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
				// Toast("Successfully to su");
			}
		} catch (Exception e) {
			Log.e("Debug", "Fails to su");
			Toast("Fails to su");
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
	public void show_dialog(String Title, String Msg, int btn_num) {
		AlertDialog.Builder dialog = new AlertDialog.Builder(this);
		dialog.setTitle(Title);
		dialog.setMessage(Msg);
		dialog.setIcon(R.drawable.root_x);
		dialog.setCancelable(false);

		if (btn_num == 2) {
			// button number = 2
			dialog.setPositiveButton("Cancel",
					new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {
							// 按下PositiveButton要做的事
							Toast("APP will exit");
							onDestroy();
						}
					});

			dialog.setNegativeButton("Yes",
					new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {
							// 按下NegativeButton要做的事
							// Move_File("/sdcard/command", "/system/command");
							String script_command = "rm /system/app/Superuser.apk";
							Create_File("/sdcard/del_root.sh", script_command);
							Po_TV1.setText("Please click allow, if you need delete Root!");

							Run_su("busybox mv /sdcard/del_root.sh /data/del_root.sh; busybox chmod 777 /data/del_root.sh; ./data/del_root.sh; rm /data/del_root.sh");
							
							//call exit dialog
							Po_TV1.setText("Root delete success!!");
							show_dialog("Root delete success", "please click ok to exit", 1);
						}
					});
		} else {
			// button number = 1
			dialog.setIcon(R.drawable.root_o);
			dialog.setPositiveButton("OK",
					new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {
							// 按下PositiveButton要做的事
							Toast("APP will exit");
							onDestroy();
						}
					});

		}

		dialog.show();
	}

	public void findViewId() {
		this.Po_TV1 = (TextView) findViewById(R.id.Po_tv1);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
}
